#define _GNU_SOURCE
#include <sched.h>
#include <errno.h>
#include "rocc.h"
#include "custom.h"
#include <unistd.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#include "util.h"
#include "debug.h"
#include "dedupdef.h"
#include "encoder.h"
#include "decoder.h"
#include "config.h"
#include "queue.h"

#ifdef ENABLE_DMALLOC
#include <dmalloc.h>
#endif //ENABLE_DMALLOC

#ifdef ENABLE_PTHREADS
#include <pthread.h>
#endif //ENABLE_PTHREADS

#ifdef ENABLE_PARSEC_HOOKS
#include <hooks.h>
#endif //ENABLE_PARSEC_HOOKS

// extern void gcStartup (void);
// extern void gcCleanup (void);

config_t * conf;



/*--------------------------------------------------------------------------*/
static void
usage(char* prog)
{
  printf("usage: %s [-cusfvh] [-w gzip/bzip2/none] [-i file] [-o file] [-t number_of_threads]\n",prog);
  printf("-c \t\t\tcompress\n");
  printf("-u \t\t\tuncompress\n");
  printf("-p \t\t\tpreloading (for benchmarking purposes)\n");
  printf("-w \t\t\tcompression type: gzip/bzip2/none\n");
  printf("-i file\t\t\tthe input file\n");
  printf("-o file\t\t\tthe output file\n");
  printf("-t \t\t\tnumber of threads per stage \n");
  printf("-v \t\t\tverbose output\n");
  printf("-h \t\t\thelp\n");
}
/*--------------------------------------------------------------------------*/

pthread_t tidp;
pthread_t tidp1;
volatile int ret[2]  = {0};
volatile int initial[2] = {0};
volatile unsigned long num1 = 0x12L;
volatile unsigned long HartID1 = 0x1023L;

void myStartupFun (void) __attribute__ ((constructor)); 
  
/* Apply the destructor attribute to myCleanupFun() so that it 
   is executed after main() */
void myCleanupFun (void) __attribute__ ((destructor)); 

static double __attribute__((noinline)) cal(double x){
  double y = x * x;
  double m = y * y;
  return m / 3;
}

static inline void  check(){
  while(check_mode() != 0b0011){//0x1400450b
    if(check_sreceving() == 0b1010){//0x1000450b
      ROCC_INSTRUCTION_S(0, 1, 2);//slave core start receving rf_data 0x405a00b
      R_INSTRUCTION_JLR(3, 0x01);
    }
  }
}

static void __attribute__((noinline)) *mypthreadFunction1(void *pvoid)
{
	// int i = 0;
  // int cpus = 0;
  // cpus = sysconf(_SC_NPROCESSORS_ONLN);
  // printf("cpus: %d\n", cpus);	
	// while(1)
	// {
	// 	printf("thread function 1, i: %d\n", i++);
	// 	sleep(1);
	// }

  //initialization
  change(num1, HartID1);
  ROCC_INSTRUCTION_S(0, 5000, 12);
  printf("Initialization complete from core 3!\n");
  initial[1] = 1;
  while (initial[0] != 1 || initial[1] != 1 || initial[2] != 1){}

  start_tracing(); 
  double a1;
  double i1 = 0.15;
  double b1 = 0.24;
  a1 = i1 * b1;
  accum_write(0, i1);
  ROCC_INSTRUCTION(0, 9);//slave core recode context and pc //0x1200000b
  //check();
  while(check_mode() != 0b0011){//0x1400450b
    if(check_sreceving() == 0b1010){//0x1000450b
      ROCC_INSTRUCTION_S(0, 1, 2);//slave core start receving rf_data 0x405a00b
      R_INSTRUCTION_JLR(3, 0x01);
    }
  }
  
  printf("check complete from slavecore 3!\n");
  end_tracing();
  return NULL;
}

static void __attribute__((noinline)) *mypthreadFunction2(void *pvoid)
{
	// int i = 0;
  // int cpus = 0;
  // cpus = sysconf(_SC_NPROCESSORS_ONLN);
  // printf("cpus: %d\n", cpus);	
	// while(1)
	// {
	// 	printf("thread function 1, i: %d\n", i++);
	// 	sleep(1);
	// }

  //initialization
  change(num1, HartID1);
  ROCC_INSTRUCTION_S(0, 5000, 12);
  printf("Initialization complete from core 2!\n");
  initial[2] = 1;
  while (initial[0] != 1 || initial[1] != 1 || initial[2] != 1){}

  start_tracing(); 
  double a1;
  double i1 = 0.15;
  double b1 = 0.24;
  a1 = i1 * b1;
  accum_write(0, i1);
  ROCC_INSTRUCTION(0, 9);//slave core recode context and pc //0x1200000b
  //check();
  while(check_mode() != 0b0011){//0x1400450b
    if(check_sreceving() == 0b1010){//0x1000450b
      ROCC_INSTRUCTION_S(0, 1, 2);//slave core start receving rf_data 0x405a00b
      R_INSTRUCTION_JLR(3, 0x01);
    }
  }
  
  printf("check complete from slavecore 2!\n");
  end_tracing();
  return NULL;
}

