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
#include <QTabWidget>
#include "student.h"
#include "course.h"
#include "choose.h"
extern student Student;
extern course Course;
extern choose Choose;
extern QByteArray AnsiToUtf8(QByteArray &ansi);

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

//============DisplayToTableWeight
void MainWindow::display_student_info()
{
    ui->tableWidget->clearContents();
    for(int loop = 0; loop < ui->tableWidget->rowCount();)
        ui->tableWidget->removeRow(loop);
    StuInfo *p = Student.head;
    while(p != NULL)
    {
        int rows = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(rows);
        ui->tableWidget->setItem(rows, 0, new QTableWidgetItem(p->name));
        ui->tableWidget->setItem(rows, 1, new QTableWidgetItem(p->id));
        p = p->next;
    }
}

void MainWindow::display_course_info()
{
    ui->tableWidget_2->clearContents();
    for(int loop = 0; loop < ui->tableWidget_2->rowCount();)
        ui->tableWidget_2->removeRow(loop);
    CourseInfo *p = Course.head;
    while(p != NULL)
    {
        int rows = ui->tableWidget_2->rowCount();
        ui->tableWidget_2->insertRow(rows);
        ui->tableWidget_2->setItem(rows, 0, new QTableWidgetItem(p->name));
        ui->tableWidget_2->setItem(rows, 1, new QTableWidgetItem(QString::number(p->score)));
        p = p->next;
    }
}

void MainWindow::display_choose_info()
{
    ui->tableWidget_3->clearContents();
    for(int loop = 0; loop < ui->tableWidget_3->rowCount();)
        ui->tableWidget_3->removeRow(loop);
    StuCrsInfo *p = Choose.head;
    while(p != NULL)
    {
        StuInfo *ps = new StuInfo;
        ps->next = NULL;
        strcpy(ps->id, p->stu);
        char name_tmp[20] = {'\0'};
        StuInfo *pss = Student.serh(ps, 1);
        strcpy(name_tmp, (pss == NULL)?"":pss->name);
        delete ps;

        CourseInfo *pc = new CourseInfo;
        pc->next = NULL;
        strcpy(pc->name, p->crs);
        int score_tmp;
        CourseInfo *pcs = Course.serh(pc, 0);
        score_tmp = (pcs == NULL)?-1:pcs->score;
        delete pc;

        int rows = ui->tableWidget_3->rowCount();
        ui->tableWidget_3->insertRow(rows);
        ui->tableWidget_3->setItem(rows, 0, new QTableWidgetItem(p->stu));
        ui->tableWidget_3->setItem(rows, 1, new QTableWidgetItem(name_tmp));
        ui->tableWidget_3->setItem(rows, 2, new QTableWidgetItem(p->crs));
        ui->tableWidget_3->setItem(rows, 3, new QTableWidgetItem(QString::number(score_tmp)));
        p = p->next;
    }
}

//============StudentInfos
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
    strcat(output, "导入完成，当前一共有");
    strcat(output, buffer);
    strcat(output, "个数据");
    QMessageBox::information(NULL, "导入完成", output, QMessageBox::Ok, QMessageBox::Ok);
    MainWindow::display_student_info();
    MainWindow::display_choose_info();
}

void MainWindow::on_stu_save_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "另存为...", ".", ("文本文件(*.txt)"));
    if(fileName.length() == 0) return;
    QByteArray bytearray = fileName.toLocal8Bit();
    char *temp = bytearray.data();
    Student.save(temp);
    QMessageBox::information(NULL, "保存完成", "保存完成", QMessageBox::Ok, QMessageBox::Ok);
    MainWindow::display_student_info();
}

