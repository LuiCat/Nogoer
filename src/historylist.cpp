#include "historylist.h"

#include <QMap>

HistoryList::HistoryList(QObject* parent)
    :QAbstractListModel(parent)
{

}

void HistoryList::append(QString text, int stepNum, int gameNum)
{
    beginInsertRows(QModelIndex(), list.size(), list.size());

    ListData data={text, stepNum, gameNum};
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
        return QVariant::fromValue(&data);
    }
    return QVariant();
}

const HistoryList::ListData&HistoryList::getHistoryData(int row)
{
    return list.at(row);
}
