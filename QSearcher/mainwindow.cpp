#include "htmlDatabase.h"
#include "mainwindow.h"
#include "dialog.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->label_7->setOpenExternalLinks(true);
    ui->label_7->setText(QStringLiteral("GitHub地址：<a href=\"https://github.com/LuoBingjun/QSearcher\">Open it!"));
    connect(ui->lineEdit, SIGNAL(returnPressed()), ui->pushButton, SIGNAL(clicked()), Qt::UniqueConnection);
    connect(ui->lineEdit_2, SIGNAL(returnPressed()), ui->pushButton_2, SIGNAL(clicked()), Qt::UniqueConnection);
    inittb();
    inittb_2();
    updateSettings();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::inittb()
{
    ui->tableWidget->setColumnWidth(0, 30);
    ui->tableWidget->setColumnWidth(1, 227);
    ui->tableWidget->setColumnWidth(2, 60);
    ui->tableWidget->setColumnWidth(3, 56);
    ui->tableWidget->setColumnWidth(4, 56);
    ui->tableWidget->setColumnWidth(5, 56);
    ui->tableWidget->setColumnWidth(6, 34);
    ui->tableWidget->setColumnWidth(7, 138);
    ui->tableWidget->setColumnWidth(8, 320);
    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{font-weight:bold;};");
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void MainWindow::inittb_2()
{
    ui->tableWidget_2->setColumnWidth(0, 30);
    ui->tableWidget_2->setColumnWidth(1, 227);
    ui->tableWidget_2->setColumnWidth(2, 70);
    ui->tableWidget_2->setColumnWidth(3, 56);
    ui->tableWidget_2->setColumnWidth(4, 56);
    ui->tableWidget_2->setColumnWidth(5, 34);
    ui->tableWidget_2->setColumnWidth(6, 138);
    ui->tableWidget_2->setColumnWidth(7, 320);
    ui->tableWidget_2->horizontalHeader()->setStyleSheet("QHeaderView::section{font-weight:bold;};");
    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void MainWindow::updateSettings()
{
    ui->lineEdit_3->setText(gSettings->value("xmlpath").toString());
    ui->lineEdit_4->setText(gSettings->value("hmifpath").toString());
    ui->lineEdit_5->setText(gSettings->value("w2lpath").toString());
}

void MainWindow::on_pushButton_2_clicked()
{
    CharString str = ui->lineEdit_2->text().toStdWString();
    auto&& result2 = globalBase.query2(str);
    QString result;
    int n = 0;
    for(auto i = result2.head(); i != nullptr; i = ++*i)
    {
        if(n == 5)
            break;
        auto &p = globalBase.htmls[i->data.urlID];
        ui->tableWidget_2->setRowCount(n+1);
        ui->tableWidget_2->setItem(n,0,new QTableWidgetItem(QString::number(p.ID)));
        ui->tableWidget_2->setItem(n,1,new QTableWidgetItem(QString::fromStdWString(p.title)));
        ui->tableWidget_2->setItem(n,2,new QTableWidgetItem(QString::number(i->data.similarity)));
        ui->tableWidget_2->setItem(n,3,new QTableWidgetItem(QStringLiteral("双击查看")));
        ui->tableWidget_2->setItem(n,4,new QTableWidgetItem(QStringLiteral("双击查看")));
        ui->tableWidget_2->setItem(n,5,new QTableWidgetItem(QString::fromStdWString(p.source)));
        ui->tableWidget_2->setItem(n,6,new QTableWidgetItem(QString::fromStdWString(p.time)));
        ui->tableWidget_2->setItem(n,7,new QTableWidgetItem(QString::fromStdWString(p.description)));
        ++n;
    }
}

void MainWindow::on_pushButton_clicked()
{
    CharString str = ui->lineEdit->text().toStdWString();
    query1 = htmlHandler::divideWords(str);
    QString text = QStringLiteral("提取关键词：");
    for(auto i = query1.head(); i != nullptr; i = ++*i)
    {
        text+=(QString::fromStdWString(i->data) + ' ');
    }
    ui->label->setText(text);

    auto&& result1 = globalBase.query1(query1);
    ui->tableWidget->setRowCount(result1.length());
    int n = 0;
    for(auto i = result1.head(); i != nullptr; i = ++*i)
    {
        auto &p = globalBase.htmls[i->data.urlID];
        ui->tableWidget->setItem(n,0,new QTableWidgetItem(QString::number(p.ID)));
        ui->tableWidget->setItem(n,1,new QTableWidgetItem(QString::fromStdWString(p.title)));
        ui->tableWidget->setItem(n,2,new QTableWidgetItem(QString::number(i->data.occurPages)));
        ui->tableWidget->setItem(n,3,new QTableWidgetItem(QString::number(i->data.occurTimes)));
        ui->tableWidget->setItem(n,4,new QTableWidgetItem(QStringLiteral("双击查看")));
        ui->tableWidget->setItem(n,5,new QTableWidgetItem(QStringLiteral("双击查看")));
        ui->tableWidget->setItem(n,6,new QTableWidgetItem(QString::fromStdWString(p.source)));
        ui->tableWidget->setItem(n,7,new QTableWidgetItem(QString::fromStdWString(p.time)));
        ui->tableWidget->setItem(n,8,new QTableWidgetItem(QString::fromStdWString(p.description)));
        ++n;
    }
}

void MainWindow::on_tableWidget_cellDoubleClicked(int row, int column)
{
    if(column == 4)
    {
        int ID = ui->tableWidget->item(row, 0)->text().toInt();
        Dialog* d = new Dialog(&globalBase.htmls[ID], query1);
        d->show();
    }
    else if(column == 5)
    {
        ui->lineEdit_2->setText(ui->tableWidget->item(row, 1)->text());
        ui->pushButton_2->click();
        ui->tabWidget->setCurrentIndex(1);
    }
}

void MainWindow::on_tableWidget_2_cellDoubleClicked(int row, int column)
{
    if(column == 3)
    {
        int ID = ui->tableWidget_2->item(row, 0)->text().toInt();
        Dialog* d = new Dialog(&globalBase.htmls[ID], LinkedList<CharString>());
        d->show();
    }
    else if(column == 4)
    {
        ui->lineEdit_2->setText(ui->tableWidget_2->item(row, 1)->text());
        ui->pushButton_2->click();
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    if(MyThread::processing)
    {
        QMessageBox::critical(this, QStringLiteral("未完成的导入"), QStringLiteral("当前有未完成的导入任务，请稍后再试"), QMessageBox::Ok);
    }
    else
    {
        //定义文件对话框类
        QFileDialog *fileDialog = new QFileDialog(this);
        //定义文件对话框标题
        fileDialog->setWindowTitle(QStringLiteral("打开网页文件"));
        //设置默认文件路径
        //fileDialog->setDirectory("..");
        //设置文件过滤器
        fileDialog->setNameFilter(QStringLiteral("网页(*.html *.htm)"));
        //设置可以选择多个文件,默认为只能选择一个文件QFileDialog::ExistingFiles
        fileDialog->setFileMode(QFileDialog::ExistingFiles);
        //设置视图模式
        fileDialog->setViewMode(QFileDialog::Detail);
        //打印所有选择的文件的路径
        QStringList fileNames;
        if(fileDialog->exec())
        {
            MyThread* thread = new MyThread(fileDialog->selectedFiles(), ui->progressBar);
            connect(thread, &MyThread::finished, thread, [=](){
                QMessageBox::information(this, QStringLiteral("已完成导入"), thread->info);
            });
            connect(thread, &MyThread::refresh, ui->progressBar, &QProgressBar::setValue);
            thread->start();
        }
    }
}

volatile bool MyThread::processing = false;
MyThread::MyThread(const QStringList& _htmls, QProgressBar* _bar):
 htmls(_htmls), bar(_bar)
{
}

void MyThread::run()
{
    processing = true;
    bar->setRange(0, htmls.length());
    int i = 1;
    int success = 0;
    for(auto &p : htmls)
    {
        emit refresh(i);
        QFileInfo file(p);
        QFile::copy(p, tr(".\\input\\") + QString::number(globalBase.htmls.length()) + tr(".html"));
        htmlHandler handle((QString::number(globalBase.htmls.length()) + tr(".html")).toStdString());
        handle.extractInfo();
        if(handle.htmlType == -1)
        {
            QFile::remove(tr(".\\input\\") + QString::number(globalBase.htmls.length()) + tr(".html"));
        }
        else
        {
            ++success;
            handle.divideWords();
            globalBase.addHtml(std::move(handle.info));
        }
        i++;
    }
    info = QStringLiteral("已完成导入%1个网页文件，其中成功%2个，失败%3个").arg(htmls.length()).arg(success).arg(htmls.length() - success);
    globalBase.preTreat();
    processing = false;
}

void MainWindow::on_pushButton_8_clicked()
{
    gSettings->setValue("xmlpath", ".\\config.xml");
    gSettings->setValue("hmifpath", ".\\data.hmif");
    gSettings->setValue("w2lpath", ".\\data.w2l");
    updateSettings();
    QMessageBox::information(this, QStringLiteral("已重置设置"), QStringLiteral("已重置设置，将在重启程序后生效"));
}

void MainWindow::on_pushButton_7_clicked()
{
    gSettings->setValue("xmlpath", ui->lineEdit_3->text());
    gSettings->setValue("hmifpath", ui->lineEdit_4->text());
    gSettings->setValue("w2lpath", ui->lineEdit_5->text());
    QMessageBox::information(this, QStringLiteral("已应用设置"), QStringLiteral("已应用设置，将在重启程序后生效"));
}

void MainWindow::on_pushButton_4_clicked()
{
    //定义文件对话框类
    QFileDialog *fileDialog = new QFileDialog(this);
    //定义文件对话框标题
    fileDialog->setWindowTitle(QStringLiteral("打开文件"));
    //设置文件过滤器
    fileDialog->setNameFilter(QStringLiteral("匹配规则设置(*.xml)"));
    //设置可以选择多个文件,默认为只能选择一个文件QFileDialog::ExistingFiles
    //设置视图模式
    fileDialog->setViewMode(QFileDialog::Detail);
    //打印所有选择的文件的路径
    if(fileDialog->exec())
    {
        ui->lineEdit_3->setText(fileDialog->selectedFiles().front());
        fileDialog->deleteLater();
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    //定义文件对话框类
    QFileDialog *fileDialog = new QFileDialog(this);
    //定义文件对话框标题
    fileDialog->setWindowTitle(QStringLiteral("打开文件"));
    //设置文件过滤器
    fileDialog->setNameFilter(QStringLiteral("网页信息数据(*.hmif)"));
    //设置可以选择多个文件,默认为只能选择一个文件QFileDialog::ExistingFiles
    //设置视图模式
    fileDialog->setViewMode(QFileDialog::Detail);
    //打印所有选择的文件的路径
    if(fileDialog->exec())
    {
        ui->lineEdit_4->setText(fileDialog->selectedFiles().front());
        fileDialog->deleteLater();
    }
}

void MainWindow::on_pushButton_6_clicked()
{
    //定义文件对话框类
    QFileDialog *fileDialog = new QFileDialog(this);
    //定义文件对话框标题
    fileDialog->setWindowTitle(QStringLiteral("打开文件"));
    //设置文件过滤器
    fileDialog->setNameFilter(QStringLiteral("倒排文档数据(*.w2l)"));
    //设置可以选择多个文件,默认为只能选择一个文件QFileDialog::ExistingFiles
    //设置视图模式
    fileDialog->setViewMode(QFileDialog::Detail);
    //打印所有选择的文件的路径
    if(fileDialog->exec())
    {
        ui->lineEdit_5->setText(fileDialog->selectedFiles().front());
        fileDialog->deleteLater();
    }
}
