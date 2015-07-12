#include "historyitem.h"
#include "historylist.h"

#include <QPainter>

HistoryItem::HistoryItem(QObject* parent)
    :QAbstractItemDelegate(parent)
    ,infoHeight(20)
    ,moveHeight(30)
{

}

void HistoryItem::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    const HistoryList::ListData* data=index.data().value<const HistoryList::ListData*>();
    if(!data)
        return;
    painter->fillRect(option.rect.marginsRemoved(QMargins(1, 1, 1, 1)), "#CCFFCC");
    painter->drawText(option.rect.marginsRemoved(QMargins(2, 2, 2, 2)),
                       Qt::AlignCenter, data->text);
}

QSize HistoryItem::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    const HistoryList::ListData* data=index.data().value<const HistoryList::ListData*>();
    if(!data)
        return QSize();
    return QSize(option.rect.width(), data->step>0?moveHeight:infoHeight);
}
