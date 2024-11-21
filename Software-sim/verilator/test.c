#include"rocc.h"
#include <riscv-pk/encoding.h>
#include <stdio.h>
#include "marchid.h"
#include "spin_lock.h"
#include "custom.h"
#include "test_template.h"
static size_t n_cores = 8;
int uart_lock;
static void __attribute__((noinline)) barrier()
{
  static volatile int sense;
  static volatile int count;
  static __thread int threadsense;

  __sync_synchronize();

  threadsense = !threadsense;
  if (__sync_fetch_and_add(&count, 1) == n_cores-1)
  {
    count = 0;
    sense = threadsense;
  }
  else while(sense != threadsense)
    ;

  __sync_synchronize();
}

static double __attribute__((noinline)) cal(double x){
  double y = x * x;
  double m = y * y;
  return m / 3;
}



int uart_lock;

volatile int ret[8]  = {0};
volatile int ret1[8]  = {0};

volatile int initial[8] = {0};
volatile int initial1[8] = {0};

volatile unsigned long num1 = 0x41L;
volatile unsigned long HartID1 = 0x67450123L;

volatile unsigned long num2 = 0x41L;
volatile unsigned long HartID2 = 0x01234567L;


static inline void  check(){
  while(check_mode() != 0b0011){//0x1400450b
    if(check_sreceving() == 0b1010){//0x1000450b
      ROCC_INSTRUCTION_S(0, 1, 2);//slave core start receving rf_data 0x405a00b
      R_INSTRUCTION_JLR(3, 0x01);
    }
  }
}

void idle()
{
  while(1){};
}


