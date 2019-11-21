#ifndef DRW_H
#define DRW_H

// INCLUDE ---------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QPainter>


//DEFINE------------------------------------------------------------------------
//------------------------------------------------------------------------------

//Цвета.----------------------------------------
//----------------------------------------------

//Общие.
#define CL_LBL_STD QColor(0xFFFFFF) // Стандартный цвет (белый).
#define CL_LBL_HLT QColor(0x494949) // Цвет выделения (серый-модифицированный).

//Базовые макросы.------------------------------
//----------------------------------------------

//Выполняет подготовку к отрисовке.
#define BGN_PAINT   \
    const QSortFilterProxyModel *prx =                              \
        qobject_cast<const QSortFilterProxyModel*>(index.model());  \
    QModelIndex idx(prx->mapToSource(index));                       \
    const QSqlQueryModel *mdl =                                     \
        qobject_cast<const QSqlQueryModel*>(idx.model());           \
    painter->save();

//Вызывает отрисовку по умолчанию.
#define DFT_PAINT QItemDelegate::paint(painter, option, index);

//Восстанавливает исходное состояние отрисовщика.
#define END_PAINT painter->restore();

//Возвращает индекс колонки по её имени.
#define IDX_OF(f) mdl->record().indexOf(f)

//Извлекает значение из текущей строки.
#define CUR_VAL(f) mdl->record(idx.row()).value(f)

//Имя текущей колонки.
#define CUR_COL_NAME index.model()->headerData(index.column(), Qt::Horizontal, Qt::EditRole).toString()

//Рисует заполненный прямоугольник.
#define FILL_RECT(r, c) painter->fillRect(r, c)

//Рисует текст.
#define DRAW_TEXT_L(r, t) painter->drawText(r.x()+3, r.y(), r.width(), r.height(), Qt::AlignVCenter, t)
#define DRAW_TEXT_C(r, t) painter->drawText(r, Qt::AlignCenter, t)

//Текущая ячейка.
#define RCT option.rect

//Положение ячейки.
#define R_X RCT.x()         //x
#define R_Y RCT.y()         //y

//Края ячейки.
#define RGT RCT.right()     //Правый.
#define LFT RCT.left()      //Левый.
#define TOP RCT.top()       //Верхний.
#define BTM RCT.bottom()    //Нижний.

//Размеры ячейки
#define WDT RCT.width()     //Ширина.
#define HGT RCT.height()    //Высота.

//Рамки внутри ячейки.
#define FRM QRect(LFT-1, TOP-1, WDT+1, HGT+1)

//Макросы отрисовки.----------------------------
//----------------------------------------------

//Отрисовка текста в прямоугольнике.
#define RCT_TXT(r, c, t) do{ \
    painter->fillRect(r, c);                    \
    painter->drawText(r, Qt::AlignCenter, t);   \
    painter->setPen(Qt::white);                 \
    painter->drawRect(r);                       \
}while(false)

//Отрисовка цветного текста в прямоугольнике.
#define RCT_TXT_CLR(r, rc, t, tc) do{ \
    painter->fillRect(r, rc);                   \
    painter->setPen(tc);                        \
    painter->drawText(r, Qt::AlignCenter, t);   \
    painter->setPen(Qt::white);                 \
    painter->drawRect(r);                       \
}while(false)

//Общие макросы.--------------------------------
//----------------------------------------------

//Изменение цвета.
#define BACKGROUND_CLR(w, c) do {QPalette p = w->palette(); p.setColor(QPalette::Window, c); w->setPalette(p); w->setAutoFillBackground(true);} while(false)
#define TEXT_CLR(w, c) do {QPalette p = w->palette(); p.setColor(QPalette::WindowText, c); w->setPalette(p); w->setAutoFillBackground(true);} while(false)


//Общие функции.--------------------------------
//----------------------------------------------


//---------------------------------------------------------------------------
#endif // DRW_H
