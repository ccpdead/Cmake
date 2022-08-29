#include "2.h"

add::add(int a, int b){
    this->a = a;
    this->b = b;
    cout<<"add构造函数"<<endl;
}

add::~add(){
    cout<<"add析构函数"<<endl;
}

int add::ADD(){
    return this->a + this->b; 
}