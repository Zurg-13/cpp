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
    QMap<QString, QString> p_list;
    QString ref, name;

    explicit Command(const QString& xml);

    QString toString(void);
};// Command

/*
### команда ###
element cmd {
    ## связка команды с ответом. Для event не указывается
    element ref { text }

  , ## действие
    element name { text }

  , element p_list {
        element p {
            attribute key {text}

          , ## значение параметра
            text
        }
    }
}
*/

//------------------------------------------------------------------------------
#endif // COMMAND_H
