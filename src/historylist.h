#ifndef HISTORYLIST_H
#define HISTORYLIST_H

#include <QAbstractListModel>
#include <QList>

class HistoryList : public QAbstractListModel
{
    Q_OBJECT

public:

    struct ListData {
        QString text;
        int step;
        int game;
    };

    explicit HistoryList(QObject* parent=0);

    int rowCount(const QModelIndex& parent) const;
    QVariant data(const QModelIndex& index, int role) const;

    inline HistoryList& operator<<(QString text)
    {
        append(text);
        return *this;
    }

public slots:

    void append(QString text, int stepNum=0, int gameNum=0);

private:

    QList<ListData> list;

};

Q_DECLARE_METATYPE(const HistoryList::ListData*)

#endif // HISTORYLIST_H
