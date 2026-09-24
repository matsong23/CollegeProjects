	.data
.newline: .asciiz "\n"
	.align 2
arraya: .space 40
	.text
	.globl main
main:	nop
	move	$fp,$sp
	li $s0, 3
	la $s1, arraya
	sll $s2, $s0, 2
	add $s1, $s1, $s2
	li $s0, 4
	sw $s0, 0($s1)
	li $s0, 3
	la $s1, arraya
	sll $s3, $s0, 2
	add $s1, $s1, $s3
	lw $s0, 0($s1)
	move $a0, $s0
	li $v0, 1
	syscall
	li $v0, 4
	la, $a0, .newline
	syscall
	li $v0, 10
	syscall
