  # hexmain.S
  # Written 2015-09-04 by F Lundevall
  # Copyright abandonded - this file is in the public domain.

	.text
	.globl hex2asc
#hexa till assci
hex2asc:
	li	a0, 12                        # test number (from 0 to 15), li laddar konstanten "det man väljer" i a0,
	
	addi    sp,sp,-4                        # sp = stack pointer (callee saved) flyttar pekaren neråt i minnet, skapar plats i stacken för att spara något
	sw      ra,0(sp)                        #vi sparar returaddressen  på stacken då vi vill anropa hexac och vill återvända korrekt sen , 
	
	jal	hexasc		                # call hexasc, konverterar talet i a0 till ASCII, 
	
	li	a7, 11	                        # printar
	ecall

	lw      ra,0(sp)                        #Läser tillbaka den sparade returadressen från stacken till ra, så att funktionen kan returnera till rätt ställe.
	addi    sp,sp,4                         # flyttar pekaren uppåt, alltså återställer stackpekaren 
	jr      ra	                 

  # You can write your own code for hexasc here
  
  
  #funktion i assembly som övversätter ett heltal (0-15) till motsvarande ASCII
  hexasc: 
  
  andi a0,a0, 0xf                               #maskerar de 4 sista/lägsta bitarna i addressen dvs garanterar 0 till 15 i a0, 0x17 & 0xf
  
  #vi behöver kolla om värdet är mellan 0-9 eller 10 - 15
  
  li  t0, 10 
  blt  a0, t0, lessthan10                       # 0m a0 < 10 hoppar vi till labeln lesthan10 annars hoppar vi till nästa rad, som en if-else
  
  
  #här har vi värdet mellan 10-15
  li t1, 'A'                                    #laddar ASCII 65 
  addi a0, a0, -10                              # tar bor 10 från inpt a0, tex a0 = 12 , 12-10 = 2 från A tar vi två steg = c, eller 65 till 67 = c
  add a0, a0, t1                                #adderar det till 'A' så vi får ASCII-koden  för 'A'–'F'
  andi a0, a0, 0x7F                             #maskar bort allt utom de 7 lägsta bitarna (så vi får en giltig ASCII), 127
  jr ra 
  
  
  lessthan10:
  addi a0, a0, 48                               #48 = 0, tex a0 = 7 --> 7 + 48 = 55 som motsvarar 7
  jr ra                                         #hoppar tillbaka till hex2asc med resultatet i a0
  
  
