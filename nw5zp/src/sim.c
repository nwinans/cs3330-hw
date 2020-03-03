/**
 * Part 1 of Lab 2 for CS 3330
 * Nick Winans - nw5zp
 * Extra Credit - Implemented Int Print, File Open, File Read, File Write, and File close SYSCALLS.
 */

#include <stdio.h>
#include "shell.h"

#define OP_SPECIAL    0x00
#define SUBOP_SLL     0x00
#define SUBOP_SRL     0x02
#define SUBOP_SRA     0x03
#define SUBOP_SLLV    0x04
#define SUBOP_SRLV    0x06
#define SUBOP_SRAV    0x07
#define SUBOP_JR      0x08
#define SUBOP_JALR    0x09
#define SUBOP_SYSCALL 0x0C
#define SUBOP_MFHI    0x10
#define SUBOP_MTHI    0x11
#define SUBOP_MFLO    0x12
#define SUBOP_MTLO    0x13
#define SUBOP_MULT    0x18
#define SUBOP_MULTU   0x19
#define SUBOP_DIV     0x1A
#define SUBOP_DIVU    0x1B
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
#define OP_REGIMM     0x01
#define OP_J          0x02
#define OP_JAL        0x03
#define OP_BEQ        0x04
#define OP_BNE        0x05
#define OP_BLEZ       0x06
#define OP_BGTZ       0x07
#define OP_ADDI       0x08
#define OP_ADDIU      0x09
#define OP_SLTI       0x0A
#define OP_SLTIU      0x0B
#define OP_ANDI       0x0C
#define OP_ORI        0x0D
#define OP_XORI       0x0E
#define OP_LUI        0x0F
#define OP_LB         0x20
#define OP_LH         0x21
#define OP_LW         0x23
#define OP_LBU        0x24
#define OP_LHU        0x25
#define OP_SB         0x28
#define OP_SH         0x29
#define OP_SW         0x2B
#define REGOP_BGEZ    0x01
#define REGOP_BLTZAL  0x10
#define REGOP_BGEZAL  0x11

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
uint64_t product;

uint32_t sign_extend_h2w(uint16_t c)
{
    return (c & 0x8000) ? (c | 0xffff8000) : c;
}

uint32_t sign_extend_b2w(uint8_t c)
{
    return (c & 0x80) ? (c | 0xFFFFFF80) : c;
}

int32_t un_to_sign(uint32_t c)
{
    return c;
}

