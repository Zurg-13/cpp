// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include "Calculate.h"

#include <QTextStream>


/* Calculate. *****************************************************************/
/******************************************************************************/

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
Calculate::Calculate(void) {
    this->add_fun("sin", &sin); this->add_fun("asin", &asin);
    this->add_fun("cos", &cos); this->add_fun("acos", &acos);
    this->add_fun("tan", &tan); this->add_fun("atan", &atan);

    this->add_fun("sqrt",&sqrt); this->add_fun("abs", &fabs);
    this->add_fun("exp", &exp); this->add_fun("log", &log);
}// Calculate

// Вычислить. ------------------------------------------------------------------
//------------------------------------------------------------------------------
double Calculate::eval(const char* expr) {
    stringstream ios;
    stack<int> ops;
    string k;
    bool unary(true);
    int  cnt(0);
    fres pf;
    vres pv;

    // Проверка на правильность скобок.
    for(const char* i = expr; *i; ++i){
        if(*i == '(') {
            ++cnt;
        } else if(*i == ')') {
            --cnt;
        }// else if // if(*i == '(')
    }// i
    if(cnt != 0) { throw(invalid_argument("Expression!")); }

    // Преобразовать инфиксную в постфиксную запись.
    while(*expr) {
        if(*expr == ')') {
            while(!ops.empty() && (ops.top() != '(')) {
                ios << ops.top() << ' ';
                ops.pop();
            }// while(!ops.empty() && (ops.top() != '('))

            ops.pop(); unary = false;
        } else if(*expr == '('){
            ops.push(*expr);
            unary = true;
        } else if(isdigit(*expr)){ //число
            ios << NUMBER << ' ';
            for(; isdigit(*expr); ++expr) { ios << *expr; }
            if(*expr == '.')              { ios << *expr++; }
            for(; isdigit(*expr); ++expr) { ios << *expr; }

            ios << ' '; unary = false;
            continue;
        } else if(isalpha(*expr)) { //переменная или функция
            const char* p = expr;
            while(isalnum(*p)) { ++p; }

            k.assign(expr, p);
            if(*p == '(') { //функция
                if((pf = ifuns.find(k)) == ifuns.end())
                    { throw(invalid_argument("Function not found!")); }

                cnt = FUNCTION + pf->second;
                while(!ops.empty() && (prior(ops.top()) >= prior(cnt)))
                    { ios << ops.top() << ' '; ops.pop(); }
                ops.push(cnt);
            } else { //переменная
                if((pv = vars.find(k)) == vars.end())
                    { throw(invalid_argument("Var not found!")); }
                ios << NUMBER << ' ' << pv->second << ' ';
            }// else // if(*p == '(')

            expr = p; unary = false;
            continue;
        } else if(*expr == '+' || *expr == '-' || *expr == '*' || *expr == '/'){
            cnt = *expr;
            if(unary) {
                if(cnt == '-') {
                    cnt = UNARY_MINUS;
                } else if(cnt == '+') {
                    cnt = UNARY_PLUS;
                }// else if // if(c == '-')
            }// if(unary)

            while(!ops.empty() && (prior(ops.top()) >= prior(cnt))) {
                ios << ops.top() << ' ';
                ops.pop();
            }// while(!ops.empty() && (prior(ops.top()) >= prior(c)))

            ops.push(cnt); unary = true;
        } else if(! isspace(*expr)) {
            throw(invalid_argument("Unknown char!"));
        }// else if // if(*expr == ')')

        ++expr;
    }// while(*expr)

    for(; !ops.empty(); ops.pop()) { ios << ops.top() << ' '; }

    // Вычислить ОПЗ.
    stack<double> cs;
    double v;

    while((ios >> cnt) && !ios.fail()) {
        switch(cnt) {
          case NUMBER:
            ios >> v; cs.push(v);
            break;

          case UNARY_MINUS:
            if(cs.empty())      { goto _err; }
            cs.top() = -cs.top();
            break;

          case UNARY_PLUS:
            if(cs.empty())      { goto _err; }
            break;

          case '+':
            if(cs.size() < 2)   { goto _err; }
            v = cs.top(), cs.pop(), cs.top() += v;
            break;

          case '-':
            if(cs.size() < 2)   { goto _err; }
            v = cs.top(), cs.pop(), cs.top() -= v;
            break;

        case '*':
            if(cs.size() < 2)   { goto _err; }
            v = cs.top(), cs.pop(), cs.top() *= v;
            break;

        case '/':
            if(cs.size() < 2)   { goto _err; }
            if((v = cs.top()) == 0.0)
                { throw(invalid_argument("Div by zero!")); }
            cs.pop(), cs.top() /= v;
            break;

        default:
            if(cs.empty())      { goto _err; }
            if(cnt >= FUNCTION)
                { cs.top() = (*funs[cnt - FUNCTION])(cs.top()); }
            break;
        }// switch(c)
    }// while((ios >> c) && !ios.fail())

    if(cs.size() != 1) {
_err:
        throw(invalid_argument("Error calc!"));
    }// if(cs.size() != 1)

    return cs.top();
}// eval

// Добавление функций. ---------------------------------------------------------
//------------------------------------------------------------------------------
void Calculate::add_fun(const string& name, double (*pfn)(double)) {
    if(ifuns.find(name) == ifuns.end()) {
        ifuns.insert(make_pair(name, funs.size()));
        funs.push_back(pfn);
    }// if(ifuns.find(name) == ifuns.end())
}// add_fun

// Изменить значение переменной. -----------------------------------------------
//------------------------------------------------------------------------------
void Calculate::set_var(const string& name, double val) {
    vres i = vars.find(name);
    if(i != vars.end()) { i->second = val; }
}// set_var

// Приоритет операций. ---------------------------------------------------------
//------------------------------------------------------------------------------
int Calculate::prior(int c) {
    int ret;

    switch(c) {

      case UNARY_MINUS:
      case UNARY_PLUS:
        ret = 4;
        break;

      case '*':
      case '/':
        ret = 3;
        break;

      case '-':
      case '+':
        ret = 2;
        break;

      case '(':
        ret = 1;
        break;

      default:
        ret = (c >= FUNCTION) ? 5 : 0;
        break;

    }// switch(c)

    return ret;
}// prior

//------------------------------------------------------------------------------


