#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NUMINTRU  21 // the number of instrunction
#define LENGINTRU 6 // the maximum length of instruction
#define MAXLINE 100 // the maximum line that the program can accept

void pass1(); // one pass 
void pass2();// two pass

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

int PC = 4194308; // program counter
int CUR_ADDR = 4194304; // current address
int bits[32] = { 0 }; // store each bit before translate into hexa code
// define register (0 - 31) 
struct REGISTER reg[32]; // define register
struct SYMBOL_TABLE table[10]; // define symbol table for one pass
int sym_count = 0; // the number of symbol label
int line_count = 0; // the number of instruction set 
// instruction set
char instruction[NUMINTRU][LENGINTRU] = { "ADDIU","ADDU","AND","ANDI","BEQ","BNE","J","JAL","JR","LUI","LW","LA","NOR","OR","ORI","SLTIU",
"SLTU","SLL","SRL","SW","SUBU" };



int main() {
	
	pass1(); // pass 1

	pass2(); // pass 2
	
	return 0;
	
}
// make symbol table
void pass1() {
	FILE *fin; // file open
	char s[81]; // store each line of input data
	char *ptr; // variable for erasing " " 
	char *sArr[6] = { "" }; // store string after erasing " " 
	char *sArr_2[6] = { "" }; // store string after erasing "," 
	char *sArr_3[6] = { "" }; // store string after erasing "\t" 
	int i = 0;

	fin = fopen("test_in.s", "r");
	//open error check
	if (fin == NULL) {
		printf("failed to file open\n");
		return 1;
	}
	while (!feof(fin)) {
		// read a line
		fgets(s, 80, fin);
		// increment of PC 
		PC = PC + 4;

		// erasing " "
		ptr = strtok(s, " ");

		// main 안에만 구현했다고 가정

		while (ptr != NULL) {
			sArr[i] = ptr;  
			i++;
			ptr = strtok(NULL, " ");
		}
		// erasing ','
		for (int j = 0; j < i; j++) {
			sArr_2[j] = strtok(sArr[j], ",");
		}
		// erasing '\t'
		for (int j = 0; j < i; j++) {
			sArr_3[j] = strtok(sArr_2[j], "\t");
			//printf("%s", sArr_3[j]);
		}
		
		// if the string is label
		if (!(strchr(sArr_3[0], ':') == NULL)) {
			// adds symbol table
			strcpy(table[sym_count].symbol, strtok(sArr_3[0], ":"));
			table[sym_count].address = CUR_ADDR;
			sym_count++;
		}
		
		CUR_ADDR = PC;
		i = 0;
	}
	// prints symbol table
	printf("--------SYMBOL_TABLE---------\n");
	for (int z = 0; z < sym_count; z++) {
		printf("SYMBOL : %s ADDR : 0x%x\n", table[z].symbol, table[z].address);
	}
	fclose(fin);
}
void pass2() {
	int modevalue;
	int result;

	struct LINE line[MAXLINE];
	FILE *fin;
	FILE *fout;
	char s[81]; // store each line of input data
	char *ptr; // variable for erasing " " 
	char *sArr[6] = { "" }; // store string after erasing " " 
	char *sArr_2[6] = { "" }; // store string after erasing "," 
	char *sArr_3[6] = { "" }; // store string after erasing "\t" 
	int i = 0; // the number of token by line

	fin = fopen("test_in.s", "r");
	fout = fopen("test_out.txt", "w");

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
		// read a line
		fgets(s, 80, fin);
		// increment of PC 
		PC = PC + 4;

		// erasing " "
		ptr = strtok(s, " ");
		
		while (ptr != NULL) {
			sArr[i] = ptr; 
			i++;
			ptr = strtok(NULL, " ");
		}
		// erasing ","
		for (int j = 0; j < i; j++) {
			sArr_2[j] = strtok(sArr[j], ",");
		}
		// erasing "\t"
		for (int j = 0; j < i; j++) {
			sArr_3[j] = strtok(sArr_2[j], "\t");
			//printf("%s", sArr_3[j]);
		}
		
		// comparing with instruction set
		for (int k = 0; k < NUMINTRU; k++) {
			// if the string is LABEL
			if (!(strchr(sArr_3[0], ':') == NULL)) {
				line[line_count].address = CUR_ADDR;
				line[line_count].content = NULL;
				break;
			}
			// if the string is not LABEL
			if (!strcmp(sArr_3[0], instruction[k])) {
				modevalue = k;
				// initializes bits
				for (int a = 0; a < 32; a++) {
					bits[a] = 0;
				}
				switch (modevalue)
				{
				case 0:
					line[line_count].address = CUR_ADDR;
					line[line_count].content = addiu(sArr_3);
					break;
				case 1:
					line[line_count].address = CUR_ADDR;
					line[line_count].content = addu(sArr_3);
					break;
				case 2:
					line[line_count].address = CUR_ADDR;
					line[line_count].content = and (sArr_3);
					break;
				case 3:
					line[line_count].address = CUR_ADDR;
					line[line_count].content = andi(sArr_3);
					break;
				case 4:
					line[line_count].address = CUR_ADDR;
					line[line_count].content = beq(sArr_3);
					break;
				case 5:
					line[line_count].address = CUR_ADDR;
					line[line_count].content = bne(sArr_3);
					break;
				case 6:
					line[line_count].address = CUR_ADDR;
					line[line_count].content = j(sArr_3);
					break;
				case 7:
					line[line_count].address = CUR_ADDR;
					line[line_count].content = jal(sArr_3);
					break;
				case 8:
					line[line_count].address = CUR_ADDR;
					line[line_count].content = jr(sArr_3);
					break;
				case 9:
					line[line_count].address = CUR_ADDR;
					line[line_count].content = lui(sArr_3);
					break;
				case 10:
					line[line_count].address = CUR_ADDR;
					line[line_count].content = lw(sArr_3);
					break;
				case 11:
					line[line_count].address = CUR_ADDR;
					line[line_count].content = la(sArr_3);
					break;
				case 12:
					line[line_count].address = CUR_ADDR;
					line[line_count].content = nor(sArr_3);
					break;
				case 13:
					line[line_count].address = CUR_ADDR;
					line[line_count].content = or (sArr_3);
					break;
				case 14:
					line[line_count].address = CUR_ADDR;
					line[line_count].content = ori(sArr_3);
					break;
				case 15:
					line[line_count].address = CUR_ADDR;
					line[line_count].content = sltiu(sArr_3);
					break;
				case 16:
					line[line_count].address = CUR_ADDR;
					line[line_count].content = sltu(sArr_3);
					break;
				case 17:
					line[line_count].address = CUR_ADDR;
					line[line_count].content = sll(sArr_3);
					break;
				case 18:
					line[line_count].address = CUR_ADDR;
					line[line_count].content = srl(sArr_3);
					break;
				case 19:
					line[line_count].address = CUR_ADDR;
					line[line_count].content = sw(sArr_3);
					break;
				case 20:
					line[line_count].address = CUR_ADDR;
					line[line_count].content = subu(sArr_3);
					break;
				default:
					break;
				}
			}

		}

		CUR_ADDR = PC;
		line_count++;
		i = 0;
	}
	// prints the result 
	printf("---------INSTRUCTION----------\n");
	for (int a = 0; a < line_count; a++) {
		printf("address : 0x%06x  content : 0x%08x\n", line[a].address, line[a].content);
	}
	// prints the number of label
	printf("line : %d \n", line_count);
	//printf("Cur addr : %x\n", CUR_ADDR);
	//printf("pc : %x\n", PC);

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
		//printf("%s ", sArr_result[j]);
	}
	reg_num1 = atoi(sArr_result[1]);
	reg_num2 = atoi(sArr_result[2]);
	immediate = atoi(sArr_result[3]);
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
	immediate = atoi(sArr_result[3]);

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
	
	

	// execute operation (beq) 
	if (reg[reg_num1].content == reg[reg_num2].content) {
		// searchin SYMBOL_TABLE 
		for (int i = 0; i < sym_count; i++) {
			// if it is correct
			if (!(strcmp(table[i].symbol, sArr_result[3]))) {
				addr = table[i].address;
				break;
			}
		}
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
	
	
	// execute operation (bne) 
	if (reg[reg_num1].content != reg[reg_num2].content) {
		// searchin SYMBOL_TABLE 
		for (int i = 0; i < sym_count; i++) {
			// if it is correct
			if (!(strcmp(table[i].symbol, sArr_result[3]))) {
				addr = table[i].address;
				break;
			}
		}
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
	// symbol table의 address
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
	// symbol table의 address
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
		printf("%s ", sArr_result[j]);
	}
	reg_num1 = atoi(sArr_result[1]);
	reg_num2 = atoi(sArr_result[2]);
	immediate = atoi(sArr_result[3]);

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
		//printf("%s ", sArr_result[j]);
	}
	reg_num1 = atoi(sArr_result[1]);
	reg_num2 = atoi(sArr_result[2]);
	immediate = atoi(sArr_result[3]);
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
		//printf("%s ", sArr_result[j]);
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
		//printf("%s ", sArr_result[j]);
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
