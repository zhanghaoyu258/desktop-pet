#include "widget.h"

#include <QApplication>
#include <QSystemTrayIcon>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;

    QSystemTrayIcon sysTray(QIcon(":/static/img/static/b_0.png"),&w);

    QMenu menu;
    auto showAct=new QAction("show",&sysTray);
    auto exitAct=new QAction("exit",&sysTray);

    QObject::connect(showAct,&QAction::triggered,[&](){
        w.setVisible(true);
    });
    QObject::connect(exitAct,&QAction::triggered,[&](){
        QApplication::quit();
    });

    menu.addAction(showAct);
    menu.addAction(exitAct);

    sysTray.setContextMenu(&menu);
    sysTray.show();
    w.show();
    return a.exec();
}
