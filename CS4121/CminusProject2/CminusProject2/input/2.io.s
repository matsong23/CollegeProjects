        .data
globals: .space 4
.newline: .asciiz "\n"
.string0: .asciiz "input an integer:"
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
        li $v0, 5
        syscall
        addi $s0, $gp, 0
        sw $v0, 0($s0)
        addi $s0, $gp, 0
        lw $s1, 0($s0)
        move $a0, $s1
        li $v0, 1
        syscall
        li  $v0, 4
        la  $a0, .newline
        syscall
        move $a0, $s1
        li $v0, 1
        syscall
        li  $v0, 4
        la  $a0, .newline
        syscall
        add $s0, $s1, $s2
        move $a0, $s3
        li $v0, 1
        syscall
        li  $v0, 4
        la  $a0, .newline
        syscall
