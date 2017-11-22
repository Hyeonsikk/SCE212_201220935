#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>


#define NUMINTRU  21 // the number of instrunction
#define LENGINTRU 6 // the maximum length of instruction
#define MAXLINE 100 // the maximum line that the program can accept

void pass1(char* input); // one pass 
void pass2(char* input);// two pass

// divide functions by each instrunction set
// each of functions returns machine language (integer)
// it is written in hexa code by using "%x"
int addiu(char* sArr[]);
int addu(char* sArr[]);
int and(char* sArr[]);
int andi(char* sArr[]);
int beq(char* sArr[]);
int bne(char* sArr[]);
int j(char* sArr[]);
int jal(char* sArr[]);
int jr(char* sArr[]);
int lui(char* sArr[]);
int lw(char* sArr[]);
int la(char* sArr[]);
int nor(char* sArr[]);
int or(char* sArr[]);
int ori(char* sArr[]);
int sltiu(char* sArr[]);
int sltu(char* sArr[]);
int sll(char* sArr[]);
int srl(char* sArr[]);
int sw(char* sArr[]);
int subu(char* sArr[]);
// translate hexa number into decimal number
int hexTodec(char* sArr);

// SYMBOL table
struct SYMBOL_TABLE {
	char symbol[10];
	int address;
};
// structure of each line 
struct LINE {
	int address;
	int content;
};
// structure of REGISTER 
struct REGISTER {
	int content;
	int address;
};
struct DATA {
	char dataname[10];
	int content;
	int address;
};
struct MEMORY {
	int content;
	int address;
};

int PC = 4194308; // program counter
int CUR_ADDR = 4194304; // current address
int DATA_ADDR = 268435456; // data address
int bits[32] = { 0 }; // store each bit before translate into hexa code
// define register (0 - 31) 
struct REGISTER reg[32]; // define register
struct SYMBOL_TABLE table[10]; // define symbol table for one pass
struct DATA data[100];
struct MEMORY memory[4096];
//struct MEMORY MEMORY[pow(2,32)-1];
int sym_count = 0; // the number of symbol label
int line_count = 0; // the number of instruction set
int data_count = 0;
int flag = 0; // if flag = 1 , it is data section // if flag = 0, it is code section
char *datasec = ".data\n";
char *textsec = ".text\n";
char *labelmain = "main:\n";
// instruction set
char instruction[NUMINTRU][LENGINTRU] = { "addiu","addu","and","andi","beq","bne","j","jal","jr","lui","lw","la","nor","or","ori","sltiu",
"sltu","sll","srl","sw","subu" };

