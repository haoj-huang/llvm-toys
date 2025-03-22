
int bar(int x, int y) {
	return x * y;
}

int foo(int x, int y) {
	int a = bar(x, y);
	return a;
}

int test(int x, int y) {
	return x > 0 ? x - y : x + y;
}

