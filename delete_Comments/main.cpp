#include "widget.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setStyle(QStyleFactory::create("windowsvista"));
    Widget w;
    w.show();

    return a.exec();
}
