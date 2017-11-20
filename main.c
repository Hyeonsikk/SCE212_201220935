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
		// �� �پ� �о����
		fgets(s, 80, fin);
		// " "�� �ڸ���
		ptr = strtok(s, " ");
		
		while (ptr != NULL) {
			sArr[i] = ptr;             // ���ڿ��� �ڸ� �� �޸� �ּҸ� ���ڿ� ������ �迭�� ����
			i++;
			ptr = strtok(NULL, " ");
		}
		for (int j = 0; j < 4; j++)
		{// sArr[0,1,2,3] �� ���� ��ɾ�� operand ����
			if (sArr[j] != NULL) {// ���ڿ� ������ �迭�� ��Ұ� NULL�� �ƴ� ����
				if (sArr[j] == "\n") {

				}else {
					printf("%s\n", sArr[j]);   // ���ڿ� ������ �迭�� �ε����� �����Ͽ� �� ���ڿ� ���
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