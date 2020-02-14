#ifndef LST_H
#define LST_H

// INCLUDE ---------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QMap>
#include <QList>
#include <QPair>
#include <QString>

/* Библиотека работы со списками. *********************************************/
/******************************************************************************/

// Декорировать каждый элемент списка. -----------------------------------------
//------------------------------------------------------------------------------
QList<QString> DCR(const QString &pfx, const QList<QString> &lst);
QList<QString> DCR(const QList<QString> &lst, const QString &pst);
QList<QString> DCR(
    const QString &pfx, const QList<QString> &lst, const QString &pst );

// Приведение к списку пар. ----------------------------------------------------
//------------------------------------------------------------------------------
template <typename K, typename V>
QList<QPair<K, V>> LST(QMap<K, V> map) {
    QList<QPair<K, V>> ret;
    for(const K &key: map.keys()) { ret.append(QPair<K, V>(key, map[key])); }
    return ret;
}// LST

// Собрать строку из списка элементов. -----------------------------------------
//------------------------------------------------------------------------------
QString BLD(const QList<QString> &lst, QString sep = ", ");
QString BLD(const QList<QPair<QString, QString>> &lst, QString sep = ", ");

template <typename T, typename F>
QString BLD(const QList<T> &lst, F cst, const QString &sep) {
    QList<QString> bld;

    if(lst.size()) {
        typename QList<T>::const_iterator it = lst.constBegin();
        bld.append(cst(*it)); it++;
        while(it != lst.constEnd()) { bld.append(sep + cst(*it)); it++; }
    }// if(lst.size())

    return bld.join(QString());
}// BLD

// Создание пары. --------------------------------------------------------------
//------------------------------------------------------------------------------
template <typename F, typename S>
QPair<F, S> PR(F fst, S scd) { return QPair<F, S>(fst,scd ); }

// Удаление объектов по всем указателям в списке. ------------------------------
//------------------------------------------------------------------------------
template <typename T>
void DEL(QList<T> lst) { for(T val : lst) { delete val; }}

//------------------------------------------------------------------------------
#endif // LST_H
