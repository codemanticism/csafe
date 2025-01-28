#define LIMIT_32_BITS 0x100000000
#define LIMIT_64_BITS_DIVIDED_BY_2 0x1000000000000000
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "astp.c" //Pre-processer stuff
struct ast{
	unsigned char* data;
	unsigned int length;
	unsigned int capacity;	
};
typedef unsigned int type;
typedef unsigned char id;
typedef char* somewhere;
unsigned int linenum = 0;
unsigned int column = 0;
struct var{
	char* name; 
	type some_type;
	var* next;
};
struct map{
	struct ast tree;	
	//Variables
	//Functions
};
FILE *file;
const char keywords[] = "else for while do if switch;int char short long float double;const volatile restrict;unsigned signed;auto register static extern;struct union;inline;typedef;include define;void;enum;goto return;break continue";
/*
ASSIGNMENT OPERATORS
 OPERATORS
0xD0 => !
0xD1 => <<
0xD2 => >>
0xD3 => ^
0xD4 => ~
0xE0 => +
0xE1 => -
0xE2 => *
0xE3 => /
0xE4 => &
0xE5 => |
0xE6 => 
0xE7 => &&
0xE8 => ||
0xE9 => ==
0xEA => !=
0xEB => >
0xEC => =
0xED => <
0xEE => <=
0xEF => >=
variable => type, name
Atomic types in C

TYPES SIGN SPECIFIER QUALIFIER 
___   _    __        __
lowest               highest    
*/
void add_unsigned_long_to_ast(ast tree, unsigned long integer){
	if((tree.length + 1) > tree.capacity){
		tree.data = malloc(tree.capacity * sizeof(unsigned long) * 2);
	}
	tree.data[tree.length] = integer;
	tree.length += sizeof(unsigned long);
}
void add_unsigned_int_to_ast(ast tree, unsigned int integer){
	if((tree.length + 1) > tree.capacity){
		tree.data = malloc(tree.capacity * sizeof(unsigned int) * 2);
	}
	tree.data[tree.length] = integer;
	tree.length += sizeof(unsigned int);
}
void add_unsigned_short_to_ast(ast tree, unsigned short integer){
	if((tree.length + 1) > tree.capacity){
		tree.data = malloc(tree.capacity * sizeof(unsigned short) * 2);
	}
	tree.data[tree.length] = integer;
	tree.length += sizeof(unsigned short);
}
void add_to_ast(ast tree, char byte){
	if((tree.length + 1) > tree.capacity){
		tree.data = malloc(tree.capacity * sizeof(char) * 2);
	}
	tree.data[tree.length] = byte;
	tree.length++;
}
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
struct rFunc{
	unsigned int number;
	char* address;	
};
char* unicode(map info, unsigned int codepoint){
	if(codepoint < 0x80){
		add_to_ast(info.map, codepoint);
	}else if((codepoint >= 0x80) && (codepoint <= 0x7FF)){
		unsigned char calc = (codepoint - 0x80);
		add_to_ast(info.map, (calc & 0b00011111) + 0b11000000);
		add_to_ast(info.map, ((calc >> 6) & 0b00111111) + 0b10000000);
	}else if((codepoint >= 0x800) && (codepoint <= 0xFFFF)){
		unsigned char calc = (codepoint - 0x800);
		add_to_ast(info.map, (calc & 0b00001111) + 0b11100000);
		add_to_ast(info.map, ((calc >> 4) & 0b00111111) + 0b10000000);
		add_to_ast(info.map, ((calc >> 10) & 0b00111111) + 0b10000000);
	}else if((codepoint >= 0x10000) && (codepoint <= 0x10FFFF)){
		unsigned char calc = (codepoint - 0x10000);
		add_to_ast(info.map, (calc & 0b00000111) + 0b11110000);
		add_to_ast(info.map, ((calc >> 3) & 0b00111111) + 0b10000000);
		add_to_ast(info.map, ((calc >> 9) & 0b00111111) + 0b10000000);
		add_to_ast(info.map, ((calc >> 15) & 0b00111111) + 0b10000000);
	}
}
unsigned long dec(char* code){
	unsigned long number = 0;
	for(;1;code++){
		if((*code >= '0') && (*code <= '9')){
			number = number * 10;
			number += (*code) - '0';
		}else{
			return number;
		}
	}
}
unsigned long bin(char* code){
	unsigned long number = 0;
	for(;1;code++){
		if((*code) == '0'){
			number = number * 2;
		}else if((*code) == '1'){
			number = number * 2;
			number++;
		}else{
			return number;
		}
	}
}
unsigned long hex(char* code){
	unsigned long number = 0;
	for(;1;code++){
		if((*code >= 'a') && (*code <= 'z')){
			number = number * 16;
			number += ((*code) - 'a') + 10;
		}else if((*code >= '0') && (*code <= '9')){
			number = number * 16;
			number += (*code) - '0';
		}else{
			return number;
		}
	}
}
rFunc oct(char* code){
	unsigned int number = 0;
	for(;1;code++){
		if((*code >= '0') && (*code <= '7')){
			number = number * 8;
			number += (*code) - '0';
		}else{
			return {code, number};
		}
	}
}
char* character(char* code){
	if((*code) == '\\'){
		code--;
		switch((*code)){
			case 'a':
				return code - 1;
			case 'b':
				return code - 1;
			case 'f':
				return code - 1;
			case 'n':
				return code - 1;
			case 'r':
				return code - 1;
			case 't':
				return code - 1;
			case 'v':
				return code - 1;
			case '\\':
				return code - 1;
			case '\'':
				return code - 1;
			case '\"':
				return code - 1;
			case '?':
				return code - 1;
			case 'x':
				rFunc return_value = hex(code - 1);							
				return return_value; 
			case 'u':				
				rFunc return_value = hex(code - 1);			
				return {unicode(return_value.number), return_value.address};
			case 'U':
				rFunc return_value = hex(code - 1);			
				return {unicode(return_value.number), return_value.address};
			default:
				if((*code) >= '0'){
					if((*code) <= '9'){
						return octal(code);		
					}
				}
				return {'\\', (code - 1)};
		}
	}
}
rFunc char_func(char* code, map info){
	character();
} 
void error(char* message){
	printf("[ERROR]", linenum);
	printf(":");
	printf("%u", column);
	printf(" ");
	printf(message);
	exit(1);
}
unsigned int validate_char(char* code){
	rFunc return_value = character(code*);
	if((return_value.address*) == '\''){
		return return_value.number;	
	}else{
		error("A char cannot hold more than a single character.");
	}
}
rFunc numeral(char* code, map info){
	if(((code*) >= '0') && (((code + 1)*) <= 'b')){
		code += 2;
		return bin(code, info);
	}else if(((code*) >= '0') && (((code + 1)*) <= 'x')){
		code += 2;
		return hex(code, info);
	}
	return dec(code, info);
}
char* skipspace(char* code){
	while((code*) == ' '){
		code++;
	}
	return code;
}
//+ - * / %
//&& ||
//& | *
//== !=
//NUMBER COSTANT
//STRING
//* &
id operation(char* code){
	switch(*code){
		case '+':
			return ADD;
		case '-':
			return SUB;
		case '*':
			return MUL;
		case '/':
			return DIV;
		case '%':
			return MOD;
		case '&':
			return BITWISE_AND;
		case '|':
			return BITWISE_OR;
		case '^':
			return BITWISE_XOR;
		case '=':
			if(*(code - 1) == '='){
				return EQUALS;	
			}else if(*(code - 1) == '>'){
				return GREATER_EQUAL;
			}else if(*(code - 1) == '<'){
				return LESS_EQUAL;
			}else if(*(code - 1) == '!'){
				return XOR; //!=
			}
		case '!':
			return NOT;
		case '>':
			return GREATER;
		case '<':
			return LESS;
		case '~':
			return BITWISE_NOT;	
	}
	return 0xFF;
}
bool expression_check(char* ptr, char* start){
	for(;ptr > start;ptr--){
		if(*ptr != ' '){
			return false;	
		}
	}
	return true;
}
bool alphabet_letter(char character){
	if(character >= 'a'){
		if(character <= 'z'){
			return true;
		}
	}else if(character >= 'A'){
		if(character <= 'Z'){
			return true;
		}
	}
	return false;
}
id operator_check(char* ptr, map info, char* start){
	id operator = operation(code);
	if(operator == BITWISE_AND){ //&
		type = 5;
		if(expression_check(code, start)){
			operator = ADDRESS;
		}
	}else if(operator == MUL){ //*
		type = 5;
		if(expression_check(code, start)){
			operator = DEREF;
		}
	}
	if(operator != 0xFF){
		add_to_ast(info.ast, operator);
	}
}
char* expression(char* code, map info, char* start){
	//(x + y) + z	
	unsigned char type;
	id operator;
	if((*code) == '"'){ //string
		type = 1;
		//add_to_ast(info.ast, 0xC4);
		//add_unsigned_int_to_ast(info.ast, code);
		code--;
		while(true){
			if((*code) == '"'){							
				break;
			}
			code = character(code, info);
		}
		operator = operation_check(code, info, start);
		add_to_ast(info.tree, 0xC2);
		add_unsigned_int_to_ast(info.tree, code+1);		
	}else if((*code) == '\''){ //character
		type = 2;
		rFunc result = character(code - 1, info);	
		code = result.address;
		if(code != '\''){
			//RETURN ERROR
		}
		code--;
		operator = operation_check(code, info.tree, start);
		add_to_ast(info.tree, 0xC1);
		add_unsigned_long_to_ast(info.tree, result.number);		
	}else if((*code) >= '0'){ //number
		if((*code) <= '9'){
			type = 3;
			while(true){
				if((*code) < '0'){
					break;
				}
				if((*code) > '9'){
					break;
				}
				code--;
			}
			bool is_it_signed = false;
			unsigned long long number;
			if(*(code - 1) == '0'){
				if((*code) == 'b'){
					number = bin(code + 1);
					code -= 2;	
				}else if((*code) == 'x'){
					number = hex(code + 1);
					code -= 2;
				}else{
					number = dec(code + 1);
				}
			}else{
				number = dec(code + 1);
			}
			if((*code) == '-'){
				is_it_signed = true;
				number = 0 - number;
			}
			unsigned char* h = info.ast.data + info.ast.length - 1;
			operator = operation_check(code, info, start);
			if(is_it_signed){ //signed
				add_to_ast(info.tree, 0xC0);
			}else{ //unsigned
				add_to_ast(info.tree, 0xC1);
			}
			add_unsigned_long_to_ast(info.tree, number);
		}
	}else if((*code) == ')'){
		code = expression(code - 1, info, NULL);
		operator = operation_check(code, info, start);
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
