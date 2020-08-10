	.equ inkey, 1        
	.equ put_char, 3
        .data
        .text
        .globl main
main:   li $t0, '0'

do:     
waitKey:li $v0, inkey
        syscall
        beq $v0, $0, waitKey
	move $t0, $v0
        addi $t0, $t0, 5
        beq $t0, '\n', endwhile
        li $v0, put_char
        move $a0, $t0
        syscall
        j do
endwhile: jr $ra
