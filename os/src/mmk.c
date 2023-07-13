#include "mmk.h"


int mmk_call(unsigned long id, unsigned long *args, unsigned int arglen, unsigned long *retval)
{
	unsigned long vec[5] = {0,0,0,0,0};
	for(int a = 0;a<5;a++){
		if(a < arglen){
			vec[a] = args[a];
		}
	}
		
	unsigned long ret = 0;
	unsigned long status = 0;
	asm volatile(
		"mv t3, %2 \n\t"
		"mv x17, %3 \n\t"
		"mv x10, %4 \n\t"
		"mv x11, %5 \n\t"
		"mv x12, %6 \n\t"
		"mv x13, %7 \n\t"
		"mv x14, %8 \n\t"
                "jalr x1, t3, 0 \n\t"
                "mv %0, a0 \n\t"
                "mv %1, a1 \n\t"
                : "=r" (ret), "=r" (status)
                : "r" (-0x1000), "r" (id*8),
                "r" (vec[0]), "r" (vec[1]), "r" (vec[2]), "r" (vec[3]), "r" (vec[4])
            );
        *retval = ret;
        return status;
}


int nkapi_time(unsigned long* time){
	unsigned long params[5] = {0,0,0,0,0};
	return mmk_call(NKAPI_TIME,params,5,time);
}

int nkapi_translate(unsigned long pt_handle, unsigned long vpn, unsigned char write, unsigned long *ppn){
	unsigned long params[3] = {pt_handle, vpn, (unsigned long)write};
	return mmk_call(NKAPI_TRANSLATE, params, 3, ppn);
}
int nkapi_translate_va(unsigned long pt_handle, unsigned long va, unsigned long *pa){
	return 0;
}
int nkapi_get_pte(unsigned long pt_handle, unsigned long vpn, unsigned long *pte){
	return 0;
}
int nkapi_fork_pte(unsigned long pt_handle, unsigned long vpn, unsigned char cow, unsigned long *ppn){
	return 0;
}
int nkapi_alloc(unsigned long pt_handle, unsigned long vpn, 
	unsigned long map_type, unsigned long map_perm, unsigned long *ppn){
	return 0;
}
int nkapi_dealloc(unsigned long pt_handle, unsigned long vpn){
	return 0;
}
int nkapi_pt_init(unsigned long pt_handle, unsigned char regenerate){
	return 0;
}
int nkapi_activate(unsigned long pt_handle){
	return 0;
}
int nkapi_write(unsigned long pt_handle, unsigned long vpn, unsigned char *data, unsigned long offset){
	return 0;
}
int nkapi_set_permission(unsigned long pt_handle, unsigned long vpn, unsigned long map_perm){
	return 0;
}
int nkapi_print_pt(unsigned long pt_handle, unsigned long from, unsigned long to){
	return 0;
}
int nkapi_config_delegate_handler(unsigned long entry){
	return 0;
}
int nkapi_config_signal_handler(unsigned long entry){
	return 0;
}
int nkapi_config_allocator_range(unsigned long begin, unsigned long end){
	return 0;
}



