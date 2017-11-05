#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "dram.h"

#define ROWBUF_SIZE         1024
#define DRAM_BANKS          16

//---- Latency for Part B ------

#define DRAM_LATENCY_FIXED  100

//---- Latencies for Part C ------

#define DRAM_T_ACT         45
#define DRAM_T_CAS         45
#define DRAM_T_PRE         45
#define DRAM_T_BUS         10


extern MODE   SIM_MODE;
extern uns64  CACHE_LINESIZE;


///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

DRAM   *dram_new(){
  DRAM *dram = (DRAM *) calloc (1, sizeof (DRAM));
  assert(DRAM_BANKS <= MAX_DRAM_BANKS);
  return dram;
}

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

void    dram_print_stats(DRAM *dram){
  double rddelay_avg=0;
  double wrdelay_avg=0;
  char header[256];
  sprintf(header, "DRAM");
  
  if(dram->stat_read_access){
    rddelay_avg=(double)(dram->stat_read_delay)/(double)(dram->stat_read_access);
  }

  if(dram->stat_write_access){
    wrdelay_avg=(double)(dram->stat_write_delay)/(double)(dram->stat_write_access);
  }

  printf("\n%s_READ_ACCESS\t\t : %10llu", header, dram->stat_read_access);
  printf("\n%s_WRITE_ACCESS\t\t : %10llu", header, dram->stat_write_access);
  printf("\n%s_READ_DELAY_AVG\t\t : %10.3f", header, rddelay_avg);
  printf("\n%s_WRITE_DELAY_AVG\t\t : %10.3f", header, wrdelay_avg);


}

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

uns64   dram_access(DRAM *dram,Addr lineaddr, Flag is_dram_write){
  uns64 delay=DRAM_LATENCY_FIXED;

  if(SIM_MODE!=SIM_MODE_B){
    delay = dram_access_sim_rowbuf(dram, lineaddr, is_dram_write);
  }

  // Update stats
  if(is_dram_write){
    dram->stat_write_access++;
    dram->stat_write_delay+=delay;
  }else{
    dram->stat_read_access++;
    dram->stat_read_delay+=delay;
  }
  
  return delay;
}

///////////////////////////////////////////////////////////////////
// ------------ DO NOT MODIFY THE CODE ABOVE THIS LINE -----------
// Modify the function below only if you are attempting Part C 
///////////////////////////////////////////////////////////////////

uns64   dram_access_sim_rowbuf(DRAM *dram,Addr lineaddr, Flag is_dram_write){
    uns64 delay = DRAM_T_CAS + DRAM_T_BUS;
    uns64 bank_idx = 0;
    uns64 row = 0;
    // Assume a mapping with consecutive lines in the same row
    // Assume a mapping with consecutive rowbufs in consecutive rows
    // You need to write this fuction to track open rows 
    // You will need to compute delay based on row hit/miss/empty

    // With mapping of consecutive rows being in same bank, we have addressing
    // BankID (n bits) | Row Num (n bits) | Row Column Offset (n bits) | Cache Line Offset (n bits)


    // Reveal bits used for bank indexing
    bank_idx = (lineaddr / (ROWBUF_SIZE / CACHE_LINESIZE));
    // Use modulo to find the bank index
    bank_idx = bank_idx % DRAM_BANKS;

    // Reveal bits used for row indexing
    row = lineaddr / (ROWBUF_SIZE / CACHE_LINESIZE);
    // Use modulo to find row index
    row /= DRAM_BANKS;

    // // Remove byte_in_line offset
    // lineaddr /= CACHE_LINESIZE;

    // // Remove line_in_row offset
    // uns64 num_line_in_row = ROWBUF_SIZE / CACHE_LINESIZE;
    // lineaddr /= num_line_in_row;

    // // Find row in bank for the address (find value for row)
    // row = lineaddr % ROWBUF_SIZE;
    // // Remove lower bits from address to index bank
    // lineaddr /= ROWBUF_SIZE;

    // // Index lineaddr into relevant bank (find value for bank_idx)
    // // Index is the lower ln(DRAM_BANKS) bits 
    // bank_idx = lineaddr % DRAM_BANKS;

    // Check if row buffer is empty or doesn't contain the row we seek
    Rowbuf_Entry* bank_buf = &dram->perbank_row_buf[bank_idx];
    // If is write
    if (is_dram_write) {
        bank_buf->valid = TRUE;
        bank_buf->rowid = row;
        return delay + DRAM_T_PRE + DRAM_T_ACT;
    }

    if(!bank_buf->valid) {
        // precharge (close row and prepare bank for access)
        delay += DRAM_T_PRE;
        // activate (opens row and places into row buffer)
        delay += DRAM_T_ACT;
        // Update rowid and validity of row
        bank_buf->rowid = row;
        bank_buf->valid = TRUE;
    } else if (bank_buf->rowid != row) {
        delay += DRAM_T_PRE + DRAM_T_ACT;
        bank_buf->rowid = row;
        bank_buf->valid = TRUE;
    } else if (bank_buf->rowid == row) {
        bank_buf->rowid = row;
        bank_buf->valid = TRUE;
    }

    return delay;
}


