.data
var: .word 0, 0
stack: .space 512
.text
.globl main

main:
	la $s5, stack
	li $v0, 5
	syscall
	sw $v0, var+4
	li $v0, 5
	syscall
	sw $v0, var+0
	lw $s0, var+4
	sw $s0, 0($s5)
	addi $s5, $s5, 4
	lw $s0, var+0
	sw $s0, 0($s5)
	addi $s5, $s5, 4
	lw $s2, -4($s5)
	lw $s1, -8($s5)
	addi $s5, $s5, -8
	addi $s0, $zero, 1
l0:	addi $s2, $s2, -1
	blt $s2, $zero, l1
	mul $s0, $s0, $s1
	j l0
l1:	sw $s0, 0($s5)
	addi $s5, $s5, 4
	add $a0, $zero, $s0
	li $v0, 1
	syscall
	addi $a0, $zero, 10
	addi $v0, $zero, 11
	syscall

	li $v0, 10
	syscall
