#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cstring>
#include <cstdio>
#include <QByteArray>
#include <QCursor>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QInputDialog>
#include <QIODevice>
#include <QMenu>
#include <QMessageBox>
#include <QString>
#include <QStringList>
#include <QTextCodec>
#include <QTextStream>
#include <QTabWidget>
#include "student.h"
#include "course.h"
#include "choose.h"

student Student;
course Course;
choose Choose;

extern QByteArray AnsiToUtf8(QByteArray &ansi);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //预先读取文件
    Student.read("stu.txt");
    Course.read("crs.txt");
    Choose.read("cho.txt");
    MainWindow::display_course_info();
    MainWindow::display_choose_info();
    MainWindow::display_student_info();
    //窗口大小设置
    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
    setFixedSize(this->width(), this->height());
    //右键菜单设置
    Act_Stu_Del = new QAction(tr("删除学生信息"), this);
    Act_Stu_Chg = new QAction(tr("修改学生信息"), this);
    Act_Stu_Add = new QAction(tr("添加学生信息"), this);
    Act_Crs_Del = new QAction(tr("删除课程"), this);
    Act_Crs_Add = new QAction(tr("添加课程"), this);
    Act_Cho_Del = new QAction(tr("退课"), this);
    Act_Cho_Add = new QAction(tr("选课"), this);
    Act_Srh_Stu = new QAction(tr("搜索当前学生所选的课程"), this);
    Act_Srh_Crs = new QAction(tr("搜索选择当前课程的学生"), this);
    Act_ADDGRADE = new QAction(tr("添加成绩"), this);
    Act_DELGRADE = new QAction(tr("删除成绩"), this);
    Act_Calc = new QAction(tr("计算学分绩"), this);
    connect(Act_Stu_Del, SIGNAL(triggered()), this, SLOT(on_stu_del_triggered()));
    connect(Act_Stu_Chg, SIGNAL(triggered()), this, SLOT(on_stu_chg_triggered()));
    connect(Act_Stu_Add, SIGNAL(triggered()), this, SLOT(on_stu_add_triggered()));
    connect(Act_Crs_Del, SIGNAL(triggered()), this, SLOT(on_class_del_triggered()));
    connect(Act_Crs_Add, SIGNAL(triggered()), this, SLOT(on_class_add_triggered()));
    connect(Act_Cho_Del, SIGNAL(triggered()), this, SLOT(on_delete_class_triggered()));
    connect(Act_Cho_Add, SIGNAL(triggered()), this, SLOT(on_choose_class_triggered()));
    connect(Act_Srh_Stu, SIGNAL(triggered()), this, SLOT(on_search_stu_triggered()));
    connect(Act_Srh_Crs, SIGNAL(triggered()), this, SLOT(on_search_class_triggered()));
    connect(Act_ADDGRADE, SIGNAL(triggered()), this, SLOT(on_add_grade_triggered()));
    connect(Act_DELGRADE, SIGNAL(triggered()), this, SLOT(on_del_grade_triggered()));
    connect(Act_Calc, SIGNAL(triggered()), this, SLOT(on_caculate_triggered()));
}

void MainWindow::contextMenuEvent(QContextMenuEvent *)
{
    //qDebug()<<ui->tabWidget->currentIndex();
    QCursor cur=this->cursor();
    QMenu *menu = new QMenu;
    if(ui->tabWidget->currentIndex() == 0)
    {
        menu->addAction(Act_Stu_Del);
        menu->addAction(Act_Stu_Chg);
        menu->addAction(Act_Stu_Add);
        menu->addAction(Act_Srh_Stu);
        menu->addAction(Act_Calc);
    }
    else if(ui->tabWidget->currentIndex() == 1)
    {
        menu->addAction(Act_Crs_Del);
        menu->addAction(Act_Crs_Add);
        menu->addAction(Act_Srh_Crs);
    }
    else if(ui->tabWidget->currentIndex() == 2)
    {
        menu->addAction(Act_Cho_Del);
        menu->addAction(Act_Cho_Add);
        menu->addAction(Act_Srh_Stu);
        menu->addAction(Act_Srh_Crs);
        menu->addAction(Act_ADDGRADE);
        menu->addAction(Act_DELGRADE);
        menu->addAction(Act_Calc);
    }
    menu->exec(cur.pos());
    delete menu;
}

