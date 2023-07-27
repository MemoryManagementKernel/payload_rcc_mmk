#include "elfparse.h"
#include "log.h"
#include "mm.h"
#include "riscv.h"
#include "string.h"

#include "mmk.h"

static void map_area_from_another(MapArea *map_area, MapArea *another) {
  map_area->vpn_range.l = another->vpn_range.l;
  map_area->vpn_range.r = another->vpn_range.r;
  map_area->map_type = another->map_type;
  map_area->map_perm = another->map_perm;
}

//Yan_ice: need to modify
static void map_area_map_one(MapArea *map_area, PtHandle pt,
                             VirtPageNum vpn) {
  PhysPageNum ppn;
  if (nkapi_alloc(pt, vpn, map_area->map_type, map_area->map_perm, &ppn)==0)
  {
    trace("map vpn 0x%llx - ppn 0x%llx\n", vpn, ppn);
  }
}

//Yan_ice: need to modify
static void map_area_unmap_one(MapArea *map_area, PtHandle pt,
                               VirtPageNum vpn, bool dealloc) {
  if (nkapi_dealloc(pt, vpn)==0)
  {
    trace("unmap vpn 0x%llx\n", vpn);
  }
}

static void map_area_map(MapArea *map_area, PtHandle pt) {
  if(pt == 1){
    // panic("1\n");
  }
  for (VirtPageNum vpn = map_area->vpn_range.l; vpn < map_area->vpn_range.r;
       vpn++) {
    
    map_area_map_one(map_area, pt, vpn);
  }
}

static void map_area_unmap(MapArea *map_area, PtHandle pt, bool dealloc) {
  for (VirtPageNum vpn = map_area->vpn_range.l; vpn < map_area->vpn_range.r;
       vpn++) {
    map_area_unmap_one(map_area, pt, vpn, dealloc);
  }
}

//Yan_ice: need to modify
static void map_area_copy_data(MapArea *map_area, PtHandle pt, uint8_t *data,
                               uint64_t len) {
  uint64_t start = 0;
  VirtPageNum current_vpn = map_area->vpn_range.l;
  for (;;) {
    uint8_t *src = &data[start];
    uint64_t cpy_len = (len - start >= PAGE_SIZE) ? PAGE_SIZE : (len - start);
    nkapi_write(pt, current_vpn, src, cpy_len, 0);
    start += PAGE_SIZE;
    if (start >= len) {
      break;
    }
    current_vpn += 1;
  }
}

static void memory_set_new_bare(MemorySet *memory_set) {
  info("pid new here\n");
  nkapi_pt_init(memory_set->page_table, 0);
  vector_new(&memory_set->areas, sizeof(MapArea));
}

uint64_t memory_set_id(MemorySet *memory_set) {
  return memory_set->page_table;
}

static void memory_set_push(MemorySet *memory_set, MapArea *map_area,
                            uint8_t *data, uint64_t len) {

  map_area_map(map_area, memory_set->page_table);
  if (data && len >= 0) {
    map_area_copy_data(map_area, memory_set->page_table, data, len);
  }
  vector_push(&memory_set->areas, map_area);
}

// Assume that no conflicts.
static void memory_set_insert_framed_area(MemorySet *memory_set,
                                          VirtAddr start_va, VirtAddr end_va,
                                          MapPermission permission) {
  MapArea map_area;
  map_area.vpn_range.l = page_floor(start_va);
  map_area.vpn_range.r = page_ceil(end_va);
  // map_area.map_type = MAP_FRAMED;
  map_area.map_type = 0xfffffffffffffffdul;
  map_area.map_perm = permission;
  memory_set_push(memory_set, &map_area, NULL, 0);
}

static void memory_set_remove_area_with_start_vpn(MemorySet *memory_set,
                                                  VirtPageNum start_vpn) {
  MapArea *x = (MapArea *)(memory_set->areas.buffer);
  uint64_t i = 0;
  while (i < memory_set->areas.size) {
    if (x[i].vpn_range.l == start_vpn) {
      map_area_unmap(&x[i], memory_set->page_table, true);
      vector_remove(&memory_set->areas, i);
    } else {
      i++;
    }
  }
}

void memory_set_free(MemorySet *memory_set) {
  nkapi_pt_destroy(memory_set->page_table);
  // MapArea *x = (MapArea *)(memory_set->areas.buffer);
  // for (uint64_t i = 0; i < memory_set->areas.size; i++) {
  //   map_area_unmap(&x[i], &memory_set->page_table, false);
  // }
  vector_free(&memory_set->areas);
  // page_table_free(&memory_set->page_table);
}