int __main(void){
  uint64_t Hart_id = 0;
  asm volatile ("csrr %0, mhartid"  : "=r"(Hart_id));

  switch (Hart_id)
  {
  case 0x01:
    // ROCC_INSTRUCTION_S(0, 500, 12);
    // lock[1] = 1;
    // while(lock[0] != 1 || lock[1] != 1 || lock[2] != 1 || lock[3] != 1 || lock[4] != 1 || lock[5] != 1 || lock[6] != 1 || lock[7] != 1){}
    // volatile double y, z;
    // transmission(1); //0x0205A00B
    // transmission(1);
    // transmission(1);
    // transmission(1);
    // volatile double x = 1.23;
    // y = cal(x);
    // z = cal(y); 
    // //while(check_mrunning() != 0b1001){}
    // ROCC_INSTRUCTION(0, 3); //Master core call for starting check  0x0600000b
    // y = cal(z);
    // z = cal(y); 
    // accum_write(0, 1);//0x00C5B00B
    // accum_write(0, 2);
    // accum_write(0, 3);
    // accum_write(0, 4);
    // accum_write(0, 5);
    // accum_write(0, 6);
    // accum_write(0, 7);
    // accum_write(0, 8);
    // accum_write(0, 9);
    // accum_write(0, 10);

    // uint64_t CSR = 0;
    // /* Testing CSR Registers */
    // asm volatile ("csrr %0, cycle"  : "=r"(CSR));
    // asm volatile ("csrr %0, instret"  : "=r"(CSR));
  
    // volatile double m = 1.13;
    // volatile int j = 1;
    // for(int i = 0; i < 100; i++){
    //     m = cal(i * 2);
    //     j = j + i;
    // }
    // __asm__ volatile(
    //                  ".LR_SC1:"
    //                  "li       t0,  0x797;"
    //                  "li       t1,  0xa;"
    //                  "li       a5,  0x800002e4;"
    //                  "mv       a5,  a5;"
    //                  "li       a4,  1;"
    //                  "lr.w     a0,  (a5);"
    //                  "sc.w     a3,  a4,  (a5);"
    // );
  
    // __asm__ volatile(
    //                  ".amo_add1:"
    //                  "li       a7,  0x800002e0;"
    //                  "mv       a7,  a7;"
    //                  "li       a6,  1;"
    //                  "amoadd.w a0,  a6,  (a7);"
    // );
  
    // __asm__ volatile(
    //                  ".amo_sub1:"
    //                  "li       a7,  0x800002e0;"
    //                  "mv       a7,  a7;"
    //                  "li       a6,  -1;"
    //                  "amoadd.w a0,  a6,  (a7);"
    // );
    // __asm__ volatile(
    //                  ".amo_or1:"
    //                  "li       a7,  0x800002e0;"
    //                  "mv       a7,  a7;"
    //                  "li       a6,  1;"
    //                  "amoor.w  a0,  a6,  (a7);"
    // );
  
    // //int32_t a = CONCAT_3(_,atomic_fetch_add_explicit, memory_order_relaxed);
  
    // ROCC_INSTRUCTION(0, 11); //Mcore call for ending check 0x1600000b
    // lock_acquire(&uart_lock);
    // printf("helloworld from core 1\n");
    // lock_release(&uart_lock);
  
    // for(int i = 0; i < 10; i++){
    //     j = j + i;
    // }
  
    // lock_acquire(&uart_lock);
    // printf("helloworld again from core 1\n");
    // lock_release(&uart_lock);
    // ret[1] = 1;
    //initialization
    configure(num1, HartID1);
    ROCC_INSTRUCTION_S(0, 500, 12);
    initial[1] = 1;
    while(initial[0] != 1 || initial[1] != 1 || initial[2] != 1 || initial[3] != 1 || initial[4] != 1 || initial[5] != 1 || initial[6] != 1 || initial[7] != 1){}


    double a1;
    double i1 = 0.15;
    double b1 = 0.24;
    a1 = i1 * b1;
    accum_write(0, i1);
    ROCC_INSTRUCTION(0, 9);//slave core recode context and pc //0x1200000b

    while(check_mode() != 0b0011){//0x1400450b
      if(check_sreceving() == 0b1010){//0x1000450b
        ROCC_INSTRUCTION_S(0, 1, 2);//slave core start receving rf_data 0x405a00b
        R_INSTRUCTION_JLR(3, 0x01);
      }
    }
    //check();
    
    for(i1 = 0; i1 < 10; i1++){
      b1 = b1 * 2;
    }

    lock_acquire(&uart_lock);
    printf("helloworld from slavecore 1\n");
    lock_release(&uart_lock);
    ret[1] = 1;
  break;

  case 0x02:
    //initialization
    configure(num1, HartID1);
    ROCC_INSTRUCTION_S(0, 500, 12);
    initial[2] = 1;
    while(initial[0] != 1 || initial[1] != 1 || initial[2] != 1 || initial[3] != 1 || initial[4] != 1 || initial[5] != 1 || initial[6] != 1 || initial[7] != 1){}
    
    double a;
    double i = 0.15;
    double b = 0.24;
    a = i * b;
    accum_write(0, i);
    ROCC_INSTRUCTION(0, 9);//slave core recode context and pc //0x1200000b

    while(check_mode() != 0b0011){//0x1400450b
      if(check_sreceving() == 0b1010){//0x1000450b
        ROCC_INSTRUCTION_S(0, 1, 2);//slave core start receving rf_data 0x405a00b
        R_INSTRUCTION_JLR(3, 0x01);
      }
    }
    
    for(i = 0; i < 10; i++){
      b = b * 2;
    }

    lock_acquire(&uart_lock);
    printf("helloworld from slavecore 2\n");
    lock_release(&uart_lock);
    ret[2] = 1;
  break;

  case 0x03:
    //initialization
    configure(num1, HartID1);
    ROCC_INSTRUCTION_S(0, 500, 12);
    initial[3] = 1;
    while(initial[0] != 1 || initial[1] != 1 || initial[2] != 1 || initial[3] != 1 || initial[4] != 1 || initial[5] != 1 || initial[6] != 1 || initial[7] != 1){}
    
    double o;
    double p = 0.15;
    double q = 0.24;
    o = p * q;
    accum_write(0, p);
    ROCC_INSTRUCTION(0, 9);//slave core recode context and pc //0x1200000b

    while(check_mode() != 0b0011){//0x1400450b
      if(check_sreceving() == 0b1010){//0x1000450b
        ROCC_INSTRUCTION_S(0, 1, 2);//slave core start receving rf_data 0x405a00b
        R_INSTRUCTION_JLR(3, 0x01);
      }
    }
    
    for(p = 0; p < 10; p++){
      q = q * 2;
    }

    lock_acquire(&uart_lock);
    printf("helloworld from slavecore 3\n");
    lock_release(&uart_lock);
    ret[3] = 1;
  break;

  case 0x04:
    //initialization
    configure(num1, HartID1);
    ROCC_INSTRUCTION_S(0, 500, 12);
    initial[4] = 1;
    while(initial[0] != 1 || initial[1] != 1 || initial[2] != 1 || initial[3] != 1 || initial[4] != 1 || initial[5] != 1 || initial[6] != 1 || initial[7] != 1){}
    
    volatile double f, g;
    transmission(1); //0x0205A00B
    transmission(1);
    transmission(1);
    transmission(1);
    volatile double h = 1.23;
    f = cal(h);
    g = cal(f); 
    //while(check_mrunning() != 0b1001){}
    ROCC_INSTRUCTION(0, 3); //Master core call for starting check  0x0600000b
    h = cal(g);
    g = cal(f); 
    accum_write(0, 1);//0x00C5B00B
    accum_write(0, 2);
    accum_write(0, 3);
    accum_write(0, 4);
    accum_write(0, 5);
    accum_write(0, 6);
    accum_write(0, 7);
    accum_write(0, 8);
    accum_write(0, 9);
    accum_write(0, 10);

    uint64_t CSR4 = 0;
    /* Testing CSR Registers */
    asm volatile ("csrr %0, cycle"  : "=r"(CSR4));
    asm volatile ("csrr %0, instret"  : "=r"(CSR4));
  
    volatile double m4 = 1.13;
    volatile int j4 = 1;
    for(int i = 0; i < 100; i++){
        m4 = cal(i * 2);
        j4 = j4 + i;
    }
    __asm__ volatile(
                     ".LR_SC4:"
                     "li       t0,  0x797;"
                     "li       t1,  0xa;"
                     "li       a5,  0x800002e4;"
                     "mv       a5,  a5;"
                     "li       a4,  1;"
                     "lr.w     a0,  (a5);"
                     "sc.w     a3,  a4,  (a5);"
    );
  
    __asm__ volatile(
                     ".amo_add4:"
                     "li       a7,  0x800002e0;"
                     "mv       a7,  a7;"
                     "li       a6,  1;"
                     "amoadd.w a0,  a6,  (a7);"
    );
  
    __asm__ volatile(
                     ".amo_sub4:"
                     "li       a7,  0x800002e0;"
                     "mv       a7,  a7;"
                     "li       a6,  -1;"
                     "amoadd.w a0,  a6,  (a7);"
    );
    __asm__ volatile(
                     ".amo_or4:"
                     "li       a7,  0x800002e0;"
                     "mv       a7,  a7;"
                     "li       a6,  1;"
                     "amoor.w  a0,  a6,  (a7);"
    );
  
    //int32_t a = CONCAT_3(_,atomic_fetch_add_explicit, memory_order_relaxed);
  
    ROCC_INSTRUCTION(0, 11); //Mcore call for ending check 0x1600000b
    lock_acquire(&uart_lock);
    printf("helloworld from core 4\n");
    lock_release(&uart_lock);
  
   for(int i = 0; i < 10; i++){
        j4 = j4 + i;
    }
  
    lock_acquire(&uart_lock);
    printf("helloworld again from core 4\n");
    lock_release(&uart_lock);
    ret[4] = 1;

    // double o4;
    // double p4 = 0.15;
    // double q4 = 0.24;
    // o4 = p4 * q4;
    // accum_write(0, i);
    // ROCC_INSTRUCTION(0, 9);//slave core recode context and pc //0x1200000b

    // while(check_mode() != 0b0011){//0x1400450b
    //   if(check_sreceving() == 0b1010){//0x1000450b
    //     ROCC_INSTRUCTION_S(0, 1, 2);//slave core start receving rf_data 0x405a00b
    //     R_INSTRUCTION_JLR(3, 0x01);
    //   }
    // }
    
    // for(p4 = 0; p4 < 10; p4++){
    //   q4 = q4 * 2;
    // }
  
    // lock_acquire(&uart_lock);
    // printf("helloworld from slavecore 4\n");
    // lock_release(&uart_lock);
    // ret[4] = 1;
  break;

  case 0x05:
    //initialization
    configure(num1, HartID1);
    ROCC_INSTRUCTION_S(0, 500, 12);
    initial[5] = 1;
    while(initial[0] != 1 || initial[1] != 1 || initial[2] != 1 || initial[3] != 1 || initial[4] != 1 || initial[5] != 1 || initial[6] != 1 || initial[7] != 1){}
    
    volatile double y5, z5;
    transmission(1); //0x0205A00B
    transmission(1);
    transmission(1);
    transmission(1);
    volatile double x5 = 1.23;
    y5 = cal(x5);
    z5 = cal(y5); 
    //while(check_mrunning() != 0b1001){}
    ROCC_INSTRUCTION(0, 3); //Master core call for starting check  0x0600000b
    y5 = cal(z5);
    z5 = cal(y5); 
    accum_write(0, 1);//0x00C5B00B
    accum_write(0, 2);
    accum_write(0, 3);
    accum_write(0, 4);
    accum_write(0, 5);
    accum_write(0, 6);
    accum_write(0, 7);
    accum_write(0, 8);
    accum_write(0, 9);
    accum_write(0, 10);

    uint64_t CSR5 = 0;
    /* Testing CSR Registers */
    asm volatile ("csrr %0, cycle"  : "=r"(CSR5));
    asm volatile ("csrr %0, instret"  : "=r"(CSR5));
  
    volatile double m5 = 1.13;
    volatile int j5 = 1;
    for(int i = 0; i < 100; i++){
        m5 = cal(i * 2);
        j5 = j5 + i;
    }
    __asm__ volatile(
                     ".LR_SC5:"
                     "li       t0,  0x797;"
                     "li       t1,  0xa;"
                     "li       a5,  0x800002e4;"
                     "mv       a5,  a5;"
                     "li       a4,  1;"
                     "lr.w     a0,  (a5);"
                     "sc.w     a3,  a4,  (a5);"
    );
  
    __asm__ volatile(
                     ".amo_add5:"
                     "li       a7,  0x800002e0;"
                     "mv       a7,  a7;"
                     "li       a6,  1;"
                     "amoadd.w a0,  a6,  (a7);"
    );
  
    __asm__ volatile(
                     ".amo_sub5:"
                     "li       a7,  0x800002e0;"
                     "mv       a7,  a7;"
                     "li       a6,  -1;"
                     "amoadd.w a0,  a6,  (a7);"
    );
    __asm__ volatile(
                     ".amo_or5:"
                     "li       a7,  0x800002e0;"
                     "mv       a7,  a7;"
                     "li       a6,  1;"
                     "amoor.w  a0,  a6,  (a7);"
    );
  
    //int32_t a = CONCAT_3(_,atomic_fetch_add_explicit, memory_order_relaxed);
  
    ROCC_INSTRUCTION(0, 11); //Mcore call for ending check 0x1600000b
    lock_acquire(&uart_lock);
    printf("helloworld from core 5\n");
    lock_release(&uart_lock);
  
    for(int i = 0; i < 10; i++){
        j5 = j5 + i;
    }
  
    lock_acquire(&uart_lock);
    printf("helloworld again from core 5\n");
    lock_release(&uart_lock);
    ret[5] = 1;

    // double o5;
    // double p5 = 0.15;
    // double q5 = 0.24;
    // o5 = p5 * q5;
    // accum_write(0, p5);
    // ROCC_INSTRUCTION(0, 9);//slave core recode context and pc //0x1200000b

    // while(check_mode() != 0b0011){//0x1400450b
    //   if(check_sreceving() == 0b1010){//0x1000450b
    //     ROCC_INSTRUCTION_S(0, 1, 2);//slave core start receving rf_data 0x405a00b
    //     R_INSTRUCTION_JLR(3, 0x01);
    //   }
    // }
    
    // for(p5 = 0; p5 < 10; p5++){
    //   q5 = q5 * 2;
    // }
  
    // lock_acquire(&uart_lock);
    // printf("helloworld from slavecore 5\n");
    // lock_release(&uart_lock);
    // ret[5] = 1;
  break;

  case 0x06:
    //initialization
    configure(num1, HartID1);
    ROCC_INSTRUCTION_S(0, 500, 12);
    initial[6] = 1;
    while(initial[0] != 1 || initial[1] != 1 || initial[2] != 1 || initial[3] != 1 || initial[4] != 1 || initial[5] != 1 || initial[6] != 1 || initial[7] != 1){}
    
    // double r;
    // double s = 0.15;
    // double t = 0.24;
    // r = s * t;
    // accum_write(0, i);
    // ROCC_INSTRUCTION(0, 9);//slave core recode context and pc //0x1200000b

    // while(check_mode() != 0b0011){//0x1400450b
    //   if(check_sreceving() == 0b1010){//0x1000450b
    //     ROCC_INSTRUCTION_S(0, 1, 2);//slave core start receving rf_data 0x405a00b
    //     R_INSTRUCTION_JLR(3, 0x01);
    //   }
    // }
    
    // for(i = 0; i < 10; i++){
    //   t = t * 2;
    // }

    // lock_acquire(&uart_lock);
    // printf("helloworld from core 6\n");
    // lock_release(&uart_lock);
    // ret[6] = 1;

    volatile double y6, z6;
    transmission(1); //0x0205A00B
    transmission(1);
    transmission(1);
    transmission(1);
    volatile double x6 = 1.23;
    y6 = cal(x6);
    z6 = cal(y6); 
    //while(check_mrunning() != 0b1001){}
    ROCC_INSTRUCTION(0, 3); //Master core call for starting check  0x0600000b
    y6 = cal(z6);
    z6 = cal(y6); 
    accum_write(0, 1);//0x00C5B00B
    accum_write(0, 2);
    accum_write(0, 3);
    accum_write(0, 4);
    accum_write(0, 5);
    accum_write(0, 6);
    accum_write(0, 7);
    accum_write(0, 8);
    accum_write(0, 9);
    accum_write(0, 10);

    uint64_t CSR6 = 0;
    /* Testing CSR Registers */
    asm volatile ("csrr %0, cycle"  : "=r"(CSR6));
    asm volatile ("csrr %0, instret"  : "=r"(CSR6));
  
    volatile double m6 = 1.13;
    volatile int j6 = 1;
    for(int i = 0; i < 100; i++){
        m6 = cal(i * 2);
        j6 = j6 + i;
    }
    __asm__ volatile(
                     ".LR_SC6:"
                     "li       t0,  0x797;"
                     "li       t1,  0xa;"
                     "li       a5,  0x800002e4;"
                     "mv       a5,  a5;"
                     "li       a4,  1;"
                     "lr.w     a0,  (a5);"
                     "sc.w     a3,  a4,  (a5);"
    );
  
    __asm__ volatile(
                     ".amo_add6:"
                     "li       a7,  0x800002e0;"
                     "mv       a7,  a7;"
                     "li       a6,  1;"
                     "amoadd.w a0,  a6,  (a7);"
    );
  
    __asm__ volatile(
                     ".amo_sub6:"
                     "li       a7,  0x800002e0;"
                     "mv       a7,  a7;"
                     "li       a6,  -1;"
                     "amoadd.w a0,  a6,  (a7);"
    );
    __asm__ volatile(
                     ".amo_or6:"
                     "li       a7,  0x800002e0;"
                     "mv       a7,  a7;"
                     "li       a6,  1;"
                     "amoor.w  a0,  a6,  (a7);"
    );
  
    //int32_t a = CONCAT_3(_,atomic_fetch_add_explicit, memory_order_relaxed);
  
    ROCC_INSTRUCTION(0, 11); //Mcore call for ending check 0x1600000b
    lock_acquire(&uart_lock);
    printf("helloworld from mastercore 6\n");
    lock_release(&uart_lock);
  
    for(int i = 0; i < 10; i++){
        j6 = j6 + i;
    }
  
    lock_acquire(&uart_lock);
    printf("helloworld again from mastercore 6\n");
    lock_release(&uart_lock);
    ret[6] = 1;
  break;

  case 0x07:
    //initialization
    configure(num1, HartID1);
    ROCC_INSTRUCTION_S(0, 500, 12);
    initial[7] = 1;
    while(initial[0] != 1 || initial[1] != 1 || initial[2] != 1 || initial[3] != 1 || initial[4] != 1 || initial[5] != 1 || initial[6] != 1 || initial[7] != 1){}


    // double u;
    // double v = 0.15;
    // double w = 0.24;
    // u = v * w;
    // accum_write(0, i);
    // ROCC_INSTRUCTION(0, 9);//slave core recode context and pc //0x1200000b

    // while(check_mode() != 0b0011){//0x1400450b
    //   if(check_sreceving() == 0b1010){//0x1000450b
    //     ROCC_INSTRUCTION_S(0, 1, 2);//slave core start receving rf_data 0x405a00b
    //     R_INSTRUCTION_JLR(3, 0x01);
    //   }
    // }
    
    // for(i = 0; i < 10; i++){
    //   w = w * 2;
    // }

    // lock_acquire(&uart_lock);
    // printf("helloworld from core 7\n");
    // lock_release(&uart_lock);
    // ret[7] = 1;

    volatile double y7, z7;
    transmission(1); //0x0205A00B
    transmission(1);
    transmission(1);
    transmission(1);
    volatile double x7 = 1.23;
    y7 = cal(x7);
    z7 = cal(y7); 
    //while(check_mrunning() != 0b1001){}
    ROCC_INSTRUCTION(0, 3); //Master core call for starting check  0x0600000b
    y7 = cal(z7);
    z7 = cal(y7); 
    accum_write(0, 1);//0x00C5B00B
    accum_write(0, 2);
    accum_write(0, 3);
    accum_write(0, 4);
    accum_write(0, 5);
    accum_write(0, 6);
    accum_write(0, 7);
    accum_write(0, 8);
    accum_write(0, 9);
    accum_write(0, 10);

    uint64_t CSR7 = 0;
    /* Testing CSR Registers */
    asm volatile ("csrr %0, cycle"  : "=r"(CSR7));
    asm volatile ("csrr %0, instret"  : "=r"(CSR7));
  
    volatile double m7 = 1.13;
    volatile int j7 = 1;
    for(int i = 0; i < 100; i++){
        m7 = cal(i * 2);
        j7 = j7 + i;
    }
    __asm__ volatile(
                     ".LR_SC7:"
                     "li       t0,  0x797;"
                     "li       t1,  0xa;"
                     "li       a5,  0x800002e4;"
                     "mv       a5,  a5;"
                     "li       a4,  1;"
                     "lr.w     a0,  (a5);"
                     "sc.w     a3,  a4,  (a5);"
    );
  
    __asm__ volatile(
                     ".amo_add7:"
                     "li       a7,  0x800002e0;"
                     "mv       a7,  a7;"
                     "li       a6,  1;"
                     "amoadd.w a0,  a6,  (a7);"
    );
  
    __asm__ volatile(
                     ".amo_sub7:"
                     "li       a7,  0x800002e0;"
                     "mv       a7,  a7;"
                     "li       a6,  -1;"
                     "amoadd.w a0,  a6,  (a7);"
    );
    __asm__ volatile(
                     ".amo_or7:"
                     "li       a7,  0x800002e0;"
                     "mv       a7,  a7;"
                     "li       a6,  1;"
                     "amoor.w  a0,  a6,  (a7);"
    );
  
    //int32_t a = CONCAT_3(_,atomic_fetch_add_explicit, memory_order_relaxed);
  
    ROCC_INSTRUCTION(0, 11); //Mcore call for ending check 0x1600000b
    lock_acquire(&uart_lock);
    printf("helloworld from mastercore 7\n");
    lock_release(&uart_lock);
  
    for(int i = 0; i < 10; i++){
        j7 = j7 + i;
    }
  
    lock_acquire(&uart_lock);
    printf("helloworld again from mastercore 7\n");
    lock_release(&uart_lock);
    ret[7] = 1;
  break;

  case 0x08:
    //initialization
    configure(num2, HartID2);
    ROCC_INSTRUCTION_S(0, 500, 12);
    initial1[0] = 1;
    while(initial1[0] != 1 || initial1[1] != 1 || initial1[2] != 1 || initial1[3] != 1 || initial1[4] != 1 || initial1[5] != 1 || initial1[6] != 1 || initial1[7] != 1){}


    // double u;
    // double v = 0.15;
    // double w = 0.24;
    // u = v * w;
    // accum_write(0, i);
    // ROCC_INSTRUCTION(0, 9);//slave core recode context and pc //0x1200000b

    // while(check_mode() != 0b0011){//0x1400450b
    //   if(check_sreceving() == 0b1010){//0x1000450b
    //     ROCC_INSTRUCTION_S(0, 1, 2);//slave core start receving rf_data 0x405a00b
    //     R_INSTRUCTION_JLR(3, 0x01);
    //   }
    // }
    
    // for(i = 0; i < 10; i++){
    //   w = w * 2;
    // }

    // lock_acquire(&uart_lock);
    // printf("helloworld from core 7\n");
    // lock_release(&uart_lock);
    // ret[7] = 1;

    volatile double y8, z8;
    transmission(1); //0x0205A00B
    transmission(1);
    transmission(1);
    transmission(1);
    volatile double x8 = 1.23;
    y8 = cal(x8);
    z8 = cal(y8); 
    //while(check_mrunning() != 0b1001){}
    ROCC_INSTRUCTION(0, 3); //Master core call for starting check  0x0600000b
    y8 = cal(z8);
    z8 = cal(y8); 
    accum_write(0, 1);//0x00C5B00B
    accum_write(0, 2);
    accum_write(0, 3);
    accum_write(0, 4);
    accum_write(0, 5);
    accum_write(0, 6);
    accum_write(0, 7);
    accum_write(0, 8);
    accum_write(0, 9);
    accum_write(0, 10);

    uint64_t CSR8 = 0;
    /* Testing CSR Registers */
    asm volatile ("csrr %0, cycle"  : "=r"(CSR8));
    asm volatile ("csrr %0, instret"  : "=r"(CSR8));
  
    volatile double m8 = 1.13;
    volatile int j8 = 1;
    for(int i = 0; i < 100; i++){
        m8 = cal(i * 2);
        j8 = j8 + i;
    }
    __asm__ volatile(
                     ".LR_SC8:"
                     "li       t0,  0x797;"
                     "li       t1,  0xa;"
                     "li       a5,  0x800002e4;"
                     "mv       a5,  a5;"
                     "li       a4,  1;"
                     "lr.w     a0,  (a5);"
                     "sc.w     a3,  a4,  (a5);"
    );
  
    __asm__ volatile(
                     ".amo_add8:"
                     "li       a7,  0x800002e0;"
                     "mv       a7,  a7;"
                     "li       a6,  1;"
                     "amoadd.w a0,  a6,  (a7);"
    );
  
    __asm__ volatile(
                     ".amo_sub8:"
                     "li       a7,  0x800002e0;"
                     "mv       a7,  a7;"
                     "li       a6,  -1;"
                     "amoadd.w a0,  a6,  (a7);"
    );
    __asm__ volatile(
                     ".amo_or8:"
                     "li       a7,  0x800002e0;"
                     "mv       a7,  a7;"
                     "li       a6,  1;"
                     "amoor.w  a0,  a6,  (a7);"
    );
  
    //int32_t a = CONCAT_3(_,atomic_fetch_add_explicit, memory_order_relaxed);
  
    ROCC_INSTRUCTION(0, 11); //Mcore call for ending check 0x1600000b
    lock_acquire(&uart_lock);
    printf("helloworld from mastercore 8\n");
    lock_release(&uart_lock);
  
    for(int i = 0; i < 10; i++){
        j8 = j8 + i;
    }
  
    lock_acquire(&uart_lock);
    printf("helloworld again from mastercore 8\n");
    lock_release(&uart_lock);
    ret1[0] = 1;
  break;

  case 0x09:
    //initialization
    configure(num2, HartID2);
    ROCC_INSTRUCTION_S(0, 500, 12);
    initial1[1] = 1;
    while(initial1[0] != 1 || initial1[1] != 1 || initial1[2] != 1 || initial1[3] != 1 || initial1[4] != 1 || initial1[5] != 1 || initial1[6] != 1 || initial1[7] != 1){}


    // double u;
    // double v = 0.15;
    // double w = 0.24;
    // u = v * w;
    // accum_write(0, i);
    // ROCC_INSTRUCTION(0, 9);//slave core recode context and pc //0x1200000b

    // while(check_mode() != 0b0011){//0x1400450b
    //   if(check_sreceving() == 0b1010){//0x1000450b
    //     ROCC_INSTRUCTION_S(0, 1, 2);//slave core start receving rf_data 0x405a00b
    //     R_INSTRUCTION_JLR(3, 0x01);
    //   }
    // }
    
    // for(i = 0; i < 10; i++){
    //   w = w * 2;
    // }

    // lock_acquire(&uart_lock);
    // printf("helloworld from core 7\n");
    // lock_release(&uart_lock);
    // ret[7] = 1;

    volatile double y9, z9;
    transmission(1); //0x0205A00B
    transmission(1);
    transmission(1);
    transmission(1);
    volatile double x9 = 1.23;
    y9 = cal(x9);
    z9 = cal(y9); 
    //while(check_mrunning() != 0b1001){}
    ROCC_INSTRUCTION(0, 3); //Master core call for starting check  0x0600000b
    y9 = cal(z9);
    z9 = cal(y9); 
    accum_write(0, 1);//0x00C5B00B
    accum_write(0, 2);
    accum_write(0, 3);
    accum_write(0, 4);
    accum_write(0, 5);
    accum_write(0, 6);
    accum_write(0, 7);
    accum_write(0, 8);
    accum_write(0, 9);
    accum_write(0, 10);

    uint64_t CSR9 = 0;
    /* Testing CSR Registers */
    asm volatile ("csrr %0, cycle"  : "=r"(CSR9));
    asm volatile ("csrr %0, instret"  : "=r"(CSR9));
  
    volatile double m9 = 1.13;
    volatile int j9 = 1;
    for(int i = 0; i < 100; i++){
        m9 = cal(i * 2);
        j9 = j9 + i;
    }
    __asm__ volatile(
                     ".LR_SC9:"
                     "li       t0,  0x797;"
                     "li       t1,  0xa;"
                     "li       a5,  0x800002e4;"
                     "mv       a5,  a5;"
                     "li       a4,  1;"
                     "lr.w     a0,  (a5);"
                     "sc.w     a3,  a4,  (a5);"
    );
  
    __asm__ volatile(
                     ".amo_add9:"
                     "li       a7,  0x800002e0;"
                     "mv       a7,  a7;"
                     "li       a6,  1;"
                     "amoadd.w a0,  a6,  (a7);"
    );
  
    __asm__ volatile(
                     ".amo_sub9:"
                     "li       a7,  0x800002e0;"
                     "mv       a7,  a7;"
                     "li       a6,  -1;"
                     "amoadd.w a0,  a6,  (a7);"
    );
    __asm__ volatile(
                     ".amo_or9:"
                     "li       a7,  0x800002e0;"
                     "mv       a7,  a7;"
                     "li       a6,  1;"
                     "amoor.w  a0,  a6,  (a7);"
    );
  
    //int32_t a = CONCAT_3(_,atomic_fetch_add_explicit, memory_order_relaxed);
  
    ROCC_INSTRUCTION(0, 11); //Mcore call for ending check 0x1600000b
    lock_acquire(&uart_lock);
    printf("helloworld from mastercore 9\n");
    lock_release(&uart_lock);
  
    for(int i = 0; i < 10; i++){
        j9 = j9 + i;
    }
  
    lock_acquire(&uart_lock);
    printf("helloworld again from mastercore 9\n");
    lock_release(&uart_lock);
    ret1[1] = 1;
  break;

  case 0x0a:
    //initialization
    configure(num2, HartID2);
    ROCC_INSTRUCTION_S(0, 500, 12);
    initial1[2] = 1;
    while(initial1[0] != 1 || initial1[1] != 1 || initial1[2] != 1 || initial1[3] != 1 || initial1[4] != 1 || initial1[5] != 1 || initial1[6] != 1 || initial1[7] != 1){}


    // double u;
    // double v = 0.15;
    // double w = 0.24;
    // u = v * w;
    // accum_write(0, i);
    // ROCC_INSTRUCTION(0, 9);//slave core recode context and pc //0x1200000b

    // while(check_mode() != 0b0011){//0x1400450b
    //   if(check_sreceving() == 0b1010){//0x1000450b
    //     ROCC_INSTRUCTION_S(0, 1, 2);//slave core start receving rf_data 0x405a00b
    //     R_INSTRUCTION_JLR(3, 0x01);
    //   }
    // }
    
    // for(i = 0; i < 10; i++){
    //   w = w * 2;
    // }

    // lock_acquire(&uart_lock);
    // printf("helloworld from core 7\n");
    // lock_release(&uart_lock);
    // ret[7] = 1;

    volatile double ya, za;
    transmission(1); //0x0205A00B
    transmission(1);
    transmission(1);
    transmission(1);
    volatile double xa = 1.23;
    ya = cal(xa);
    za = cal(ya); 
    //while(check_mrunning() != 0b1001){}
    ROCC_INSTRUCTION(0, 3); //Master core call for starting check  0x0600000b
    ya = cal(za);
    za = cal(ya); 
    accum_write(0, 1);//0x00C5B00B
    accum_write(0, 2);
    accum_write(0, 3);
    accum_write(0, 4);
    accum_write(0, 5);
    accum_write(0, 6);
    accum_write(0, 7);
    accum_write(0, 8);
    accum_write(0, 9);
    accum_write(0, 10);

    uint64_t CSRa = 0;
    /* Testing CSR Registers */
    asm volatile ("csrr %0, cycle"  : "=r"(CSRa));
    asm volatile ("csrr %0, instret"  : "=r"(CSRa));
  
    volatile double ma = 1.13;
    volatile int ja = 1;
    for(int i = 0; i < 100; i++){
        ma = cal(i * 2);
        ja = ja + i;
    }
    __asm__ volatile(
                     ".LR_SCa:"
                     "li       t0,  0x797;"
                     "li       t1,  0xa;"
                     "li       a5,  0x800002e4;"
                     "mv       a5,  a5;"
                     "li       a4,  1;"
                     "lr.w     a0,  (a5);"
                     "sc.w     a3,  a4,  (a5);"
    );
  
    __asm__ volatile(
                     ".amo_adda:"
                     "li       a7,  0x800002e0;"
                     "mv       a7,  a7;"
                     "li       a6,  1;"
                     "amoadd.w a0,  a6,  (a7);"
    );
  
    __asm__ volatile(
                     ".amo_suba:"
                     "li       a7,  0x800002e0;"
                     "mv       a7,  a7;"
                     "li       a6,  -1;"
                     "amoadd.w a0,  a6,  (a7);"
    );
    __asm__ volatile(
                     ".amo_ora:"
                     "li       a7,  0x800002e0;"
                     "mv       a7,  a7;"
                     "li       a6,  1;"
                     "amoor.w  a0,  a6,  (a7);"
    );
  
    //int32_t a = CONCAT_3(_,atomic_fetch_add_explicit, memory_order_relaxed);
  
    ROCC_INSTRUCTION(0, 11); //Mcore call for ending check 0x1600000b
    lock_acquire(&uart_lock);
    printf("helloworld from mastercore a\n");
    lock_release(&uart_lock);
  
    for(int i = 0; i < 10; i++){
        ja = ja + i;
    }
  
    lock_acquire(&uart_lock);
    printf("helloworld again from mastercore a\n");
    lock_release(&uart_lock);
    ret1[2] = 1;
  break;

  case 0x0b:
    //initialization
    configure(num2, HartID2);
    ROCC_INSTRUCTION_S(0, 500, 12);
    initial1[3] = 1;
    while(initial1[0] != 1 || initial1[1] != 1 || initial1[2] != 1 || initial1[3] != 1 || initial1[4] != 1 || initial1[5] != 1 || initial1[6] != 1 || initial1[7] != 1){}


    // double u;
    // double v = 0.15;
    // double w = 0.24;
    // u = v * w;
    // accum_write(0, i);
    // ROCC_INSTRUCTION(0, 9);//slave core recode context and pc //0x1200000b

    // while(check_mode() != 0b0011){//0x1400450b
    //   if(check_sreceving() == 0b1010){//0x1000450b
    //     ROCC_INSTRUCTION_S(0, 1, 2);//slave core start receving rf_data 0x405a00b
    //     R_INSTRUCTION_JLR(3, 0x01);
    //   }
    // }
    
    // for(i = 0; i < 10; i++){
    //   w = w * 2;
    // }

    // lock_acquire(&uart_lock);
    // printf("helloworld from core 7\n");
    // lock_release(&uart_lock);
    // ret[7] = 1;

    volatile double yb, zb;
    transmission(1); //0x0205A00B
    transmission(1);
    transmission(1);
    transmission(1);
    volatile double xb = 1.23;
    yb = cal(xb);
    zb = cal(yb); 
    //while(check_mrunning() != 0b1001){}
    ROCC_INSTRUCTION(0, 3); //Master core call for starting check  0x0600000b
    yb = cal(zb);
    zb = cal(yb); 
    accum_write(0, 1);//0x00C5B00B
    accum_write(0, 2);
    accum_write(0, 3);
    accum_write(0, 4);
    accum_write(0, 5);
    accum_write(0, 6);
    accum_write(0, 7);
    accum_write(0, 8);
    accum_write(0, 9);
    accum_write(0, 10);

    uint64_t CSRb = 0;
    /* Testing CSR Registers */
    asm volatile ("csrr %0, cycle"  : "=r"(CSRb));
    asm volatile ("csrr %0, instret"  : "=r"(CSRb));
  
    volatile double mb = 1.13;
    volatile int jb = 1;
    for(int i = 0; i < 100; i++){
        mb = cal(i * 2);
        jb = jb + i;
    }
    __asm__ volatile(
                     ".LR_SCb:"
                     "li       t0,  0x797;"
                     "li       t1,  0xa;"
                     "li       a5,  0x800002e4;"
                     "mv       a5,  a5;"
                     "li       a4,  1;"
                     "lr.w     a0,  (a5);"
                     "sc.w     a3,  a4,  (a5);"
    );
  
    __asm__ volatile(
                     ".amo_addb:"
                     "li       a7,  0x800002e0;"
                     "mv       a7,  a7;"
                     "li       a6,  1;"
                     "amoadd.w a0,  a6,  (a7);"
    );
  
    __asm__ volatile(
                     ".amo_subb:"
                     "li       a7,  0x800002e0;"
                     "mv       a7,  a7;"
                     "li       a6,  -1;"
                     "amoadd.w a0,  a6,  (a7);"
    );
    __asm__ volatile(
                     ".amo_orb:"
                     "li       a7,  0x800002e0;"
                     "mv       a7,  a7;"
                     "li       a6,  1;"
                     "amoor.w  a0,  a6,  (a7);"
    );
  
    //int32_t a = CONCAT_3(_,atomic_fetch_add_explicit, memory_order_relaxed);
  
    ROCC_INSTRUCTION(0, 11); //Mcore call for ending check 0x1600000b
    lock_acquire(&uart_lock);
    printf("helloworld from mastercore b\n");
    lock_release(&uart_lock);
  
    for(int i = 0; i < 10; i++){
        jb = jb + i;
    }
  
    lock_acquire(&uart_lock);
    printf("helloworld again from mastercore b\n");
    lock_release(&uart_lock);
    ret1[3] = 1;
  break;

  case 0x0c:
    //initialization
    configure(num2, HartID2);
    ROCC_INSTRUCTION_S(0, 500, 12);
    initial1[4] = 1;
    while(initial1[0] != 1 || initial1[1] != 1 || initial1[2] != 1 || initial1[3] != 1 || initial1[4] != 1 || initial1[5] != 1 || initial1[6] != 1 || initial1[7] != 1){}
    
    double oc;
    double pc = 0.15;
    double qc = 0.24;
    oc = pc * qc;
    accum_write(0, pc);
    ROCC_INSTRUCTION(0, 9);//slave core recode context and pc //0x1200000b

    while(check_mode() != 0b0011){//0x1400450b
      if(check_sreceving() == 0b1010){//0x1000450b
        ROCC_INSTRUCTION_S(0, 1, 2);//slave core start receving rf_data 0x405a00b
        R_INSTRUCTION_JLR(3, 0x01);
      }
    }
    
    for(pc = 0; pc < 10; pc++){
      qc = qc * 2;
    }

    lock_acquire(&uart_lock);
    printf("helloworld from slavecore c\n");
    lock_release(&uart_lock);
    ret1[4] = 1;
  break;

  case 0x0d:
    //initialization
    configure(num2, HartID2);
    ROCC_INSTRUCTION_S(0, 500, 12);
    initial1[5] = 1;
    while(initial1[0] != 1 || initial1[1] != 1 || initial1[2] != 1 || initial1[3] != 1 || initial1[4] != 1 || initial1[5] != 1 || initial1[6] != 1 || initial1[7] != 1){}
    
    double od;
    double pd = 0.15;
    double qd = 0.24;
    od = pd * qd;
    accum_write(0, pd);
    ROCC_INSTRUCTION(0, 9);//slave core recode context and pc //0x1200000b

    while(check_mode() != 0b0011){//0x1400450b
      if(check_sreceving() == 0b1010){//0x1000450b
        ROCC_INSTRUCTION_S(0, 1, 2);//slave core start receving rf_data 0x405a00b
        R_INSTRUCTION_JLR(3, 0x01);
      }
    }
    
    for(pd = 0; pd < 10; pd++){
      qd = qd * 2;
    }

    lock_acquire(&uart_lock);
    printf("helloworld from slavecore d\n");
    lock_release(&uart_lock);
    ret1[5] = 1;
  break;

  case 0x0e:
    //initialization
    configure(num2, HartID2);
    ROCC_INSTRUCTION_S(0, 500, 12);
    initial1[6] = 1;
    while(initial1[0] != 1 || initial1[1] != 1 || initial1[2] != 1 || initial1[3] != 1 || initial1[4] != 1 || initial1[5] != 1 || initial1[6] != 1 || initial1[7] != 1){}
    
    double oe;
    double pe = 0.15;
    double qe = 0.24;
    oe = pe * qe;
    accum_write(0, pe);
    ROCC_INSTRUCTION(0, 9);//slave core recode context and pc //0x1200000b

    while(check_mode() != 0b0011){//0x1400450b
      if(check_sreceving() == 0b1010){//0x1000450b
        ROCC_INSTRUCTION_S(0, 1, 2);//slave core start receving rf_data 0x405a00b
        R_INSTRUCTION_JLR(3, 0x01);
      }
    }
    
    for(pe = 0; pe < 10; pe++){
      qe = qe * 2;
    }

    lock_acquire(&uart_lock);
    printf("helloworld from slavecore e\n");
    lock_release(&uart_lock);
    ret1[6] = 1;
  break;

  case 0x0f:
    //initialization
    configure(num2, HartID2);
    ROCC_INSTRUCTION_S(0, 500, 12);
    initial1[7] = 1;
    while(initial1[0] != 1 || initial1[1] != 1 || initial1[2] != 1 || initial1[3] != 1 || initial1[4] != 1 || initial1[5] != 1 || initial1[6] != 1 || initial1[7] != 1){}
    
    double of;
    double pf = 0.15;
    double qf = 0.24;
    of = pf * qf;
    accum_write(0, pf);
    ROCC_INSTRUCTION(0, 9);//slave core recode context and pc //0x1200000b

    while(check_mode() != 0b0011){//0x1400450b
      if(check_sreceving() == 0b1010){//0x1000450b
        ROCC_INSTRUCTION_S(0, 1, 2);//slave core start receving rf_data 0x405a00b
        R_INSTRUCTION_JLR(3, 0x01);
      }
    }
    
    for(pf = 0; pf < 10; pf++){
      qf = qf * 2;
    }

    lock_acquire(&uart_lock);
    printf("helloworld from slavecore f\n");
    lock_release(&uart_lock);
    ret1[7] = 1;
  break;


  default:
    break;
  }

  idle();
  return 0;
}


