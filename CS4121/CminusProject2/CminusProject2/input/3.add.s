        .data
globals: .space 16
.newline: .asciiz "\n"
        .text
        .globl main
main:   nop
        la $gp, globals
        li $s0, 10
        li $s1, 20
        add $s2, $s0, $s1
        move $a0, $s2
        li $v0, 1
        syscall
        li  $v0, 4
        la  $a0, .newline
        syscall
        li $s0, 1
        addi $s1, $gp, 0
        sw $s0, 0($s1)
        li $s0, 3
        addi $s1, $gp, 8
        sw $s0, 0($s1)
        li $s0, 4
        addi $s1, $gp, 12
        sw $s0, 0($s1)
        addi $s0, $gp, 0
        lw $s1, 0($s0)
        addi $s0, $gp, 8
        lw $s3, 0($s0)
        add $s0, $s1, $s3
        addi $s1, $gp, 12
        lw $s3, 0($s1)
        add $s1, $s0, $s3
        addi $s0, $gp, 4
        sw $s1, 0($s0)
        addi $s0, $gp, 4
        lw $s1, 0($s0)
        move $a0, $s1
        li $v0, 1
        syscall
        li  $v0, 4
        la  $a0, .newline
        syscall