int main() {

	char input[32] = {NULL,};
	printf("input file name : ");
	scanf("%s", input);
	pass1(input); // pass 1

	pass2(input); // pass 2
	
	return 0;
	
}
// make symbol table
void pass1(char* input) {
	FILE *fin; // file open
	char s[81]; // store each line of input data
	char *ptr; 
	char *sArr[6] = { "" };
	char *sArr_2[6] = { "" }; 
	
	int i = 0;
	int i2 = 0;
	

	fin = fopen(input, "r");
	//open error check
	if (fin == NULL) {
		printf("failed to file open\n");
		return 1;
	}
	while (!feof(fin)) {
		
		// increment of PC 
		PC = PC + 4;

		// read a line
		fgets(s, 80, fin);
		ptr = strtok(s, "\t");
		// erasing "\t"
		while (ptr != NULL){
			sArr[i] = ptr;     
			i++;                       
			ptr = strtok(NULL, "\t");
		}
		// erasing " "
		for (int j = 0; j < i; j++) {
			sArr_2[j] = strtok(sArr[j], " ");

			while (sArr[j] != NULL){
				sArr_2[i2] = sArr[j];     
				i2++;                       
				sArr[j] = strtok(NULL, " ");
			}
		}

		// erasing ','
		for (int j = 0; j < i2; j++) {
			sArr_2[j] = strtok(sArr_2[j], ",");
		}
		if (!strcmp(sArr_2[0],datasec) ){
			PC = PC - 4;
			i = 0;
			i2 = 0;
			flag = 1;
		}else if (!strcmp(sArr_2[0], textsec)) {
			PC = PC - 4;
			i = 0;
			i2 = 0;
			flag = 0;
		}
		
		
		// if the string is label
		if (!(strchr(sArr_2[0], ':') == NULL)) {
			// data section
			if (flag == 1) {
				strcpy(data[data_count].dataname, strtok(sArr_2[0], ":"));
				// data type is always .word
				if (!((strchr(sArr_2[2], 'x') == NULL))) {
					data[data_count].address = DATA_ADDR;
					DATA_ADDR += 4;
					data[data_count].content = atoi(sArr_2[2]);
					data_count++;
				}
				else {
					data[data_count].address = DATA_ADDR;
					DATA_ADDR += (4 * atoi(sArr_2[2]));
					data_count ++;
				}
			}else{
				if (!strcmp(sArr_2[0], labelmain)) {
					PC = 4194308; // program counter
					CUR_ADDR = 4194304; // current address
				}else {
					strcpy(table[sym_count].symbol, strtok(sArr_2[0], ":"));
					table[sym_count].address = CUR_ADDR;
					sym_count++;
					
				}// adds symbol table
			}// code section
		}
		CUR_ADDR = PC;
		i = 0;
		i2 = 0;
	}
	// prints symbol table
	printf("--------SYMBOL_TABLE---------\n");
	for (int z = 0; z < sym_count; z++) {
		printf("SYMBOL : %s ADDR : 0x%x\n", table[z].symbol, table[z].address);
	}
	printf("--------DATA_TABLE---------\n");
	for (int z = 0; z < data_count; z++) {
		printf("DATA : %s ADDR : 0x%x\n", data[z].dataname, data[z].address);
	}
	fclose(fin);
}
void pass2(char* input) {
	int modevalue;
	int result;

	struct LINE line[MAXLINE];
	FILE *fin;
	FILE *fout;
	char s[81]; // store each line of input data
	char *ptr; 
	char *sArr[6] = { "" }; 
	char *sArr_2[6] = { "" }; 
	int i = 0; // the number of token by line
	int i2 = 0;

	fin = fopen(input, "r");
	fout = fopen("test_result.txt", "w");

	if (fin == NULL) {
		printf("failed to file open\n");
		return 1;
	}
	if (fout == NULL) {
		printf("failed to file open\n");
		return 1;
	}

	// initializes the value of pass 2 current_address, PC, line_count 
	CUR_ADDR = 4194304;
	PC = CUR_ADDR + 4;
	line_count = 0;
	while (!feof(fin)) {
		// increment of PC 
		PC = PC + 4;

		// read a line
		fgets(s, 80, fin);
		ptr = strtok(s, "\t");
		// erasing "\t"
		while (ptr != NULL)           
		{
			sArr[i] = ptr;   
			i++;             
			ptr = strtok(NULL, "\t");
		}
		// erasing " "
		for (int j = 0; j < i; j++) {
			sArr_2[j] = strtok(sArr[j], " ");

			while (sArr[j] != NULL)            
			{
				sArr_2[i2] = sArr[j];    
				i2++;                      
				sArr[j] = strtok(NULL, " ");
			}
		}

		// erasing ','
		for (int j = 0; j < i2; j++) {
			sArr_2[j] = strtok(sArr_2[j], ",");
		}

		if (!strcmp(sArr_2[0], datasec)) {
			PC = PC - 4;
			i = 0;
			i2 = 0;
			flag = 1;
			line_count--;
		}
		else if (!strcmp(sArr_2[0], textsec)) {
			PC = PC - 4;
			i = 0;
			i2 = 0;
			flag = 0;
			line_count--;
		}
		// if the string is LABEL or data section
		if (!(strchr(sArr_2[0], ':') == NULL)) {
			// data section
			if (flag == 1) {
				line_count--;
			}
			else {
				if (!strcmp(sArr_2[0], labelmain)) {
					PC = 4194308; // program counter
					CUR_ADDR = 4194304; // current address
				}
				line[line_count].address = CUR_ADDR;
				line[line_count].content = NULL;	
			}
		}

		// if the string is not LABEL
		// comparing with instruction set
		for (int k = 0; k < NUMINTRU; k++) {
			
			if (!strcmp(sArr_2[0], instruction[k])) {
				modevalue = k;
				// initializes bits
				for (int a = 0; a < 32; a++) {
					bits[a] = 0;
				}
				switch (modevalue)
				{
				case 0:
					line[line_count].address = CUR_ADDR;
					line[line_count].content = addiu(sArr_2);
					break;
				case 1:
					line[line_count].address = CUR_ADDR;
					line[line_count].content = addu(sArr_2);
					break;
				case 2:
					line[line_count].address = CUR_ADDR;
					line[line_count].content = and (sArr_2);
					break;
				case 3:
					line[line_count].address = CUR_ADDR;
					line[line_count].content = andi(sArr_2);
					break;
				case 4:
					line[line_count].address = CUR_ADDR;
					line[line_count].content = beq(sArr_2);
					break;
				case 5:
					line[line_count].address = CUR_ADDR;
					line[line_count].content = bne(sArr_2);
					break;
				case 6:
					line[line_count].address = CUR_ADDR;
					line[line_count].content = j(sArr_2);
					break;
				case 7:
					line[line_count].address = CUR_ADDR;
					line[line_count].content = jal(sArr_2);
					break;
				case 8:
					line[line_count].address = CUR_ADDR;
					line[line_count].content = jr(sArr_2);
					break;
				case 9:
					line[line_count].address = CUR_ADDR;
					line[line_count].content = lui(sArr_2);
					break;
				case 10:
					line[line_count].address = CUR_ADDR;
					line[line_count].content = lw(sArr_2);
					break;
				case 11:
					line[line_count].address = CUR_ADDR;
					line[line_count].content = la(sArr_2);
					break;
				case 12:
					line[line_count].address = CUR_ADDR;
					line[line_count].content = nor(sArr_2);
					break;
				case 13:
					line[line_count].address = CUR_ADDR;
					line[line_count].content = or (sArr_2);
					break;
				case 14:
					line[line_count].address = CUR_ADDR;
					line[line_count].content = ori(sArr_2);
					break;
				case 15:
					line[line_count].address = CUR_ADDR;
					line[line_count].content = sltiu(sArr_2);
					break;
				case 16:
					line[line_count].address = CUR_ADDR;
					line[line_count].content = sltu(sArr_2);
					break;
				case 17:
					line[line_count].address = CUR_ADDR;
					line[line_count].content = sll(sArr_2);
					break;
				case 18:
					line[line_count].address = CUR_ADDR;
					line[line_count].content = srl(sArr_2);
					break;
				case 19:
					line[line_count].address = CUR_ADDR;
					line[line_count].content = sw(sArr_2);
					break;
				case 20:
					line[line_count].address = CUR_ADDR;
					line[line_count].content = subu(sArr_2);
					break;
				default:
					break;
				}
			}
			
		}
		CUR_ADDR = PC;
		line_count++;
		i = 0;
		i2 = 0;
	}
	// prints the result 
	if (flag == 0) {
		printf("---------INSTRUCTION----------\n");
		for (int a = 0; a < line_count; a++) {
			printf("address : 0x%06x  content : 0x%08x\n", line[a].address, line[a].content);
			fprintf(fout,"0x%08x\n" ,line[a].content);
		}
		// prints the number of label
		printf("line : %d \n", line_count);
	}

	fclose(fin);
	fclose(fout);
}
int addiu(char * sArr_3[]) {
	char * sArr_result[4] = { "" };
	int reg_num1;
	int reg_num2;
	int temp_value;
	int immediate;
	int i;
	int result_temp;
	int result = 0;
	// erasing '$'
	for (int j = 0; j < 4; j++) {
		sArr_result[j] = strtok(sArr_3[j], "$");
	}
	reg_num1 = atoi(sArr_result[1]);
	reg_num2 = atoi(sArr_result[2]);
	// if immediate value is in hexa number 
	if (!((strchr(sArr_result[3], 'x') == NULL))) {
		immediate = hexTodec(sArr_result[3]);
	}
	else {
		immediate = atoi(sArr_result[3]);
	}
	// translate into machine code 
	temp_value = reg_num1;
	for (i = 0; temp_value > 0; i++) {
		bits[16 + i] = temp_value % 2;
		temp_value /= 2;

	}
	temp_value = reg_num2;
	for (i = 0; temp_value > 0; i++) {
		bits[21 + i] = temp_value % 2;
		temp_value /= 2;
	}

	temp_value = immediate;
	for (i = 0; temp_value>0; i++) {
		bits[i] = temp_value % 2;
		temp_value /= 2;	
	}
	
	// addiu -> opcode = 9 (hex)
	temp_value = 9;
	for (i = 0; temp_value>0; i++) {
		bits[26+i] = temp_value % 2;
		temp_value /= 2;
	}
	
	// adds all value of bits 
	for (i = 0; i < 32; i++) {
		result_temp = 1;
		if (bits[i] == 1) {
			for (int d = 0; d < i; d++) {
				result_temp *= 2;
			}
			result += result_temp;
		}
		 
	}
	// execute operation (addiu) 
	reg[reg_num1].content = reg[reg_num2].content + immediate;

	
	return result;
}
int addu(char* sArr_3[]) {
	char * sArr_result[4] = { "" };
	int reg_num1;
	int reg_num2;
	int reg_num3;
	int temp_value;
	int i;
	int result_temp;
	int result = 0;

	// erasing '$'
	for (int j = 0; j < 4; j++) {
		sArr_result[j] = strtok(sArr_3[j], "$");
	}
	reg_num1 = atoi(sArr_result[1]);
	reg_num2 = atoi(sArr_result[2]);
	reg_num3 = atoi(sArr_result[3]);

	

	// translate into machine code
	temp_value = reg_num1;
	for (i = 0; temp_value > 0; i++) {
		bits[11 + i] = temp_value % 2;
		temp_value /= 2;
	}

	temp_value = reg_num2;
	for (i = 0; temp_value > 0; i++) {
		bits[21 + i] = temp_value % 2;
		temp_value /= 2;
	}

	temp_value = reg_num3;
	for (i = 0; temp_value > 0; i++) {
		bits[16 + i] = temp_value % 2;
		temp_value /= 2;
	}
	// addu -> opcode = 0/21 (hex)
	temp_value = 33;
	for (i = 0; temp_value>0; i++) {
		bits[i] = temp_value % 2;
		temp_value /= 2;
	}
	// adds all value of bits 
	for (i = 0; i < 32; i++) {
		result_temp = 1;
		if (bits[i] == 1) {
			for (int d = 0; d < i; d++) {
				result_temp *= 2;
			}
			result += result_temp;
		}

	}

	// execute operation (addu) 
	reg[reg_num1].content = reg[reg_num2].content + reg[reg_num3].content;
	return result;
}
int and(char* sArr_3[]) {
	char * sArr_result[4] = { "" };
	int reg_num1;
	int reg_num2;
	int reg_num3;
	int temp_value;
	int i;
	int result_temp;
	int result = 0;

	// erasing '$'
	for (int j = 0; j < 4; j++) {
		sArr_result[j] = strtok(sArr_3[j], "$");
		//printf("%s ", sArr_result[j]);
	}
	reg_num1 = atoi(sArr_result[1]);
	reg_num2 = atoi(sArr_result[2]);
	reg_num3 = atoi(sArr_result[3]);


	// translate into machine code 

	temp_value = reg_num1;
	for (i = 0; temp_value > 0; i++) {
		bits[11 + i] = temp_value % 2;
		temp_value /= 2;
	}

	temp_value = reg_num2;
	for (i = 0; temp_value > 0; i++) {
		bits[21 + i] = temp_value % 2;
		temp_value /= 2;
	}

	temp_value = reg_num3;
	for (i = 0; temp_value > 0; i++) {
		bits[16 + i] = temp_value % 2;
		temp_value /= 2;
	}
	// addiu -> opcode = 0/24 (hex)
	temp_value = 36;
	for (i = 0; temp_value>0; i++) {
		bits[i] = temp_value % 2;
		temp_value /= 2;
	}
	// adds all value of bits 
	for (i = 0; i < 32; i++) {
		result_temp = 1;
		if (bits[i] == 1) {
			for (int d = 0; d < i; d++) {
				result_temp *= 2;
			}
			result += result_temp;
		}

	}

	// execute operation (and) 
	reg[reg_num1].content = reg[reg_num2].content & reg[reg_num3].content;

	return result;
}
int andi(char* sArr_3[]) {
	char * sArr_result[4] = { "" };
	int reg_num1;
	int reg_num2;
	int immediate;
	int temp_value;
	int i;
	int result_temp;
	int result = 0;

	//erasing '$'
	for (int j = 0; j < 4; j++) {
		sArr_result[j] = strtok(sArr_3[j], "$");
		printf("%s ", sArr_result[j]);
	}
	reg_num1 = atoi(sArr_result[1]);
	reg_num2 = atoi(sArr_result[2]);

	// if immediate value is in hexa number 
	if (!((strchr(sArr_result[3], 'x') == NULL))) {
		immediate = hexTodec(sArr_result[3]);
	}
	else {
		immediate = atoi(sArr_result[3]);
	}

	// translate into machine code 
	temp_value = reg_num1;
	for (i = 0; temp_value > 0; i++) {
		bits[16 + i] = temp_value % 2;
		temp_value /= 2;

	}

	temp_value = reg_num2;
	for (i = 0; temp_value > 0; i++) {
		bits[21 + i] = temp_value % 2;
		temp_value /= 2;
	}

	temp_value = immediate;
	for (i = 0; temp_value>0; i++) {
		bits[i] = temp_value % 2;
		temp_value /= 2;
	}

	// andi -> opcode = c (hex)
	temp_value = 12;
	for (i = 0; temp_value>0; i++) {
		bits[26 + i] = temp_value % 2;
		temp_value /= 2;
	}

	// adds all value of bits 
	for (i = 0; i < 32; i++) {
		result_temp = 1;
		if (bits[i] == 1) {
			for (int d = 0; d < i; d++) {
				result_temp *= 2;
			}
			result += result_temp;
		}

	}

	// execute operation (andi) 
	reg[reg_num1].content = reg[reg_num2].content & immediate;
	
	return result;
}
int beq(char* sArr_3[]) {
	char * sArr_result[4] = { "" };
	int reg_num1;
	int reg_num2;
	int addr;
	int temp_value;
	int i;
	int result_temp;
	int result = 0;

	//erasing '$'
	for (int j = 0; j < 4; j++) {
		sArr_result[j] = strtok(sArr_3[j], "$");
		//printf("%s ", sArr_result[j]);
	}
	reg_num1 = atoi(sArr_result[1]);
	reg_num2 = atoi(sArr_result[2]);
	// searchin SYMBOL_TABLE 
	for (int i = 0; i < sym_count; i++) {
		// if it is correct
		if (!(strcmp(table[i].symbol, sArr_result[3]))) {
			addr = table[i].address;
			break;
		}
	}
	// execute operation (beq) 
	if (reg[reg_num1].content == reg[reg_num2].content) {
		PC = PC + 4 + addr;
	}

	// translate into machine code
	temp_value = reg_num1;
	for (i = 0; temp_value > 0; i++) {
		bits[16 + i] = temp_value % 2;
		temp_value /= 2;

	}

	temp_value = reg_num2;
	for (i = 0; temp_value > 0; i++) {
		bits[21 + i] = temp_value % 2;
		temp_value /= 2;
	}

	temp_value = addr;
	for (i = 0; temp_value>0; i++) {
		bits[i] = temp_value % 2;
		temp_value /= 2;
	}
	// beq -> opcode = 4 (hex)
	temp_value = 4;
	for (i = 0; temp_value>0; i++) {
		bits[26 + i] = temp_value % 2;
		temp_value /= 2;
	}

	// adds all value of bits 
	for (i = 0; i < 32; i++) {
		result_temp = 1;
		if (bits[i] == 1) {
			for (int d = 0; d < i; d++) {
				result_temp *= 2;
			}
			result += result_temp;
		}

	}
	
	
	return result;
}
int bne(char* sArr_3[]) {
	char * sArr_result[4] = { "" };
	int reg_num1;
	int reg_num2;
	int addr;
	int temp_value;
	int i;
	int result_temp;
	int result = 0;

	//'erasing '$'
	for (int j = 0; j < 4; j++) {
		sArr_result[j] = strtok(sArr_3[j], "$");
		//printf("%s ", sArr_result[j]);
	}
	reg_num1 = atoi(sArr_result[1]);
	reg_num2 = atoi(sArr_result[2]);
	// searchin SYMBOL_TABLE 
	for (int i = 0; i < sym_count; i++) {
		// if it is correct
		if (!(strcmp(table[i].symbol, sArr_result[3]))) {
			addr = table[i].address;
			break;
		}
	}
	
	// execute operation (bne) 
	if (reg[reg_num1].content != reg[reg_num2].content) {
		 PC = PC+4+addr;
	}
	// translate into machine code
	temp_value = reg_num1;
	for (i = 0; temp_value > 0; i++) {
		bits[16 + i] = temp_value % 2;
		temp_value /= 2;

	}

	temp_value = reg_num2;
	for (i = 0; temp_value > 0; i++) {
		bits[21 + i] = temp_value % 2;
		temp_value /= 2;
	}

	temp_value = addr;
	for (i = 0; temp_value>0; i++) {
		bits[i] = temp_value % 2;
		temp_value /= 2;
	}
	// bne -> opcode = 5 (hex)
	temp_value = 5;
	for (i = 0; temp_value>0; i++) {
		bits[26 + i] = temp_value % 2;
		temp_value /= 2;
	}

	// adds all value of bits 
	for (i = 0; i < 32; i++) {
		result_temp = 1;
		if (bits[i] == 1) {
			for (int d = 0; d < i; d++) {
				result_temp *= 2;
			}
			result += result_temp;
		}

	}
	
	return result;
}
int j(char* sArr_3[]) {

	int addr;
	int temp_value;
	int i;
	int result_temp;
	int result = 0;
	
	// searchin SYMBOL_TABLE 
	for (int i = 0; i < sym_count; i++) {
		// if it is correct
		if (!(strcmp(table[i].symbol, sArr_3[1]))) {
			PC = table[i].address;
			addr  = table[i].address;
			break;
		}
	}
	// translate into machine code
	
	// j -> opcode = 2 (hex)
	temp_value = 2;
	for (i = 0; temp_value>0; i++) {
		bits[26 + i] = temp_value % 2;
		temp_value /= 2;
	}
	// symbol table\C0\C7 address
	temp_value = addr;
	for (i = 0; temp_value>0; i++) {
		bits[i] = temp_value % 2;
		temp_value /= 2;
	}

	// adds all value of bits 
	for (i = 0; i < 32; i++) {
		result_temp = 1;
		if (bits[i] == 1) {
			for (int d = 0; d < i; d++) {
				result_temp *= 2;
			}
			result += result_temp;
		}

	}
	
	return result;
}
int jal(char* sArr_3[]) {

	int addr;
	int temp_value;
	int i;
	int result_temp;
	int result = 0;

	// execute operation (jar) 
	reg[31].content = PC + 8;
	// searchin SYMBOL_TABLE 
	for (int i = 0; i < sym_count; i++) {
		// if it is correct
		if (!(strcmp(table[i].symbol, sArr_3[1]))) {
			PC = table[i].address;
			addr = table[i].address;
			break;
		}
	}

	// translate into machine code

	// j -> opcode = 2 (hex)
	temp_value = 2;
	for (i = 0; temp_value>0; i++) {
		bits[26 + i] = temp_value % 2;
		temp_value /= 2;
	}
	// symbol table\C0\C7 address
	temp_value = addr;
	for (i = 0; temp_value>0; i++) {
		bits[i] = temp_value % 2;
		temp_value /= 2;
	}

	// adds all value of bits 
	for (i = 0; i < 32; i++) {
		result_temp = 1;
		if (bits[i] == 1) {
			for (int d = 0; d < i; d++) {
				result_temp *= 2;
			}
			result += result_temp;
		}

	}

	return result;
}
int jr(char* sArr_3[]) {
	char * sArr_result[2] = { "" };
	//R PC <- R[rs]
	int reg_num1;
	int temp_value;
	int i;
	int result_temp;
	int result = 0;

	//erasing '$'
	
	sArr_result[1] = strtok(sArr_3[1], "$");
	reg_num1 = atoi(sArr_result[1]);
	
	// translate into machine code

	temp_value = reg_num1;
	for (i = 0; temp_value > 0; i++) {
		bits[21 + i] = temp_value % 2;
		temp_value /= 2;
	}

	// addiu -> opcode = 0/08 (hex)
	temp_value = 8;
	for (i = 0; temp_value>0; i++) {
		bits[i] = temp_value % 2;
		temp_value /= 2;
	}
	// adds all value of bits 
	for (i = 0; i < 32; i++) {
		result_temp = 1;
		if (bits[i] == 1) {
			for (int d = 0; d < i; d++) {
				result_temp *= 2;
			}
			result += result_temp;
		}

	}
	return result;
}
int lui(char* sArr_3[]) {
	return 0;
}
int lw(char* sArr_3[]) {
	return 0;
}
int la(char* sArr_3[]) {
	return 0;
}
int nor(char* sArr_3[]) {
	char * sArr_result[4] = { "" };
	int reg_num1;
	int reg_num2;
	int reg_num3;
	int temp_value;
	int i;
	int result_temp;
	int result = 0;

	//erasing '$'
	for (int j = 0; j < 4; j++) {
		sArr_result[j] = strtok(sArr_3[j], "$");
		//printf("%s ", sArr_result[j]);
	}
	reg_num1 = atoi(sArr_result[1]);
	reg_num2 = atoi(sArr_result[2]);
	reg_num3 = atoi(sArr_result[3]);

	// translate into machine code

	temp_value = reg_num1;
	for (i = 0; temp_value > 0; i++) {
		bits[11 + i] = temp_value % 2;
		temp_value /= 2;
	}

	temp_value = reg_num2;
	for (i = 0; temp_value > 0; i++) {
		bits[21 + i] = temp_value % 2;
		temp_value /= 2;
	}

	temp_value = reg_num3;
	for (i = 0; temp_value > 0; i++) {
		bits[16 + i] = temp_value % 2;
		temp_value /= 2;
	}
	// nor -> opcode = 27 (hex)
	temp_value = 39;
	for (i = 0; temp_value>0; i++) {
		bits[i] = temp_value % 2;
		temp_value /= 2;
	}
	// adds all value of bits 
	for (i = 0; i < 32; i++) {
		result_temp = 1;
		if (bits[i] == 1) {
			for (int d = 0; d < i; d++) {
				result_temp *= 2;
			}
			result += result_temp;
		}

	}

	// execute operation (nor) 
	reg[reg_num1].content = !(reg[reg_num2].content || reg[reg_num3].content);

	return result;
}
int or(char* sArr_3[]) {
	char * sArr_result[4] = { "" };
	int reg_num1;
	int reg_num2;
	int reg_num3;
	int temp_value;
	int i;
	int result_temp;
	int result = 0;

	//erasing '$'
	for (int j = 0; j < 4; j++) {
		sArr_result[j] = strtok(sArr_3[j], "$");
	}
	reg_num1 = atoi(sArr_result[1]);
	reg_num2 = atoi(sArr_result[2]);
	reg_num3 = atoi(sArr_result[3]);

	// translate into machine code

	temp_value = reg_num1;
	for (i = 0; temp_value > 0; i++) {
		bits[11 + i] = temp_value % 2;
		temp_value /= 2;
	}

	temp_value = reg_num2;
	for (i = 0; temp_value > 0; i++) {
		bits[21 + i] = temp_value % 2;
		temp_value /= 2;
	}

	temp_value = reg_num3;
	for (i = 0; temp_value > 0; i++) {
		bits[16 + i] = temp_value % 2;
		temp_value /= 2;
	}
	// or -> opcode = 25 (hex)
	temp_value = 37;
	for (i = 0; temp_value>0; i++) {
		bits[i] = temp_value % 2;
		temp_value /= 2;
	}
	// adds all value of bits 
	for (i = 0; i < 32; i++) {
		result_temp = 1;
		if (bits[i] == 1) {
			for (int d = 0; d < i; d++) {
				result_temp *= 2;
			}
			result += result_temp;
		}

	}

	// execute operation (or) 
	reg[reg_num1].content = reg[reg_num2].content || reg[reg_num3].content;

	return result;
}
int ori(char* sArr_3[]) {
	char * sArr_result[4] = { "" };
	int reg_num1;
	int reg_num2;
	int immediate;
	int temp_value;
	int i;
	int result_temp;
	int result = 0;

	// erasing '$'
	for (int j = 0; j < 4; j++) {
		sArr_result[j] = strtok(sArr_3[j], "$");
		
	}
	reg_num1 = atoi(sArr_result[1]);
	reg_num2 = atoi(sArr_result[2]);

	// if immediate value is in hexa number 
	if (!((strchr(sArr_result[3], 'x') == NULL))) {
		immediate = hexTodec(sArr_result[3]);
	}
	else {
		immediate = atoi(sArr_result[3]);
	}

	// translate into machine code
	temp_value = reg_num1;
	for (i = 0; temp_value > 0; i++) {
		bits[16 + i] = temp_value % 2;
		temp_value /= 2;

	}

	temp_value = reg_num2;
	for (i = 0; temp_value > 0; i++) {
		bits[21 + i] = temp_value % 2;
		temp_value /= 2;
	}

	temp_value = immediate;
	for (i = 0; temp_value>0; i++) {
		bits[i] = temp_value % 2;
		temp_value /= 2;
	}

	// ori -> opcode = c (hex)
	temp_value = 13;
	for (i = 0; temp_value>0; i++) {
		bits[26 + i] = temp_value % 2;
		temp_value /= 2;
	}

	// adds all value of bits 
	for (i = 0; i < 32; i++) {
		result_temp = 1;
		if (bits[i] == 1) {
			for (int d = 0; d < i; d++) {
				result_temp *= 2;
			}
			result += result_temp;
		}

	}

	// execute operation (ori) 
	reg[reg_num1].content = reg[reg_num2].content || immediate;
	return result;
}
int sltiu(char* sArr_3[]) {
	// R[rt] = (R[rs] < SignExtImm) ? 1 : 0

	char * sArr_result[4] = { "" };
	int reg_num1;
	int reg_num2;
	int temp_value;
	int immediate;
	int i;
	int result_temp;
	int result = 0;
	//erasing '$'
	for (int j = 0; j < 4; j++) {
		sArr_result[j] = strtok(sArr_3[j], "$");
	}
	reg_num1 = atoi(sArr_result[1]);
	reg_num2 = atoi(sArr_result[2]);

	// if immediate value is in hexa number 
	if (!((strchr(sArr_result[3], 'x') == NULL))) {
		immediate = hexTodec(sArr_result[3]);
	}
	else {
		immediate = atoi(sArr_result[3]);
	}

	// translate into machine code
	temp_value = reg_num1;
	for (i = 0; temp_value > 0; i++) {
		bits[16 + i] = temp_value % 2;
		temp_value /= 2;

	}

	temp_value = reg_num2;
	for (i = 0; temp_value > 0; i++) {
		bits[21 + i] = temp_value % 2;
		temp_value /= 2;
	}

	temp_value = immediate;
	for (i = 0; temp_value>0; i++) {
		bits[i] = temp_value % 2;
		temp_value /= 2;
	}

	// sltiu -> opcode = b (hex)
	temp_value = 11;
	for (i = 0; temp_value>0; i++) {
		bits[26 + i] = temp_value % 2;
		temp_value /= 2;
	}

	// adds all value of bits 
	for (i = 0; i < 32; i++) {
		result_temp = 1;
		if (bits[i] == 1) {
			for (int d = 0; d < i; d++) {
				result_temp *= 2;
			}
			result += result_temp;
		}

	}
	// execute operation (sltiu) 
	reg[reg_num1].content = (reg[reg_num2].content, immediate)? 1:0;

	return result;
}
int sltu(char* sArr_3[]) {
	//R[rd] = (R[rs] < R[rt]) ? 1 : 0
	char * sArr_result[4] = { "" };
	int reg_num1;
	int reg_num2;
	int reg_num3;
	int temp_value;
	int i;
	int result_temp;
	int result = 0;

	//erasing '$'
	for (int j = 0; j < 4; j++) {
		sArr_result[j] = strtok(sArr_3[j], "$");

	}
	reg_num1 = atoi(sArr_result[1]);
	reg_num2 = atoi(sArr_result[2]);
	reg_num3 = atoi(sArr_result[3]);



	// translate into machine code

	temp_value = reg_num1;
	for (i = 0; temp_value > 0; i++) {
		bits[11 + i] = temp_value % 2;
		temp_value /= 2;
	}

	temp_value = reg_num2;
	for (i = 0; temp_value > 0; i++) {
		bits[21 + i] = temp_value % 2;
		temp_value /= 2;
	}

	temp_value = reg_num3;
	for (i = 0; temp_value > 0; i++) {
		bits[16 + i] = temp_value % 2;
		temp_value /= 2;
	}
	// sltu -> opcode = 0/2b (hex)
	temp_value = 43;
	for (i = 0; temp_value>0; i++) {
		bits[i] = temp_value % 2;
		temp_value /= 2;
	}
	// adds all value of bits 
	for (i = 0; i < 32; i++) {
		result_temp = 1;
		if (bits[i] == 1) {
			for (int d = 0; d < i; d++) {
				result_temp *= 2;
			}
			result += result_temp;
		}

	}

	// execute operation (sltu) 
	//R[rd] = (R[rs] < R[rt]) ? 1 : 0
	reg[reg_num1].content = (reg[reg_num2].content < reg[reg_num3].content)? 1:0;
	return result;

	return 0;
}
int sll(char* sArr_3[]) {
	char * sArr_result[4] = { "" };
	int reg_num1;
	int reg_num2;
	int shamt;
	int temp_value;
	int i;
	int result_temp;
	int result = 0;

	//erasing '$'
	for (int j = 0; j < 4; j++) {
		sArr_result[j] = strtok(sArr_3[j], "$");
	
	}
	reg_num1 = atoi(sArr_result[1]);
	reg_num2 = atoi(sArr_result[2]);
	shamt = atoi(sArr_result[3]);



	//translate into machine code

	temp_value = reg_num1;
	for (i = 0; temp_value > 0; i++) {
		bits[11 + i] = temp_value % 2;
		temp_value /= 2;
	}

	temp_value = reg_num2;
	for (i = 0; temp_value > 0; i++) {
		bits[16 + i] = temp_value % 2;
		temp_value /= 2;
	}

	temp_value = shamt;
	for (i = 0; temp_value > 0; i++) {
		bits[6 + i] = temp_value % 2;
		temp_value /= 2;
	}

	// sll -> opcode = 0/00 (hex)

	
	// adds all value of bits 
	for (i = 0; i < 32; i++) {
		result_temp = 1;
		if (bits[i] == 1) {
			for (int d = 0; d < i; d++) {
				result_temp *= 2;
			}
			result += result_temp;
		}

	}
	
	// execute operation (sll) 
	for (int a = 0; a < reg[shamt].content * 2; a++) {
		reg[reg_num1].content *= 2;
	}
	
	return result;
}
int srl(char* sArr_3[]) {
	char * sArr_result[4] = { "" };
	int reg_num1;
	int reg_num2;
	int shamt;
	int temp_value;
	int i;
	int result_temp;
	int result = 0;

	//erasing '$'
	for (int j = 0; j < 4; j++) {
		sArr_result[j] = strtok(sArr_3[j], "$");

	}
	reg_num1 = atoi(sArr_result[1]);
	reg_num2 = atoi(sArr_result[2]);
	shamt = atoi(sArr_result[3]);

	// translate into machine code

	temp_value = reg_num1;
	for (i = 0; temp_value > 0; i++) {
		bits[11 + i] = temp_value % 2;
		temp_value /= 2;
	}

	temp_value = reg_num2;
	for (i = 0; temp_value > 0; i++) {
		bits[16 + i] = temp_value % 2;
		temp_value /= 2;
	}

	temp_value = shamt;
	for (i = 0; temp_value > 0; i++) {
		bits[6 + i] = temp_value % 2;
		temp_value /= 2;
	}

	// srl -> opcode = 0/02 (hex)
	temp_value = 2;
	for (i = 0; temp_value>0; i++) {
		bits[26 + i] = temp_value % 2;
		temp_value /= 2;
	}

	// adds all value of bits 
	for (i = 0; i < 32; i++) {
		result_temp = 1;
		if (bits[i] == 1) {
			for (int d = 0; d < i; d++) {
				result_temp *= 2;
			}
			result += result_temp;
		}

	}

	// execute operation (srl) 
	for (int a = 0; a < reg[shamt].content * 2; a++) {
		reg[reg_num1].content /= 2;
	}

	return result;
}
int sw(char * sArr_3[]) {
	char * sArr_result[3] = { "" };
	char * ptr;
	char * sArr_memory[2] = { "" };
	int i = 0;

	for (int j = 0; j < 4; j++) {
		sArr_result[j] = strtok(sArr_3[j], "$");
	}

	// memory reference
	// divide format a($b) into a b
	sArr_memory[0] = strtok(sArr_result[2], "(");
	ptr = strtok(sArr_result[2], "(");
	while (ptr != NULL) {
		sArr_memory[i] = ptr;
		i++;
		ptr = strtok(NULL, "(");
	}
		sArr_memory[1] = strtok(sArr_memory[1], ")");
		sArr_memory[1] = strtok(sArr_memory[1], "$");

	

	
	

	return 0;
}
int subu(char * sArr_3[]) {
	char * sArr_result[4] = { "" };
	int reg_num1;
	int reg_num2;
	int reg_num3;
	int temp_value;
	int i;
	int result_temp;
	int result = 0;

	//erasing '$'
	for (int j = 0; j < 4; j++) {
		sArr_result[j] = strtok(sArr_3[j], "$");
	
	}
	reg_num1 = atoi(sArr_result[1]);
	reg_num2 = atoi(sArr_result[2]);
	reg_num3 = atoi(sArr_result[3]);

	// translate into machine code

	temp_value = reg_num1;
	for (i = 0; temp_value > 0; i++) {
		bits[11 + i] = temp_value % 2;
		temp_value /= 2;
	}

	temp_value = reg_num2;
	for (i = 0; temp_value > 0; i++) {
		bits[21 + i] = temp_value % 2;
		temp_value /= 2;
	}

	temp_value = reg_num3;
	for (i = 0; temp_value > 0; i++) {
		bits[16 + i] = temp_value % 2;
		temp_value /= 2;
	}
	// subu -> opcode = 0/23 (hex)
	temp_value = 33;
	for (i = 0; temp_value>0; i++) {
		bits[i] = temp_value % 2;
		temp_value /= 2;
	}
	// adds all value of bits 
	for (i = 0; i < 32; i++) {
		result_temp = 1;
		if (bits[i] == 1) {
			for (int d = 0; d < i; d++) {
				result_temp *= 2;
			}
			result += result_temp;
		}

	}

	// execute operation (subu) 
	reg[reg_num1].content = reg[reg_num2].content - reg[reg_num3].content;
	return result;

}
int hexTodec(char* sArr) {
	char* hexadecimal = { NULL, };
	hexadecimal = sArr;
	int decimal = 0;             

	int position = 0;
	for (int i = strlen(hexadecimal) - 2; i >= 2; i--){
		char ch = hexadecimal[i]; 
		if (ch >= 48 && ch <= 57){
			decimal += (ch - 48) * pow(16, position);
		}
		else if (ch >= 65 && ch <= 70){                                 
			decimal += (ch - (65 - 10)) * pow(16, position);
		}
		else if (ch >= 97 && ch <= 102){
			decimal += (ch - (97 - 10)) * pow(16, position);
		}

		position++;
	}
	return decimal;
}
