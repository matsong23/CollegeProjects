        .data
.newline: .asciiz "\n"
.string0: .asciiz "Your grader"
.string1: .asciiz "will put"
.string2: .asciiz "a random"
.string3: .asciiz "string here"
        .text
        .globl main
main:   nop
        move $fp, $sp
        sub $sp, $sp, -4
        la $a0, .string0
        li $v0, 4
        syscall
        li  $v0, 4
        la  $a0, .newline
        syscall
        la $a0, .string1
        li $v0, 4
        syscall
        li  $v0, 4
        la  $a0, .newline
        syscall
        la $a0, .string2
        li $v0, 4
        syscall
        li  $v0, 4
        la  $a0, .newline
        syscall
        la $a0, .string3
        li $v0, 4
        syscall
        li  $v0, 4
        la  $a0, .newline
        syscall
