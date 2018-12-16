#include <QApplication>

#include "World.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon("://images/writer.ico"));
    World w;
    w.show();

    return a.exec();
}
/* EOF */
