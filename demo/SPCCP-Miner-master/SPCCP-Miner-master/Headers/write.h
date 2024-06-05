#ifndef WRITE_H
#define WRITE_H
#include <iostream>
#include <QMainWindow>
#include <string>
using namespace std;
namespace Ui {
class write;
}

class write : public QMainWindow
{
    Q_OBJECT

public:
    explicit write(QWidget *parent = nullptr);
    ~write();
    string data;
private:
    Ui::write *ui;
};

#endif // WRITE_H