void myStartupFun (void) 
{ 
    printf ("startup code before main()\n"); 
    //pthread
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(0, &set);
    /* 设置主线程亲和性为cpu0，这样的话，默认新线程亲和性也是cpu0 */
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
    /* 移除CPU集合中的cpu0，此时集合中没有任何CPU */
    CPU_CLR(0, &set);
    /* 增加cpu1，此时集合中只有cpu3 */
    CPU_SET(3, &set);
    /* 设置th1的亲和性为cpu3 */
    pthread_setaffinity_np(tidp, sizeof(cpu_set_t), &set);
    int result = pthread_getaffinity_np(tidp, sizeof(cpu_set_t), &set);
    if(!result){
      printf("pthread_setaffinity_np success! The tidp thread is running on CPU 3\n");
    }
    else{
      perror("pthread_setaffinity_np");
      return -1;
    }

    pthread_create(&tidp1, NULL, mypthreadFunction2, NULL);
    /* 移除CPU集合中的cpu0，此时集合中没有任何CPU */
    CPU_CLR(3, &set);
    /* 增加cpu1，此时集合中只有cpu3 */
    CPU_SET(2, &set);
    /* 设置th1的亲和性为cpu3 */
    pthread_setaffinity_np(tidp1, sizeof(cpu_set_t), &set);
    int result1 = pthread_getaffinity_np(tidp1, sizeof(cpu_set_t), &set);
    if(!result1){
      printf("pthread_setaffinity_np success! The tidp1 thread is running on CPU 2\n");
    }
    else{
      perror("pthread_setaffinity_np");
      return -1;
    }

    //initialization
    change(num1, HartID1);
    ROCC_INSTRUCTION_S(0, 5000, 12);
    printf("Initialization complete from core 1!\n");
    initial[0] = 1;
    while (initial[0] != 1 || initial[1] != 1 || initial[2] != 1){}
    start_tracing();
    start_tracing();
    start_tracing();
    start_tracing();
    start_tracing();
    ROCC_INSTRUCTION(0, 3); //Master core call for starting check  0x0600000b

} 

/* implementation of myCleanupFun */
void myCleanupFun (void) 
{ 
    ROCC_INSTRUCTION(0, 11); //Mcore call for ending check 0x1600000b
    end_tracing();
    pthread_join(tidp, NULL);
    pthread_join(tidp1, NULL);
    printf ("cleanup code after main()\n"); 
} 


int main(int argc, char** argv) {
  // gcStartup ();
#ifdef PARSEC_VERSION
#define __PARSEC_STRING(x) #x
#define __PARSEC_XSTRING(x) __PARSEC_STRING(x)
  printf("PARSEC Benchmark Suite Version "__PARSEC_XSTRING(PARSEC_VERSION)"\n");
#else
  printf("PARSEC Benchmark Suite\n");
#endif //PARSEC_VERSION
#ifdef ENABLE_PARSEC_HOOKS
        __parsec_bench_begin(__parsec_dedup);
#endif //ENABLE_PARSEC_HOOKS

  int32 compress = TRUE;

  //We force the sha1 sum to be integer-aligned, check that the length of a sha1 sum is a multiple of unsigned int
  assert(SHA1_LEN % sizeof(unsigned int) == 0);

  conf = (config_t *) malloc(sizeof(config_t));
  if (conf == NULL) {
    EXIT_TRACE("Memory allocation failed\n");
  }

  strcpy(conf->outfile, "");
  conf->compress_type = COMPRESS_GZIP;
  conf->preloading = 0;
  conf->nthreads = 1;
  conf->verbose = 0;

  //parse the args
  int ch;
  opterr = 0;
  optind = 1;
  while (-1 != (ch = getopt(argc, argv, "cupvo:i:w:t:h"))) {
    switch (ch) {
    case 'c':
      compress = TRUE;
      strcpy(conf->infile, "test.txt");
      strcpy(conf->outfile, "out.ddp");
      break;
    case 'u':
      compress = FALSE;
      strcpy(conf->infile, "out.ddp");
      strcpy(conf->outfile, "new.txt");
      break;
    case 'w':
      if (strcmp(optarg, "gzip") == 0)
        conf->compress_type = COMPRESS_GZIP;
      else if (strcmp(optarg, "bzip2") == 0) 
        conf->compress_type = COMPRESS_BZIP2;
      else if (strcmp(optarg, "none") == 0)
        conf->compress_type = COMPRESS_NONE;
      else {
        fprintf(stdout, "Unknown compression type `%s'.\n", optarg);
        usage(argv[0]);
        return -1;
      }
      break;
    case 'o':
      strcpy(conf->outfile, optarg);
      break;
    case 'i':
      strcpy(conf->infile, optarg);
      break;
    case 'h':
      usage(argv[0]);
      return -1;
    case 'p':
      conf->preloading = TRUE;
      break;
    case 't':
      conf->nthreads = atoi(optarg);
      break;
    case 'v':
      conf->verbose = TRUE;
      break;
    case '?':
      fprintf(stdout, "Unknown option `-%c'.\n", optopt);
      usage(argv[0]);
      return -1;
    }
  }

#ifndef ENABLE_BZIP2_COMPRESSION
 if (conf->compress_type == COMPRESS_BZIP2){
    printf("Bzip2 compression not supported\n");
    exit(1);
  }
#endif

#ifndef ENABLE_GZIP_COMPRESSION
 if (conf->compress_type == COMPRESS_GZIP){
    printf("Gzip compression not supported\n");
    exit(1);
  }
#endif

#ifndef ENABLE_STATISTICS
 if (conf->verbose){
    printf("Statistics collection not supported\n");
    exit(1);
  }
#endif

#ifndef ENABLE_PTHREADS
 if (conf->nthreads != 1){
    printf("Number of threads must be 1 (serial version)\n");
    exit(1);
  }
#endif

  if (compress) {
    Encode(conf);
  } else {
    Decode(conf);
  }

  free(conf);

#ifdef ENABLE_PARSEC_HOOKS
  __parsec_bench_end();
#endif
  // gcCleanup ();
  return 0;
}

