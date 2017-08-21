/*
 * 本程序中读取文件编码请使用ansi!
*/

#include "mainwindow.h"
#include <QApplication>
#include "student.h"
#include "course.h"
#include "choose.h"
student Student;
course_ac CourseAC;
course_lv CourseLV;
course_sc CourseSC;
course Course;
choose Choose;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Student.init();
    CourseAC.init();
    CourseLV.init();
    CourseSC.init();
    Course.init();
    Choose.init();
    MainWindow w;
    w.show();
    return a.exec();
}
