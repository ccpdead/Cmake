#include "widget.h"
#include "./ui_widget.h"
#include<QFileDialog>//打开文件位置
#include<QMessageBox>//警告框
#include<QFile>//打开文件
#include<QFileInfo>//文件信息

#include<QDebug>
#include<QDateTime>
#include<QTextStream>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    //点击按钮，选择文件。
    connect(ui->pushButton,&QPushButton::clicked,this,[=](){
        QString path = QFileDialog::getOpenFileName(this,"open documents","/home/ubuntu/Desktop");
        if(path.isEmpty())
        {
            QMessageBox::warning(this,"error","count open this documents");
        }else{
            ui->lineEdit->setText(path);

            QFile file(path);

            file.open(QFileDevice::ReadOnly);
            QByteArray array;
            while(!file.atEnd()){
                array+=file.readLine();
            }
            ui->textBrowser->setText(array);
            file.close();
        }
    });
}

Widget::~Widget()
{
    delete ui;
}

