#pragma once

#include <QDialog>
#include <QRegExp>
#include <QDesktopServices>
#include <QUrl>
#include "LinkedList.hpp"
#include "htmlHandler.h"
#include "dialog.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(htmlInfo* _html, const LinkedList<CharString>& keywords, QWidget *parent = nullptr);
    ~Dialog();
protected:
    virtual void closeEvent(QCloseEvent *e);
private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::Dialog *ui;
    QString str;
    htmlInfo* html;
};
