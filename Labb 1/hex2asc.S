  # hexmain.S
  # Written 2015-09-04 by F Lundevall
  # Copyright abandonded - this file is in the public domain.

	.text
	.globl hex2asc

hex2asc:
	li	a0, 11	# test number (from 0 to 15)
	
	addi    sp,sp,-4
	sw      ra,0(sp)
	
	jal	hexasc		# call hexasc
	
	li	a7, 11	# write a0 to stdout
	ecall

	lw      ra,0(sp)
	addi    sp,sp,4
	jr      ra	

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
  	
