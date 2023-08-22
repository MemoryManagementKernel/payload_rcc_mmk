    .align 3
    .section .data
    .global _app_num
_app_num:
    .quad 3

    .global _app_datas
_app_datas:
    .quad app_0 # value defined by "quad" is always 64 bit.
    .quad app_1
    .quad app_2
    .quad app_end

    .global _app_names
_app_names:
    .string "user_shell"
    .string "initproc"
    .string "hello_world"

    .global app_0
app_0:
    .incbin "../user/build/user_shell.elf"

    .global app_1
app_1:
    .incbin "../user/build/initproc.elf"

    .global app_2
app_2:
    .incbin "../user/build/hello_world.elf"

    .global app_end
app_end:
