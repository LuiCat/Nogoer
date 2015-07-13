#ifndef HISTORYITEM_H
#define HISTORYITEM_H

#include <QAbstractItemDelegate>

class HistoryItem : public QAbstractItemDelegate
{
    Q_OBJECT

public:

    explicit HistoryItem(QObject* parent=0);

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;

public slots:

    void setCurrentIndex(const QModelIndex& index);

private:

    int infoHeight;
    int moveHeight;

    QImage imgBlack;
    QImage imgWhite;

    QModelIndex currentIndex;

    static QPainterPath makePath(const QSizeF& size, double radiusArc);

};

#endif // HISTORYITEM_H
