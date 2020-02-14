#ifndef COMMAND_H
#define COMMAND_H


// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QString>
#include <QMap>

/* Формирование команды. ******************************************************/
/******************************************************************************/
class Command {

 public:
    QString ref, action, module;
    QMap<QString, QString> p_list;

    explicit Command(const QString &xml);
             Command(
                const QString &ref, const QString &action, const QString &module
             ) : Command(ref, action, module, {}) {}
             Command(
                const QString &ref, const QString &action, const QString &module
              , const QMap<QString, QString> &p_list
             ) : ref(ref), action(action), module(module), p_list(p_list) {}

    QString toString(void);
};// Command

/*
### команда ###
element cmd {
    ## связка команды с ответом. Для event не указывается
    element ref { text }

  , ## модуль, которуму предназначена команда
    element module { text }

  , ## действие
    element action { text }

  , element p_map {
        element p {
            attribute key {text}

          , ## значение параметра
            text
    }
}}
*/

//------------------------------------------------------------------------------
#endif // COMMAND_H
