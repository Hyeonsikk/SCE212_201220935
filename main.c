#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NUMINTRU  21
#define LENGINTRU 6
#define MAXLINE 100

void pass1();
void pass2();
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
// Line �� ����
struct LINE {
	int address;
	int content;
};
// REGISTER ����
struct REGISTER {
	int content;
	int address;
};

int PC = 4194308;
int CUR_ADDR = 4194304;
int bits[32] = { 0 };
// define register (0 - 31) 
struct REGISTER $reg[32];
struct SYMBOL_TABLE table[10];
int sym_count = 0;
int line_count = 0;
char instruction[NUMINTRU][LENGINTRU] = { "ADDIU","ADDU","AND","ANDI","BEQ","BNE","J","JAL","JR","LUI","LW","LA","NOR","OR","ORI","SLTIU",
"SLTU","SLL","SRL","SW","SUBU" };



int main() {
	
	pass1();

	pass2();
	
	return 0;
	
}
void pass1() {
	FILE *fin;
	char s[81];
	char *ptr; // " " �ڸ��� ����
	char *sArr[6] = { "" }; // " "�� �ڸ� ���ڿ� ����
	char *sArr_2[6] = { "" }; // ","�� �ڸ� ���ڿ� ����
	char *sArr_3[6] = { "" }; // "\t"�� �ڸ� ���ڿ� ����
	int i = 0;

	fin = fopen("test_in.txt", "r");
	//open error check
	if (fin == NULL) {
		printf("failed to file open\n");
		return 1;
	}
	while (!feof(fin)) {
		// �� �پ� �о����
		fgets(s, 80, fin);
		// PC �� ����
		PC = PC + 4;

		//printf("PC : 0x%06x", pc); --> pc�� ���

		// " "�� �ڸ���
		ptr = strtok(s, " ");
		// main �ȿ��� �����ߴٰ� ����

		while (ptr != NULL) {
			sArr[i] = ptr; // ���ڿ��� �ڸ� �� �޸� �ּҸ� ���ڿ� ������ �迭�� ����
			i++;
			ptr = strtok(NULL, " ");
		}
		// ',' �ڸ���
		for (int j = 0; j < i; j++) {
			sArr_2[j] = strtok(sArr[j], ",");
		}
		// '\t' �ڸ���
		for (int j = 0; j < i; j++) {
			sArr_3[j] = strtok(sArr_2[j], "\t");
			//printf("%s", sArr_3[j]);
		}
		//printf("\n");
		
		// label�� ���
		if (!(strchr(sArr_3[0], ':') == NULL)) {
			// symbol table�� �߰�
			strcpy(table[sym_count].symbol, strtok(sArr_3[0], ":"));
			table[sym_count].address = CUR_ADDR;
			sym_count++;
		}
		
		CUR_ADDR = PC;
		i = 0;
	}
	// symbol table ���
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
	char s[81];
	char *ptr; // " " �ڸ��� ����
	char *sArr[6] = { "" }; // " "�� �ڸ� ���ڿ� ����
	char *sArr_2[6] = { "" }; // ","�� �ڸ� ���ڿ� ����
	char *sArr_3[6] = { "" }; // "\t"�� �ڸ� ���ڿ� ����
	int i = 0; // ���� �� token �� 

	fin = fopen("test_in.txt", "r");
	fout = fopen("test_out.txt", "w");

	if (fin == NULL) {
		printf("failed to file open\n");
		return 1;
	}
	if (fout == NULL) {
		printf("failed to file open\n");
		return 1;
	}

	// pass 2 current_address, PC, line_count �� �ʱ�ȭ
	CUR_ADDR = 4194304;
	PC = CUR_ADDR + 4;
	line_count = 0;
	while (!feof(fin)) {
		// �� �پ� �о����
		fgets(s, 80, fin);
		// PC �� ����
		PC = PC + 4;

		//printf("PC : 0x%06x", pc); --> pc�� ���

		// " "�� �ڸ���
		ptr = strtok(s, " ");
		// main �ȿ��� �����ߴٰ� ����

		while (ptr != NULL) {
			sArr[i] = ptr; // ���ڿ��� �ڸ� �� �޸� �ּҸ� ���ڿ� ������ �迭�� ����
			i++;
			ptr = strtok(NULL, " ");
		}
		// ',' �ڸ���
		for (int j = 0; j < i; j++) {
			sArr_2[j] = strtok(sArr[j], ",");
		}
		// '\t' �ڸ���
		for (int j = 0; j < i; j++) {
			sArr_3[j] = strtok(sArr_2[j], "\t");
			//printf("%s", sArr_3[j]);
		}
		//printf("\n");
		// instruction set�� �� ����
		for (int k = 0; k < NUMINTRU; k++) {
			//LABEL�� ���
			if (!(strchr(sArr_3[0], ':') == NULL)) {
				line[line_count].address = CUR_ADDR;
				line[line_count].content = NULL;
				break;
			}
			// �ƴ� ���
			if (!strcmp(sArr_3[0], instruction[k])) {
				modevalue = k;
				// bits �ʱ�ȭ
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
	// ����� ���
	printf("---------INSTRUCTION----------\n");
	for (int a = 0; a < line_count; a++) {
		printf("address : 0x%06x  content : 0x%08x\n", line[a].address, line[a].content);
	}
	// line �� ���
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
	//'$'�ڸ���
	for (int j = 0; j < 4; j++) {
		sArr_result[j] = strtok(sArr_3[j], "$");
		//printf("%s ", sArr_result[j]);
	}
	reg_num1 = atoi(sArr_result[1]);
	reg_num2 = atoi(sArr_result[2]);
	immediate = atoi(sArr_result[3]);
	// machine code ��ȯ
	temp_value = reg_num1;
	for (i = 0; temp_value > 0; i++) {
		 bits[16+i] = temp_value % 2;
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
	
	// ��� ���Ѵ�
	for (i = 0; i < 32; i++) {
		result_temp = 1;
		if (bits[i] == 1) {
			for (int d = 0; d < i; d++) {
				result_temp *= 2;
			}
			result += result_temp;
		}
		 
	}
	// addiu ���� ����
	$reg[reg_num1].content = $reg[reg_num2].content + immediate;
	
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

	//'$'�ڸ���
	for (int j = 0; j < 4; j++) {
		sArr_result[j] = strtok(sArr_3[j], "$");
		//printf("%s ", sArr_result[j]);
	}
	reg_num1 = atoi(sArr_result[1]);
	reg_num2 = atoi(sArr_result[2]);
	reg_num3 = atoi(sArr_result[3]);

	

	// machine code ��ȯ
	
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
	// ��� ���Ѵ�
	for (i = 0; i < 32; i++) {
		result_temp = 1;
		if (bits[i] == 1) {
			for (int d = 0; d < i; d++) {
				result_temp *= 2;
			}
			result += result_temp;
		}

	}

	// addu ���� ����
	$reg[reg_num1].content = $reg[reg_num2].content + $reg[reg_num3].content;
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

	//'$'�ڸ���
	for (int j = 0; j < 4; j++) {
		sArr_result[j] = strtok(sArr_3[j], "$");
		//printf("%s ", sArr_result[j]);
	}
	reg_num1 = atoi(sArr_result[1]);
	reg_num2 = atoi(sArr_result[2]);
	reg_num3 = atoi(sArr_result[3]);


	// machine code ��ȯ

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
	// ��� ���Ѵ�
	for (i = 0; i < 32; i++) {
		result_temp = 1;
		if (bits[i] == 1) {
			for (int d = 0; d < i; d++) {
				result_temp *= 2;
			}
			result += result_temp;
		}

	}

	// and ���� ����
	$reg[reg_num1].content = $reg[reg_num2].content & $reg[reg_num3].content;

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

	//'$'�ڸ���
	for (int j = 0; j < 4; j++) {
		sArr_result[j] = strtok(sArr_3[j], "$");
		printf("%s ", sArr_result[j]);
	}
	reg_num1 = atoi(sArr_result[1]);
	reg_num2 = atoi(sArr_result[2]);
	immediate = atoi(sArr_result[3]);

	// machine code ��ȯ
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

	// ��� ���Ѵ�
	for (i = 0; i < 32; i++) {
		result_temp = 1;
		if (bits[i] == 1) {
			for (int d = 0; d < i; d++) {
				result_temp *= 2;
			}
			result += result_temp;
		}

	}

	// andi ���� ����
	$reg[reg_num1].content = $reg[reg_num2].content & immediate;
	
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

	//'$'�ڸ���
	for (int j = 0; j < 4; j++) {
		sArr_result[j] = strtok(sArr_3[j], "$");
		//printf("%s ", sArr_result[j]);
	}
	reg_num1 = atoi(sArr_result[1]);
	reg_num2 = atoi(sArr_result[2]);
	
	

	// beq ���� ����
	if ($reg[reg_num1].content == $reg[reg_num2].content) {
		// SYMBOL_TABLE �˻�
		for (int i = 0; i < sym_count; i++) {
			// ��ġ�ϴ� ���
			if (!(strcmp(table[i].symbol, sArr_result[3]))) {
				addr = table[i].address;
				break;
			}
		}
		PC = PC + 4 + addr;
	}

	// machine code ��ȯ
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

	// ��� ���Ѵ�
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

	//'$'�ڸ���
	for (int j = 0; j < 4; j++) {
		sArr_result[j] = strtok(sArr_3[j], "$");
		//printf("%s ", sArr_result[j]);
	}
	reg_num1 = atoi(sArr_result[1]);
	reg_num2 = atoi(sArr_result[2]);
	
	
	// bne ���� ����
	if ($reg[reg_num1].content != $reg[reg_num2].content) {
		// SYMBOL_TABLE �˻�
		for (int i = 0; i < sym_count; i++) {
			// ��ġ�ϴ� ���
			if (!(strcmp(table[i].symbol, sArr_result[3]))) {
				addr = table[i].address;
				break;
			}
		}
		 PC = PC+4+addr;
	}
	// machine code ��ȯ
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

	// ��� ���Ѵ�
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
	
	// SYMBOL_TABLE �˻�
	for (int i = 0; i < sym_count; i++) {
		// ��ġ�ϴ� ���
		if (!(strcmp(table[i].symbol, sArr_3[1]))) {
			PC = table[i].address;
			addr  = table[i].address;
			break;
		}
	}
	// machine code ��ȯ
	
	// j -> opcode = 2 (hex)
	temp_value = 2;
	for (i = 0; temp_value>0; i++) {
		bits[26 + i] = temp_value % 2;
		temp_value /= 2;
	}
	// symbol table�� address
	temp_value = addr;
	for (i = 0; temp_value>0; i++) {
		bits[i] = temp_value % 2;
		temp_value /= 2;
	}

	// ��� ���Ѵ�
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

	// jal ���� ����
	$reg[31].content = PC + 8;
	// SYMBOL_TABLE �˻�
	for (int i = 0; i < sym_count; i++) {
		// ��ġ�ϴ� ���
		if (!(strcmp(table[i].symbol, sArr_3[1]))) {
			PC = table[i].address;
			addr = table[i].address;
			break;
		}
	}

	// machine code ��ȯ

	// j -> opcode = 2 (hex)
	temp_value = 2;
	for (i = 0; temp_value>0; i++) {
		bits[26 + i] = temp_value % 2;
		temp_value /= 2;
	}
	// symbol table�� address
	temp_value = addr;
	for (i = 0; temp_value>0; i++) {
		bits[i] = temp_value % 2;
		temp_value /= 2;
	}

	// ��� ���Ѵ�
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

	//'$'�ڸ���
	
	sArr_result[1] = strtok(sArr_3[1], "$");
	reg_num1 = atoi(sArr_result[1]);
	
	// machine code ��ȯ

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
	// ��� ���Ѵ�
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

	//'$'�ڸ���
	for (int j = 0; j < 4; j++) {
		sArr_result[j] = strtok(sArr_3[j], "$");
		//printf("%s ", sArr_result[j]);
	}
	reg_num1 = atoi(sArr_result[1]);
	reg_num2 = atoi(sArr_result[2]);
	reg_num3 = atoi(sArr_result[3]);

	// machine code ��ȯ

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
	// ��� ���Ѵ�
	for (i = 0; i < 32; i++) {
		result_temp = 1;
		if (bits[i] == 1) {
			for (int d = 0; d < i; d++) {
				result_temp *= 2;
			}
			result += result_temp;
		}

	}

	// and ���� ����
	$reg[reg_num1].content = !($reg[reg_num2].content || $reg[reg_num3].content);

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

	//'$'�ڸ���
	for (int j = 0; j < 4; j++) {
		sArr_result[j] = strtok(sArr_3[j], "$");
		//printf("%s ", sArr_result[j]);
	}
	reg_num1 = atoi(sArr_result[1]);
	reg_num2 = atoi(sArr_result[2]);
	reg_num3 = atoi(sArr_result[3]);

	// machine code ��ȯ

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
	// ��� ���Ѵ�
	for (i = 0; i < 32; i++) {
		result_temp = 1;
		if (bits[i] == 1) {
			for (int d = 0; d < i; d++) {
				result_temp *= 2;
			}
			result += result_temp;
		}

	}

	// and ���� ����
	$reg[reg_num1].content = $reg[reg_num2].content || $reg[reg_num3].content;

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

	//'$'�ڸ���
	for (int j = 0; j < 4; j++) {
		sArr_result[j] = strtok(sArr_3[j], "$");
		printf("%s ", sArr_result[j]);
	}
	reg_num1 = atoi(sArr_result[1]);
	reg_num2 = atoi(sArr_result[2]);
	immediate = atoi(sArr_result[3]);

	// machine code ��ȯ
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

	// ��� ���Ѵ�
	for (i = 0; i < 32; i++) {
		result_temp = 1;
		if (bits[i] == 1) {
			for (int d = 0; d < i; d++) {
				result_temp *= 2;
			}
			result += result_temp;
		}

	}

	// and ���� ����
	$reg[reg_num1].content = $reg[reg_num2].content || immediate;
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
	//'$'�ڸ���
	for (int j = 0; j < 4; j++) {
		sArr_result[j] = strtok(sArr_3[j], "$");
		//printf("%s ", sArr_result[j]);
	}
	reg_num1 = atoi(sArr_result[1]);
	reg_num2 = atoi(sArr_result[2]);
	immediate = atoi(sArr_result[3]);
	// machine code ��ȯ
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

	// ��� ���Ѵ�
	for (i = 0; i < 32; i++) {
		result_temp = 1;
		if (bits[i] == 1) {
			for (int d = 0; d < i; d++) {
				result_temp *= 2;
			}
			result += result_temp;
		}

	}
	// sltiu ���� ����
	$reg[reg_num1].content = ($reg[reg_num2].content, immediate)? 1:0;

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

	//'$'�ڸ���
	for (int j = 0; j < 4; j++) {
		sArr_result[j] = strtok(sArr_3[j], "$");
		//printf("%s ", sArr_result[j]);
	}
	reg_num1 = atoi(sArr_result[1]);
	reg_num2 = atoi(sArr_result[2]);
	reg_num3 = atoi(sArr_result[3]);



	// machine code ��ȯ

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
	// ��� ���Ѵ�
	for (i = 0; i < 32; i++) {
		result_temp = 1;
		if (bits[i] == 1) {
			for (int d = 0; d < i; d++) {
				result_temp *= 2;
			}
			result += result_temp;
		}

	}

	// sltu ���� ����
	//R[rd] = (R[rs] < R[rt]) ? 1 : 0
	$reg[reg_num1].content = ($reg[reg_num2].content < $reg[reg_num3].content)? 1:0;
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

	//'$'�ڸ���
	for (int j = 0; j < 4; j++) {
		sArr_result[j] = strtok(sArr_3[j], "$");
		//printf("%s ", sArr_result[j]);
	}
	reg_num1 = atoi(sArr_result[1]);
	reg_num2 = atoi(sArr_result[2]);
	shamt = atoi(sArr_result[3]);



	// machine code ��ȯ

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

	
	// ��� ���Ѵ�
	for (i = 0; i < 32; i++) {
		result_temp = 1;
		if (bits[i] == 1) {
			for (int d = 0; d < i; d++) {
				result_temp *= 2;
			}
			result += result_temp;
		}

	}
	
	// ����
	for (int a = 0; a < $reg[shamt].content * 2; a++) {
		$reg[reg_num1].content *= 2;
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

	//'$'�ڸ���
	for (int j = 0; j < 4; j++) {
		sArr_result[j] = strtok(sArr_3[j], "$");
		//printf("%s ", sArr_result[j]);
	}
	reg_num1 = atoi(sArr_result[1]);
	reg_num2 = atoi(sArr_result[2]);
	shamt = atoi(sArr_result[3]);

	// machine code ��ȯ

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

	// ��� ���Ѵ�
	for (i = 0; i < 32; i++) {
		result_temp = 1;
		if (bits[i] == 1) {
			for (int d = 0; d < i; d++) {
				result_temp *= 2;
			}
			result += result_temp;
		}

	}

	// ����
	for (int a = 0; a < $reg[shamt].content * 2; a++) {
		$reg[reg_num1].content /= 2;
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

	//'$'�ڸ���
	for (int j = 0; j < 4; j++) {
		sArr_result[j] = strtok(sArr_3[j], "$");
		//printf("%s ", sArr_result[j]);
	}
	reg_num1 = atoi(sArr_result[1]);
	reg_num2 = atoi(sArr_result[2]);
	reg_num3 = atoi(sArr_result[3]);

	// machine code ��ȯ

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
	// ��� ���Ѵ�
	for (i = 0; i < 32; i++) {
		result_temp = 1;
		if (bits[i] == 1) {
			for (int d = 0; d < i; d++) {
				result_temp *= 2;
			}
			result += result_temp;
		}

	}

	// addu ���� ����
	$reg[reg_num1].content = $reg[reg_num2].content - $reg[reg_num3].content;
	return result;

}