void MainWindow::on_stu_del_triggered()
{
    QString text = QInputDialog::getText(NULL, tr("删除学生数据"), tr("请输入学号信息"));
    if(text == NULL)
        return;
    QByteArray bytearray = text.toLocal8Bit();
    bytearray = AnsiToUtf8(bytearray);
    char *temp = bytearray.data();
    StuInfo *p = new StuInfo;
    p->next = NULL;
    strcpy(p->id, temp);
    if(Student.del(p, 1))
    {
        StuCrsInfo *ps = new StuCrsInfo;
        ps->next = NULL;
        strcpy(ps->stu, p->id);
        while(Choose.del(ps, 0));
        delete ps;
        QMessageBox::information(NULL, "删除结果", "删除成功", QMessageBox::Ok, QMessageBox::Ok);
    }
    else
        QMessageBox::information(NULL, "删除结果", "删除失败-不存在此学号", QMessageBox::Ok, QMessageBox::Ok);
    delete p;
    MainWindow::display_student_info();
    MainWindow::display_choose_info();
}

void MainWindow::on_stu_add_triggered()
{
    QString text = QInputDialog::getText(NULL, tr("添加学生数据"), tr("请输入学号信息"));
    if(text == NULL)
        return;
    QByteArray bytearray = text.toLocal8Bit();
    bytearray = AnsiToUtf8(bytearray);
    char *stuid = bytearray.data();

    QString text2 = QInputDialog::getText(NULL, tr("添加学生数据"), tr("请输入姓名信息"));
    if(text2 == NULL)
        return;
    QByteArray bytearray2 = text2.toLocal8Bit();
    bytearray2 = AnsiToUtf8(bytearray2);
    char *stuname = bytearray2.data();

    StuInfo *p = new StuInfo;
    p->next = NULL;
    strcpy(p->id, stuid);
    strcpy(p->name, stuname);

    if(Student.add(p))
        QMessageBox::information(NULL, "添加结果", "添加成功", QMessageBox::Ok, QMessageBox::Ok);
    else
        QMessageBox::information(NULL, "添加结果", "添加失败-学号重复", QMessageBox::Ok, QMessageBox::Ok);
    MainWindow::display_student_info();
    MainWindow::display_choose_info();
}

void MainWindow::on_stu_chg_triggered()
{
    QString text = QInputDialog::getText(NULL, tr("修改学生数据"), tr("请输入学号信息"));
    if(text == NULL)
        return;
    QByteArray bytearray = text.toLocal8Bit();
    bytearray = AnsiToUtf8(bytearray);
    char *stuid = bytearray.data();
    QString text2 = QInputDialog::getText(NULL, tr("修改学生数据"), tr("请输入新姓名信息"));
    if(text2 == NULL)
        return;
    QByteArray bytearray2 = text2.toLocal8Bit();
    bytearray2 = AnsiToUtf8(bytearray2);
    char *stunewname = bytearray2.data();
    StuInfo *p = new StuInfo;
    p->next = NULL;
    strcpy(p->id, stuid);
    strcpy(p->name, stunewname);
    if(Student.chg(p, 0))
        QMessageBox::information(NULL, "修改结果", "修改成功", QMessageBox::Ok, QMessageBox::Ok);
    else
        QMessageBox::information(NULL, "修改结果", "修改失败-学号不存在", QMessageBox::Ok, QMessageBox::Ok);
    delete p;
    MainWindow::display_student_info();
    MainWindow::display_choose_info();
}

//============CourseInfos
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
    strcat(output, "导入完成，当前一共有");
    strcat(output, buffer);
    strcat(output, "个课程信息");
    QMessageBox::information(NULL, "导入完成", output, QMessageBox::Ok, QMessageBox::Ok);
    MainWindow::display_course_info();
    MainWindow::display_choose_info();
}

void MainWindow::on_class_save_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "另存为...", ".", ("文本文件(*.txt)"));
    if(fileName.length() == 0) return;
    QByteArray bytearray = fileName.toLocal8Bit();
    char *temp = bytearray.data();
    Course.save(temp);
    QMessageBox::information(NULL, "保存完成", "保存完成", QMessageBox::Ok, QMessageBox::Ok);
    MainWindow::display_course_info();
}

