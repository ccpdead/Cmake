#include<iostream>
#include "1.h"

compute::compute(int a, int b):C_Add(a,b)
{
    this->a = a;
    this->b = b;
    cout<<"compute构造函数"<<endl;
    
}

compute::~compute(){
    cout<<"compute析构函数"<<endl;  
}

int compute::Compute(){
    cout<<"compute is "<<C_Add.ADD()<<endl;
}
