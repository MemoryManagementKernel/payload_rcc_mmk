#include "task.h"
#include "fcntl.h"
#include "fs.h"
#include "log.h"
#include "string.h"

static TaskControlBlock INITPROC;

typedef struct _app_name {
  char name[32];
} app_name;

extern uint8_t _app_num;
extern uint64_t _app_data;
extern app_name _app_names;



uint64_t mem_load_pgms(char* name, uint8_t* load_data){
  info("load app from %d apps %p\n", _app_num, &_app_num);
  for(int a = 0;a<_app_num;a++){

    info("checking app %s\n", (&_app_names)[a]);
    if( strcmp(name, (&_app_names)+a) == 0 ){
      uint64_t siz = (&_app_data)[a+1] - (&_app_data)[a];
      memcpy(load_data, (uint8_t*)((&_app_data)[a]), siz);
      return siz;
    }
  }
  return 0;
}

void task_init() {
  pid_allocator_init();
  task_manager_init();

  static uint8_t initproc_elf[MAX_APP_SIZE];
  uint64_t initproc_elf_size = 0;
  
  if(fs_status() == 1){
    info("loading initproc from fs\n");
    //load elf from file system.
    INITPROC.elf_inode = inode_open_file("initproc", O_RDONLY);
    if (!INITPROC.elf_inode) {
      panic("Fail to create initproc\n");
    }
    initproc_elf_size = inode_read_all(INITPROC.elf_inode, initproc_elf);
  
  }else{
    info("loading initproc from memory\n");
    //TODO: load elf from memory here.
    initproc_elf_size = mem_load_pgms("initproc", initproc_elf);
    if(initproc_elf_size == 0){
      panic("Fail to load initproc from mem\n");
    }
  }
  
  // panic("unreachable task init\n");
  task_control_block_new(&INITPROC, initproc_elf, initproc_elf_size);
  task_manager_add_task(&INITPROC);
}

void task_suspend_current_and_run_next() {
  // There must be an application running.
  TaskControlBlock *task = processor_take_current_task();
  TaskContext *task_cx_ptr = &task->task_cx;

  // Change status to Ready
  task->task_status = TASK_STATUS_READY;

  // push back to ready queue
  task_manager_add_task(task);

  // jump to scheduling cycle
  processor_schedule(task_cx_ptr);
}

void task_exit_current_and_run_next(int exit_code) {
  // take from Processor
  TaskControlBlock *task = processor_take_current_task();

  // Change status to Zombie
  task->task_status = TASK_STATUS_ZOMBIE;

  // Record exit code
  task->exit_code = exit_code;
  // do not move to its parent but under initproc
  info("exit 0");
  TaskControlBlock **x = (TaskControlBlock **)(task->children.buffer);
  for (uint64_t i = 0; i < task->children.size; i++) {
    x[i]->parent = &INITPROC;
    vector_push(&INITPROC.children, x[i]);
  }
  vector_free(&task->children);
  // deallocate user space
  memory_set_recycle_data_pages(&task->memory_set);
  // deallocate kernel stack

  //Yan_ice: temporarily not free kernel stack, it would cause mem leak
  //kernel_stack_free(&task->kernel_stack);
  
  // we do not have to save task context
  TaskContext _unused;
  task_context_zero_init(&_unused);
  processor_schedule(&_unused);
  
}

MemorySet *task_current_memory_set() {
  TaskControlBlock *task = processor_take_current_task();
  return &task->memory_set;
}
