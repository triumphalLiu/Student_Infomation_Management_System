#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void display_student_info();
    void display_course_info();
    void display_choose_info();
    void contextMenuEvent(QContextMenuEvent *);

private slots:
    void on_stu_read_triggered();
    void on_stu_save_triggered();
    void on_stu_del_triggered();
    void on_stu_add_triggered();
    void on_stu_chg_triggered();
    void on_class_read_triggered();
    void on_class_save_triggered();
    void on_class_del_triggered();
    void on_class_add_triggered();
    void on_choose_class_triggered();
    void on_delete_class_triggered();
    void on_choose_read_triggered();
    void on_choose_save_triggered();
    void on_search_stu_triggered();
    void on_search_class_triggered();
    void on_add_grade_triggered();
    void on_del_grade_triggered();
    void on_caculate_triggered();

private:
    Ui::MainWindow *ui;
    QAction *Act_Stu_Del;
    QAction *Act_Stu_Chg;
    QAction *Act_Stu_Add;
    QAction *Act_Crs_Del;
    QAction *Act_Crs_Add;
    QAction *Act_Cho_Del;
    QAction *Act_Cho_Add;
    QAction *Act_Srh_Stu;
    QAction *Act_Srh_Crs;
    QAction *Act_ADDGRADE;
    QAction *Act_DELGRADE;
    QAction *Act_Calc;
};

#endif // MAINWINDOW_H
