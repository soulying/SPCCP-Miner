#include "write.h"
#include "ui_write.h"

write::write(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::write)
{
    ui->setupUi(this);
    connect(ui->pushButton,&QPushButton::clicked,[=](){
        data = ui->textEdit->toPlainText().toStdString();
    });
    connect(ui->pushButton_2, &QPushButton::clicked, [=](){
        write::close();
    });

}

write::~write()
{
    delete ui;
}
