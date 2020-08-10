
		.equ READ_CORE_TIMER,11
		.equ RESET_CORE_TIMER,12
		.equ PUT_CHAR,3
		.equ PRINT_INT, 6
		.data
		.text
		.globl main
main:   addi $sp, $sp, -4
        sw $ra, 0($sp)
while:  lui $t0, 0xBF88
        lw $t1, 0x6050($t0)
        li $a0, '\n'
        li $v0, PUT_CHAR
        move $a0, $t1
        lui $a1, 4
        li $a1, 2
        syscall
        li $a0, 500
        jal delay
        j while
        lw $ra, 0($sp)
        addi $sp, $sp, 4
        li $v0, 0
        jr $ra


delay:  # void delay(unsigned int ms)  
        li $t0, 20000
for:    ble $a0, 0, endfor      #for(; ms > 0; ms--) {
        li $v0, RESET_CORE_TIMER#
		syscall                 #resetCoreTimer();
while:  li $v0,READ_CORE_TIMER 	#
		syscall 				#
		blt $v0, $t0, while	    #while(readCoreTimer() < K);
        addi $a0, $a0, -1                        
        j for                   #}
endfor: jr $ra 