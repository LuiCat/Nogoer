#include "historylist.h"

#include <QMap>

HistoryList::HistoryList(QObject* parent)
    :QAbstractListModel(parent)
{

}

void HistoryList::append(QString text, int moveNum, int historyID)
{
    beginInsertRows(QModelIndex(), list.size(), list.size());

    ListData data={text, moveNum, historyID};

    list.append(data);

    endInsertRows();
}

int HistoryList::rowCount(const QModelIndex&) const
{
    return list.size();
}

QVariant HistoryList::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (index.row()<0||index.row()>=list.size())
        return QVariant();
    if (role == Qt::DisplayRole)
    {
        const ListData& data=list.at(index.row());
        QMap<QString, QVariant> map;
        map.insert("text", QVariant(data.text));
        map.insert("step", QVariant(data.step));
        map.insert("game", QVariant(data.game));
        return QVariant(map);
    }
    return QVariant();
}