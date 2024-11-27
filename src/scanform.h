#ifndef SCANFORM_H
#define SCANFORM_H

#include <QWidget>
#include "scardscanner.h"

namespace Ui {
class Form;
}

class ScanForm : public QWidget
{
    Q_OBJECT

public:
    explicit ScanForm(QWidget *parent = nullptr);
    ~ScanForm();

private:
    Ui::Form *ui;
    SCardScanner mScanner;
};

#endif // SCANFORM_H
