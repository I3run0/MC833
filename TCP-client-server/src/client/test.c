#include <stdio.h>
#include <string.h>

int main(void) {
	char msg[100];

	scanf("%[^\n]", msg);
	printf("%s", msg);
}
