#include <stdio.h>
#include <string.h>

#define NUMINTRU  21
#define LENGINTRU 6
int main() {

	char instruction[NUMINTRU][LENGINTRU] = { "ADDIU","ADDU","AND","ANDI","BEQ","BNE","J","JAL","JR","LUI","LW","LA","NOR","OR","ORI","SLTIU",
		"SLTU","SLL","SRL","SW","SUBU" };
	
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
		for (int j = 0; j < 4; j++)
		{// sArr[0,1,2,3] 에 각각 명령어와 operand 저장
			if (sArr[j] != NULL) {// 문자열 포인터 배열의 요소가 NULL이 아닐 때만
				if (sArr[j] == "\n") {

				}else {
					printf("%s\n", sArr[j]);   // 문자열 포인터 배열에 인덱스로 접근하여 각 문자열 출력
				}
			}
		}

		i = 0;
	}
//	for (int i = 0; i < NUMINTRU; i++) {
//		if (!strcmp(ptr, instruction[i])) {
//			printf("%d", i);
//		}
//	}

	fclose(fin);
	fclose(fout);
	return 0;
	
}