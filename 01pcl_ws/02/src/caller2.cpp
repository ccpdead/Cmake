#include<iostream>
template<typename T>
void func(T const&v)
{
	std::cout<<"func2:"<<v<<std::endl;
}
void caller2(){
	func<int>(2);
	func<float>(0.2f);
}

