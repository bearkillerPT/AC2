	.equ get_char, 2
	.equ put_char, 3
 	.data
	.text
 	.globl main
main:	li $t0, '0'

do:	li $v0, get_char
	syscall
	move $t0, $v0
	addi $t0, $t0, 5
	beq $t0, '\n', endwhile
	li $v0, put_char
	move $a0, $t0
	syscall
	j do
endwhile: jr $ra 
