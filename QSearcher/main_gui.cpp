#include <iostream>
#include <locale>
#include <io.h>
#include "htmlDatabase.h"
#include "htmlHandler.h"
#include "CharString.h"
#include "Stack.hpp"
#include "LinkedList.hpp"
#include "Vector.hpp"
#include "mainwindow.h"
#include <QApplication>

QSettings* gSettings = nullptr;

void init()
{
    std::locale::global(std::locale("chs"));
    gSettings = new QSettings("settings.ini", QSettings::IniFormat);

    htmlHandler::init(gSettings->value("xmlpath").toString().toStdString().data());
    htmlHandler::initDictionary();
    globalBase.loadFromFile(gSettings->value("hmifpath").toString().toStdWString(), gSettings->value("w2lpath").toString().toStdWString());
}

int main(int argc, char *argv[])
{
    init();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
