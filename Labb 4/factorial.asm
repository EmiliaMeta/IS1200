        addi t0, zero, 8   # t0 = n (yttre räknare)
        addi t2, zero, 1     # result = 1
        addi gp, zero, 1     # gp = konstant 1

        beq  t0, zero, done  # om n=0 -> klart
        beq  t0, gp,   done  # om n=1 -> klart

outer:  add  t1, t0, zero    # t1 = i (inre räknare)
        add  ra, t2, zero    # ra = multiplicand = gamla result
        addi t2, zero, 0     # nollställ result (för ny produkt)

inner:  beq  t1, zero, next
        add  t2, t2, ra      # result += multiplicand
        addi t1, t1, -1      # inre räknare
        beq  zero, zero, inner

next:   addi t0, t0, -1      # yttre räknare
        beq  t0, gp, done    # om t0 == 1 -> klart
        beq  zero, zero, outer

done:   beq  zero, zero, done  # evig stop-loop


