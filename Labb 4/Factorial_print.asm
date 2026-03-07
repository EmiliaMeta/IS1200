        .eqv N, 0   # ändra factorial här

        .text
        addi t0, zero, N     # t0 = n (yttre räknare)
        addi t2, zero, 1     # t2 = result = 1
        addi t1, zero, 1     # t1 = konstant 1

        beq  t0, zero, done
        beq  t0, t1,   done

outer:  add  t1, t0, zero    # t1 = i (inre räknare)
        add  gp, t2, zero    # gp = multiplicand = gamla result
        addi t2, zero, 0     # nollställ result

inner:  beq  t1, zero, next
        add  t2, t2, gp      # result += multiplicand
        addi t1, t1, -1      # inre räknare--
        beq  zero, zero, inner

next:   addi t0, t0, -1      # yttre räknare--
        beq  t0, x1, done    # om t0 == 1 ? klart (OBS: x1 används som konstant 1 här om du laddar det i början)
        beq  zero, zero, outer

done:
        # för RARS: printa resultat (ta bort i Logisim)
        addi a0, t2, 0
        addi a7, zero, 1
        ecall

stop:   beq zero, zero, stop
