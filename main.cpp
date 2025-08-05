#include <QApplication>
#include "loginUI.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    LoginUI w;
    w.show();

    return a.exec();

}
