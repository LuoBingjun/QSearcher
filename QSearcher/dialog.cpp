#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(htmlInfo* _html, const LinkedList<CharString>& keywords, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    html = _html;
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::Dialog);

    str = QString::fromStdWString(html->content);
    for(auto p = keywords.head(); p!=nullptr; p = ++*p)
    {
        QString k = QString::fromStdWString(p->data);
        QRegExp valueRegExp(k, Qt::CaseInsensitive);
        str = str.replace(valueRegExp, QObject::tr("<font color = red>%1</font>").arg(k));
    }
    ui->textBrowser->setHtml(str.replace('\n', tr("<br />")));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::closeEvent(QCloseEvent *e)
{
    this->deleteLater();
}

void Dialog::on_pushButton_2_clicked()
{
    close();
}

void Dialog::on_pushButton_clicked()
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(".\\input\\" + QString::number(html->ID) + ".html"));
}
