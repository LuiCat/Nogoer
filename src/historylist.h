#ifndef HISTORYLIST_H
#define HISTORYLIST_H

#include <QAbstractListModel>
#include <QList>

class HistoryList : public QAbstractListModel
{
    Q_OBJECT

public:

    explicit HistoryList(QObject* parent=0);

    int rowCount(const QModelIndex& parent) const;
    QVariant data(const QModelIndex& index, int role) const;

    inline HistoryList& operator<<(QString text)
    {
        append(text);
        return *this;
    }

public slots:

    void append(QString text, int moveNum=0, int historyID=-1);

private:

    struct ListData {
        QString text;
        int step;
        int game;
    };

    QList<ListData> list;

};

#endif // HISTORYLIST_H
