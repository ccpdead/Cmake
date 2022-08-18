#include<iostream>
#include<fstream>
#include<string>

using namespace std;

int main(int argc, char **argv)
{
    char buffer[1024]={0};
    fstream out;
    out.open("/home/jhr/Desktop/pose.txt",ios::in);
    int line = 1;
    while(!out.eof())
    {   char buff[1024]={0};
        line +=1;
        out.getline(buff, 1024);
    }
    out.close();
    cout<<line-2<<endl;
    
    //$ 加载数据并显示
    out.open("/home/jhr/Desktop/pose.txt",ios::in);
    //% 每行
     for(int L = 0;L<line-2;L++)
    {
        
        int b = 0;
        out.getline(buffer,1024);
        cout<<buffer<<endl;
        //% 每行有七组数据
        for(int a = 0;a<7;a++)
        {   
            string data ="";
            while(buffer[b]!=',')
            {
                static int c = 0;
                data += buffer[b];
                c++;
                b++;
            }
            b +=1;  
            //￥ 字符串转换int型
            cout<<"data->"<<atof(data.c_str())<<endl;
        }
    }

    out.close();
    return 0;
}