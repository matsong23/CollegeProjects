	.data
.newline: .asciiz "\n"
	.text
	.globl main
main:	nop
	move	$fp,$sp
	add $s0, $gp, 4
	li $s1, 0
	sw $s1, 0($s0)
	add $s0, $gp, 8
	add $s1, $gp, 4
	lw $s2, 0($s1)
	li $s1, 2
	sub $s3, $s2, $s1
	sw $s3, 0($s0)
	add $s0, $gp, 4
	lw $s1, 0($s0)
	beq $s1, $0, else0
	li $s0, 1
	move $a0, $s0
	li $v0, 1
	syscall
	li $v0, 4
	la, $a0, .newline
	syscall
	b endif0
else0:	nop
	li $s0, 0
	move $a0, $s0
	li $v0, 1
	syscall
	li $v0, 4
	la, $a0, .newline
	syscall
endif0:	nop
	add $s0, $gp, 8
	lw $s2, 0($s0)
	beq $s2, $0, else1
	add $s0, $gp, 4
	lw $s3, 0($s0)
	beq $s3, $0, else2
	li $s0, 0
	move $a0, $s0
	li $v0, 1
	syscall
	li $v0, 4
	la, $a0, .newline
	syscall
	b endif2
else2:	nop
	li $s0, 1
	move $a0, $s0
	li $v0, 1
	syscall
	li $v0, 4
	la, $a0, .newline
	syscall
endif2:	nop
	b endif1
else1:	nop
	li $s0, 0
	move $a0, $s0
	li $v0, 1
	syscall
	li $v0, 4
	la, $a0, .newline
	syscall
endif1:	nop
	li $v0, 10
	syscall
