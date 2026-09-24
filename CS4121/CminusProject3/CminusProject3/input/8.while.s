	.data
.newline: .asciiz "\n"
	.text
	.globl main
main:	nop
	move	$fp,$sp
	add $s0, $gp, 8
	li $s1, 1
	sw $s1, 0($s0)
startLoop0:	nop
	add $s0, $gp, 8
	lw $s1, 0($s0)
	li $s0, 10
	sle $s2, $s1, $s0
	beq $s2, $0, endLoop0
	add $s0, $gp, 4
	li $s1, 1
	sw $s1, 0($s0)
startLoop1:	nop
	add $s0, $gp, 4
	lw $s1, 0($s0)
	li $s0, 10
	sle $s3, $s1, $s0
	beq $s3, $0, endLoop1
	add $s0, $gp, 4
	lw $s1, 0($s0)
	add $s0, $gp, 8
	lw $s4, 0($s0)
	mul $s0, $s1, $s4
	move $a0, $s0
	li $v0, 1
	syscall
	li $v0, 4
	la, $a0, .newline
	syscall
	add $s0, $gp, 4
	add $s1, $gp, 4
	lw $s4, 0($s1)
	li $s1, 1
	add $s5, $s4, $s1
	sw $s5, 0($s0)
	b startLoop1
endLoop1:	nop
	add $s0, $gp, 8
	add $s1, $gp, 8
	lw $s4, 0($s1)
	li $s1, 1
	add $s5, $s4, $s1
	sw $s5, 0($s0)
	b startLoop0
endLoop0:	nop
	li $v0, 10
	syscall
