#ifndef SCARDSCANNER_H
#define SCARDSCANNER_H

#include <winscard.h>
#include <QObject>

class SCardScanner : public QObject
{
    Q_OBJECT
public:
    SCardScanner();
    virtual ~SCardScanner();

    bool init();

signals:
    void log(const QString &msg);

private slots:
    void scan();

private:
     SCARDCONTEXT mContext;
};

#endif // SCARDSCANNER_H