int64_t sign_extend_w2dw(uint32_t c)
{
    return (c & 0x80000000) ? (c | 0xFFFFFFFF80000000) : c;
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
                    NEXT_STATE.REGS[dcd_rd] = CURRENT_STATE.REGS[dcd_rt] << (dcd_shamt & 0x1F);
                    break;
                case SUBOP_SRL:
                    NEXT_STATE.REGS[dcd_rd] = CURRENT_STATE.REGS[dcd_rt] >> (dcd_shamt & 0x1F);
                    break;
                case SUBOP_SRA:
                    NEXT_STATE.REGS[dcd_rd] = ((int32_t) CURRENT_STATE.REGS[dcd_rt]) >> (dcd_shamt & 0x1F);
                    break;
                case SUBOP_SLLV:
                    NEXT_STATE.REGS[dcd_rd] = CURRENT_STATE.REGS[dcd_rt] << (CURRENT_STATE.REGS[dcd_rs] & 0x1F);
                    break;
                case SUBOP_SRLV:
                    NEXT_STATE.REGS[dcd_rd] = CURRENT_STATE.REGS[dcd_rt] >> (CURRENT_STATE.REGS[dcd_rs] & 0x1F);
                    break;
                case SUBOP_SRAV:
                    NEXT_STATE.REGS[dcd_rd] = ((int32_t) CURRENT_STATE.REGS[dcd_rt]) >> (CURRENT_STATE.REGS[dcd_rs] & 0x1F);
                    break;
                case SUBOP_JR:
                    NEXT_STATE.PC = CURRENT_STATE.REGS[dcd_rs];
                    break;
                case SUBOP_JALR:
                    NEXT_STATE.REGS[dcd_rd] = CURRENT_STATE.PC + 4; 
                    NEXT_STATE.PC = CURRENT_STATE.REGS[dcd_rs];
                    break;
                case SUBOP_MFHI:
                    NEXT_STATE.REGS[dcd_rd] = CURRENT_STATE.HI;
                    break;
                case SUBOP_MTHI:
                    NEXT_STATE.HI = CURRENT_STATE.REGS[dcd_rs];
                    break;
                case SUBOP_MFLO:
                    NEXT_STATE.REGS[dcd_rd] = CURRENT_STATE.LO;
                    break;
                case SUBOP_MTLO:
                    NEXT_STATE.LO = CURRENT_STATE.REGS[dcd_rs];
                    break;
                case SUBOP_MULT:
                    product = sign_extend_w2dw(CURRENT_STATE.REGS[dcd_rs]) * sign_extend_w2dw(CURRENT_STATE.REGS[dcd_rt]);
                    NEXT_STATE.HI = (product >> 32) & 0xFFFFFFFF;
                    NEXT_STATE.LO = (product) & 0xFFFFFFFF;
                    break;
                case SUBOP_MULTU:
                    product = ((uint64_t) CURRENT_STATE.REGS[dcd_rs]) * ((uint64_t) CURRENT_STATE.REGS[dcd_rt]);
                    NEXT_STATE.HI = (product >> 32) & 0xFFFFFFFF;
                    NEXT_STATE.LO = (product) & 0xFFFFFFFF;
                    break;
                case SUBOP_DIV:
                    NEXT_STATE.LO = un_to_sign(CURRENT_STATE.REGS[dcd_rs]) / un_to_sign(CURRENT_STATE.REGS[dcd_rt]);
                    NEXT_STATE.HI = un_to_sign(CURRENT_STATE.REGS[dcd_rs]) % un_to_sign(CURRENT_STATE.REGS[dcd_rt]);
                    break;      
                case SUBOP_DIVU:
                    NEXT_STATE.LO = CURRENT_STATE.REGS[dcd_rs] / CURRENT_STATE.REGS[dcd_rt];
                    NEXT_STATE.HI = CURRENT_STATE.REGS[dcd_rs] % CURRENT_STATE.REGS[dcd_rt];
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
                    if (CURRENT_STATE.REGS[2] == 1)
                        printf("%d", CURRENT_STATE.REGS[4]);
                    else if (CURRENT_STATE.REGS[2] == 4)
                        printf("%s", (char) mem_read_32(CURRENT_STATE.REGS[4]));
                    else if (CURRENT_STATE.REGS[2] == 9)
                        CURRENT_STATE.REGS[2] = (int) malloc(CURRENT_STATE.REGS[4]);
                    else if (CURRENT_STATE.REGS[2] == 10)
                        RUN_BIT = 0;
                    else if (CURRENT_STATE.REGS[2] == 11)
                        printf("%s", (char) CURRENT_STATE.REGS[4]);
                    else if (CURRENT_STATE.REGS[2] == 13)
                    {
                        FILE *f = fopen((char *) (intptr_t) CURRENT_STATE.REGS[4], (CURRENT_STATE.REGS[5] == 0) ? "r" : "w");
                        NEXT_STATE.REGS[2] = (uint32_t) f;
                    }
                    else if (CURRENT_STATE.REGS[2] == 14) 
                    {
                        fgets((char *) CURRENT_STATE.REGS[5], CURRENT_STATE.REGS[6], (FILE *) CURRENT_STATE.REGS[4]);
                        NEXT_STATE.REGS[2] = CURRENT_STATE.REGS[6];
                    }
                    else if (CURRENT_STATE.REGS[2] == 15)
                    {
                        char str[CURRENT_STATE.REGS[6]];
                        *str = (char *) CURRENT_STATE.REGS[5]; 
                        fprintf((FILE *) CURRENT_STATE.REGS[4], "%s", str);
                        NEXT_STATE.REGS[2] = CURRENT_STATE.REGS[6];
                    }
                    else if (CURRENT_STATE.REGS[2] == 16)
                    {
                        fclose((FILE *) CURRENT_STATE.REGS[4]);
                    }
                    else if (CURRENT_STATE.REGS[2] == 17)
                    {
                        printf("Exiting with code %d", CURRENT_STATE.REGS[4]);
                        RUN_BIT = 0;
                    }
                    else if (CURRENT_STATE.REGS[2] == 34)
                    {
                        printf("%x", CURRENT_STATE.REGS[4]);
                    }
                    break;
                case SUBOP_SLT:
                    NEXT_STATE.REGS[dcd_rd] = (un_to_sign(CURRENT_STATE.REGS[dcd_rs]) < un_to_sign(CURRENT_STATE.REGS[dcd_rt])) ? 1 : 0;
                    break;
                case SUBOP_SLTU:
                    NEXT_STATE.REGS[dcd_rd] = (CURRENT_STATE.REGS[dcd_rs] < CURRENT_STATE.REGS[dcd_rt]) ? 1 : 0;
                    break;
            }
            break;
        case OP_REGIMM:
            switch (CURRENT_STATE.REGS[dcd_rt])
            {
                case REGOP_BGEZ:
                    if (CURRENT_STATE.REGS[dcd_rs] >= 0)
                        NEXT_STATE.PC = CURRENT_STATE.PC + (dcd_se_imm << 2);
                    break;
                case REGOP_BLTZAL:
                    if (CURRENT_STATE.REGS[dcd_rs] < 0)
                    {
                        NEXT_STATE.REGS[31] = CURRENT_STATE.PC + 4;
                        NEXT_STATE.PC = CURRENT_STATE.PC + (dcd_se_imm << 2);
                    }
                    break;
                case REGOP_BGEZAL:
                    if (CURRENT_STATE.REGS[dcd_rs] >= 0)
                    {
                        NEXT_STATE.REGS[31] = CURRENT_STATE.PC + 4;
                        NEXT_STATE.PC = CURRENT_STATE.PC + (dcd_se_imm << 2);
                    }
                    break;
            }
        case OP_J:
            NEXT_STATE.PC = (CURRENT_STATE.PC & 0xF0000000) + (dcd_target << 2);
            break;
        case OP_JAL:
            NEXT_STATE.PC = (CURRENT_STATE.PC & 0xF0000000);
            NEXT_STATE.REGS[31] = CURRENT_STATE.PC + 4;
            break;
        case OP_BEQ:
            if (CURRENT_STATE.REGS[dcd_rs] == CURRENT_STATE.REGS[dcd_rt])
                NEXT_STATE.PC += (dcd_se_imm << 2) - 4;
            break;
        case OP_BNE:
            if (CURRENT_STATE.REGS[dcd_rs] != CURRENT_STATE.REGS[dcd_rt])
                NEXT_STATE.PC += (dcd_se_imm << 2) - 4;
            break;
        case OP_BLEZ:
            if (CURRENT_STATE.REGS[dcd_rs] <= 0)
                NEXT_STATE.PC += (dcd_se_imm << 2) - 4;
            break;
        case OP_BGTZ:
            if (CURRENT_STATE.REGS[dcd_rs] > 0) 
                NEXT_STATE.PC += (dcd_se_imm << 2) - 4;
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
        case OP_LUI:
            NEXT_STATE.REGS[dcd_rt] = dcd_imm << 16;
            break;
        case OP_LB:
            NEXT_STATE.REGS[dcd_rt] = sign_extend_b2w(mem_read_32(dcd_se_imm + CURRENT_STATE.REGS[dcd_rs]) & 0xFF);
            break;
        case OP_LH:
            NEXT_STATE.REGS[dcd_rt] = sign_extend_h2w(mem_read_32(dcd_se_imm + CURRENT_STATE.REGS[dcd_rs]) & 0xFFFF);
            break;
        case OP_LBU:
            NEXT_STATE.REGS[dcd_rt] = mem_read_32(dcd_se_imm + CURRENT_STATE.REGS[dcd_rs]) & 0xFF;
            break;
        case OP_LHU:
            NEXT_STATE.REGS[dcd_rt] = mem_read_32(dcd_se_imm + CURRENT_STATE.REGS[dcd_rs]) & 0xFFFF;
            break;
        case OP_LW:
            NEXT_STATE.REGS[dcd_rt] = mem_read_32(dcd_se_imm + CURRENT_STATE.REGS[dcd_rs]);
            break;
        case OP_SB:
            mem_write_32(dcd_se_imm + CURRENT_STATE.REGS[dcd_rs], CURRENT_STATE.REGS[dcd_rt] & 0xFF);
            break;
        case OP_SH:
            mem_write_32(dcd_se_imm + CURRENT_STATE.REGS[dcd_rs], CURRENT_STATE.REGS[dcd_rt] & 0xFFFF);
            break;
        case OP_SW:
            mem_write_32(dcd_se_imm + CURRENT_STATE.REGS[dcd_rs], CURRENT_STATE.REGS[dcd_rt]);
            break;
    }

    CURRENT_STATE.REGS[0] = 0;
}


void process_instruction()
{
   fetch();
   decode();
   execute();
}