extern uint8_t stext;
extern uint8_t etext;
extern uint8_t srodata;
extern uint8_t erodata;
extern uint8_t sdata;
extern uint8_t edata;
extern uint8_t sbss_with_stack;
extern uint8_t ebss;
extern uint8_t ekernel;
extern uint8_t strampoline;

// Mention that trampoline is not collected by areas.
// Yan_ice: No need for mmk.
static inline void memory_set_map_trampoline(MemorySet *memory_set) {
  //page_table_map(&memory_set->page_table, addr2pn(TRAMPOLINE),
  //               addr2pn((PhysAddr)&strampoline), PTE_R | PTE_X);
}

static MemorySet KERNEL_SPACE;

static void memory_set_new_kernel() {
  MemorySet *memory_set = &KERNEL_SPACE;
  memory_set->page_table = 0;
  memory_set_new_bare(memory_set);

  // map trampoline
  memory_set_map_trampoline(memory_set);

  // map kernel sections
  info(".text      [0x%llx, 0x%llx)\n", &stext, &etext);
  info(".rodata    [0x%llx, 0x%llx)\n", &srodata, &erodata);
  info(".data      [0x%llx, 0x%llx)\n", &sdata, &edata);
  info(".bss       [0x%llx, 0x%llx)\n", &sbss_with_stack, &ebss);
  info(".physical memory  [0x%llx, 0x%llx)\n", &ekernel, 0x81000000);

  MapArea map_area;

  info("mapping .text section\n");
  map_area.vpn_range.l = page_floor((PhysAddr)&stext);
  map_area.vpn_range.r = page_ceil((PhysAddr)&etext);
  map_area.map_type = MAP_IDENTICAL;
  map_area.map_perm = MAP_PERM_R | MAP_PERM_X;
  memory_set_push(memory_set, &map_area, NULL, 0);

  info("mapping .rodata section\n");
  map_area.vpn_range.l = page_floor((PhysAddr)&srodata);
  map_area.vpn_range.r = page_ceil((PhysAddr)&erodata);
  map_area.map_type = MAP_IDENTICAL;
  map_area.map_perm = MAP_PERM_R;
  memory_set_push(memory_set, &map_area, NULL, 0);

  info("mapping .data section\n");
  map_area.vpn_range.l = page_floor((PhysAddr)&sdata);
  map_area.vpn_range.r = page_ceil((PhysAddr)&edata);
  map_area.map_type = MAP_IDENTICAL;
  map_area.map_perm = MAP_PERM_R | MAP_PERM_W;
  memory_set_push(memory_set, &map_area, NULL, 0);

  info("mapping .bss section\n");
  map_area.vpn_range.l = page_floor((PhysAddr)&sbss_with_stack);
  map_area.vpn_range.r = page_ceil((PhysAddr)&ebss);
  map_area.map_type = MAP_IDENTICAL;
  map_area.map_perm = MAP_PERM_R | MAP_PERM_W;
  memory_set_push(memory_set, &map_area, NULL, 0);

  info("mapping physical memory\n");
  map_area.vpn_range.l = page_floor((PhysAddr)&ekernel);
  map_area.vpn_range.r = page_ceil((PhysAddr)MEMORY_END);
  map_area.map_type = MAP_IDENTICAL;
  map_area.map_perm = MAP_PERM_R | MAP_PERM_W;
  memory_set_push(memory_set, &map_area, NULL, 0);

  info("mapping memory-mapped registers\n");
  for (uint64_t i = 0; i < MMIO_NUM; i++) {
    map_area.vpn_range.l = page_floor((PhysAddr)MMIO[i][0]);
    map_area.vpn_range.r = page_ceil((PhysAddr)(MMIO[i][0] + MMIO[i][1]));
    map_area.map_type = MAP_IDENTICAL;
    map_area.map_perm = MAP_PERM_R | MAP_PERM_W;
    memory_set_push(memory_set, &map_area, NULL, 0);
  }

  info("mapping plic\n");
  map_area.vpn_range.l = page_floor((PhysAddr)PLIC);
  map_area.vpn_range.r = page_ceil((PhysAddr)(PLIC + 0x400000));
  map_area.map_type = MAP_IDENTICAL;
  map_area.map_perm = MAP_PERM_R | MAP_PERM_W;
  memory_set_push(memory_set, &map_area, NULL, 0);
}

