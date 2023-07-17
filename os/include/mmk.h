#include "mmk_declare.h"

///////////////////////////////////
/// 
/// the value below is NK call number.
/// 
unsigned long NKAPI_CONFIG = 0;
unsigned long NKAPI_TRAP_HANDLE = 1;
unsigned long NKAPI_PT_INIT = 2;
unsigned long NKAPI_ALLOC = 3;
unsigned long NKAPI_DEALLOC = 4;
unsigned long NKAPI_ACTIVATE = 5;
unsigned long NKAPI_WRITE = 6;
unsigned long NKAPI_TRANSLATE = 7;
unsigned long NKAPI_GET_PTE = 8;
unsigned long NKAPI_FORK_PTE = 9;
unsigned long NKAPI_SET_PERM = 10;
unsigned long NKAPI_TIME = 11;
unsigned long NKAPI_DEBUG = 12;
///
///////////////////////////////////


///////////////////////////////////
/// 
/// the value below is NK_TRAP_HANDLE param.
/// 
unsigned long NKCFG_DELEGATE = 1;
unsigned long NKCFG_SIGNAL = 2;
unsigned long NKCFG_ALLOCATOR_START = 3;
unsigned long NKCFG_ALLOCATOR_END = 4;
///
///////////////////////////////////


int nkapi_time(unsigned long *time);
int nkapi_translate(unsigned long pt_handle, VirtPageNum vpn, unsigned char write, PhysPageNum *ppn);
int nkapi_translate_va(unsigned long pt_handle, VirtAddr va, PhysAddr *pa);
int nkapi_get_pte(unsigned long pt_handle, VirtPageNum vpn, unsigned long *pte);
int nkapi_fork_pte(unsigned long pt_handle, VirtPageNum vpn, unsigned char cow, PhysPageNum *ppn);
int nkapi_alloc(unsigned long pt_handle, VirtPageNum vpn, 
	MapType map_type, MapPermission map_perm, PhysPageNum *ppn);
int nkapi_dealloc(unsigned long pt_handle, VirtPageNum vpn);
int nkapi_pt_init(unsigned long pt_handle, unsigned char regenerate);
int nkapi_activate(unsigned long pt_handle);
int nkapi_write(unsigned long pt_handle, VirtPageNum vpn, unsigned char *data, unsigned long len, unsigned long offset);
int nkapi_set_permission(unsigned long pt_handle, VirtPageNum vpn, MapPermission map_perm);
int nkapi_print_pt(unsigned long pt_handle, unsigned long from, unsigned long to);

int nkapi_config_delegate_handler(unsigned long entry);
int nkapi_config_signal_handler(unsigned long entry);
int nkapi_config_allocator_range(unsigned long begin, unsigned long end);


