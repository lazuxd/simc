.data
var: .word 0, 5, 35, 1
stack: .space 512
.text
.globl main

main:
	la $s5, stack
	lw $s0, var+4
	sw $s0, 0($s5)
	addi $s5, $s5, 4
	sw $s0, var+0
l0:	lw $s0, var+0
	sw $s0, 0($s5)
	addi $s5, $s5, 4
	lw $s0, var+8
	sw $s0, 0($s5)
	addi $s5, $s5, 4
	lw $s2, -4($s5)
	lw $s1, -8($s5)
	addi $s5, $s5, -8
	slt $s0, $s1, $s2
	sw $s0, 0($s5)
	addi $s5, $s5, 4
	beq $zero, $s0, l1
	lw $s0, var+0
	sw $s0, 0($s5)
	addi $s5, $s5, 4
	add $a0, $zero, $s0
	li $v0, 1
	syscall
	addi $a0, $zero, 10
	addi $v0, $zero, 11
	syscall
	lw $s0, var+0
	sw $s0, 0($s5)
	addi $s5, $s5, 4
	lw $s0, var+12
	sw $s0, 0($s5)
	addi $s5, $s5, 4
	lw $s2, -4($s5)
	lw $s1, -8($s5)
	addi $s5, $s5, -8
	add $s0, $s1, $s2
	sw $s0, 0($s5)
	addi $s5, $s5, 4
	sw $s0, var+0

	j l0
l1:
	li $v0, 10
	syscall
