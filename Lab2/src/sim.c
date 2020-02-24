#include <stdio.h>
#include "shell.h"
#define OP_SPECIAL    0x00
#define SUBOP_SLL     0x00
#define SUBOP_SRL     0x02
#define SUBOP_SRA     0x03
#define SUBOP_ADD     0x20
#define SUBOP_ADDU    0x21
#define SUBOP_SUB     0x22
#define SUBOP_SUBU    0x23
#define SUBOP_AND     0x24
#define SUBOP_OR      0x25
#define SUBOP_XOR     0x26
#define SUBOP_NOR     0x27
#define SUBOP_SLT     0x2A
#define SUBOP_SLTU    0x2B
#define OP_ADDI       0x08
#define OP_ADDIU      0x09
#define OP_SLTI       0x0A
#define OP_SLTIU      0x0B
#define OP_ANDI       0x0C
#define OP_ORI        0x0D
#define OP_XORI       0x0E
#define SUBOP_SYSCALL 0x0c
uint32_t dcd_op;     /* decoded opcode */
uint32_t dcd_rs;     /* decoded rs operand */
uint32_t dcd_rt;     /* decoded rt operand */
uint32_t dcd_rd;     /* decoded rd operand */
uint32_t dcd_shamt;  /* decoded shift amount */
uint32_t dcd_funct;  /* decoded function */
uint32_t dcd_imm;    /* decoded immediate value */
uint32_t dcd_target; /* decoded target address */
int      dcd_se_imm; /* decoded sign-extended immediate value */
uint32_t inst;       /* machine instruction */

uint32_t sign_extend_h2w(uint16_t c)
{
    return (c & 0x8000) ? (c | 0xffff8000) : c;
}

int32_t un_to_sign(uint32_t c)
{
    return c;
}
// adapted from: https://stackoverflow.com/a/38803993
uint32_t sign_extension(uint32_t c)
{
    return (c&(1<<(dcd_shamt-1))) ? (c-(1<<dcd_shamt)) : c;
}

void fetch()
{
    /* fetch the 4 bytes of the current instruction */
    inst = mem_read_32(CURRENT_STATE.PC);
}

void decode()
{
    /* decoding an instruction */
    dcd_op     = (inst >> 26) & 0x3F;
    dcd_rs     = (inst >> 21) & 0x1F;
    dcd_rt     = (inst >> 16) & 0x1F;
    dcd_rd     = (inst >> 11) & 0x1F;
    dcd_shamt  = (inst >> 6) & 0x1F;
    dcd_funct  = (inst >> 0) & 0x3F;
    dcd_imm    = (inst >> 0) & 0xFFFF;
    dcd_se_imm = sign_extend_h2w(dcd_imm);
    dcd_target = (inst >> 0) & ((1UL << 26) - 1);
}

void execute()
{
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    CURRENT_STATE.REGS[0] = 0;

    switch (dcd_op)
    {
        case OP_SPECIAL: 
            switch (dcd_funct)
            {
                case SUBOP_SLL:
                    NEXT_STATE.REGS[dcd_rd] = CURRENT_STATE.REGS[dcd_rt] << dcd_shamt;
                    break;
                case SUBOP_SRL:
                    NEXT_STATE.REGS[dcd_rd] = CURRENT_STATE.REGS[dcd_rt] >> dcd_shamt;
                    break;
                case SUBOP_SRA:
                    NEXT_STATE.REGS[dcd_rd] = sign_extension(CURRENT_STATE.REGS[dcd_rt] >> dcd_shamt);
                    break;
                case SUBOP_ADD: 
                    NEXT_STATE.REGS[dcd_rd] = un_to_sign(CURRENT_STATE.REGS[dcd_rs]) + un_to_sign(CURRENT_STATE.REGS[dcd_rt]);
                    break;
                case SUBOP_ADDU: 
                    NEXT_STATE.REGS[dcd_rd] = CURRENT_STATE.REGS[dcd_rs] + CURRENT_STATE.REGS[dcd_rt];
                    break;   
                case SUBOP_SUB:
                    NEXT_STATE.REGS[dcd_rd] = un_to_sign(CURRENT_STATE.REGS[dcd_rs]) - un_to_sign(CURRENT_STATE.REGS[dcd_rt]);
                    break;   
                case SUBOP_SUBU:
                    NEXT_STATE.REGS[dcd_rd] = CURRENT_STATE.REGS[dcd_rs] - CURRENT_STATE.REGS[dcd_rt];
                    break;
                case SUBOP_AND:
                    NEXT_STATE.REGS[dcd_rd] = CURRENT_STATE.REGS[dcd_rs] & CURRENT_STATE.REGS[dcd_rt];
                    break;     
                case SUBOP_OR:
                    NEXT_STATE.REGS[dcd_rd] = CURRENT_STATE.REGS[dcd_rs] | CURRENT_STATE.REGS[dcd_rt];
                    break;     
                case SUBOP_XOR:
                    NEXT_STATE.REGS[dcd_rd] = CURRENT_STATE.REGS[dcd_rs] ^ CURRENT_STATE.REGS[dcd_rt];
                    break;
                case SUBOP_NOR:
                    NEXT_STATE.REGS[dcd_rd] = ~ (CURRENT_STATE.REGS[dcd_rs] | CURRENT_STATE.REGS[dcd_rt]);
                    break;
                case SUBOP_SYSCALL:
                    if (CURRENT_STATE.REGS[2] == 10)
                        RUN_BIT = 0;
                    break;
                case SUBOP_SLT:
                    NEXT_STATE.REGS[dcd_rd] = (un_to_sign(CURRENT_STATE.REGS[dcd_rs]) < un_to_sign(CURRENT_STATE.REGS[dcd_rt])) ? 1 : 0;
                    break;
                case SUBOP_SLTU:
                    NEXT_STATE.REGS[dcd_rd] = (CURRENT_STATE.REGS[dcd_rs])
            }
            break;

        case OP_ADDI:
            NEXT_STATE.REGS[dcd_rt] = un_to_sign(CURRENT_STATE.REGS[dcd_rs]) + dcd_se_imm;
            break;
        case OP_ADDIU:
            NEXT_STATE.REGS[dcd_rt] = CURRENT_STATE.REGS[dcd_rs] + dcd_se_imm;
            break;
        case OP_SLTI:
            NEXT_STATE.REGS[dcd_rt] = (un_to_sign(CURRENT_STATE.REGS[dcd_rs]) < dcd_se_imm) ? 1 : 0;
            break;
        case OP_SLTIU:
            NEXT_STATE.REGS[dcd_rt] = (CURRENT_STATE.REGS[dcd_rs] < dcd_se_imm) ? 1 : 0;
            break;
        case OP_ANDI:
            NEXT_STATE.REGS[dcd_rt] = dcd_imm & CURRENT_STATE.REGS[dcd_rs];
            break;
        case OP_ORI:
            NEXT_STATE.REGS[dcd_rt] = dcd_imm | CURRENT_STATE.REGS[dcd_rs];
            break;
        case OP_XORI:
            NEXT_STATE.REGS[dcd_rt] = dcd_imm ^ CURRENT_STATE.REGS[dcd_rs];
            break;
    }

    CURRENT_STATE.REGS[0] = 0;
}


void process_instruction()
{
   fetch();
   decode();
   execute();
    /* execute one instruction here. You should use CURRENT_STATE and modify
     * values in NEXT_STATE. You can call mem_read_32() and mem_write_32() to
     * access memory. */
}
