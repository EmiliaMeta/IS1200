.text
	addi t0,zero,4     	#t0=4	# set t0 to 4
	addi t2,t0,2	  	#t2=6	# test addi, set t2
	add  t2,t2,t0  		#t2=10	# start of counter. Should be 10
loop:	beq  t2,t0,done 	#t2=t0 => jump to done. 
        addi t2,t2,-3   	#t2=7	# decrement. Loops twice.
        beq  zero,zero,loop   	#jump to loop
done:	add  zero,zero,zero	#NOP	
	
