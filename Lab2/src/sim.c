#include <stdio.h>
#include "shell.h"
#define OP_SPECIAL    0x00
#define SUBOP_ADD     0x20
#define SUBOP_ADDU    0x21
#define SUBOP_AND     0x24
#define SUBOP_OR      0x25
#define SUBOP_XOR     0x26
#define SUBOP_NOR     0x27
#define OP_ADDI       0x08
#define OP_ADDIU      0x09
#define OP_ANDI       0x0c
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
                case SUBOP_ADD: 
                    break;
                case SUBOP_ADDU: 
                    NEXT_STATE.REGS[dcd_rd] = CURRENT_STATE.REGS[dcd_rs] + CURRENT_STATE.REGS[dcd_rt];
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
                    NEXT_STATE.REGS[dcd_rd] = CURRENT_STATE.REGS[dcd_rs] ~ CURRENT_STATE.REGS[dcd_rt];
                case SUBOP_SYSCALL:
                    if (CURRENT_STATE.REGS[2] == 10)
                        RUN_BIT = 0;
                    break;
            }
            break;

        case OP_ADDI:
            //CHECK FOR OVERFLOW
            //IF OVERFLOW
            NEXT_STATE.REGS[14] = NEXT_STATE.PC;     // EPC = PC
            NEXT_STATE.REGS[13] = 12;                   // Cause = Ov
            NEXT_STATE.REGS[12] << 4;                   // Status << 4
            NEXT_STATE.PC = 4;
            //ELSE 
            NEXT_STATE.REGS[dcd_rt] = CURRENT_STATE.REGS[dcd_rs] + dcd_se_imm;
        case OP_ADDIU:
            NEXT_STATE.REGS[dcd_rt] = CURRENT_STATE.REGS[dcd_rs] + dcd_se_imm;
            break;
        case OP_ANDI:
            NEXT_STATE.REGS[dcd_rt] = dcd_se_imm & CURRENT_STATE.REGS[dcd_rs]
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
