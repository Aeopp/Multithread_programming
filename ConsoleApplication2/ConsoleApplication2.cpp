

#include <iostream>

template<typename t,typename ...a>
void foo(t _t,a&&... as)
{
	auto zxc = std::move(as);
}

void qwe(int a ,int b ,int c)
{
	std::cout << a << b << c << std::endl;
}
int main()
{
	foo(qwe, 1, 1, 1);

}