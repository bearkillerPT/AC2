# TABELA DE REGISTOS:
# s = $s0 s
# cnt = $s1
# c = $s2
#start/stop = $s3
	.equ inkey, 1
        .equ get_char, 2
	.equ put_char, 3
        .equ print_int, 6
	.data
        .text
        .globl main
main:   li $s0, 0
	li $s1, 0
	li $s2, '0'
	li $s3, 1
	
do:	li $a0, '\r'
	li $v0, put_char
	syscall
	move $a0, $s1
	li $a1, 3
	sll $a1, 16
	ori $a1, $a1, 10
	li $v0, print_int
	syscall
	li $a0, '\t'
	li $v0, put_char
	syscall
	move $a0, $s1
	li $a1, 8
	sll $a1, 16
	ori $a1, $a1, 2
	li $v0, print_int
	syscall
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	li $a0, 5
	jal wait
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	li $v0, inkey
	syscall
	beq $v0, 'q', endDo
	move $s2, $v0
	bne $s2, '+', nplus
	li $s0, 0
nplus:  bne $s2, '-', nminus
	li $s0, 1
nminus:	bne $s2, 's', ns
	xor $s3, $s3, 1 
ns:	bne $s2, 'r', nr
	li $s1, 0
nr:	beq $s3, $0, do 
	bne $s0, 0, nzero
	addi $s1, $s1, 1
	j endLoop
nzero:	sub $s1, $s1, 1
	
endLoop:andi $s1, $s1, 0xFF
	j do
	
endDo:  jr $ra

wait:	li $t0, 0
	mul $t1, $a0, 515000
while:	beq $t0, $t1, return
	addi $t0, $t0, 1
	j while
return: jr $ra
	
