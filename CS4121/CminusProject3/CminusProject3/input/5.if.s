	.data
.newline: .asciiz "\n"
.string0: .asciiz "enter a:"
.string1: .asciiz "enter b:"
.string2: .asciiz "a = "
.string3: .asciiz "b = "
.string4: .asciiz "a = "
.string5: .asciiz "b = "
	.text
	.globl main
main:	nop
	move	$fp,$sp
	la $s0, .string0
	move $a0, $s0
	li $v0, 4
	syscall
	li $v0, 4
	la, $a0, .newline
	syscall
	add $s0, $gp, 4
	li $v0, 5
	syscall
	sw $v0, 0($s0)
	la $s0, .string1
	move $a0, $s0
	li $v0, 4
	syscall
	li $v0, 4
	la, $a0, .newline
	syscall
	add $s0, $gp, 8
	li $v0, 5
	syscall
	sw $v0, 0($s0)
	add $s0, $gp, 8
	lw $s1, 0($s0)
	add $s0, $gp, 4
	lw $s2, 0($s0)
	sgt $s0, $s1, $s2
	beq $s0, $0, else0
	add $s1, $gp, 4
	lw $s2, 0($s1)
	li $s1, 0
	sgt $s3, $s2, $s1
	beq $s3, $0, else1
	la $s1, .string2
	move $a0, $s1
	li $v0, 4
	syscall
	li $v0, 4
	la, $a0, .newline
	syscall
	add $s1, $gp, 4
	lw $s2, 0($s1)
	move $a0, $s2
	li $v0, 1
	syscall
	li $v0, 4
	la, $a0, .newline
	syscall
	b endif1
else1:	nop
	la $s1, .string3
	move $a0, $s1
	li $v0, 4
	syscall
	li $v0, 4
	la, $a0, .newline
	syscall
	add $s1, $gp, 8
	lw $s2, 0($s1)
	move $a0, $s2
	li $v0, 1
	syscall
	li $v0, 4
	la, $a0, .newline
	syscall
endif1:	nop
	b endif0
else0:	nop
	add $s1, $gp, 4
	lw $s2, 0($s1)
	li $s1, 0
	slt $s4, $s2, $s1
	beq $s4, $0, else2
	la $s1, .string4
	move $a0, $s1
	li $v0, 4
	syscall
	li $v0, 4
	la, $a0, .newline
	syscall
	add $s1, $gp, 4
	lw $s2, 0($s1)
	move $a0, $s2
	li $v0, 1
	syscall
	li $v0, 4
	la, $a0, .newline
	syscall
	b endif2
else2:	nop
	la $s1, .string5
	move $a0, $s1
	li $v0, 4
	syscall
	li $v0, 4
	la, $a0, .newline
	syscall
	add $s1, $gp, 8
	lw $s2, 0($s1)
	move $a0, $s2
	li $v0, 1
	syscall
	li $v0, 4
	la, $a0, .newline
	syscall
endif2:	nop
endif0:	nop
	li $v0, 10
	syscall
