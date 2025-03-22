
int foo(int c) {
	if (c > 10) {
		return c + 100;
	} else {
		return 42;
	}
}

void bar(int x) {
	int a;
	if (x > 10) {
		a = 87;
	} else {
		a = x - 100;
	}
}

