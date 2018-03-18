#include "listmodel.h"

ListModel::ListModel()
{

}

int ListModel::rowCount(const QModelIndex &parent) const
{
    return messageList.count();
}

QVariant ListModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DisplayRole){
        if(!messageList.isEmpty()){
            int row = index.row();
            return messageList.at(row);
        }
    }
    if (role == Qt::TextAlignmentRole){
            return int(Qt::AlignLeft | Qt::AlignVCenter);
    }
    return QVariant();
}
