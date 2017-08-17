#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

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

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
