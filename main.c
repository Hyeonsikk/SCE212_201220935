#include <stdio.h>

int main() {

	FILE *fin;
	FILE *fout;
	char s[81];

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
		fgets(s, 80, fin);
		fputs(s, fout);
	}


	fclose(fin);
	fclose(fout);
	return 0;
}