#include "mainwidget.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    MainWidget w;
    w.show();

    return a.exec();
}


//start stop clear 均可 多次调用。 start 之后的start 无效  stop 同理  clear 同理
