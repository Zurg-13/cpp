#ifndef LISTTREEMODEL_H
#define LISTTREEMODEL_H

#include <QVector>
#include <QDateTime>
#include <QAbstractItemModel>

// Структура хранения информации об узлах. -------------------------------------
//------------------------------------------------------------------------------
struct Info {
    QString nme;
    QString one, two, thr;

    Info(void) {}
    Info(QString nme, QString one, QString two, QString thr)
        : nme(nme), one(one), two(two), thr(thr) {}

    bool operator == (const Info& rgt)  const {
        return (
            this->one == rgt.one
         && this->two == rgt.thr
         && this->thr == rgt.thr
        );
    }// operator == (const ListTreeModel::Info& rgt)

    QDateTime tme(void) { return QDateTime::currentDateTime(); }
};// Info

// Структура хранения узлов дерева. --------------------------------------------
//------------------------------------------------------------------------------
struct NodeInfo {

    NodeInfo(): parent(0), mapped(false) {}
    NodeInfo(const Info& info, NodeInfo* parent = 0)
      : info(info), parent(parent), mapped(false) {}
    NodeInfo(const Info& info, bool mapped, NodeInfo* parent = 0)
      : info(info), mapped(mapped), parent(parent) {}

    bool operator ==(const NodeInfo& rgt) const {
        bool r = this->info == rgt.info;
        Q_ASSERT(!r || this->parent == rgt.parent);
        Q_ASSERT(!r || this->mapped == rgt.mapped);
        Q_ASSERT(!r || this->childs == rgt.childs);
        return r;
    }

    Info info;
    QVector<NodeInfo> childs;
    NodeInfo* parent;

    bool mapped;
};// NodeInfo



// Модель построения дерева по спискам. ----------------------------------------
//------------------------------------------------------------------------------
typedef QVector<NodeInfo> NodeInfoList;

class ListTreeModel : public QAbstractItemModel {
    Q_OBJECT

public:
    NodeInfoList nodes;


    explicit ListTreeModel(QObject *parent = 0);
             ListTreeModel(QVariant *tree, QObject *parent = 0);
            ~ListTreeModel();

    QModelIndex index(int row, int col, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;

    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

    QVariant headerData(int section, Qt::Orientation orient, int role) const;

    bool canFetchMore(const QModelIndex &parent) const;
    void fetchMore(const QModelIndex &parent);

    Qt::ItemFlags flags(const QModelIndex &index) const;

    bool hasChildren(const QModelIndex &parent) const;

private:
    QVariant *tree = nullptr;


    enum Col {
        ForkColumn // Ветвление - ВСЕГДА первый.
      , NameColumn = ForkColumn
      , ChangeDateColumn
      , OneColumn, TwoColumn, ThrColumn
      , ColumnCount
    };


    void fetchRoot();
    int findRow(const NodeInfo* nodeInfo) const;
    QVariant nameData(const Info& info, int role) const;

};// ListTreeModel

//------------------------------------------------------------------------------
#endif // LISTTREEMODEL_H
