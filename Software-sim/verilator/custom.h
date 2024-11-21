#include <stdint.h>
#include "rocc.h"


static inline void accum_write(int idx, unsigned long data)
{
	ROCC_INSTRUCTION_SS(0, data, idx, 0);
}

static inline void custom_jalr()
{
	R_INSTRUCTION_JLR(3, 0x01);
}


static inline void transmission(int valid)
{
  ROCC_INSTRUCTION_S(0, valid, 1);
}

static inline void receive(int ready)
{
  ROCC_INSTRUCTION_S(0, ready, 2);
}

static inline void call_for_check()
{
  ROCC_INSTRUCTION(0, 3);
}


static inline void configure(int num, unsigned long HartID)
{
  uint64_t rd;
	ROCC_INSTRUCTION_DSS(0, rd, num, HartID, 4);
}

static inline void call_for_apply()
{
  ROCC_INSTRUCTION(0, 5);
}

static inline uint64_t check_cpdone()
{
  uint64_t cpdone;
  ROCC_INSTRUCTION_D(0, cpdone, 6);
  return cpdone;
}

static inline uint64_t check_mrunning()
{
  uint64_t running;
  ROCC_INSTRUCTION_D(0, running, 7);
  return running;
}

static inline uint64_t check_sreceving()
{
  uint64_t yes;
  ROCC_INSTRUCTION_D(0, yes, 8);
  return yes;
}

static inline uint64_t check_mode()
{
  uint64_t checkmode;
  ROCC_INSTRUCTION_D(0, checkmode, 10);
  return checkmode;
}

static inline void start_tracing(){
  ROCC_INSTRUCTION(0, 13);
}

static inline void end_tracing(){
  ROCC_INSTRUCTION(0, 14);
}