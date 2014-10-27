#include <iostream>

using namespace std;

class A {
	public:
	void ap(){cout<<"a"<<endl;}
	virtual void pv() {};
	virtual int pc() {};
};

class B : public A {
	static const int value=2;
	public:
	void bp(){cout<<"b"<<endl;}
	void pv() {cout << value<<endl;}
};

class C : public B {
	static const int value=3;
	int pc() {cout << "c"<<endl; return value;}
};

int main() {
	A *test = new B;
	test->pv();
	A *test2 = new C;
	int w=test2->pc();
	cout << w<<endl;
	char c = '\\';
	cout << c << endl;
	//int a = test->pc();
	//cout << a << endl;
	/*int *a = new int[10];
	for(int i=0; i<10; i++) a[i]=i;
	for(int i=0; i<10; i++) cout<<a[i];*/
}
