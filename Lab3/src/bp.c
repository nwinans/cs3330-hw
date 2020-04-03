/*
TODO:
Implement the functions of your branch predictor here.
*/

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

    /* TODO: Look up in BTB. Index into the branch target buffer from bits [11:2] of the PC.
       If the entry resides in the BTB, then extract the relevant fields from the BTB.*/ 
    
    /* TODO: Look up in directional predictor */
     
}

void bp_update(bp_t *b, uint32_t pc, uint8_t branch, uint8_t cond, uint8_t taken, uint32_t dest)
{
    /* TODO: Update BTB for the two following scenarios: */
   
    /* TODO: i).  We were incorrectly predicted as a branch. In this case you should clear the BTB entry by setting the valid bit to 0. */
    
  
    /* TODO: ii). We are a branch. Make sure BTB is up-to-date by updating the relevant destination, condition, valid and tag bit to its appropriate value. */
       

    /* TODO: Update gshare directional predictor */
   

    /* TODO: update global history register */

    if (cond)
    {
        if((taken) && (b->pht[gshare(b->ghr, pc)] != 3)) b->pht[gshare(b->ghr, pc)] += 1; //taken, if the pattern history is not a max of strongly taken, increase it
        else if ((!taken) && (b->pht[gshare(b->ghr, pc)] != 0)) b->pht[gshare(b->ghr, pc)] -= 1; //not taken, if the patten history is not a min of strongly not taken, decrease it
        
        b->ghr = (b->ghr << 1) | taken; //update the ghr
    }

    b->btb_valid[btb_pc(pc)] = 1;
    b->btb_tag[btb_pc(pc)] = pc;
    b->btb_cond[btb_pc(pc)] = (cond) ? 1 : 0;
    b->btb_dest[btb_pc(pc)] = dest;
   
}
