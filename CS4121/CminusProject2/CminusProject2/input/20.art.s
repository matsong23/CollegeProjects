        .text
        .globl main
main:   nop
        move $fp, $sp
        sub $sp, $sp, 0
        add newreg, reg, reg
        move $a0, $s3
        li $v0, 1
        syscall
        li  $v0, 4
        la  $a0, .newline
        syscall
        sub newreg, reg, reg
        move $a0, $s1
        li $v0, 1
        syscall
        li  $v0, 4
        la  $a0, .newline
        syscall
