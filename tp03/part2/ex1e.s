		.equ PUT_CHAR,3
		.equ PRINT_INT, 6
        .equ SFR_BASE_HI, 0xBF88 # 16 MSbits of SFR area
        .equ TRISE, 0x6100 # TRISE address is 0xBF886100
        .equ LATE, 0x6120 # LATE address is 0xBF886120
        .equ READ_CORE_TIMER,11
		.equ RESET_CORE_TIMER,12
		.data
		.text
		.globl main
main:   
    lui $s0, SFR_BASE_HI #
    lw $s1, TRISE($s0) # READ (Mem_addr = 0xBF880000 + 0x6100)
    andi $s1, $s1, 0xFFF0 # MODIFY (bit0,1,2,3=0 (0 means OUTPUT))
    sw $s1, TRISE($s0) # WRITE (Write TRISE register)
    li $s1, 0 #(cada um dos 4 LSB = 1 led)

do: 
    andi $s3, $s1, 0x0001
    addi $s3, $s3, 0xFFFE
    nor $s3, $s3, $0
    srl $s1, $s1, 1
    sll $s3, $s3, 3
    add $s1, $s1, $s3
    move $a0, $s1
    lw $s2, LATE($s0)
    andi $s2, $s2, 0xFFF0
    add $s2, $s2, $s1
    sw $s2, LATE($s0)
    li $a0, 666
    jal delay
    j do

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
