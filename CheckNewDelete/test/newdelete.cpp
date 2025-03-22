
template<typename T>
struct sharedptr {
	T *t_;
	explicit sharedptr(T *t) : t_(t) {}
	~sharedptr() { delete t_; }
};

struct A {};

struct B : public A {};

void foo() {
	A *a = new A;
	delete a;

	B *b(new B);
	delete b;

	sharedptr<A> sa(new A);
	sharedptr<A> sa1 = sharedptr<A>(new A);
}

sharedptr<A> ga(new A);

int main(){
	return 0;
}

