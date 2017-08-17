/*
 * 本程序中读取文件编码请使用ansi!
*/

#include "mainwindow.h"
#include <QApplication>
#include "student.h"
#include "course.h"
#include "choose.h"
student Student;
course Course;
choose Choose;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Student.init();
    Course.init();
    Choose.init();
    MainWindow w;
    w.show();
    return a.exec();
}
