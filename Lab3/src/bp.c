#include "bp.h"
#include <stdlib.h>
#include <stdio.h>

#define log2(i) (__builtin_ffs( (i) ) - 1)

int32_t btb_pc(uint32_t x) 
{
    return x>>2 && 0x3FF; // we want bits 11-2, so shift right and then grab the next 10
}

uint8_t gshare(uint8_t x, uint8_t y)
{
    return ((0xFF) & x) ^ ((y >> 2) & 0xFF);
}

bp_t *bp_new(int ghr_bits, int btb_size)
{
    bp_t *b = calloc(sizeof(bp_t), 1);
    b->ghr_bits = ghr_bits;
    b->btb_size = btb_size;
    b->btb_bits = log2(btb_size);

    b->ghr = 0;
    b->pht = calloc(1 << ghr_bits, sizeof(uint8_t));
    b->btb_tag = calloc(btb_size, sizeof(uint32_t));
    b->btb_dest = calloc(btb_size, sizeof(uint32_t));
    b->btb_valid = calloc(btb_size, sizeof(uint8_t));
    b->btb_cond = calloc(btb_size, sizeof(uint8_t));

    return b;
}

void bp_destroy(bp_t *b)
{
    if (b) {
        free(b->pht);
        free(b->btb_tag);
        free(b->btb_dest);
        free(b->btb_valid);
        free(b->btb_cond);
        free(b);
    }
}

void bp_predict(bp_t *b, uint32_t pc, uint8_t *branch, uint8_t *cond, uint8_t *taken, uint32_t *dest)
{
    *branch = 0;
    *cond = 0;
    *taken = 0;
    *dest = 0;

    // if the tag at pc doesn't equal the pc, the entry doesn't exist in the btb, our prediction is just pc + 4
    // if the valid tag = 0 (is false), then we predicted this was a branch but its really not
    if ((b->btb_tag[btb_pc(pc)] != pc) || !(b->btb_valid[btb_pc(pc)])) 
    {
        // don't need to touch branch, it isnt one
        // don't need to touch cond, 0 means unconditional, which this technically is
        // don't need to touch taken, this isn't a branch
        *dest = pc + 4;
    }
    //if the entry exits and is valid, we have an actual branch
    else if ((b->btb_tag[btb_pc(pc)] == pc) && (b->btb_valid[btb_pc(pc)]))
    {
        *branch = 1; //we have an actual branch
        *cond = b->btb_cond[btb_pc(pc)];

        //if the branch is unconditionally taken or the patten history table suggests its strongly or weakly taken, we are gonna take it
        if ((b->btb_cond[btb_pc(pc)] == 0) || (b->pht[gshare(b->ghr, pc)] > 1))
        {
            *taken = 1;
            *dest = b->btb_dest[btb_pc(pc)];
        } else //not taking the branch
        {
            *taken = 0;
            *dest = pc + 4;
        }
    }
    else *dest = pc + 4; // catch all
}

void bp_update(bp_t *b, uint32_t pc, uint8_t branch, uint8_t cond, uint8_t taken, uint32_t dest)
{
    if (!branch) {
        b->btb_valid[btb_pc(pc)] = 0;
    } else {
        if (cond)
        {
            if((taken) && (b->pht[gshare(b->ghr, pc)] != 3)) b->pht[gshare(b->ghr, pc)] += 1; //taken, if the pattern history is not a max of strongly taken, increase it
            else if ((!taken) && (b->pht[gshare(b->ghr, pc)] != 0)) b->pht[gshare(b->ghr, pc)] -= 1; //not taken, if the patten history is not a min of strongly not taken, decrease it
            
            b->ghr = (b->ghr << 1) | taken; //update the global history register
        }

        b->btb_valid[btb_pc(pc)] = 1;
        b->btb_tag[btb_pc(pc)] = pc;
        b->btb_cond[btb_pc(pc)] = cond;
        b->btb_dest[btb_pc(pc)] = dest;
    }
}
