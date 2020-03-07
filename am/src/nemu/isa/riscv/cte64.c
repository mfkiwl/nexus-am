#include <riscv.h>

#ifdef __ARCH_RISCV64_NOOP
#define CLINT_MMIO 0x48000000
#define TIME_INC 0x80000
#else
#define CLINT_MMIO 0xa2000000
#define TIME_INC 0x800
#endif

#define CLINT_MTIMECMP (CLINT_MMIO + 0x4000)

extern void __am_timervec(void);

static void init_timer() {
  static struct {
    uintptr_t mtimecmp;
    uintptr_t time_inc;
    uintptr_t temp[3];
  } handle;

  handle.mtimecmp = CLINT_MTIMECMP;
  handle.time_inc = TIME_INC;

  asm volatile("csrw mscratch, %0" : : "r"(&handle));

  // set M-mode exception entry to handle machine timer interrupt
  asm volatile("csrw mtvec, %0" : : "r"(__am_timervec));

  // set machine timer interrupt
  asm volatile("csrs mie, %0" : : "r"((1 << 7) | (1 << 1)));
  outd(CLINT_MTIMECMP, 0);
}

void __am_init_cte64() {
  // set delegation
  asm volatile("csrw mideleg, %0" : : "r"(0xffff));
  asm volatile("csrw medeleg, %0" : : "r"(0xffff));

  init_timer();

  // enter S-mode
  uintptr_t status = MSTATUS_SPP(MODE_S);
  extern char _here;
  asm volatile(
    "csrw sstatus, %0;"
    "csrw sepc, %1;"
    "sret;"
    "_here:"
    : : "r"(status), "r"(&_here));
}
