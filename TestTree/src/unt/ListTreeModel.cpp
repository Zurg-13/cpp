#include "ListTreeModel.h"

#include <QDebug>
#include <QFileInfo>



/*************************** ListTreeModel ************************************/

// Конструкторы. ---------------------------------------------------------------
//------------------------------------------------------------------------------
ListTreeModel::ListTreeModel(QObject *parent) : QAbstractItemModel(parent)
    { nodes.clear(); }
ListTreeModel::ListTreeModel(QVariant *tree, QObject *parent)
  : tree(tree), QAbstractItemModel(parent)
    { fetchRoot(); }

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
ListTreeModel::~ListTreeModel() {

}//~ListTreeModel

// Вернуть индекс элемента. ----------------------------------------------------
//------------------------------------------------------------------------------
QModelIndex ListTreeModel::index(
    int row, int col, const QModelIndex &parent ) const
{
    if(!this->hasIndex(row, col, parent)) { return QModelIndex(); }

    if(!parent.isValid()) { // Запрошены индексы корневых узлов.
        Q_ASSERT(this->nodes.size() > row);
        return createIndex(row, col, const_cast<NodeInfo*>(&nodes[row]));
    }// if(!parent.isValid())

    NodeInfo* parentInfo = static_cast<NodeInfo*>(parent.internalPointer());
    Q_ASSERT(parentInfo != 0);
    Q_ASSERT(parentInfo->mapped);
    Q_ASSERT(parentInfo->childs.size() > row);
    return createIndex(row, col, &parentInfo->childs[row]);
}// index

// Вернуть индекс предка. ------------------------------------------------------
//------------------------------------------------------------------------------
QModelIndex ListTreeModel::parent(const QModelIndex &child) const {
    if (!child.isValid()) { return QModelIndex(); }

    NodeInfo* childInfo = static_cast<NodeInfo*>(child.internalPointer());

    Q_ASSERT(childInfo != 0);
    NodeInfo* parentInfo = childInfo->parent;
    if (parentInfo != 0)
        { return createIndex(findRow(parentInfo), ForkColumn, parentInfo); }
    else
        { return QModelIndex();}

}// parent

// Найти строку (подсчёт).------------------------------------------------------
//------------------------------------------------------------------------------
int ListTreeModel::findRow(const NodeInfo *nodeInfo) const {
    Q_ASSERT(nodeInfo != 0);
    const NodeInfoList& parentInfoChildren =
        (nodeInfo->parent != 0 ? nodeInfo->parent->childs: nodes);
    NodeInfoList::const_iterator position =
        qFind(parentInfoChildren, *nodeInfo);
    Q_ASSERT(position != parentInfoChildren.end());
    return std::distance(parentInfoChildren.begin(), position);
}// findRow

// Количество строк в предке. --------------------------------------------------
//------------------------------------------------------------------------------
int ListTreeModel::rowCount(const QModelIndex &parent) const {
    if (!parent.isValid()) { return nodes.size(); }

    const NodeInfo* parentInfo =
        static_cast<const NodeInfo*>(parent.internalPointer());
    Q_ASSERT(parentInfo != 0);

    return parentInfo->childs.size();
}// rowCount

// Наличие потомков. -----------------------------------------------------------
//------------------------------------------------------------------------------
bool ListTreeModel::hasChildren(const QModelIndex &parent) const {

    if (parent.isValid()) {
        const NodeInfo* parentInfo =
            static_cast<const NodeInfo*>(parent.internalPointer());
        Q_ASSERT(parentInfo != 0);
        if (!parentInfo->mapped) { return true; }
    }// if (parent.isValid())

    return QAbstractItemModel::hasChildren(parent);
}// hasChildren

// Количество столбцов. --------------------------------------------------------
//------------------------------------------------------------------------------
int ListTreeModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return ColumnCount;
}// columnCount

// Вернуть данные. -------------------------------------------------------------
//------------------------------------------------------------------------------
QVariant ListTreeModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) { return QVariant(); }

    const NodeInfo* nodeInfo = static_cast<NodeInfo*>(index.internalPointer());
//    const Info& info = nodeInfo->info;
    Q_ASSERT(nodeInfo != 0);

    switch(index.column()) {
      case NameColumn:
        return nameData(nodeInfo->info, role);

/*
      case ChangeDateColumn:
        if(role == Qt::DisplayRole) { return info.tme(); }
        break;
*/

      case OneColumn:
        if(role == Qt::DisplayRole) { return nodeInfo->info.one; }
        break;

      case TwoColumn:
        if(role == Qt::DisplayRole) { return nodeInfo->info.two; }
        break;

      case ThrColumn:
        if(role == Qt::DisplayRole) { return nodeInfo->info.thr; }
        break;

      default: break;
    }// switch(index.column())

    return QVariant();
}// data

