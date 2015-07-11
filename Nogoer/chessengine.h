#ifndef CHESSENGINE_H
#define CHESSENGINE_H

#include <QProcess>

class ChessEngine : public QObject
{
    Q_OBJECT

public:

    explicit ChessEngine(QObject *parent = 0);

    bool isAvailable();

    bool loadEngine(QString path);
    bool reloadEngine();
    void unloadEngine();

    void writeLine(const QByteArray& line);

    QString getName();

    void setMoveOrder(bool isFirst);

public slots:

    void start();
    void writeMove(int x, int y);

signals:

    void moveChess(int x, int y);
    void nameChanged(QString name=QString());
    void engineExited(bool isCrashed);

protected slots:

    void dealLine();
    void dealError(QProcess::ProcessError e);
    void dealExit(int exitCode, QProcess::ExitStatus exitStatus);

private:

    QString execPath;

    QProcess* process;

    QString engineName;
    bool nameRead;

    int engineType;

    bool first;

};

#endif // CHESSENGINE_H
