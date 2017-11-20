#include <stdio.h>
#include <string.h>

#define NUMINTRU  21
#define LENGINTRU 6

int addiu();
int addu();
int and();
int andi();
int beq();
int bne();
int j();
int jal();
int jr();
int lui();
int lw();
int la();
int nor();
int or();
int ori();
int sltiu();
int sltu();
int sll();
int srl();
int sw();
int subu();

int main() {

	char instruction[NUMINTRU][LENGINTRU] = { "ADDIU","ADDU","AND","ANDI","BEQ","BNE","J","JAL","JR","LUI","LW","LA","NOR","OR","ORI","SLTIU",
		"SLTU","SLL","SRL","SW","SUBU" };
	int modevalue;
	int result;
	
	FILE *fin;
	FILE *fout;
	char s[81];
	char *ptr;
	char *sArr[6] = {""};
	int i = 0;
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

	while (!feof(fin)) {
		// 한 줄씩 읽어오기
		fgets(s, 80, fin);
		// " "로 자른다
		ptr = strtok(s, " ");
		
		while (ptr != NULL) {
			sArr[i] = ptr;             // 문자열을 자른 뒤 메모리 주소를 문자열 포인터 배열에 저장
			i++;
			ptr = strtok(NULL, " ");
		}
		// instruction set과 비교 성공
		for (int k = 0; k < NUMINTRU; k++) {
			if (!strcmp(sArr[0], instruction[k])) {
				modevalue = k;
				switch (modevalue)
				{
					case 1 :
						result = addiu();
						break;
					case 2 :
						result = addu();
						break;
					case 3:
						result = and();
						break;
					case 4:
						result = andi();
						break;
					case 5:
						result = beq();
						break;
					case 6:
						result = bne();
						break;
					case 7:
						result = j();
						break;
					case 8:
						result = jal();
						break;
					case 9:
						result = jr();
						break;
					case 10:
						result = lui();
						break;
					case 11:
						result = lw();
						break;
					case 12:
						result = la();
						break;
					case 13:
						result = nor();
						break;
					case 14:
						result = or();
						break;
					case 15:
						result = ori();
						break;
					case 16:
						result = sltiu();
						break;
					case 17:
						result = sltu();
						break;
					case 18:
						result = sll();
						break;
					case 19:
						result = srl();
						break;
					case 20:
						result = sw();
						break;
					case 21:
						result = subu();
						break;
					default:
						break;
				}
				//modevalue = k;
			}
		}

		/*
		for (int j = 0; j < 4; j++)
		{// sArr[0,1,2,3] 에 각각 명령어와 operand 저장

			if (sArr[j] != NULL) {// 문자열 포인터 배열의 요소가 NULL이 아닐 때만
				if(sArr[j] != EOF)
				printf("%s\n", sArr[j]);   // 문자열 포인터 배열에 인덱스로 접근하여 각 문자열 출력
			}
		}
		*/
	
		i = 0;
	}

	fclose(fin);
	fclose(fout);
	return 0;
	
}
int addiu() {

	printf("1\n");
	return 0;
}
int addu() {

	printf("2\n");
	return 0;
}
int and() {

	printf("3\n");
	return 0;
}
int andi() {

	printf("4\n");
	return 0;
}
int beq() {

	printf("5\n");
	return 0;
}
int bne() {

	printf("6\n");
	return 0;
}
int j() {
	printf("7\n");
	return 0;
}
int jal() {
	printf("8\n");
	return 0;
}
int jr() {
	printf("9\n");
	return 0;
}
int lui() {

	printf("10\n");
	return 0;
}
int lw() {

	printf("11\n");
	return 0;
}
int la() {

	printf("12\n");
	return 0;
}
int nor() {

	printf("13\n");
	return 0;
}
int or() {

	printf("14\n");
	return 0;
}
int ori() {

	printf("15\n");
	return 0;
}
int sltiu() {

	printf("16\n");
	return 0;
}
int sltu() {

	printf("17\n");
	return 0;
}
int sll() {

	printf("18\n");
	return 0;
}
int srl() {

	printf("19\n");
	return 0;
}
int sw() {

	printf("20\n");
	return 0;
}
int subu() {

	printf("21\n");
	return 0;
}