		.equ PUT_CHAR,3
		.equ PRINT_INT, 6
        .equ SFR_BASE_HI, 0xBF88 # 16 MSbits of SFR area
        .equ TRISE, 0x6100 # TRISE address is 0xBF886100
        .equ TRISB, 0x6040 # TRISB
        .equ LATE, 0x6120 # LATE address is 0xBF886120
        .equ PORTB, 0x6050
		.data
		.text
		.globl main
main:   
    lui $t0, SFR_BASE_HI #
    lw $t1, TRISE($t0) # READ (Mem_addr = 0xBF880000 + 0x6100)
    andi $t1, $t1, 0xFFF0 # MODIFY (bit0=0 (0 means OUTPUT))
    sw $t1, TRISE($t0) # WRITE (Write TRISE register)
    lw $t1, TRISB($t0) # READ (Mem_addr = 0xBF880000 + 0x6040)
    andi $t1, $t1, 0xFFF0 # MODIFY (bit0=0 (0 means OUTPUT))
    xori $t1, $t1, 0x000F # MODIFY ALL TO 1 == input
    sw $t1, TRISB($t0) # WRITE (Write TRISB register)

do: lw $t1, PORTB($t0)
    andi $t1, $t1, 0x000F
    xori $t1, $t1, 1
    xori $t1, $t1, 8
    sw $t1, LATE($t0)
    j do

    li $v0, 0
    jr $ra


