    .align 3
    .section .data
    .global _app_num
_app_num:
    .quad 7

    .global _app_datas
_app_datas:
    .quad app_0 # value defined by "quad" is always 64 bit.
    .quad app_1
    .quad app_2
    .quad app_3
    .quad app_4
    .quad app_5
    .quad app_end

    .global _app_names
_app_names:
    .string "user_shell"
    .string "initproc"
    .string "hello_world"
    .string "mmktest"
    .string "syscall"
    .string "msyscall"

    .global app_0
app_0:
    .incbin "../user/build/user_shell.elf"

    .global app_1
app_1:
    .incbin "../user/build/initproc.elf"

    .global app_2
app_2:
    .incbin "../user/build/hello_world.elf"

    .global app_3
app_3:
    .incbin "../user/build/mmktest.elf"

    .global app_4
app_4:
    .incbin "../elf/syscall"

    .global app_5
app_5:
    .incbin "../user/build/msyscall.elf"    

    .global app_end
app_end:
