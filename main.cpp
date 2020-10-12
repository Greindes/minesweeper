#include "mainwindow.h"
#include "minefieldwidget.h"
#include <QApplication>

#include <QTableView>
#include <QHeaderView>
#include <QStringListModel>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
