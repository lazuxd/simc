.data
var: .word 0
stack: .space 512
.text
.globl main

main:
	la $s5, stack


	li $v0, 10
	syscall