void memory_set_from_elf(MemorySet *memory_set, uint8_t *elf_data,
                         size_t elf_size, uint64_t *user_sp,
                         uint64_t *entry_point) {

  memory_set_new_bare(memory_set);

  // map trampoline
  // memory_set_map_trampoline(memory_set);
  // map progam headers of elf, with U flag
  t_elf elf;
  int elf_load_ret = elf_load(elf_data, elf_size, &elf);
  if (elf_load_ret != 0) {
    panic("Elf load error.\n");
  }

  size_t ph_count = elf_header_get_phnum(&elf);
  VirtAddr start_va, end_va;
  MapPermission map_perm;
  uint64_t ph_flags;
  MapArea map_area;
  VirtPageNum max_end_vpn = 0;
  
  // panic("stop here\n");
  
  for (size_t i = 0; i < ph_count; i++) {
    t_elf_program *ph = &elf.programs[i];
    if (elf_program_get_type(&elf, ph) == PT_LOAD) {
      start_va = (VirtAddr)elf_program_get_vaddr(&elf, ph);
      end_va = (VirtAddr)(start_va + elf_program_get_memsz(&elf, ph));
      map_perm = MAP_PERM_U;
      ph_flags = elf_program_get_flags(&elf, ph);
      if (ph_flags | PF_R) {
        map_perm |= MAP_PERM_R;
      }
      if (ph_flags | PF_W) {
        map_perm |= MAP_PERM_W;
      }
      if (ph_flags | PF_X) {
        map_perm |= MAP_PERM_X;
      }
      map_area.vpn_range.l = page_floor(start_va);
      map_area.vpn_range.r = page_ceil(end_va);
      // map_area.map_type = MAP_FRAMED;
      map_area.map_type = 0xfffffffffffffffdul;
      map_area.map_perm = map_perm;
      max_end_vpn = map_area.vpn_range.r;
      // panic("stop here\n");
      memory_set_push(memory_set, &map_area,
                      elf_data + elf_program_get_offset(&elf, ph),
                      elf_program_get_filesz(&elf, ph));
    }
  }

  // map user stack with U flags
  VirtAddr max_end_va = pn2addr(max_end_vpn);
  VirtAddr user_stack_bottom = max_end_va;
  // guard page
  user_stack_bottom += PAGE_SIZE;
  VirtAddr user_stack_top = user_stack_bottom + USER_STACK_SIZE;
  map_area.vpn_range.l = page_floor(user_stack_bottom);
  map_area.vpn_range.r = page_ceil(user_stack_top) + 1;
  // map_area.map_type = MAP_FRAMED;
  map_area.map_type = 0xfffffffffffffffdul;
  map_area.map_perm = MAP_PERM_R | MAP_PERM_W | MAP_PERM_U;
  memory_set_push(memory_set, &map_area, NULL, 0);

  info("map trap context\n");
  // map TrapContext
  map_area.vpn_range.l = page_floor(TRAP_CONTEXT);
  map_area.vpn_range.r = page_ceil(TRAP_CONTEXT) + 1;
  // map_area.map_type = MAP_FRAMED;
  map_area.map_type = 0xfffffffffffffffdul;
  map_area.map_perm = MAP_PERM_R | MAP_PERM_W;
  memory_set_push(memory_set, &map_area, NULL, 0);

  // return
  *user_sp = (uint64_t)user_stack_top;
  *entry_point = elf_header_get_entry(&elf);
}

void memory_set_from_existed_user(MemorySet *memory_set,
                                  MemorySet *user_space) {

  memory_set_new_bare(memory_set);

  // map trampoline
  memory_set_map_trampoline(memory_set);

  // copy data sections / trap_context / user_stack
  MapArea new_area;
  MapArea *x = (MapArea *)(user_space->areas.buffer);
  PhysPageNum src_ppn;
  for (uint64_t i = 0; i < user_space->areas.size; i++) {
    map_area_from_another(&new_area, &x[i]);
    memory_set_push(memory_set, &new_area, NULL, 0);
    // copy data from another space
    for (VirtPageNum vpn = x[i].vpn_range.l; vpn < x[i].vpn_range.r; vpn++) {
      
      src_ppn = nkapi_translate(user_space->page_table, vpn, 0, &src_ppn);
      //dst_ppn = nkapi_translate(memory_set->page_table, vpn, 0, &dst_ppn);
      //memcpy(ppn_get_bytes_array(dst_ppn), ppn_get_bytes_array(src_ppn),
      //      PAGE_SIZE);
      nkapi_write(memory_set->page_table, vpn, ppn_get_bytes_array(src_ppn), 
      PAGE_SIZE, 0);
    }
  }
}

