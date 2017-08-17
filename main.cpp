/*
 * 本程序中读取文件编码请使用ansi!
*/

#include "mainwindow.h"
#include <QApplication>
#include "student.h"
student Student;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Student.init();
    MainWindow w;
    w.show();
    return a.exec();
}
