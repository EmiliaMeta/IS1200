# timetemplate.S
# Written 2015 by F Lundevall
# Ported 2024/06 by W Szczerek (from MIPS to RISC-V)
# Copyright abandonded - this file is in the public domain.

#############################################################
# Choose the macro syntax for RARS or DTEK-V board.         #
# The syntax for RARS is probably due to its MIPS heritage. #
#############################################################
###################
# Macros for RARS #
###################
#.macro	PUSH (%reg)
#	addi	sp,sp,-4
#	sw	%reg,0(sp) 
#.end_macro

#.macro	POP (%reg)
#	lw	%reg,0(sp)
#	addi	sp,sp,4
#.end_macro
###################
# Macros for DTEK #
###################
#.macro	PUSH reg
#	addi sp,sp,-4
#	sw \reg,0(sp) 
#.endm

#.macro	POP reg
#	lw	\reg,0(sp)
#	addi	sp,sp,4
#.endm
#############################################################
	.eqv ms, 16000
	.data
	.align 2
mytime:	.word 	0x5957
timstr:	.asciz 	"text more text lots of text\0"
	.text
	.globl timetemplate, tick, time2string

# Function for displaying a string with a newline at the end	
display_string:	
	li a7,4
	ecall
	li a0, 10
	li a7,11
	ecall
	jr ra
	
timetemplate:
	la	a0, timstr
	jal     display_string
	
	# wait a little
	li	a0, 2		# ms
	jal	delay
	
	# call tick
	la 	a0, mytime
	jal	tick
	
	# call your function time2string
	la	a0, timstr
	la	t0, mytime
	lw	a1, 0(t0)
	jal	time2string
	
	# go back and do it all again
	j	timetemplate

	
# tick: update time pointed to by $a0
tick:	lw	t0, 0(a0)	# get time
	addi	t0, t0, 1	# increase
	andi	t1, t0, 0xf	# check lowest digit
	sltiu	t2, t1, 0xa	# if digit < a, okay
	bnez	t2, tiend
	addi	t0, t0, 0x6	# adjust lowest digit
	
	andi	t1, t0, 0xf0	# check next digit
	sltiu	t2, t1, 0x60	# if digit < 6, okay
	bnez	t2, tiend
	addi	t0, t0, 0xa0	# adjust digit
	
	li	t3, 0xF
	slli	t3, t3, 0x8
	and	t1, t0, t3	# check minute digit
	addi	t3, x0, 0xA
	slli	t3, t3, 0x8
	slt	t2, t1, t3	# if digit < a, okay
	bnez	t2, tiend
	addi	t0, t0, 0x600	# adjust digit - this one's okay, it's lower than 0x7FF 
	
	li	t3, 0xF
	slli	t3, t3, 0xC
	and	t1, t0, t3	# check last digit
	addi	t3, x0, 0x6
	slli	t3, t3, 0xC
	slt	t2, t1, t3	# if digit < 6, okay
	bnez	t2, tiend
	
	li	t3, 0xA
	slli	t3, t3, 0xC
	add	t0, t0, t3	# adjust last digit
tiend:	sw	t0,0(a0)	# save updated result
	jr	ra		# return

#########################################################
# Place for your functions: time2string, hex2asc, delay.#
time2string: 
	
	addi  sp, sp, -4
    	sw    ra, 4(sp)
    	sw    s0,  0(sp)

    	mv    s0, a0        	# s0 = destination

    	# minuter tiotal, bitar 15:12
    	srli  t0, a1, 12
    	andi  a0, t0, 0xF
    	jal   hexasc
    	sb    a0, 0(s0)

    	# minuter ental, bitar 11:8
    	srli  t0, a1, 8
    	andi  a0, t0, 0xF
    	jal   hexasc
    	sb    a0, 1(s0)

    	# kolon
    	li    t1, ':'
    	sb    t1, 2(s0)

    	# sekunder tiotal, bitar 7:4
    	srli  t0, a1, 4
    	andi  a0, t0, 0xF
    	jal   hexasc
    	sb    a0, 3(s0)

    	# sekunder ental, bitar 3:0
    	li t3, 9
    	andi  a0, a1, 0xF
    	bne   a0, t3, NOTNINE
    	li    t4, 'N'
    	sb    t4, 4(s0)
    	li    t4, 'I'
    	sb    t4, 5(s0)
    	li    t4, 'N'
    	sb    t4, 6(s0)
    	li    t4, 'E'
    	sb    t4, 7(s0)
    	j NULL
    	
    	NOTNINE:
    	jal   hexasc
    	sb    a0, 4(s0)
    	sb	zero, 5(s0)

	# Null ASCII
	NULL:
    	sb    zero, 8(s0)

    	lw    s0,  0(sp)
    	lw    ra, 4(sp)
    	addi  sp, sp, 4
    	jr    ra
	
# You can write your own code for hexasc here
hexasc:
  	andi a0, a0, 0xF #Behåller bara värdet i de sista 4 bitarna, dvs 0-15
  	
  	#Behöver kolla om värdet i a0 är 0-9 eller 10-15
  	li t0, 10
  	blt a0, t0, lessthan10
  	
  	#Här är värdet i a0 10-15
  	li t1, 'A'
  	addi a0, a0, -10
  	add a0, a0, t1
  	andi a0, a0, 0x7F
  	
  	jr ra
  
	lessthan10:
  	#Här är värdet i a0 0-9
  	li t1, '0' #Sätter basvärdet till ASCII 0. 
  	add a0, a0, t1
  	andi a0, a0, 0x7F
  	jr ra
  	
delay:
  	li t1, ms

	while: 
	bge zero, a0, done #a0 = ms, antal millisekunders delay
	addi a0, a0, -1
	addi t0, zero, 0 # i = 0

	for: 
	bge t0, t1, while
	addi t0, t0, 1
	j for
 
	done:
	jr ra #eller jr 0(ra)
#########################################################
