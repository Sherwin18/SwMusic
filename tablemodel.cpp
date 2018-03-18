#include "tablemodel.h"
#include <QDebug>

TableModel::TableModel()
{
}

int TableModel::rowCount(const QModelIndex &parent) const
{
    if(musicList.isEmpty()){
//        return 100;
    }
    else{
        return musicList.count();
    }
}

int TableModel::columnCount(const QModelIndex &parent) const
{
    return 4;
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DisplayRole){
        if(!musicList.isEmpty()){
            int row = index.row();
            FileInfo info = musicList.at(row);
            if(index.column() == 0){
                return info.title;
            }else if(index.column() == 1){
                return info.autor;
            }else if(index.column() == 2){
                return info.album;
            }else if(index.column() == 3){
                return info.time;
            }
        }
    }
    if (role == Qt::TextAlignmentRole){
            return int(Qt::AlignLeft | Qt::AlignVCenter);
    }
    return QVariant();
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal){
        if(role == Qt::DisplayRole){

            if (section == 0){
                return "音乐标题";
            }
            if (section == 1){
                return "歌手";
            }
            if (section == 2){
                return "专辑";
            }
            if (section == 3){
                return "时长";
            }
        }
    }
    else if (orientation == Qt::Vertical){
        if(role == Qt::DisplayRole){
             return section + 1;
        }
    }
    if (role == Qt::TextAlignmentRole){
            return int(Qt::AlignLeft | Qt::AlignVCenter);
    }
    return QVariant();
}
