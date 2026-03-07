//Inom print_sieves

//Skriver ut hur många gånger man hittat primtal med skillnad 4
/*
for (i = 2; i <= n; i++) {

		if (numbers[i] != 0) {	// if a given i is set to true in numbers, print it

			print_number(i);

			if (i - previous == 4) { // if i - previous yields 4 add 1 to counter
				count++;
			}

			previous = i;	// set previous to current i before exting iteration
		}
	}

	printf("\n\nThis many occurences of B-A == 4: %d\n\n", count);
*/


//Printar medelavståndet mellan primtal
/*
    int temp = 0;
    int avstand = 0;
    for(int i = 2; i <= array_size; i++)
    {
        if(numbers[i] == 1)
        {
          print_numbers(i); // från förut
          avstand += i - temp;
          temp = i;
        }
    }
    avstand /= column_count;
    printf("\n Medelavståndet mellan primtalen är %d ", avstand);
*/

//print out how many occurences of a length of 4 between the primes.
/* 
    int steps = 0;
    int x = 0; // prime x < y
    for(int y = 2; y <= array_size; y++){
      if(numbers[y] == 1){ // When we get to next prime
        if(y - x == 4){
          steps++;
        }
        x = y; // assign x to y so we calculate from there next time
      }
    }
    printf("\n\nNumber of occurences with 4 steps between primes: %d\n", steps);
*/