MainWindow::~MainWindow()
{
    delete ui;
}

//============DisplayToTableWeight
void MainWindow::display_student_info()
{
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
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
    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
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
        ui->tableWidget_2->setItem(rows, 2, new QTableWidgetItem((p->type == 0)?"通过型":((p->type == 1)?"等级型":"分数型")));
        p = p->next;
    }
}

void MainWindow::display_choose_info()
{
    ui->tableWidget_3->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
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
        CourseInfo *pcs = Course.serh(pc, 0, Course.head);
        score_tmp = (pcs == NULL)?-1:pcs->score;
        delete pc;

        int rows = ui->tableWidget_3->rowCount();
        ui->tableWidget_3->insertRow(rows);
        ui->tableWidget_3->setItem(rows, 0, new QTableWidgetItem(p->stu));
        ui->tableWidget_3->setItem(rows, 1, new QTableWidgetItem(name_tmp));
        ui->tableWidget_3->setItem(rows, 2, new QTableWidgetItem(p->crs));
        ui->tableWidget_3->setItem(rows, 3, new QTableWidgetItem(QString::number(score_tmp)));
        ui->tableWidget_3->setItem(rows, 4, new QTableWidgetItem(((p->grade)==-1)?"":QString::number(p->grade)));
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
    QList<QTableWidgetItem*>items = (ui->tabWidget->currentIndex() == 0) ? ui->tableWidget->selectedItems() :
                                                                           ((ui->tabWidget->currentIndex() == 1) ? ui->tableWidget_2->selectedItems() :
                                                                                                                   ui->tableWidget_3->selectedItems());
    char temp[20] = {'\0'};
    QString text;
    if(!items.count() || ui->tabWidget->currentIndex() == 1)
    {
        text = QInputDialog::getText(NULL, tr("删除学生数据"), tr("请输入学号信息"));
    }
    else
    {
        QTableWidgetItem *item=(ui->tabWidget->currentIndex() == 0)?items.at(1):items.at(0);
        text = item->text();
    }
    if(text == NULL)
        return;
    QByteArray bytearray = text.toLocal8Bit();
    bytearray = AnsiToUtf8(bytearray);
    strcpy(temp, bytearray.data());
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
    QList<QTableWidgetItem*>items = (ui->tabWidget->currentIndex() == 0) ? ui->tableWidget->selectedItems() :
                                                                           ((ui->tabWidget->currentIndex() == 1) ? ui->tableWidget_2->selectedItems() :
                                                                                                                   ui->tableWidget_3->selectedItems());
    char stuid[20] = {'\0'};
    QString text;
    if(!items.count() || ui->tabWidget->currentIndex() == 1)
    {
        text = QInputDialog::getText(NULL, tr("修改学生数据"), tr("请输入学号信息"));
    }
    else
    {
        QTableWidgetItem *item=(ui->tabWidget->currentIndex() == 0)?items.at(1):items.at(0);
        text = item->text();
    }
    if(text == NULL)
        return;
    QByteArray bytearray = text.toLocal8Bit();
    bytearray = AnsiToUtf8(bytearray);
    strcpy(stuid, bytearray.data());

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
    Course.save(temp, Course.head);
    QMessageBox::information(NULL, "保存完成", "保存完成", QMessageBox::Ok, QMessageBox::Ok);
    MainWindow::display_course_info();
}

