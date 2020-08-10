		
		.equ READ_CORE_TIMER,11
		.equ RESET_CORE_TIMER,12
		.equ PUT_CHAR,3
		.equ PRINT_INT, 6
		.equ Co0F, 20000000
		.data
		.text
		.globl main
main: 	addi $sp, $sp, -4
        sw $ra, 0($sp)
        li $s0,0
		li $s2, Co0F
		li $s3, 100   			#Desired counter frequency
		div $s1, $s2, $s3
while: 	li $v0,READ_CORE_TIMER 	# while (1) {
		syscall 				#
		blt $v0, $s1, while	# while(readCoreTimer() < 200000);
		li $v0, RESET_CORE_TIMER#
		syscall 				# resetCoreTimer();
		li $a0,' ' 				#
		li $v0,PUT_CHAR 		#
		syscall 				# putChar(' ');
		addi $s0,$s0, 1 		#
		move $a0,$s0 			#
		li $a1,10 				#
		li $v0, PRINT_INT		#
		syscall 				# printInt(++counter, 10);
		li $a0, 10
        jal delay               # delay(10 ms)
        j while 				# }
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