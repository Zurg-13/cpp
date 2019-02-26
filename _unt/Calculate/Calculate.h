#ifndef CALCULATE_H
#define CALCULATE_H

// INCLUDE ---------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QString>

/* Константы. *****************************************************************/
/******************************************************************************/
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <stack>
#include <QVector>
#include <cctype>
#include <cmath>

using namespace std;
#define UNARY_MINUS int('#')
#define UNARY_PLUS  int('&')
#define NUMBER      int('$')
#define FUNCTION    65

/* Высисление математических выражений. ***************************************/
/******************************************************************************/
class Calculate {
    typedef map<string, int>::iterator fres;
    typedef map<string, double>::iterator vres;

private:
    map<string, int> ifuns;
    map<string, double> vars;
    vector<double (*)(double)> funs;

public:
    Calculate(void);
   ~Calculate(){ clear(); }

    // Вычислить.
    double eval(const char* expr);

    // Добавить функцию.
    void add_fun(const string& name, double (*pfn)(double));

    // Добавить переменную.
    void add_var(const string& name, double val)
        { vars.insert(make_pair(name, val)); }

    // Изменить значение переменной.
    void set_var(const string& name, double val);

    //очистка
    void clear()
        { funs.clear(); ifuns.clear(); vars.clear(); }

private:
    int prior(int c);


}; // Calculate

//------------------------------------------------------------------------------
#endif // CALCULATE_H