void MainWindow::on_class_del_triggered()
{
    QList<QTableWidgetItem*>items = (ui->tabWidget->currentIndex() == 0) ? ui->tableWidget->selectedItems() :
                                                                           ((ui->tabWidget->currentIndex() == 1) ? ui->tableWidget_2->selectedItems() :
                                                                                                                   ui->tableWidget_3->selectedItems());
    char temp[20] = {'\0'};
    QString text;
    if(!items.count() || ui->tabWidget->currentIndex() == 0)
    {
        text = QInputDialog::getText(NULL, tr("删除课程数据"), tr("请输入课程名"));
    }
    else
    {
        QTableWidgetItem *item=(ui->tabWidget->currentIndex() == 1)?items.at(0):items.at(2);
        text = item->text();
    }
    if(text == NULL)
        return;
    QByteArray bytearray = text.toLocal8Bit();
    bytearray = AnsiToUtf8(bytearray);
    strcpy(temp, bytearray.data());
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
    else if(Course.serh(pc, 0, Course.head) == NULL)
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
    QList<QTableWidgetItem*>items = (ui->tabWidget->currentIndex() == 0) ? ui->tableWidget->selectedItems() :
                                                                           ((ui->tabWidget->currentIndex() == 1) ? ui->tableWidget_2->selectedItems() :
                                                                                                                   ui->tableWidget_3->selectedItems());
    char stuid[20] = {'\0'};
    char coursename[20] = {'\0'};
    QString text, text2;
    if(!items.count() || ui->tabWidget->currentIndex() != 2)
    {
        text = QInputDialog::getText(NULL, tr("输入退课数据"), tr("请输入学号"));
        if(text == NULL)
            return;

        text2 = QInputDialog::getText(NULL, tr("输入退课数据"), tr("请输入课程名"));
        if(text2 == NULL)
            return;
    }
    else
    {
        QTableWidgetItem *item=items.at(0);
        text = item->text();
        item = items.at(2);
        text2 = item->text();
    }
    QByteArray bytearray = text.toLocal8Bit();
    bytearray = AnsiToUtf8(bytearray);
    strcpy(stuid, bytearray.data());
    QByteArray bytearray2 = text2.toLocal8Bit();
    bytearray2 = AnsiToUtf8(bytearray2);
    strcpy(coursename, bytearray2.data());

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
    else if(Course.serh(pc, 0, Course.head) == NULL)
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
    QString fileName = QFileDialog::getOpenFileName(this, "另存为...", ".", ("文本文件(*.txt)"));
    if(fileName.length() == 0) return;
    QByteArray bytearray = fileName.toLocal8Bit();
    char *temp = bytearray.data();
    Choose.save(temp);
    QMessageBox::information(NULL, "保存完成", "保存完成", QMessageBox::Ok, QMessageBox::Ok);
    MainWindow::display_choose_info();
}

