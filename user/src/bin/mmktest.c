#include "stdio.h"
#include "mmk_syscall.h"

int main() {
  printf("mmk echo:\n");
  mmk_syscall_echo(233);
  return 0;
}
