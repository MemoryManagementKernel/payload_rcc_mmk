#include "mm.h"

void mm_init() {
  // mm init
  heap_allocator_init();
  memory_set_kernel_init();
  //frame_allocator_init();
}

int64_t copy_byte_buffer(uint64_t id, uint8_t *kernel, uint8_t *user,
                          uint64_t len, uint64_t direction)
{

  if(direction == TO_USER) {
    VirtPageNum dst = ((uint64_t)user) / PAGE_SIZE;
    uint64_t offset = ((uint64_t)user) % PAGE_SIZE;
    nkapi_write(id, dst, kernel, len, offset);
  }else{
    VirtPageNum dst = ((uint64_t)kernel) / PAGE_SIZE;
    uint64_t offset = ((uint64_t)kernel) % PAGE_SIZE;
    nkapi_write(0, dst, user, len, offset);
  }
  return len;
}


void mm_free() {
  // mm free
  //frame_allocator_free();
}
