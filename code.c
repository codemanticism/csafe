
#include <stdio.h>
#include <stdlib.h>

int is_prime(int integer){
	int length = 0;
	int* array = malloc( 1000 * sizeof(int) );
	char boolean = 0;
	for(int i = 2;i <= integer;i++){
		boolean = 1;
		for(int j = 0; j < length; j++){
			if( ( i % array[j] ) == 0 ){
				boolean = 0;
				break;
			}
		}
		if(boolean){
			array[length] = i;
			length++;
		}
		if(i == integer){
			return boolean;		
		}	
	}	
}
int main(int argn, char** args){
	if(argn  == 2){
		int number = atoi(args[1]);
		if ( is_prime(number) ){
			printf("It is prime.");
		}else{
			printf("It's not prime.");			
		}
	}else{
		printf("Use: PROGRAM [integer in decimal]");
	}
	return 0;
}
