	.equ print_str, 8
	.equ read_int10, 5
        .equ print_int, 6
	.equ print_int10, 7
	.data
in:     .asciiz "\nIntroduza um numero (sinal e modulo): "
b2:	.asciiz "\nValor lido, em base 2: "
b16:	.asciiz "\nValor lido, em base 16: "
b10u:	.asciiz "\nValor lido, em base 10 (unsigned): "
b10:	.asciiz "\nValor lido, em base 10 (signed): "
	.text
        .globl main
main:   li $t0, 0

do:	la $a0, in
	li $v0, print_str
	syscall
	li $v0, read_int10
	syscall
	move $t0, $v0
	#beq $t0, '\n', endwhile
	la $a0, b2
	li $v0, print_str
	syscall
	move $a0, $t0
	li $a1, 2
	li $v0, print_int
	syscall
     	la $a0, b16
	li $v0, print_str
	syscall
        move $a0, $t0
        li $a1, 16
	li $v0, print_int
        syscall
	la $a0, b10u
	li $v0, print_str
	syscall
	move $a0, $t0
	li $a1, 10
	li $v0, print_int
	syscall
	la $a0, b10
	li $v0, print_str
	syscall
	move $a0, $t0
	li $v0, print_int10
	syscall
	j do
endwhile: jr $ra
