.data
var: .word 0
stack: .space 512
.text
.globl main

main:
	la $s5, stack
	li $v0, 5
	syscall
	sw $v0, var+0
	lw $s0, var+0
	sw $s0, 0($s5)
	addi $s5, $s5, 4
	add $a0, $zero, $s0
	li $v0, 1
	syscall

	li $v0, 10
	syscall
