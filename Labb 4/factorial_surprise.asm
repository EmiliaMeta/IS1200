        addi t0, zero, 4   # t0 = n (yttre räknare)
        addi t2, zero, 1    # result = 1
        addi gp, zero, 1    # gp = konstant 1

        beq  t0, zero, done  # om n=0 -> klart
        beq  t0, gp,   done  # om n=1 -> klart
        
loop: 	beq  t0, gp, done
	addi gp, gp, 1  
	mul  t2, t2, gp
	beq zero, zero, loop
	
done:   beq  zero, zero, done  # evig stop-loop


