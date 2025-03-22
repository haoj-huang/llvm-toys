
#pragma macro_arg_guard expr

#include <stdio.h>

#define PRINT(expr) \
printf("%d\n", ( expr ) * 9 + expr * 8);

int main() {
	PRINT(1 + 3);
	return 0;
}

