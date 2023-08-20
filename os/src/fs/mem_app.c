#include <stdint.h>
#include "fs.h"
#include "log.h"

typedef struct _app_name {
  char name[32];
} app_name;

extern uint64_t _app_num;
extern app_name _app_names;
extern uint64_t _app_datas;


uint64_t mem_load_pgms(char* name, uint8_t* load_data){
  info("load app from %d apps\n", _app_num, &_app_num);
  for(int a = 0;a<_app_num;a++){
    info("checking app %s\n", (&_app_names)[a]);
    if( strcmp(name, (&_app_names)+a) == 0 ){
      uint64_t siz = (&_app_datas)[a+1] - (&_app_datas)[a];
      memcpy(load_data, (uint8_t*)((&_app_datas)[a]), siz);
      return siz;
    }
  }

  return 0;
}