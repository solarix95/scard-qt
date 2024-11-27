#include <QPushButton>
#include "scanform.h"
#include "ui_scanform.h"

ScanForm::ScanForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Form)
{
    ui->setupUi(this);

    connect(&mScanner, &SCardScanner::log, this, [this](const QString &msg) {
        ui->edtLog->appendPlainText(msg);
    });

    connect(ui->btnStart, &QPushButton::clicked, this, [this]() {
        if (mScanner.init())
            ui->btnStart->setEnabled(false);
    });
}

ScanForm::~ScanForm()
{
    delete ui;
}
