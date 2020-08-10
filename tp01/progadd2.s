# TABELA DE REGISTOS:
# str1 = $s0 
# str2 = $s1
# str3 = $s2
#start/stop = $s3
	.equ inkey, 1
        .equ get_char, 2
	.equ put_char, 3
        .equ print_int, 6
        .equ print_int10, 7
        .equ print_str, 8
        .equ read_str, 9
        .equ STR_MAX_SIZE, 20
	.data
q:	.asciiz "Introduza 2 strings: \n"
a:	.asciiz "\nResultados:\n"
	.align 2
str1:	.space 21
	.align 2
str2:	.space 21
	.align 2
str3:	.space 41
	.align 2
        .text
        .globl main
main:   addi $sp, $sp, -4
	sw $ra, 0($sp)
	la $s0, str1
 	la $s1, str2
  	la $s2, str3
	la $a0, q
	li $v0, print_str
	syscall
	move $a0, $s0
	li $a1, 20
	li $v0, read_str
	syscall
	li $a0, '\n'
	li $v0, put_char
	syscall
	move $a0, $s1
	li $a1, 20
	li $v0, read_str
	syscall
	la $a0, a
	li $v0, print_str
	syscall
	move $a0, $s0
	jal strlen
	move $a0, $v0
	li $a1, 10
	li $v0, print_int
	syscall
	li $a0, '\n'
	li $v0, put_char
	syscall 
	move $a0, $s1
	jal strlen
	move $a0, $v0
	li $a1, 10
	li $v0, print_int
	syscall
	move $a0, $s2
	move $a1, $s0
	jal strcpy
	li $a0, '\n'
	li $v0, put_char
	syscall
	move $a0, $s2
	move $a1, $s1
	jal strcat
	move $a0, $s2
	li $v0, print_str
	syscall
	li $a0, '\n'
	li $v0, put_char
	syscall
	move $a0, $s0
	move $a1, $s1
	jal strcmp
	move $a0, $v0
	li $v0, print_int10
	syscall
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	li $v0, 0
	jr $ra
	
strlen:	# int strlen(char *s)
	move $t0, $a0 # str*
	lb $t1, 0($t0) #str[0]
	li $t2, 0 # LEN

for:	beq $t1,  $0, endfor
	addi $t0, $t0, 1
	addi $t2, $t2, 1
	lb $t1, 0($t0)
	j for
endfor:	move $v0, $t2
	jr $ra
	
strcpy:	#char *strcpy(char *dst, char *src) 
	move $t0, $a0 #char* dst
	move $t1, $a1 #char* src
	lb $t2, 0($t1)#char src[0]
for1:	beq $t2, $0, endfor1
	sb $t2, 0($t0)
	addi $t0, $t0, 1
	addi $t1, $t1, 1
	lb $t2, 0($t1)
	j for1
	
endfor1:addi $t0, $t0, 1
	sb $0, 0($t0)
	move $v0, $a0
	jr $ra	

strcat:	#char *strcat(char *dst, char *src) 
	addi $sp, $sp, -8
	sw $ra, 0($sp)
	sw $a0, 4($sp)
	move $t0, $a0 #char* dst
	move $t1, $a1 #char* src
	lb $t2, 0($t0)
for2:	beq $t2, $0, endfor2
	addi $t0, $t0, 1
	lb $t2, 0($t0)
	j for2
	
endfor2:move $a0, $t0
	move $a1, $t1
	jal strcpy
	lw $ra, 0($sp)
	lw $v0, 4($sp)
	addi $sp, $sp, 8
	jr $ra	
	
strcmp: move $t0 ,$a0
	move $t1, $a1
	lb $t2, 0($t0)
	lb $t3, 0($t1)
for3:	bne $t2, $t3, endfor3
	beq $t2, $0, endequal
	addi $t0, $t0, 1
	addi $t1, $t1, 1
	lb $t2, 0($t0)
	lb $t3, 0($t1)
	j for3
	
endfor3:sub $t2, $t3, $t2
	move $v0, $t2
	jr $ra 

endequal:li $v0, 0
	jr $ra