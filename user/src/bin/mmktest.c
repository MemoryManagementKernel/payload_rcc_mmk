#include "stdio.h"
#include "mmk_syscall.h"
#include "pkcs11.h"

int callback(){
  printf("callback\n");
}
int main() {
  printf("mmk echo:\n");
  mmk_syscall_echo(233);
  C_Initialize(callback);
  return 0;
}
