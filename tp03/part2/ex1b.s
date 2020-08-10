		.equ PUT_CHAR,3
		.equ PRINT_INT, 6
        .equ SFR_BASE_HI, 0xBF88 # 16 MSbits of SFR area
        .equ TRISE, 0x6100 # TRISE address is 0xBF886100
        .equ TRISB, 0x6040 # TRISB
        .equ LATE, 0x6120 # LATE address is 0xBF886120
        .equ PORTB, 0x6050
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
    lw $s1, TRISB($s0) # READ (Mem_addr = 0xBF880000 + 0x6040)
    andi $s1, $s1, 0xFFF0 # MODIFY (bit0,1,2,3=0 (0 means OUTPUT))
    xori $s1, $s1, 0x000F # MODIFY ALL TO 1 == input
    sw $s1, TRISB($s0) # WRITE (Write TRISB register)
    li $s1, 15
do: lw $s2, LATE($s0)
    andi $s2, $s2, 0xFFF0
    add $s2, $s2, $s1
    sw $s2, LATE($s0)
    li $a0, 250
    jal delay
    bne $s1, $0, overf
    li $s1, 15
overf: 
    addi $s1, $s1, -1
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
