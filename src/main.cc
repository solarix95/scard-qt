
#include <QApplication>
#include "scanform.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    ScanForm form;
    form.show();

    app.exec();
}


