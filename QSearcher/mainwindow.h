#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QThread>
#include <QMessageBox>
#include <QProgressBar>
#include <QSettings>
#include "htmlDatabase.h"

extern QSettings* gSettings;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void inittb();
    void inittb_2();
    void updateSettings();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_tableWidget_cellDoubleClicked(int row, int column);

    void on_tableWidget_2_cellDoubleClicked(int row, int column);

    void on_pushButton_3_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

private:
    Ui::MainWindow *ui;
    LinkedList<CharString> query1;
};


class MyThread : public QThread
{
    Q_OBJECT

public:
    MyThread(const QStringList& _htmls, QProgressBar* _bar);
    static volatile bool processing;

    QString info;

signals:
    void refresh(int i);

protected:
    QStringList htmls;
    QProgressBar* bar;

    virtual void run();
};

#endif // MAINWINDOW_H