void MainWindow::on_class_del_triggered()
{
    QString text = QInputDialog::getText(NULL, tr("删除课程数据"), tr("请输入课程名"));
    if(text == NULL)
        return;
    QByteArray bytearray = text.toLocal8Bit();
    bytearray = AnsiToUtf8(bytearray);
    char *temp = bytearray.data();
    CourseInfo *p = new CourseInfo;
    p->next = NULL;
    strcpy(p->name, temp);
    if(Course.del(p, 0))
    {
        StuCrsInfo *ps = new StuCrsInfo;
        ps->next = NULL;
        strcpy(ps->crs, p->name);
        while(Choose.del(ps, 1));
        delete ps;
        QMessageBox::information(NULL, "删除结果", "删除成功", QMessageBox::Ok, QMessageBox::Ok);
    }
    else
        QMessageBox::information(NULL, "删除结果", "删除失败-不存在此课程", QMessageBox::Ok, QMessageBox::Ok);
    delete p;
    MainWindow::display_course_info();
    MainWindow::display_choose_info();
}

void MainWindow::on_class_add_triggered()
{
    QString text = QInputDialog::getText(NULL, tr("添加课程数据"), tr("请输入课程名"));
    if(text == NULL)
        return;
    QByteArray bytearray = text.toLocal8Bit();
    bytearray = AnsiToUtf8(bytearray);
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
    MainWindow::display_course_info();
    MainWindow::display_choose_info();
}

//============ChooseInfos
void MainWindow::on_choose_class_triggered()
{
    QString text = QInputDialog::getText(NULL, tr("添加选课数据"), tr("请输入学号"));
    if(text == NULL)
        return;
    QByteArray bytearray = text.toLocal8Bit();
    bytearray = AnsiToUtf8(bytearray);
    char *stuid = bytearray.data();

    QString text2 = QInputDialog::getText(NULL, tr("添加选课数据"), tr("请输入课程名"));
    if(text2 == NULL)
        return;
    QByteArray bytearray2 = text2.toLocal8Bit();
    bytearray2 = AnsiToUtf8(bytearray2);
    char *coursename = bytearray2.data();

    StuCrsInfo *p = new StuCrsInfo;
    p->next = NULL;
    strcpy(p->stu, stuid);
    strcpy(p->crs, coursename);
    CourseInfo *pc = new CourseInfo;
    strcpy(pc->name, coursename);
    StuInfo *ps = new StuInfo;
    strcpy(ps->id, stuid);
    if(Student.serh(ps, 1) == NULL)
        QMessageBox::information(NULL, "选课结果", "选课失败-学生不存在", QMessageBox::Ok, QMessageBox::Ok);
    else if(Course.serh(pc, 0) == NULL)
        QMessageBox::information(NULL, "选课结果", "选课失败-课程不存在", QMessageBox::Ok, QMessageBox::Ok);
    else if(Choose.add(p))
        QMessageBox::information(NULL, "选课结果", "选课成功", QMessageBox::Ok, QMessageBox::Ok);
    else
        QMessageBox::information(NULL, "选课结果", "选课失败-已选过", QMessageBox::Ok, QMessageBox::Ok);
    delete ps;
    delete pc;
    MainWindow::display_choose_info();
}

void MainWindow::on_delete_class_triggered()
{
    QString text = QInputDialog::getText(NULL, tr("输入退课数据"), tr("请输入学号"));
    if(text == NULL)
        return;
    QByteArray bytearray = text.toLocal8Bit();
    bytearray = AnsiToUtf8(bytearray);
    char *stuid = bytearray.data();

    QString text2 = QInputDialog::getText(NULL, tr("输入退课数据"), tr("请输入课程名"));
    if(text2 == NULL)
        return;
    QByteArray bytearray2 = text2.toLocal8Bit();
    bytearray2 = AnsiToUtf8(bytearray2);
    char *coursename = bytearray2.data();

    StuCrsInfo *p = new StuCrsInfo;
    p->next = NULL;
    strcpy(p->stu, stuid);
    strcpy(p->crs, coursename);
    CourseInfo *pc = new CourseInfo;
    strcpy(pc->name, coursename);
    StuInfo *ps = new StuInfo;
    strcpy(ps->id, stuid);
    if(Student.serh(ps, 1) == NULL)
        QMessageBox::information(NULL, "退课结果", "退课失败-学生不存在", QMessageBox::Ok, QMessageBox::Ok);
    else if(Course.serh(pc, 0) == NULL)
        QMessageBox::information(NULL, "退课结果", "退课失败-课程不存在", QMessageBox::Ok, QMessageBox::Ok);
    else if(Choose.del(p, 2))
        QMessageBox::information(NULL, "退课结果", "退课成功", QMessageBox::Ok, QMessageBox::Ok);
    else
        QMessageBox::information(NULL, "退课结果", "退课失败-未选课", QMessageBox::Ok, QMessageBox::Ok);
    delete p;
    delete ps;
    delete pc;
    MainWindow::display_choose_info();
}

