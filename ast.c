#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct ast{
	unsigned char* data;
	unsigned int length;
	unsigned int capacity;	
};
typedef unsigned char id;
struct map{
	struct ast tree;	
	//Variables
	//Functions
};
FILE *file;
const char keywords[] = "else for while do if switch;int char short long float double;const volatile restrict;unsigned signed;auto register static extern;struct union;inline;typedef;include define;void;enum;goto return;break continue";
/*
variable => type, name
Atomic types in C

TYPES SIGN SPECIFIER QUALIFIER 
___   _    __        __
lowest               highest    
*/
id identify(char* string){
	id result = 0;
 	if((string[0] < 'a') || (string[0] > 'z')){
		return 0xFF;
	}
	for(unsigned int i = 0; keywords[i] != '\0';){
		for(unsigned int j = i; (keywords[j] >= 'a') && (keywords[j] <= 'z');j++){
			printf("%c", keywords[j]);
		}
		printf("\n");
		for(unsigned int j = 0; (string[j] >= 'a') && (string[j] <= 'z');j++){
			printf("%c", string[j]);
		}
		printf("\n");
		for(unsigned int j = 0; string[j] != '\0';j++){
			if( ( keywords[i] == ' ' ) || ( keywords[i] == ';') || (keywords[i] == '\0')){
				if((string[j] < 'a') || (string[j] > 'z')){
					return result; 
				}
			}else if(keywords[i] != string[j]){	
				break;
			}
			i++;
		}
		for(;1;i++){
			if(keywords[i] == ';'){
				result += 0x10;
				result = result-(result&0x0F);
				i++;
				break;	
			}else if(keywords[i] == ' '){
				result++;
				i++;
				break;	
			}else if(keywords[i] == '\0'){
				return 0xFF;
			}
		}
	}
	return 0xFF;
}
int octal(char* code){
	unsigned int number = 0;
	for(; ( (*code) < 'a' ) || ( (*code) > 'a' );code++){
		
	}
}
int character(char* code){
	if((*code) == '\\'){
		switch((*code)){
			case 'a':
				break;
			case 'b':
				break;
			case 'f':
				break;
			case 'n':
				break;
			case 'r':
				break;
			case 't':
				break;
			case 'v':
				break;
			case '\\':
				break;
			case '\'':
				break;
			case '\"':
				break;
			case '?':
				break;
			default:
				break;
		}
	}else{
		
	}
}
int string(char* code, map info){
	//info.tree.add();
	//''
	for( ;c; ){
		
	}
}
int expression(char* code, map info){
	unsigned int nestlevel = 1;
	char* copy = code;
	for(;nestlevel == 0;copy++){
		if(){
			
		}else if{
			
		}else if {
			
		}
		//info.tree.add();
	}
}
int oneliner(char* code, const char byte, map info){
	switch(byte){
		case 0xB0: //goto 
			//Leave this empty for now
			break;
		case 0xB1: //return
			
			break;
		case 0xC0: //break

			break;
		case 0xC1: //continue
		
			break;
		case 0xFF:
			break;
		default:
			exit(1);
	}
	//function call
	//declaration
	//assignment
	//goto
	//return
	//break
	//return
}
int process(char* code, map info){
	//include define;void;enum
	for(; (*code) != '\0'; code++){
		char byte = identify(code);
		switch((byte & 0xF0) / 0x10){
			case 0: //loops
				const char value = byte & 0x0F;
				if(byte != 0x00){
					for(;(*code) != '(';code++){}
					if(byte != 0x01){
											
					}
				}
				switch(value){ 
					
				}
				for(;(*code) != '{';code++){}
			
				break;
			case 1: //essential type words that are not modifiers
				switch(byte & 0x0F){
					
				}
				break;
			case 2: //type prefixes that take the place of "const"
				switch(byte & 0x0F){
					
				}
				break;
			case 3: //can a variable of that type have a negative value
				switch(byte & 0x0F){
					
				}
				break;
			case 4: //type words that take the place of "auto" and "register"
				switch(byte & 0x0F){
					
				}	
				break;
			case 5: //struct or union
				switch(byte & 0x0F){
					
				}
				break;
			case 6: //inline keyword
				switch(byte & 0x0F){
					
				}
				break;
			case 7: //typedef
				switch(byte & 0x0F){
					
				}
				break;
			case 8: //include or define
				switch(byte & 0x0F){
					
				}
				break;
			case 9: //void
				switch(byte & 0x0F){
					
				}
				break;
			case 10: //enum
				switch(byte & 0x0F){
					
				}
				break;
			
			default:
			
		}
	}
}
int main(){
	unsigned int filesize = ftell(file);
	file = fopen("code.c", "r");
	fseek(file, 0, SEEK_END);
	fseek(file, 0, SEEK_SET);
	char* address = malloc(sizeof(char) * (filesize + 1));
	printf("%u", filesize);
	fread(address,1,filesize,file);
//	printf("%s", address);
//	printf("%s", keywords);
	//printf("%u", filesize);
	//printf("%s", address);
	char* offset = address;
	return 0;
	/*for(unsigned int i = 0; i < ; i++){
		identified = identify(f);
		if (  ){
			fseek()			
		}
	}*/
}