// Наименование типов данных. --------------------------------------------------
//------------------------------------------------------------------------------
QVariant ListTreeModel::nameData(const Info &info, int role) const {

    switch(role) {

/*
      case Qt::EditRole: return fileInfo.fileName();
*/

      case Qt::DisplayRole: return info.nme;

/*
      case Qt::DecorationRole:
        return _metaProvider->icon(fileInfo);
*/

      default: break;
    }// switch(role)

    return QVariant();
}// nameData

// Задать данные. --------------------------------------------------------------
//------------------------------------------------------------------------------
bool ListTreeModel::setData(
    const QModelIndex &index, const QVariant &value, int role )
{
    if (role != Qt::EditRole) { return false; }
    if (!index.isValid()) { return false; }
    if (index.column() != NameColumn) { return false; }

//    QString newName = value.toString();

/*
    if (newName.contains('/') || newName.contains(QDir::separator()))
        { return false; }
*/

//    NodeInfo* nodeInfo = static_cast<NodeInfo*>(index.internalPointer());

/*
    QString fullNewName = nodeInfo->fileInfo.absoluteDir().path() +"/" + newName;
    QString fullOldName = nodeInfo->fileInfo.absoluteFilePath();
    qDebug() << fullOldName << fullNewName;
    bool renamed = QFile::rename(fullOldName, fullNewName);
    qDebug() << renamed;
    if (renamed) {
        nodeInfo->fileInfo = QFileInfo(fullNewName);
        emit dataChanged(index, index.sibling(index.row(), ColumnCount));
    }
    return renamed;
*/
    qDebug() << "setData" << value;
    return true;
}// setData

// Заголовки столбцов. ---------------------------------------------------------
//------------------------------------------------------------------------------
QVariant ListTreeModel::headerData(
    int section, Qt::Orientation orient, int role) const
{
    const QStringList headers = {"Раз", "Два", "Три", "Четыре"};
    if(orient == Qt::Horizontal
    && role == Qt::DisplayRole && section < headers.size())
        { return headers[section];}

    return QVariant();
}// headerData

// Возможность выбрать дальше. -------------------------------------------------
//------------------------------------------------------------------------------
bool ListTreeModel::canFetchMore(const QModelIndex &parent) const {
    if (!parent.isValid()) { return false; }

    const NodeInfo* parentInfo =
        static_cast<const NodeInfo*>(parent.internalPointer());
    Q_ASSERT(parentInfo != 0);

    return !parentInfo->mapped;
}// canFetchMore

// Выбрать дальше. -------------------------------------------------------------
//------------------------------------------------------------------------------
void ListTreeModel::fetchMore(const QModelIndex &parent) {

    Q_ASSERT(parent.isValid());
    NodeInfo* parentInfo = static_cast<NodeInfo*>(parent.internalPointer());
    Q_ASSERT(parentInfo != 0);
    Q_ASSERT(!parentInfo->mapped);

    const Info& info = parentInfo->info;

//    Q_ASSERT(fileInfo.isDir());
//    QDir dir = QDir(fileInfo.absoluteFilePath());
//    QFileInfoList children = dir.entryInfoList(QStringList(), QDir::AllEntries | QDir::NoDotAndDotDot, QDir::Name);


/**/
    const QList<QVariant> childs;


    int insrtCnt = (childs.size() <= 0 ? 0 : childs.size() - 1);
    beginInsertRows(parent, 0, insrtCnt);


//    parentInfo->children.reserve(children.size());
//    for (const QFileInfo& entry: children) {
//        NodeInfo nodeInfo(entry, parentInfo);
//        nodeInfo.mapped = !entry.isDir();
//        parentInfo->children.push_back(std::move(nodeInfo));
//    }

    parentInfo->mapped = true;
    endInsertRows();
/**/

    qDebug() << "ListTreeModel::fetchMore";
}// fetchMore

// Выбрать корневые элементы. --------------------------------------------------
//------------------------------------------------------------------------------
void ListTreeModel::fetchRoot() {

    const QList<QVariant> root = (*tree).toList();

    QList<QVariant>::const_iterator it = root.cbegin(), end = root.cend();
    while(it != end) {
        const QMap<QString, QVariant> map = (*it).toMap();
        nodes.append(NodeInfo(Info(
            map["nme"].toString(), map["one"].toString()
          , map["two"].toString(), map["thr"].toString() )));
        it++;
    }// while(it != end)

/*
    const QFileInfoList drives = QDir::drives();
    qCopy(drives.begin(), drives.end(), std::back_inserter(_nodes));
*/
}// fetchRoot

// Флаги. ----------------------------------------------------------------------
//------------------------------------------------------------------------------
Qt::ItemFlags ListTreeModel::flags(const QModelIndex &index) const {
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);

    if(index.isValid() && index.column() == NameColumn) {

/*
        const NodeInfo* nodeInfo =
            static_cast<const NodeInfo*>(index.internalPointer());
*/


/*
        if(!nodeInfo->fileInfo.isRoot()) {
            flags |= Qt::ItemIsEditable;
        }
*/
    }// if(index.isValid() && index.column() == NameColumn)

    return flags;
}// flags

//------------------------------------------------------------------------------



