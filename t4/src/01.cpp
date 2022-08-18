#include<iostream>
#include<fstream>

using namespace std;

int main(int argc, char **argv)
{
    double d[]={5.365738, -0.990472, 0.000000, 0.999820, 0.000000, 0.000000, -0.018984, 
                5.693102, -0.828714, 0.000000, 1.000000, 0.000000, 0.000000, -0.000380, 
                6.253308, -0.977634, 0.000000, 0.999974, 0.000000, 0.000000, 0.007266, 
                6.836521, -0.963474, 0.000000, 0.999852, 0.000000, 0.000000, 0.017203, 
                6.870844, -0.961245, 0.000000, 0.999635, 0.000000, 0.000000, -0.027025, 
                6.954378, -0.968865, 0.000000, 0.999557, 0.000000, 0.000000, -0.029757, 
                7.044826, -0.977970, 0.000000, 0.999282, 0.000000, 0.000000, -0.037883, 
                7.193473, -0.994723, 0.000000, 0.999582, 0.000000, 0.000000, -0.028916};
    ofstream in;
    in.open("/home/jhr/Desktop/pose.txt",ios::trunc);//打开一个文件，并清空内容
    int size = sizeof(d)/sizeof(d[0]);
    int b = 0;
    for(int a = 0;a<size;a+=7)
    {
        for(int i = 0;i<7;i++){
            if(i!=6){
            in<<d[i+b]<<",";
            }else{
                in<<d[i+b]<<"\n";
            }
        }
        b+=7;
    }


    in.close();

    return 0;
}