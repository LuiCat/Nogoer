#include "historyitem.h"
#include "historylist.h"

#include <QPainter>
#include <QtMath>

HistoryItem::HistoryItem(QObject* parent)
    :QAbstractItemDelegate(parent)
    ,infoHeight(16)
    ,moveHeight(22)
{
    imgBlack.load("data/chessblack.png");
    imgWhite.load("data/chesswhite.png");
}

void HistoryItem::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    painter->save();
    const HistoryList::ListData* data=index.data().value<const HistoryList::ListData*>();
    if(!data) return;
    if(data->game>0)
    {
        bool isBlack=data->step%2;
        if(option.state&QStyle::State_HasFocus)
        {
            painter->fillRect(option.rect, "#FFDD88");
        }
        painter->translate(option.rect.topLeft());
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        painter->setBrush(QBrush(isBlack?"#EECCFF":"#FFFFCC"));
        if(option.state&QStyle::State_HasFocus)
            painter->setPen("red");
        QRectF rect;
        if(isBlack)
        {
            painter->translate(moveHeight*1.5, moveHeight*0.5);
            rect=QRectF(moveHeight*1.5, 1, option.rect.width()-moveHeight*2.0, moveHeight-2);
        }
        else
        {
            painter->translate(option.rect.width()-moveHeight*1.5, moveHeight*0.5);
            painter->rotate(180);
            rect=QRectF(moveHeight*0.5, 1, option.rect.width()-moveHeight*2.0, moveHeight-2);
        }
        painter->drawPath(makePath(rect.size(), 6));
        painter->restore();
        painter->drawText(rect, Qt::AlignCenter, data->text);
        if(isBlack)
        {
            QRectF rch(0, 0, moveHeight, moveHeight);
            painter->drawImage(rch, imgBlack);
            painter->setPen("white");
            painter->drawText(rch, Qt::AlignCenter, QString("%0").arg(data->step));
        }
        else
        {
            QRectF rch(option.rect.width()-moveHeight, 0, moveHeight, moveHeight);
            painter->drawImage(rch, imgWhite);
            painter->setPen("black");
            painter->drawText(rch, Qt::AlignCenter, QString("%0").arg(data->step));
        }
    }
    else
    {
        painter->fillRect(option.rect.marginsRemoved(QMargins(1, 1, 1, 1)), data->step==0?"#CCFFCC":"#FFCCCC");
        painter->drawText(option.rect.translated(0, -1), Qt::AlignHCenter, data->text);
    }
    painter->restore();
}

QSize HistoryItem::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    const HistoryList::ListData* data=index.data().value<const HistoryList::ListData*>();
    if(!data) return QSize();
    return QSize(option.rect.width(), data->game>0?moveHeight:infoHeight);
}

QPainterPath HistoryItem::makePath(const QSizeF& size, double radiusArc)
{
    QPainterPath path;
    double height=size.height()*0.5;
    double width=size.width();
    double hsr=radiusArc*0.5*qSqrt(2);
    path.moveTo(-height, 0);
    path.lineTo(-hsr, height-hsr);
    path.quadTo(0, height, radiusArc, height);
    path.lineTo(width-radiusArc, height);
    path.quadTo(width, height, width, height-radiusArc);
    path.lineTo(width, radiusArc-height);
    path.quadTo(width, -height, width-radiusArc, -height);
    path.lineTo(radiusArc, -height);
    path.quadTo(0, -height, -hsr, hsr-height);
    path.lineTo(-height, 0);
    path.addRoundRect(-height*0.5, -height*0.2, height*0.4, height*0.4, 100, 100);
    return path;
}
