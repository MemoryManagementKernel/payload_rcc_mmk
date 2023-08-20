    .align 3
    .section .data
    .global _app_num
_app_num:
    .quad 2

    .global _app_datas
_app_datas:
    .quad app_0 # value defined by "quad" is always 64 bit.
    .quad app_1
    .quad app_1_end

    .global _app_names
_app_names:
    .string "initproc"
    .string "user_shell"

    .section .data
    .global app_0
    .global app_0_end
app_0:
    .incbin "../user/build/initproc.elf"
    .align 3
app_0_end:

    .section .data
    .global app_1
    .global app_1_end
app_1:
    .incbin "../user/build/user_shell.elf"
    .align 3
app_1_end:

