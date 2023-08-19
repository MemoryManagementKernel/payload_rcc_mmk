    .section .apps
    .align 3

    .global _app_num
_app_num:
    .quad 2,2,2

    .global _app_data
_app_data:
    .quad app_0 # value defined by "quad" is always 64 bit.
    .quad app_1
    .quad app_1_end

    .global _app_names
_app_names:
    .align 5
    .string "initproc"
    .align 5
    .string "user_shell"
    .align 5
    .string "none"
    .align 5

app_0:
    .incbin "../user/build/initproc.elf"
    .align 3
app_0_end:
    
app_1:
    .incbin "../user/build/user_shell.elf"
    .align 3
app_1_end:

