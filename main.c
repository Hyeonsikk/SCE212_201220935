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
// Line 별 정보
struct LINE {
	int address;
	int content;
};
// REGISTER 정보
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
	char *ptr; // " " 자르는 변수
	char *sArr[6] = { "" }; // " "로 자른 문자열 저장
	char *sArr_2[6] = { "" }; // ","로 자른 문자열 저장
	char *sArr_3[6] = { "" }; // "\t"로 자른 문자열 저장
	int i = 0;

	fin = fopen("test_in.txt", "r");
	//open error check
	if (fin == NULL) {
		printf("failed to file open\n");
		return 1;
	}
	while (!feof(fin)) {
		// 한 줄씩 읽어오기
		fgets(s, 80, fin);
		// PC 값 증가
		PC = PC + 4;

		//printf("PC : 0x%06x", pc); --> pc값 출력

		// " "로 자른다
		ptr = strtok(s, " ");
		// main 안에만 구현했다고 가정

		while (ptr != NULL) {
			sArr[i] = ptr; // 문자열을 자른 뒤 메모리 주소를 문자열 포인터 배열에 저장
			i++;
			ptr = strtok(NULL, " ");
		}
		// ',' 자르기
		for (int j = 0; j < i; j++) {
			sArr_2[j] = strtok(sArr[j], ",");
		}
		// '\t' 자르기
		for (int j = 0; j < i; j++) {
			sArr_3[j] = strtok(sArr_2[j], "\t");
			//printf("%s", sArr_3[j]);
		}
		//printf("\n");
		
		// label인 경우
		if (!(strchr(sArr_3[0], ':') == NULL)) {
			// symbol table에 추가
			strcpy(table[sym_count].symbol, strtok(sArr_3[0], ":"));
			table[sym_count].address = CUR_ADDR;
			sym_count++;
		}
		
		CUR_ADDR = PC;
		i = 0;
	}
	// symbol table 출력
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
	char *ptr; // " " 자르는 변수
	char *sArr[6] = { "" }; // " "로 자른 문자열 저장
	char *sArr_2[6] = { "" }; // ","로 자른 문자열 저장
	char *sArr_3[6] = { "" }; // "\t"로 자른 문자열 저장
	int i = 0; // 라인 별 token 수 

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

	// pass 2 current_address, PC, line_count 값 초기화
	CUR_ADDR = 4194304;
	PC = CUR_ADDR + 4;
	line_count = 0;
	while (!feof(fin)) {
		// 한 줄씩 읽어오기
		fgets(s, 80, fin);
		// PC 값 증가
		PC = PC + 4;

		//printf("PC : 0x%06x", pc); --> pc값 출력

		// " "로 자른다
		ptr = strtok(s, " ");
		// main 안에만 구현했다고 가정

		while (ptr != NULL) {
			sArr[i] = ptr; // 문자열을 자른 뒤 메모리 주소를 문자열 포인터 배열에 저장
			i++;
			ptr = strtok(NULL, " ");
		}
		// ',' 자르기
		for (int j = 0; j < i; j++) {
			sArr_2[j] = strtok(sArr[j], ",");
		}
		// '\t' 자르기
		for (int j = 0; j < i; j++) {
			sArr_3[j] = strtok(sArr_2[j], "\t");
			//printf("%s", sArr_3[j]);
		}
		//printf("\n");
		// instruction set과 비교 성공
		for (int k = 0; k < NUMINTRU; k++) {
			//LABEL인 경우
			if (!(strchr(sArr_3[0], ':') == NULL)) {
				line[line_count].address = CUR_ADDR;
				line[line_count].content = NULL;
				break;
			}
			// 아닌 경우
			if (!strcmp(sArr_3[0], instruction[k])) {
				modevalue = k;
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
	// 결과물 출력
	printf("---------INSTRUCTION----------\n");
	for (int a = 0; a < line_count; a++) {
		printf("address : 0x%06x  content : 0x%06x\n", line[a].address, line[a].content);
	}
	// line 수 출력
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
	int temp[5];
	int temp_value;
	int immediate;
	int i, k;
	//'$'자르기
	for (int j = 0; j < 4; j++) {
		sArr_result[j] = strtok(sArr_3[j], "$");
		//printf("%s ", sArr_result[j]);
	}
	reg_num1 = atoi(sArr_result[1]);
	reg_num2 = atoi(sArr_result[2]);
	immediate = atoi(sArr_result[3]);
	// machine code 변환
	
	// addiu 연산 실행
	$reg[reg_num1].content = $reg[reg_num2].content + immediate;
	
	return 4194396;
}
int addu(char* sArr_3[]) {
	char * sArr_result[4] = { "" };
	int reg_num1;
	int reg_num2;
	int reg_num3;

	//'$'자르기
	for (int j = 0; j < 4; j++) {
		sArr_result[j] = strtok(sArr_3[j], "$");
		//printf("%s ", sArr_result[j]);
	}
	reg_num1 = atoi(sArr_result[1]);
	reg_num2 = atoi(sArr_result[2]);
	reg_num3 = atoi(sArr_result[3]);

	// addu 연산 실행
	$reg[reg_num1].content = $reg[reg_num2].content + $reg[reg_num3].content;

	// machine code 변환
	return 4194400;
}
int and(char* sArr_3[]) {
	char * sArr_result[4] = { "" };
	int reg_num1;
	int reg_num2;
	int reg_num3;

	//'$'자르기
	for (int j = 0; j < 4; j++) {
		sArr_result[j] = strtok(sArr_3[j], "$");
		//printf("%s ", sArr_result[j]);
	}
	reg_num1 = atoi(sArr_result[1]);
	reg_num2 = atoi(sArr_result[2]);
	reg_num3 = atoi(sArr_result[3]);

	// and 연산 실행
	$reg[reg_num1].content = $reg[reg_num2].content & $reg[reg_num3].content;

	return 0;
}
int andi(char* sArr_3[]) {
	char * sArr_result[4] = { "" };
	int reg_num1;
	int reg_num2;
	int immediate;

	//'$'자르기
	for (int j = 0; j < 4; j++) {
		sArr_result[j] = strtok(sArr_3[j], "$");
		printf("%s ", sArr_result[j]);
	}
	reg_num1 = atoi(sArr_result[1]);
	reg_num2 = atoi(sArr_result[2]);
	immediate = atoi(sArr_result[3]);
	// andi 연산 실행
	$reg[reg_num1].content = $reg[reg_num2].content & immediate;
	
	return 0;
}
int beq(char* sArr_3[]) {
	char * sArr_result[4] = { "" };
	int reg_num1;
	int reg_num2;
	int addr;

	//'$'자르기
	for (int j = 0; j < 4; j++) {
		sArr_result[j] = strtok(sArr_3[j], "$");
		printf("%s ", sArr_result[j]);
	}
	reg_num1 = atoi(sArr_result[1]);
	reg_num2 = atoi(sArr_result[2]);
	
	// symbol table 만들어서 sArr_result[3] 주소값 구하기

	// beq 연산 실행
	if ($reg[reg_num1].content == $reg[reg_num2].content) {
		// SYMBOL_TABLE 검색
		for (int i = 0; i < sym_count; i++) {
			// 일치하는 경우
			if (!(strcmp(table[i].symbol, sArr_result[3]))) {
				addr = table[i].address;
				break;
			}
		}
		PC = PC + 4 + addr;
	}
	
	return 0;
}
int bne(char* sArr_3[]) {
	char * sArr_result[4] = { "" };
	int reg_num1;
	int reg_num2;
	int addr;

	//'$'자르기
	for (int j = 0; j < 4; j++) {
		sArr_result[j] = strtok(sArr_3[j], "$");
		printf("%s ", sArr_result[j]);
	}
	reg_num1 = atoi(sArr_result[1]);
	reg_num2 = atoi(sArr_result[2]);
	
	
	// bne 연산 실행
	if ($reg[reg_num1].content != $reg[reg_num2].content) {
		// SYMBOL_TABLE 검색
		for (int i = 0; i < sym_count; i++) {
			// 일치하는 경우
			if (!(strcmp(table[i].symbol, sArr_result[3]))) {
				addr = table[i].address;
				break;
			}
		}
		 PC = PC+4+addr;
	}
	
	return 0;
}
int j(char* sArr_3[]) {
	char * sArr_result[2] = { "" };
	
	// SYMBOL_TABLE 검색
	for (int i = 0; i < sym_count; i++) {
		// 일치하는 경우
		if (!(strcmp(table[i].symbol, sArr_result[1]))) {
			PC = table[i].address;
			break;
		}
	}
	// machine code 변환
		
	
	return 0;
}
int jal(char* sArr_3[]) {
	char * sArr_result[2] = { "" };


	// jal 연산 실행
	$reg[31].content = PC + 8;
	// SYMBOL_TABLE 검색
	for (int i = 0; i < sym_count; i++) {
		// 일치하는 경우
		if (!(strcmp(table[i].symbol, sArr_result[1]))) {
			PC = table[i].address;
			break;
		}
	}

	// machine code 변환
	return 0;
}
int jr(char* sArr_3[]) {
	return 0;
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

	//'$'자르기
	for (int j = 0; j < 4; j++) {
		sArr_result[j] = strtok(sArr_3[j], "$");
		//printf("%s ", sArr_result[j]);
	}
	reg_num1 = atoi(sArr_result[1]);
	reg_num2 = atoi(sArr_result[2]);
	reg_num3 = atoi(sArr_result[3]);

	// and 연산 실행
	$reg[reg_num1].content = !($reg[reg_num2].content || $reg[reg_num3].content);

	return 0;
}
int or(char* sArr_3[]) {
	char * sArr_result[4] = { "" };
	int reg_num1;
	int reg_num2;
	int reg_num3;

	//'$'자르기
	for (int j = 0; j < 4; j++) {
		sArr_result[j] = strtok(sArr_3[j], "$");
		//printf("%s ", sArr_result[j]);
	}
	reg_num1 = atoi(sArr_result[1]);
	reg_num2 = atoi(sArr_result[2]);
	reg_num3 = atoi(sArr_result[3]);

	// and 연산 실행
	$reg[reg_num1].content = $reg[reg_num2].content || $reg[reg_num3].content;

	return 0;
}
int ori(char* sArr_3[]) {
	char * sArr_result[4] = { "" };
	int reg_num1;
	int reg_num2;
	int immediate;

	//'$'자르기
	for (int j = 0; j < 4; j++) {
		sArr_result[j] = strtok(sArr_3[j], "$");
		//printf("%s ", sArr_result[j]);
	}
	reg_num1 = atoi(sArr_result[1]);
	reg_num2 = atoi(sArr_result[2]);
	immediate = atoi(sArr_result[3]);

	// and 연산 실행
	$reg[reg_num1].content = $reg[reg_num2].content || immediate;
	return 0;
}
int sltiu(char* sArr_3[]) {
	return 0;
}
int sltu(char* sArr_3[]) {
	return 0;
}
int sll(char* sArr_3[]) {
	char * sArr_result[4] = { "" };
	int reg_num1;
	int reg_num2;
	int immediate;

	//'$'자르기
	for (int j = 0; j < 4; j++) {
		sArr_result[j] = strtok(sArr_3[j], "$");
		//printf("%s ", sArr_result[j]);
	}
	reg_num1 = atoi(sArr_result[1]);
	reg_num2 = atoi(sArr_result[3]);

	// 연산

	$reg[reg_num1].content = $reg[reg_num2].content * 2;

	return 0;
}
int srl(char* sArr_3[]) {
	char * sArr_result[4] = { "" };
	int reg_num1;
	int reg_num2;
	int immediate;

	//'$'자르기
	for (int j = 0; j < 4; j++) {
		sArr_result[j] = strtok(sArr_3[j], "$");
		//printf("%s ", sArr_result[j]);
	}
	reg_num1 = atoi(sArr_result[1]);
	reg_num2 = atoi(sArr_result[3]);

	// 연산

	$reg[reg_num1].content = $reg[reg_num2].content / 2;
	return 0;
}
int sw(char* sArr_3[]) {
	return 0;
}
int subu(char* sArr_3[]) {
	char * sArr_result[4] = { "" };
	int reg_num1;
	int reg_num2;
	int reg_num3;

	//'$'자르기
	for (int j = 0; j < 4; j++) {
		sArr_result[j] = strtok(sArr_3[j], "$");
		//printf("%s ", sArr_result[j]);
	}
	reg_num1 = atoi(sArr_result[1]);
	reg_num2 = atoi(sArr_result[2]);
	reg_num3 = atoi(sArr_result[3]);

	// addu 연산 실행
	$reg[reg_num1].content = $reg[reg_num2].content - $reg[reg_num3].content;

	// machine code 변환

	return 0;
}