void MainWindow::on_search_stu_triggered()
{
    QList<QTableWidgetItem*>items = (ui->tabWidget->currentIndex() == 0) ? ui->tableWidget->selectedItems() :
                                                                            ((ui->tabWidget->currentIndex() == 1) ? ui->tableWidget_2->selectedItems() :
                                                                                                                    ui->tableWidget_3->selectedItems());
    char stuid[20] = {'\0'};
    QString text;
    if(!items.count() || ui->tabWidget->currentIndex() == 1)
    {
        text = QInputDialog::getText(NULL, tr("搜索"), tr("请输入学号"));
    }
    else
    {
        QTableWidgetItem *item = (ui->tabWidget->currentIndex() == 0)?items.at(1):items.at(0);
        text = item->text();
    }
    if(text == NULL)
        return;
    QByteArray bytearray = text.toLocal8Bit();
    bytearray = AnsiToUtf8(bytearray);
    strcpy(stuid, bytearray.data());

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
    QList<QTableWidgetItem*>items = (ui->tabWidget->currentIndex() == 0) ? ui->tableWidget->selectedItems() :
                                                                            ((ui->tabWidget->currentIndex() == 1) ? ui->tableWidget_2->selectedItems() :
                                                                                                                    ui->tableWidget_3->selectedItems());
    char coursename[20] = {'\0'};
    QString text;
    if(!items.count() || ui->tabWidget->currentIndex() == 0)
    {
        text = QInputDialog::getText(NULL, tr("搜索"), tr("请输入课程名"));
    }
    else
    {
        QTableWidgetItem *item = (ui->tabWidget->currentIndex() == 1)?items.at(0):items.at(2);
        text = item->text();
    }
    if(text == NULL)
        return;
    QByteArray bytearray = text.toLocal8Bit();
    bytearray = AnsiToUtf8(bytearray);
    strcpy(coursename, bytearray.data());

    StuCrsInfo *p = new StuCrsInfo;
    strcpy(p->crs, coursename);
    p->next = NULL;

    CourseInfo *pc = new CourseInfo;
    strcpy(pc->name, coursename);
    pc->next = NULL;
    if(Course.serh(pc, 0, Course.head) == NULL)
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

void MainWindow::on_add_grade_triggered()
{
    QList<QTableWidgetItem*>items = (ui->tabWidget->currentIndex() == 0) ? ui->tableWidget->selectedItems() :
                                                                            ((ui->tabWidget->currentIndex() == 1) ? ui->tableWidget_2->selectedItems() :
                                                                                                                    ui->tableWidget_3->selectedItems());
    QString text, text2;
    if(!items.count() || ui->tabWidget->currentIndex() != 2)
    {
        text = QInputDialog::getText(NULL, tr("添加成绩"), tr("请输入学号"));
        text2 = QInputDialog::getText(NULL, tr("添加成绩"), tr("请输入课程名"));
    }
    else
    {
        QTableWidgetItem *item = items.at(0);
        text = item->text();
        item = items.at(2);
        text2 = item->text();
    }
    if(text == NULL)
        return;
    QByteArray bytearray = text.toLocal8Bit();
    bytearray = AnsiToUtf8(bytearray);
    char *stuid = bytearray.data();
    if(text2 == NULL)
        return;
    QByteArray bytearray2 = text2.toLocal8Bit();
    bytearray2 = AnsiToUtf8(bytearray2);
    char *coursename = bytearray2.data();

    QString text3 = QInputDialog::getText(NULL, tr("添加成绩"), tr("请输入成绩"));
    if(text3 == NULL)
        return;
    QByteArray bytearray3 = text3.toLocal8Bit();
    bytearray3 = AnsiToUtf8(bytearray3);
    int score = atoi(bytearray3.data());

    StuCrsInfo *p = new StuCrsInfo;
    p->next = NULL;
    strcpy(p->stu, stuid);
    strcpy(p->crs, coursename);
    p->grade = score;
    CourseInfo *pc = new CourseInfo;
    strcpy(pc->name, coursename);
    StuInfo *ps = new StuInfo;
    strcpy(ps->id, stuid);
    if(Student.serh(ps, 1) == NULL)
        QMessageBox::information(NULL, "添加结果", "登记成绩失败-学生不存在", QMessageBox::Ok, QMessageBox::Ok);
    else if(Course.serh(pc, 0, Course.head) == NULL)
        QMessageBox::information(NULL, "添加结果", "登记成绩失败-课程不存在", QMessageBox::Ok, QMessageBox::Ok);
    else if(Choose.addgrade(p))
        QMessageBox::information(NULL, "添加结果", "登记成绩成功", QMessageBox::Ok, QMessageBox::Ok);
    else
        QMessageBox::information(NULL, "添加结果", "登记成绩失败-未选课", QMessageBox::Ok, QMessageBox::Ok);
    delete ps;
    delete pc;
    MainWindow::display_choose_info();
}

void MainWindow::on_del_grade_triggered()
{
    QList<QTableWidgetItem*>items = (ui->tabWidget->currentIndex() == 0) ? ui->tableWidget->selectedItems() :
                                                                            ((ui->tabWidget->currentIndex() == 1) ? ui->tableWidget_2->selectedItems() :
                                                                                                                    ui->tableWidget_3->selectedItems());
    QString text, text2;
    if(!items.count() || ui->tabWidget->currentIndex() != 2)
    {
        text = QInputDialog::getText(NULL, tr("删除成绩"), tr("请输入学号"));
        text2 = QInputDialog::getText(NULL, tr("删除成绩"), tr("请输入课程名"));
    }
    else
    {
        QTableWidgetItem *item = items.at(0);
        text = item->text();
        item = items.at(2);
        text2 = item->text();
    }
    if(text == NULL)
        return;
    QByteArray bytearray = text.toLocal8Bit();
    bytearray = AnsiToUtf8(bytearray);
    char *stuid = bytearray.data();
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
        QMessageBox::information(NULL, "删除结果", "删除成绩失败-学生不存在", QMessageBox::Ok, QMessageBox::Ok);
    else if(Course.serh(pc, 0, Course.head) == NULL)
        QMessageBox::information(NULL, "删除结果", "删除成绩失败-课程不存在", QMessageBox::Ok, QMessageBox::Ok);
    else if(Choose.delgrade(p))
        QMessageBox::information(NULL, "删除结果", "删除成绩成功", QMessageBox::Ok, QMessageBox::Ok);
    else
        QMessageBox::information(NULL, "删除结果", "删除成绩失败-未选课", QMessageBox::Ok, QMessageBox::Ok);
    delete ps;
    delete pc;
    MainWindow::display_choose_info();
}

void MainWindow::on_caculate_triggered()
{
    QList<QTableWidgetItem*>items = (ui->tabWidget->currentIndex() == 0) ? ui->tableWidget->selectedItems() :
                                                                            ((ui->tabWidget->currentIndex() == 1) ? ui->tableWidget_2->selectedItems() :
                                                                                                                    ui->tableWidget_3->selectedItems());
    char stuid[20] = {'\0'};
    QString text;
    if(!items.count() || ui->tabWidget->currentIndex() == 1)
    {
        text = QInputDialog::getText(NULL, tr("计算学分绩"), tr("请输入学号"));
    }
    else
    {
        QTableWidgetItem *item = (ui->tabWidget->currentIndex() == 0)?items.at(1):items.at(0);
        text = item->text();
    }
    if(text == NULL)
        return;
    QByteArray bytearray = text.toLocal8Bit();
    bytearray = AnsiToUtf8(bytearray);
    strcpy(stuid, bytearray.data());

    StuCrsInfo *p = new StuCrsInfo;
    strcpy(p->stu, stuid);
    p->next = NULL;

    StuInfo *ps = new StuInfo;
    strcpy(ps->id, stuid);
    ps->next = NULL;
    if(Student.serh(ps,1) == NULL)
    {
        QMessageBox::information(NULL, "计算学分绩", "不存在该学生", QMessageBox::Ok, QMessageBox::Ok);
        delete ps;
        return;
    }
    delete ps;

    p = Choose.serh(p, 3);

    double gpa = 0;
    int score_cnt = 0;
    while(p->next != NULL)
    {
        if(p->grade >= 0)
        {
            CourseInfo *pc = new CourseInfo;
            strcpy(pc->name, p->crs);
            CourseInfo *tmp = Course.serh(pc, 0, Course.head);
            delete pc;
            score_cnt += tmp->score;
            gpa += (tmp->score * p->grade);
        }
        p = p->next;
    }
    if(score_cnt == 0)
        gpa = 0;
    else
        gpa /= (20 * score_cnt);
    char output[20];
    sprintf(output, "%lf", gpa);
    QMessageBox::information(NULL, "计算完成", output, QMessageBox::Ok, QMessageBox::Ok);
    delete p;

}
