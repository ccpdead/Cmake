#include "widget.h"
#include "./ui_widget.h"
#include <QMessageBox>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(ui->pushButton,&QPushButton::clicked,this,[=](){
        // qDebug() << "hello";
        QMessageBox::warning(this,"警告","出现错误");
    });



    
}

Widget::~Widget()
{
    delete ui;
}

