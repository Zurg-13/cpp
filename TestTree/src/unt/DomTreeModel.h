#ifndef DOMTREEMODEL_H
#define DOMTREEMODEL_H

#include <QDomNode>
#include <QHash>
#include <QAbstractItemModel>
#include <QDomDocument>
#include <QModelIndex>

/* Элемент DOM-модели. ********************************************************/
/******************************************************************************/
class DomItem {
  public:
    DomItem(QDomNode &node, int row, DomItem *parent = 0);
   ~DomItem();

    DomItem* child(int i);
    DomItem* parent(void);
    QDomNode node(void) const;
    int row(void);

  private:
    QDomNode domNode;
    QHash<int, DomItem*> childItems;
    DomItem *parentItem;
    int rowNum;
};// DomItem


/* Модель представления DOM в виде дерева. ************************************/
/******************************************************************************/
class DomTreeModel : public QAbstractItemModel {
    Q_OBJECT

  public:
    explicit DomTreeModel(QDomDocument doc, QObject *parent = 0);
            ~DomTreeModel();

    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(
        int section, Qt::Orientation orient
      , int role = Qt::DisplayRole) const override;
    QModelIndex index(
        int row, int col
      , const QModelIndex &parnt = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

  private:
    QDomDocument domDoc;
    DomItem *rootItem;

};// DomTreeModel

#endif // DOMTREEMODEL_H
