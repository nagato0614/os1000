//
// Created by toru on 2025/01/10.
//

#ifndef OS1000__KERNEL_H_
#define OS1000__KERNEL_H_
#include "common.h"

#define PROCS_MAX 8     // プロセス数の最大値
#define PROC_UNUSED 0   // 未使用のプロセス管理構造体
#define PROC_RUNNABLE 1 // 実行可能なプロセス管理構造体
#define PROC_EXITED   2
#define KERNEL_STACK_SIZE 8192

#define SCAUSE_ECALL 8

// ページテーブルエントリ
#define SATP_SV32 (1u << 31)
#define PAGE_V (1u << 0) // 有効ビット
#define PAGE_R (1u << 1) // 読み込み可能
#define PAGE_W (1u << 2) // 書き込み可能
#define PAGE_X (1u << 3) // 実行可能
#define PAGE_U (1u << 4) // ユーザーモードで実行可能

#define SSTATUS_SPIE (1u << 5)

#define USER_BASE 0x1000000

struct process
{
  int pid;
  int state;
  vaddr_t sp;
  uint32_t *page_table;
  uint8_t stack[KERNEL_STACK_SIZE];
};

struct sbiret
{
  long error;
  long value;
};

struct trap_frame
{
  uint32_t ra;
  uint32_t gp;
  uint32_t tp;
  uint32_t t0;
  uint32_t t1;
  uint32_t t2;
  uint32_t t3;
  uint32_t t4;
  uint32_t t5;
  uint32_t t6;
  uint32_t a0;
  uint32_t a1;
  uint32_t a2;
  uint32_t a3;
  uint32_t a4;
  uint32_t a5;
  uint32_t a6;
  uint32_t a7;
  uint32_t s0;
  uint32_t s1;
  uint32_t s2;
  uint32_t s3;
  uint32_t s4;
  uint32_t s5;
  uint32_t s6;
  uint32_t s7;
  uint32_t s8;
  uint32_t s9;
  uint32_t s10;
  uint32_t s11;
  uint32_t sp;
} __attribute__((packed));

#define READ_CSR(reg)                                                          \
    ({                                                                         \
        unsigned long __tmp;                                                   \
        __asm__ __volatile__("csrr %0, " #reg : "=r"(__tmp));                  \
        __tmp;                                                                 \
    })

#define WRITE_CSR(reg, value)                                                  \
    do {                                                                       \
        uint32_t __tmp = (value);                                              \
        __asm__ __volatile__("csrw " #reg ", %0" ::"r"(__tmp));                \
    } while (0)

#define PANIC(fmt, ...)                                                   \
  do {                                                                    \
    printf("PANIC: %s:%d " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__);  \
    while(1) {}                                                           \
  } while(0)

#endif //OS1000__KERNEL_H_