void MainWindow::on_choose_read_triggered()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this, "打开文件", ".", ("文本文件(*.txt)"));
    if(fileNames.size() == 0) return;
    for(int i = 0; i < fileNames.size(); ++i)
    {
        QByteArray bytearray = fileNames[i].toLocal8Bit();
        char *temp = bytearray.data();
        if(temp == NULL) continue;
        Choose.read(temp);
    }
    char buffer[20];
    itoa(Choose.count, buffer,10);
    char output[100];
    strcat(output, "导入完成，当前一共有");
    strcat(output, buffer);
    strcat(output, "个数据");
    QMessageBox::information(NULL, "导入完成", output, QMessageBox::Ok, QMessageBox::Ok);
    MainWindow::display_choose_info();
}

void MainWindow::on_choose_save_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "另存为...", "C:", ("文本文件(*.txt)"));
    if(fileName.length() == 0) return;
    QByteArray bytearray = fileName.toLocal8Bit();
    char *temp = bytearray.data();
    Choose.save(temp);
    QMessageBox::information(NULL, "保存完成", "保存完成", QMessageBox::Ok, QMessageBox::Ok);
    MainWindow::display_choose_info();
}

void MainWindow::on_search_stu_triggered()
{
    QString text = QInputDialog::getText(NULL, tr("搜索"), tr("请输入学号"));
    if(text == NULL)
        return;
    QByteArray bytearray = text.toLocal8Bit();
    bytearray = AnsiToUtf8(bytearray);
    char *stuid = bytearray.data();

    StuCrsInfo *p = new StuCrsInfo;
    strcpy(p->stu, stuid);
    p->next = NULL;

    StuInfo *ps = new StuInfo;
    strcpy(ps->id, stuid);
    ps->next = NULL;
    if(Student.serh(ps,1) == NULL)
    {
        QMessageBox::information(NULL, "搜索完成", "不存在该学生", QMessageBox::Ok, QMessageBox::Ok);
        delete ps;
        return;
    }
    delete ps;
    p = Choose.serh(p, 3);
    char output[1000];
    strcpy(output, "该学生选择的课程有：\n");
    while(p->next != NULL)
    {
        strcat(output, p->crs);
        strcat(output, "\n");
        p = p->next;
    }
    QMessageBox::information(NULL, "搜索完成", output, QMessageBox::Ok, QMessageBox::Ok);
    delete p;
}

void MainWindow::on_search_class_triggered()
{
    QString text = QInputDialog::getText(NULL, tr("搜索"), tr("请输入课程名"));
    if(text == NULL)
        return;
    QByteArray bytearray = text.toLocal8Bit();
    bytearray = AnsiToUtf8(bytearray);
    char *coursename = bytearray.data();

    StuCrsInfo *p = new StuCrsInfo;
    strcpy(p->crs, coursename);
    p->next = NULL;

    CourseInfo *pc = new CourseInfo;
    strcpy(pc->name, coursename);
    pc->next = NULL;
    if(Course.serh(pc, 0) == NULL)
    {
        QMessageBox::information(NULL, "搜索完成", "不存在该课程", QMessageBox::Ok, QMessageBox::Ok);
        delete pc;
        return;
    }
    delete pc;
    p = Choose.serh(p, 4);
    char output[1000];
    strcpy(output, "选择该课程的学生有：\n");
    while(p->next != NULL)
    {
        strcat(output, p->stu);
        strcat(output, "\n");
        p = p->next;
    }
    QMessageBox::information(NULL, "搜索完成", output, QMessageBox::Ok, QMessageBox::Ok);
    delete p;
}
