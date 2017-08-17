#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cstring>
#include <cstdio>
#include <QByteArray>
#include <QFile>
#include <QFileDialog>
#include <QInputDialog>
#include <QIODevice>
#include <QMessageBox>
#include <QString>
#include <QStringList>
#include <QTextCodec>
#include <QTextStream>
#include "student.h"
#include "course.h"
extern student Student;
extern course Course;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_stu_read_triggered()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this, "打开文件", ".", ("文本文件(*.txt)"));
    if(fileNames.size() == 0) return;
    for(int i = 0; i < fileNames.size(); ++i)
    {
        QByteArray bytearray = fileNames[i].toLocal8Bit();
        char *temp = bytearray.data();
        if(temp == NULL) continue;
        Student.read(temp);
    }
    char buffer[20];
    itoa(Student.count, buffer,10);
    char output[100];
    strcat(output, "一共导入");
    strcat(output, buffer);
    strcat(output, "个数据");
    QMessageBox::information(NULL, "导入完成", output, QMessageBox::Ok, QMessageBox::Ok);
}

void MainWindow::on_stu_save_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "另存为...", "C:", ("文本文件(*.txt)"));
    if(fileName.length() == 0) return;
    QByteArray bytearray = fileName.toLocal8Bit();
    char *temp = bytearray.data();
    Student.save(temp);
    QMessageBox::information(NULL, "保存完成", "保存完成", QMessageBox::Ok, QMessageBox::Ok);
}

void MainWindow::on_stu_del_triggered()
{
    QString text = QInputDialog::getText(NULL, tr("删除学生数据"), tr("请输入学号信息"));
    if(text == NULL)
        return;
    QByteArray bytearray = text.toLatin1();
    char *temp = bytearray.data();
    StuInfo *p = new StuInfo;
    p->next = NULL;
    strcpy(p->id, temp);
    if(Student.del(p, 1))
        QMessageBox::information(NULL, "删除结果", "删除成功", QMessageBox::Ok, QMessageBox::Ok);
    else
        QMessageBox::information(NULL, "删除结果", "删除失败-不存在此学号", QMessageBox::Ok, QMessageBox::Ok);
}

void MainWindow::on_stu_add_triggered()
{
    QString text = QInputDialog::getText(NULL, tr("添加学生数据"), tr("请输入学号信息"));
    if(text == NULL)
        return;
    QByteArray bytearray = text.toLatin1();
    char *stuid = bytearray.data();

    QString text2 = QInputDialog::getText(NULL, tr("添加学生数据"), tr("请输入姓名信息"));
    if(text2 == NULL)
        return;
    QByteArray bytearray2 = text2.toLocal8Bit();
    char *stuname = bytearray2.data();

    StuInfo *p = new StuInfo;
    p->next = NULL;
    strcpy(p->id, stuid);
    strcpy(p->name, stuname);

    if(Student.add(p))
        QMessageBox::information(NULL, "添加结果", "添加成功", QMessageBox::Ok, QMessageBox::Ok);
    else
        QMessageBox::information(NULL, "添加结果", "添加失败-学号重复", QMessageBox::Ok, QMessageBox::Ok);
}

void MainWindow::on_stu_chg_triggered()
{
    QString text = QInputDialog::getText(NULL, tr("修改学生数据"), tr("请输入学号信息"));
    if(text == NULL)
        return;
    QByteArray bytearray = text.toLatin1();
    char *stuid = bytearray.data();
    QString text2 = QInputDialog::getText(NULL, tr("修改学生数据"), tr("请输入新姓名信息"));
    if(text2 == NULL)
        return;
    QByteArray bytearray2 = text2.toLocal8Bit();
    char *stunewname = bytearray2.data();
    StuInfo *p = new StuInfo;
    p->next = NULL;
    strcpy(p->id, stuid);
    strcpy(p->name, stunewname);
    if(Student.chg(p, 0))
        QMessageBox::information(NULL, "修改结果", "修改成功", QMessageBox::Ok, QMessageBox::Ok);
    else
        QMessageBox::information(NULL, "修改结果", "修改失败-学号不存在", QMessageBox::Ok, QMessageBox::Ok);
}

void MainWindow::on_class_read_triggered()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this, "打开文件", ".", ("文本文件(*.txt)"));
    if(fileNames.size() == 0) return;
    for(int i = 0; i < fileNames.size(); ++i)
    {
        QByteArray bytearray = fileNames[i].toLocal8Bit();
        char *temp = bytearray.data();
        if(temp == NULL) continue;
        Course.read(temp);
    }
    char buffer[20];
    itoa(Course.count, buffer,10);
    char output[100];
    strcat(output, "一共导入");
    strcat(output, buffer);
    strcat(output, "个课程信息");
    QMessageBox::information(NULL, "导入完成", output, QMessageBox::Ok, QMessageBox::Ok);
}

void MainWindow::on_class_save_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "另存为...", "C:", ("文本文件(*.txt)"));
    if(fileName.length() == 0) return;
    QByteArray bytearray = fileName.toLocal8Bit();
    char *temp = bytearray.data();
    Course.save(temp);
    QMessageBox::information(NULL, "保存完成", "保存完成", QMessageBox::Ok, QMessageBox::Ok);
}

void MainWindow::on_class_del_triggered()
{
    QString text = QInputDialog::getText(NULL, tr("删除课程数据"), tr("请输入课程名"));
    if(text == NULL)
        return;
    QByteArray bytearray = text.toLocal8Bit();
    char *temp = bytearray.data();
    CourseInfo *p = new CourseInfo;
    p->next = NULL;
    strcpy(p->name, temp);
    if(Course.del(p, 0))
        QMessageBox::information(NULL, "删除结果", "删除成功", QMessageBox::Ok, QMessageBox::Ok);
    else
        QMessageBox::information(NULL, "删除结果", "删除失败-不存在此课程", QMessageBox::Ok, QMessageBox::Ok);

}

void MainWindow::on_class_add_triggered()
{
    QString text = QInputDialog::getText(NULL, tr("添加课程数据"), tr("请输入课程名"));
    if(text == NULL)
        return;
    QByteArray bytearray = text.toLocal8Bit();
    char *coursename = bytearray.data();

    QString text2 = QInputDialog::getText(NULL, tr("添加课程数据"), tr("请输入该课程的学分"));
    if(text2 == NULL)
        return;
    QByteArray bytearray2 = text2.toLocal8Bit();
    char *coursescore = bytearray2.data();

    CourseInfo *p = new CourseInfo;
    p->next = NULL;
    p->score = atoi(coursescore);
    strcpy(p->name, coursename);

    if(Course.add(p))
        QMessageBox::information(NULL, "添加结果", "添加成功", QMessageBox::Ok, QMessageBox::Ok);
    else
        QMessageBox::information(NULL, "添加结果", "添加失败-课程重复", QMessageBox::Ok, QMessageBox::Ok);
}
