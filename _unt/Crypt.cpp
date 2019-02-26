#include "crypt.h"

// Зашифровать строку. ---------------------------------------------------------
//------------------------------------------------------------------------------
QString strEncrypt(QString inp, QString pwd) {
    int i,j;
    QString ret = "";
    QByteArray baInp = inp.toLocal8Bit();
    QByteArray baPwd = pwd.toLocal8Bit();

    // Шифрование.
    for (i=0; i < baInp.size(); i++) {
        for (j=0; j < baPwd.size(); j++) {
            // XOR - кодировка символа.
            baInp[i] = baInp[i] ^ (baPwd[j] + (i*j));
        }// j

        // Преобразование числа в шестнадцатеричную систему
        ret += QString("%1").arg(
            (int)((unsigned char)baInp[i]), 2, 16, QChar('0') );
    }// i

    return ret;
}// strEncrypt

// Раcшифровать строку. --------------------------------------------------------
//------------------------------------------------------------------------------
QString strDecrypt(QString inp, QString pwd) {
    int i,j;
    QByteArray baInp = QByteArray::fromHex(inp.toLocal8Bit());
    QByteArray baPwd = pwd.toLocal8Bit();

    // Раcшифровка.
    for (i=0; i < baInp.size(); i++) {
        for (j=0; j < baPwd.size(); j++) {
            // XOR - кодировка символа.
            baInp[i] = baInp[i] ^ (baPwd[j] + (i*j));
        }// j
    }// i

    return QString::fromLocal8Bit(baInp.data());
}// strDecrypt

//------------------------------------------------------------------------------
