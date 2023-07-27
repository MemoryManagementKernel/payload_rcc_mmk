#include "task.h"
#include "trap.h"
#include "log.h"

void task_context_zero_init(TaskContext *cx) {
  cx->ra = 0;
  cx->sp = 0;
  for (int i = 0; i < 12; i++) {
    cx->s[i] = 0;
  }
}

void task_context_goto_trap_return(TaskContext *cx, uint64_t kstack_ptr) {
  cx->ra = *(uint64_t *)(0xffffffffffffffff - 0x3000 + 1 + 93 * 8);
  cx->sp = kstack_ptr;
  for (int i = 0; i < 12; i++)
    cx->s[i] = 0;
}
