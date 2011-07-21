#include <QtGui/QApplication>
#include "qfit.h"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    qfit foo;
    foo.show();
    return app.exec();
}
