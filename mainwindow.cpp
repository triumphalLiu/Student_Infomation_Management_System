#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cstring>
#include <cstdio>
#include <QMessageBox>
#include <QInputDialog>
#include <QIODevice>
#include <QString>
#include <QByteArray>
#include <QTextCodec>
#include <QTextStream>
#include <QFile>
#include "student.h"
extern student Student;

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
    Student.read("student.txt");
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
    Student.save("student2.txt");
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
    printf("%s\n",stuname);

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
