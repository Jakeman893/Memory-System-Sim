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

    // Reveal bits used for bank indexing
    bank_idx = (lineaddr / (ROWBUF_SIZE / CACHE_LINESIZE));
    // Use modulo to find the bank index
    bank_idx %= DRAM_BANKS;

    // Reveal bits used for row indexing
    row = lineaddr / (ROWBUF_SIZE / CACHE_LINESIZE);
    // Use division to find row index (upper bits of addr)
    row /= DRAM_BANKS;

    // Check if row buffer is empty or doesn't contain the row we seek
    Rowbuf_Entry* bank_buf = &dram->perbank_row_buf[bank_idx];
    
    if(!bank_buf->valid || bank_buf->rowid != row || is_dram_write) {
        // precharge (close row and prepare bank for access)
        delay += DRAM_T_PRE;
        // activate (opens row and places into row buffer)
        delay += DRAM_T_ACT;
    }
    
    // Update rowid and validity of row
    bank_buf->rowid = row;
    bank_buf->valid = TRUE;

    return delay;
}


