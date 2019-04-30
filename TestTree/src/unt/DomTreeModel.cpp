#include "DomTreeModel.h"

#include "dbg.h"

/* DomItem (Элемент DOM-модели). **********************************************/
/******************************************************************************/

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
DomItem::DomItem(QDomNode &node, int row, DomItem *parent) {
    this->domNode = node;
    this->rowNum = row;
    this->parentItem = parent;
}// DomItem

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
DomItem::~DomItem()
    { for(DomItem* item: this->childItems.values()) { delete item; }}

// Вернуть потомка. ------------------------------------------------------------
//------------------------------------------------------------------------------
DomItem* DomItem::child(int i) {
    if(this->childItems.contains(i)) { return childItems[i]; }

    if(i >= 0 && i < this->domNode.childNodes().count()) {
        QDomNode childNode = this->domNode.childNodes().item(i);
        DomItem *childItem = new DomItem(childNode, i, this);
        this->childItems[i] = childItem;
        return childItem;
    }// if(i >= 0 && i < this->domNode.childNodes().count())

    return 0;
}// child

// Вернуть предка. -------------------------------------------------------------
//------------------------------------------------------------------------------
DomItem* DomItem::parent(void) { return parentItem; }

// Вернуть текущий узел. -------------------------------------------------------
//------------------------------------------------------------------------------
QDomNode DomItem::node(void) const { return domNode; }

// Количество строк. -----------------------------------------------------------
//------------------------------------------------------------------------------
int DomItem::row(void) { return rowNum; }


/* DomTreeModel (Модель представления DOM в виде дерева). *********************/
/******************************************************************************/

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
DomTreeModel::DomTreeModel(QDomDocument doc, QObject *parent)
  : QAbstractItemModel(parent), domDoc(doc)
{
    this->rootItem = new DomItem(this->domDoc, 0);
}// DomTreeModel

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
DomTreeModel::~DomTreeModel()  { delete this->rootItem; }

// Количество столбцов в модели. -----------------------------------------------
//------------------------------------------------------------------------------
int DomTreeModel::columnCount(const QModelIndex &/*parent*/) const
    { return 3; }

// Вернуть данные. -------------------------------------------------------------
//------------------------------------------------------------------------------
QVariant DomTreeModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) { return QVariant(); }
    if (role != Qt::DisplayRole) {return QVariant(); }

    DomItem *item = static_cast<DomItem*>(index.internalPointer());
    QDomNode node = item->node();
    QStringList attr;
    QDomNamedNodeMap attributeMap = node.attributes();

    switch(index.column()) {
        case 0:
            return node.nodeName();

        case 1:
            for(int i=0; i < attributeMap.count(); ++i) {
                QDomNode attribute = attributeMap.item(i);
                attr << attribute.nodeName() + "=\""
                      + attribute.nodeValue() + "\"";
            }// i
            return attr.join(' ');

        case 2:
            return node.nodeValue().split("\n").join(' ');

        default:
            return QVariant();
    }// switch(index.column())

}// data

// Флаги. ----------------------------------------------------------------------
//------------------------------------------------------------------------------
Qt::ItemFlags DomTreeModel::flags(const QModelIndex &index) const {
    if (!index.isValid()) { return 0; }

    return QAbstractItemModel::flags(index);
}// flags

// Заголовки. ------------------------------------------------------------------
//------------------------------------------------------------------------------
QVariant DomTreeModel::headerData(
    int section, Qt::Orientation orient, int role ) const
{
    if (orient == Qt::Horizontal && role == Qt::DisplayRole) {

        switch (section) {
            case 0:
                return tr("Имя");

            case 1:
                return tr("Атрибуты");

            case 2:
                return tr("Значение");

            default:
                return QVariant();

        }// switch (section)

    }// if (orient == Qt::Horizontal && role == Qt::DisplayRole)

    return QVariant();
}// headerData


// Индекс потомка. -------------------------------------------------------------
//------------------------------------------------------------------------------
QModelIndex DomTreeModel::index(
    int row, int col, const QModelIndex &parent ) const
{
    if(!hasIndex(row, col, parent)) { return QModelIndex(); }

    DomItem *parentItem;
    if(!parent.isValid())
        { parentItem = this->rootItem; }
    else
        { parentItem = static_cast<DomItem*>(parent.internalPointer()); }

    DomItem *childItem = parentItem->child(row);
    if(childItem)
        { return createIndex(row, col, childItem); }
    else
        { return QModelIndex(); }
}// index

// Индекс предка. --------------------------------------------------------------
//------------------------------------------------------------------------------
QModelIndex DomTreeModel::parent(const QModelIndex &child) const {
    if(!child.isValid()) { return QModelIndex(); }

    DomItem *childItem = static_cast<DomItem*>(child.internalPointer());
    DomItem *parentItem = childItem->parent();

    if(!parentItem || parentItem == this->rootItem) { return QModelIndex(); }

    return createIndex(parentItem->row(), 0, parentItem);
}// parent

// Количество строк. -----------------------------------------------------------
//------------------------------------------------------------------------------
int DomTreeModel::rowCount(const QModelIndex &parent) const {
    if(parent.column() > 0) { return 0; }

    DomItem *parentItem;
    if(!parent.isValid())
        { parentItem = this->rootItem; }
    else
        { parentItem = static_cast<DomItem*>(parent.internalPointer()); }

    return parentItem->node().childNodes().count();
}// rowCount

//------------------------------------------------------------------------------




