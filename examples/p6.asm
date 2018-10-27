.data
var: .word 0, 10
stack: .space 512
.text
.globl main

main:
	la $s5, stack
	lw $s0, var+4
	sw $s0, 0($s5)
	addi $s5, $s5, 4
	sw $s0, var+0
	lw $s0, var+0
	sw $s0, 0($s5)
	addi $s5, $s5, 4
	add $a0, $zero, $s0
	li $v0, 1
	syscall

	li $v0, 10
	syscall
