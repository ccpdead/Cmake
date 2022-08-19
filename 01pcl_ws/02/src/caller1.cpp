//文件名caller1.cpp
#include<iostream>
template<typename T>
void func(T const &v)
{
	std::cout<<"func1:"<<v<<std::endl;
}
void caller1(){
	func<int>(1);
	func<double>(0.1);
}

