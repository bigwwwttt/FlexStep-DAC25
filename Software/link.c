#define _GNU_SOURCE
#include <sched.h>
#include <errno.h>
#include "rocc.h"
#include "custom.h"
#include <unistd.h>
#include <pthread.h>




pthread_t tidp;
volatile int initial[2] = {0};
volatile unsigned long num1 = 0x21L; // Two main cores and each has one checker cores.
volatile unsigned long HartID1 = 0x0123L; // Main core 0 with its checker core 3  and main core 1 with its checker core 2.

/* Apply the constructor attribute to myStartupFun() so that it 
   is executed before main() */
void myStartupFun (void) __attribute__ ((constructor)); 
  
/* Apply the destructor attribute to myCleanupFun() so that it 
   is executed after main() */
void myCleanupFun (void) __attribute__ ((destructor)); 

static double __attribute__((noinline)) cal(double x){
  double y = x * x;
  double m = y * y;
  return m / 3;
}

static inline void check(){
  while(check_mode() != 0b0011){//inst: 0x1400450b; return the check mode.
    if(check_sreceving() == 0b1010){//inst: 0x1000450b; waiting for SCP. 
      ROCC_INSTRUCTION_S(0, 1, 2);//inst: 0x0405a00b; checker core starts receving rf_data and applies them.
      R_INSTRUCTION_JLR(3, 0x01);
    }
  }
}

static void __attribute__((noinline)) *mypthreadFunction1(void *pvoid)
{
  //initialization
  configure(num1, HartID1);
  ROCC_INSTRUCTION_S(0, 5000, 12);
  printf("Initialization complete from core 3!\n");
  initial[1] = 1;
  while (initial[0] != 1 || initial[1] != 1){}

   
  double a1;
  double i1 = 0.15;
  double b1 = 0.24;
  a1 = i1 * b1;
  accum_write(0, i1);
  start_tracing();
  ROCC_INSTRUCTION(0, 9);//checker core recode context and pc //0x1200000b
  check();

  
  printf("check complete from checkercore 3!\n");
  end_tracing();
  end_tracing();
  end_tracing();
  return NULL;
}

/* implementation of myStartupFun */
void myStartupFun (void) 
{ 
    printf ("startup code before main()\n"); 
    //pthread
    
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(0, &set);
    /* Set the affinity of the main thread to main core 0 */
    sched_setaffinity(getpid(), sizeof(cpu_set_t), &set);
    int retu = sched_getaffinity(getpid(), sizeof(cpu_set_t), &set);
    if(!retu){
      printf("sched_setaffinity success! The main thread is running on CPU 0\n");
    }
    else{
      perror("sched_setaffinity");
      return -1;
    }

    pthread_create(&tidp, NULL, mypthreadFunction1, NULL);
    /* delete the core 0 from set */
    CPU_CLR(0, &set);
    /* add the checker core 3 */
    CPU_SET(3, &set);
    /* Set the affinity of the thread1 to checker core 3 */
    pthread_setaffinity_np(tidp, sizeof(cpu_set_t), &set);
    int result = pthread_getaffinity_np(tidp, sizeof(cpu_set_t), &set);
    if(!result){
      printf("pthread_setaffinity_np success! The tidp thread is running on CPU 3\n");
    }
    else{
      perror("pthread_setaffinity_np");
      return -1;
    }

    //initialization
    configure(num1, HartID1);
    ROCC_INSTRUCTION_S(0, 5000, 12);
    initial[0] = 1;
    printf("Initialization complete from core 0!\n");
    while (initial[0] != 1 || initial[1] != 1){}
    start_tracing();
    start_tracing();
    start_tracing();
    start_tracing();
    start_tracing();
    ROCC_INSTRUCTION(0, 3); //Main core call for starting check  0x0600000b

} 

/* implementation of myCleanupFun */
void myCleanupFun (void) 
{
    
    ROCC_INSTRUCTION(0, 11); //Mcore call for ending check 0x1600000b
    end_tracing();
    pthread_join(tidp, NULL);
    printf ("cleanup code after main()\n"); 
} 