static void memory_set_activate(MemorySet *memory_set) {
  // uint64_t satp = &memory_set->page_table;
  // w_satp(satp);
  // sfence_vma();
  nkapi_activate(memory_set->page_table);
}

void memory_set_kernel_init() {
  memory_set_new_kernel();
  memory_set_activate(&KERNEL_SPACE);
}

PhysPageNum memory_set_translate(MemorySet *memory_set, VirtPageNum vpn) {
  PhysPageNum et;
  if (nkapi_translate(memory_set->page_table, vpn, 0, &et) == 0){
    return et;
  }
  return 0;
}

void memory_set_recycle_data_pages(MemorySet *memory_set) {
  memory_set_free(memory_set);
}

void kernel_space_insert_framed_area(VirtAddr start_va, VirtAddr end_va,
                                     MapPermission permission) {
  memory_set_insert_framed_area(&KERNEL_SPACE, start_va, end_va, permission);
}

void kernel_space_remove_area_with_start_vpn(VirtPageNum start_vpn) {
  memory_set_remove_area_with_start_vpn(&KERNEL_SPACE, start_vpn);
}

uint64_t kernel_space_id() { return memory_set_id(&KERNEL_SPACE); }

int64_t memory_set_mmap(MemorySet *memory_set, uint64_t start, uint64_t len,
                        uint64_t prot) {
  if (len == 0) {
    return 0;
  }

  if (!page_aligned(start) || (len > MMAP_MAX_SIZE) || ((prot & ~0x7) != 0) ||
      ((prot & 0x7) == 0)) {
    return -1;
  }

  len = page_ceil(len) * PAGE_SIZE;
  VirtPageNum vpn_start = addr2pn(start);
  VirtPageNum vpn_end = addr2pn(start + len);
  PtHandle pt = memory_set->page_table;

  // check unmapped
  for (VirtPageNum vpn = vpn_start; vpn < vpn_end; vpn++) {
    PhysPageNum ppn_abort;
    if (nkapi_translate(pt, vpn,0,&ppn_abort)) {
      return -1;
    }
  }

  MapPermission map_perm = MAP_PERM_U;
  if ((prot & 0x1) != 0)
    map_perm |= MAP_PERM_R;
  if ((prot & 0x2) != 0)
    map_perm |= MAP_PERM_W;
  if ((prot & 0x4) != 0)
    map_perm |= MAP_PERM_X;

  // map
  PhysPageNum ppn;
  for (VirtPageNum vpn = vpn_start; vpn < vpn_end; vpn++) {
    nkapi_alloc(pt,vpn,MAP_FRAMED,map_perm,&ppn);
  }

  // check mapped
  for (VirtPageNum vpn = vpn_start; vpn < vpn_end; vpn++) {
    PhysPageNum ppn_abort;
    if (nkapi_translate(pt, vpn,0,&ppn_abort)) {
      return -1;
    }
  }

  return len;
}

int64_t memory_set_munmap(MemorySet *memory_set, uint64_t start, uint64_t len) {
  if (len == 0) {
    return 0;
  }

  if (!page_aligned(start) || (len > MMAP_MAX_SIZE)) {
    return -1;
  }

  len = page_ceil(len) * PAGE_SIZE;
  VirtPageNum vpn_start = addr2pn(start);
  VirtPageNum vpn_end = addr2pn(start + len);
  PtHandle pt = memory_set->page_table;

  // check mapped
  for (VirtPageNum vpn = vpn_start; vpn < vpn_end; vpn++) {
    PhysPageNum ppn_abort;
    if (nkapi_translate(pt, vpn,0,&ppn_abort)) {
      return -1;
    }
  }

  // unmap
  for (VirtPageNum vpn = vpn_start; vpn < vpn_end; vpn++) {
    nkapi_dealloc(pt,vpn);
  }

  // check unmapped
  for (VirtPageNum vpn = vpn_start; vpn < vpn_end; vpn++) {
    PhysPageNum ppn_abort;
    if (nkapi_translate(pt, vpn,0,&ppn_abort)) {
      return -1;
    }
  }

  return len;
}