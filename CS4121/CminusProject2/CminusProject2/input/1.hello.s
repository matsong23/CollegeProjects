        .data
globals: .space 0
.newline: .asciiz "\n"
.string0: .asciiz "Hello world!"
        .text
        .globl main
main:   nop
        la $gp, globals
        la $a0, .string0
        li $v0, 4
        syscall
        li  $v0, 4
        la  $a0, .newline
        syscall
