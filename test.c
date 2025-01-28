#include <stdio.h>
char* string = "😂";
unsigned int main(){
	printf("%c", '😂');
	printf("%s", string);
	printf("%lu", sizeof(string) / sizeof(char));
	return 0;
}
