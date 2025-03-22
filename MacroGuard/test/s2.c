
#pragma macro_arg_guard val

#include <stdio.h>

#define PRINT(val) \
printf("%d\n", val * 9 + (val) * 8);

int main() {
	PRINT(1 + 3);
	return 0;
}

