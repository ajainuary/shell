#include <stdio.h>
unsigned char __end_text[123];

int main () {
	printf("%p", __end_text);
	return 0;
}