int main(void){
  //initialization
  configure(num1, HartID1);
  ROCC_INSTRUCTION_S(0, 500, 12);
  initial[0] = 1;
  while(initial[0] != 1 || initial[1] != 1 || initial[2] != 1 || initial[3] != 1 || initial[4] != 1 || initial[5] != 1 || initial[6] != 1 || initial[7] != 1){}
  while(initial1[0] != 1 || initial1[1] != 1 || initial1[2] != 1 || initial1[3] != 1 || initial1[4] != 1 || initial1[5] != 1 || initial1[6] != 1 || initial1[7] != 1){}
  lock_acquire(&uart_lock);
  printf("Initialization complete for all!\n");
  lock_release(&uart_lock);

  // uint64_t Hart_id = 0;
  // volatile double y, z;
  // transmission(1); //0x0205A00B
  // transmission(1);
  // transmission(1);
  // transmission(1);
  // volatile double x = 1.23;
  // y = cal(x);
  
  
  // z = cal(y); 
  // //while(check_mrunning() != 0b1001){}
  // ROCC_INSTRUCTION(0, 3); //Master core call for starting check  0x0600000b
  // y = cal(z);
  // z = cal(y); 
  // accum_write(0, 1);//0x00C5B00B
  // accum_write(0, 2);
  // accum_write(0, 3);
  // accum_write(0, 4);
  // accum_write(0, 5);
  // accum_write(0, 6);
  // accum_write(0, 7);
  // accum_write(0, 8);
  // accum_write(0, 9);
  // accum_write(0, 10);

  // uint64_t CSR = 0;
  // /* Testing CSR Registers */
  // asm volatile ("csrr %0, cycle"  : "=r"(CSR));
  // asm volatile ("csrr %0, instret"  : "=r"(CSR));
  // asm volatile ("csrr %0, mhartid"  : "=r"(Hart_id));

  // volatile double m = 1.13;
  // volatile int j = 1;
  // for(int i = 0; i < 100; i++){
  //     m = cal(i * 2);
  //     j = j + i;
  // }

  // __asm__ volatile(
  //                  ".LR_SC:"
  //                  "li       t0,  0x797;"
  //                  "li       t1,  0xa;"
  //                  "li       a5,  0x800002e4;"
  //                  "mv       a5,  a5;"
  //                  "li       a4,  1;"
  //                  "lr.w     a0,  (a5);"
  //                  "sc.w     a3,  a4,  (a5);"
  // );

  // __asm__ volatile(
  //                  ".amo_add:"
  //                  "li       a7,  0x800002e0;"
  //                  "mv       a7,  a7;"
  //                  "li       a6,  1;"
  //                  "amoadd.w a0,  a6,  (a7);"
  // );

  // __asm__ volatile(
  //                  ".amo_sub:"
  //                  "li       a7,  0x800002e0;"
  //                  "mv       a7,  a7;"
  //                  "li       a6,  -1;"
  //                  "amoadd.w a0,  a6,  (a7);"
  // );
  // __asm__ volatile(
  //                  ".amo_or:"
  //                  "li       a7,  0x800002e0;"
  //                  "mv       a7,  a7;"
  //                  "li       a6,  1;"
  //                  "amoor.w  a0,  a6,  (a7);"
  // );

  // //int32_t a = CONCAT_3(_,atomic_fetch_add_explicit, memory_order_relaxed);

  // ROCC_INSTRUCTION(0, 11); //Mcore call for ending check 0x1600000b
  // lock_acquire(&uart_lock);
  // printf("helloworld from mastercore 0\n");
  // lock_release(&uart_lock);

  // for(int i = 0; i < 10; i++){
  //     j = j + i;
  // }

  // lock_acquire(&uart_lock);
  // printf("helloworld again from mastercore 0\n");
  // lock_release(&uart_lock);
  // ret[0] = 1;
    double o0;
    double p0 = 0.15;
    double q0 = 0.24;
    o0 = p0 * q0;
    accum_write(0, p0);
    ROCC_INSTRUCTION(0, 9);//slave core recode context and pc //0x1200000b

    while(check_mode() != 0b0011){//0x1400450b
      if(check_sreceving() == 0b1010){//0x1000450b
        ROCC_INSTRUCTION_S(0, 1, 2);//slave core start receving rf_data 0x405a00b
        R_INSTRUCTION_JLR(3, 0x01);
      }
    }
    
    for(p0 = 0; p0 < 10; p0++){
      q0 = q0 * 2;
    }

    lock_acquire(&uart_lock);
    printf("helloworld from slavecore 0\n");
    lock_release(&uart_lock);
    ret[0] = 1;

  while(ret[0] != 1 || ret[1] != 1 || ret[2] != 1 || ret[3] != 1 || ret[4] != 1 || ret[5] != 1 || ret[6] != 1 || ret[7] != 1){}
  while(ret1[0] != 1 || ret1[1] != 1 || ret1[2] != 1 || ret1[3] != 1 || ret1[4] != 1 || ret1[5] != 1 || ret1[6] != 1 || ret1[7] != 1){}
  return 0;
}