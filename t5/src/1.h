#ifndef ONE_H
#define ONE_H

#include<iostream>
#include"2.h"
using namespace std;

class compute{
    public:
         compute(int a, int b);
         ~compute();
         int Compute();

    private:
    
    add C_Add;
    int a;
    int b;

};


#endif