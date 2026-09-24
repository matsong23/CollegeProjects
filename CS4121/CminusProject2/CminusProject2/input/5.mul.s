        .data
globals: .space 16
.newline: .asciiz "\n"
        .text
        .globl main
main:   nop
        la $gp, globals
        li $s0, 10
        li $s1, 20
        mult $s0, $s1
        mflo $s2
        move $a0, $s2
        li $v0, 1
        syscall
        li  $v0, 4
        la  $a0, .newline
        syscall
        li $s0, 7
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
        mult $s1, $s3
        mflo $s0
        addi $s1, $gp, 12
        lw $s3, 0($s1)
        mult $s0, $s3
        mflo $s1
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
