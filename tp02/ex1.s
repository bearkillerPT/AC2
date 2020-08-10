		
		.equ READ_CORE_TIMER,11
		.equ RESET_CORE_TIMER,12
		.equ PUT_CHAR,3
		.equ PRINT_INT, 6
		.equ Co0F, 20000000
		.data
		.text
		.globl main
main: 	li $t0,0
		li $t2, Co0F
		li $t3, 100   			#Desired counter frequency
		div $t1, $t2, $t3
while: 	li $v0,READ_CORE_TIMER 	# while (1) {
		syscall 				#
		blt $v0, $t1, while	# while(readCoreTimer() < 200000);
		li $v0, RESET_CORE_TIMER#
		syscall 				# resetCoreTimer();
		li $a0,' ' 				#
		li $v0,PUT_CHAR 		#
		syscall 				# putChar(' ');
		addi $t0,$t0, 1 		#
		move $a0,$t0 			#
		li $a1,10 				#
		li $v0, PRINT_INT		#
		syscall 				# printInt(++counter, 10);
		j while 				# }
		li $v0, 0
		jr $ra 					# 
		