#include <QTimer>
#include "scardscanner.h"

QString print_hex(const unsigned char *data, size_t len) {
    QString ret;
    for (size_t i = 0; i < len; i++) {
        if (!ret.isEmpty())
            ret += " ";
        ret += QString::asprintf("%02X", data[i]);
    }
    return ret;
}

//-------------------------------------------------------------------------------------------------
SCardScanner::SCardScanner()
    : mContext(0)
{
}

SCardScanner::~SCardScanner()
{
    if (mContext)
       SCardReleaseContext(mContext);
}

//-------------------------------------------------------------------------------------------------
bool SCardScanner::init()
{
    if (mContext > 0)
        return false;

    LONG rv;
    rv = SCardEstablishContext(SCARD_SCOPE_SYSTEM, NULL, NULL, &mContext);
    if (rv != SCARD_S_SUCCESS) {
        emit log("Failed to establish context");
        return false;
    };
    emit log("PC/SC context established.");

    QTimer *t = new QTimer(this);
    connect(t, &QTimer::timeout, this, &SCardScanner::scan);
    t->start(100);

    return true;
}

//-------------------------------------------------------------------------------------------------
void SCardScanner::scan()
{
    LONG rv;
    SCARDHANDLE hCard;
    DWORD dwActiveProtocol;
    char readers[256];

    DWORD readersLen = sizeof(readers);
    rv = SCardListReaders(mContext, NULL, readers, &readersLen);
    if (rv != SCARD_S_SUCCESS)
        return;

    rv = SCardConnect(mContext, readers, SCARD_SHARE_SHARED, SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1, &hCard, &dwActiveProtocol);
    if (rv != SCARD_S_SUCCESS)
        return;

    BYTE pbAtr[33];
    DWORD dwAtrLen = sizeof(pbAtr);
    rv = SCardStatus(hCard, NULL, NULL, NULL, NULL, pbAtr, &dwAtrLen);
    if (rv == SCARD_S_SUCCESS) {
        emit log("ATR: " + print_hex(pbAtr, dwAtrLen));
    } else {
        emit log("Failed to get ATR");
    }


    BYTE cmdGetUID[] = {0xFF, 0xCA, 0x00, 0x00, 0x00};
    BYTE response[256];
    DWORD responseLen = sizeof(response);
    rv = SCardTransmit(hCard, SCARD_PCI_T1, cmdGetUID, sizeof(cmdGetUID), NULL, response, &responseLen);
    if (rv == SCARD_S_SUCCESS) {
        emit log("Card UID: " + print_hex(response, responseLen));
    } else {
        emit log("Failed to get UID");
    }

    SCardDisconnect(hCard, SCARD_LEAVE_CARD);
}
