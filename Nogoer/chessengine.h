#ifndef CHESSENGINE_H
#define CHESSENGINE_H

#include <QProcess>

const int idx[]={1,1,1,1,1,1,3,3,3,3,3,3,5,5,5,5,5,5,7,7,7,7,7,7,9,9,9,9,9,9,0,2,4,6,8,10,0,2,4,6,8,10,0,2,4,6,8,10,0,2,4,6,8,10,0,2,4,6,8,10};
const int idy[]={0,2,4,6,8,10,0,2,4,6,8,10,0,2,4,6,8,10,0,2,4,6,8,10,0,2,4,6,8,10,1,1,1,1,1,1,3,3,3,3,3,3,5,5,5,5,5,5,7,7,7,7,7,7,9,9,9,9,9,9};

const int xyid[11][11]={{-1,30,-1,36,-1,42,-1,48,-1,54,-1},
                        { 0,-1, 1,-1, 2,-1, 3,-1, 4,-1, 5},
                        {-1,31,-1,37,-1,43,-1,49,-1,55,-1},
                        { 6,-1, 7,-1, 8,-1, 9,-1,10,-1,11},
                        {-1,32,-1,38,-1,44,-1,50,-1,56,-1},
                        {12,-1,13,-1,14,-1,15,-1,16,-1,17},
                        {-1,33,-1,39,-1,45,-1,51,-1,57,-1},
                        {18,-1,19,-1,20,-1,21,-1,22,-1,23},
                        {-1,34,-1,40,-1,46,-1,52,-1,58,-1},
                        {24,-1,25,-1,26,-1,27,-1,28,-1,29},
                        {-1,35,-1,41,-1,47,-1,53,-1,59,-1}};

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
    void writeMove(int move);

signals:

    void moveChess(int move);
    void nameChanged();
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
