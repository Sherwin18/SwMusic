#ifndef LISTMODEL_H
#define LISTMODEL_H
#include <QAbstractListModel>


class ListModel : public QAbstractListModel
{
public:
    ListModel();
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QStringList messageList;
};

#endif // LISTMODEL_H
