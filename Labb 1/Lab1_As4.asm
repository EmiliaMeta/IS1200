# Labb 1 assignment 4
# Rewrite the following C function as an assembly-language subroutine with the same behavior

#void delay( int ms ) /* Wait a number of milliseconds, specified by the parameter value. */
#{
# int i;
#	while( ms > 0 ){
#		ms = ms – 1;
#/* Executing the following for loop should take 1 ms */
#		for( i = 0; i < 4711; i = i + 1 ) /* The constant 4711 must be easy to change! */
#{
#/* Do nothing.
#		}
#	}
#}

.eqv ms, 4711


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
jr ra #eller jr 0(ra)?
