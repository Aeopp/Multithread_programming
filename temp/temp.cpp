#include <iostream>
struct t {
	int a;
	// 8
	double c;
	//16
	char d;
	// 17 
	short q;
	// 18~ 19 
	bool s;
	// 19~20
	// 24
};

struct st {
	bool c;
	// 1
	t sad;
	//4~8 8~16 16~17 18~20 20~21
	/*int double char short bool */
	//int a;
	//// 24~28
	//short q;
	////28~30
	//t qwe;
	///*int double char short bool */
	////32~36 40~48 48~49 50~52 52~53
	//float w;
	//// 56~60
	//char s;
	//// 60~61
	//double as;
	//// 64~ 72 
	//int xzcxz;
	//// 72~76
	//char ssssss;
	//// 77
};

int main() {
	std::cout << sizeof(st);
}