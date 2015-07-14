#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QObject>

#include "commondef.h"

class ChessBoard : public QObject
{
    Q_OBJECT

public:

    enum ChessType
    {
        invalid=-1, empty=0, black, white
    };

    explicit ChessBoard(QObject *parent = 0);

    void reset();

    ChessType getGrid(int x, int y) const;
    int getStep(int x, int y) const;
    void setGrid(int x, int y, ChessType chess, int stepNum=0);

    inline bool isEmpty(int x, int y) const
    {
        return getGrid(x, y)==empty;
    }

    inline bool isChess(int x, int y) const
    {
        ChessType type=getGrid(x, y);
        return type==black||type==white;
    }

    /// Count the life of the block at specified position, returns 0 if no chess is on the position
    int countLife(int x, int y) const;

    /// Check whether the move is possible (not KO or suicide)
    bool checkMove(int x, int y, bool isBlack) const;

    int getMoveNum(bool isBlack);

    void setFinished();
    bool isFinished() const;
    bool checkFinished(bool checkBlack);

public slots:

    bool doChess(int x, int y, bool isBlack);

signals:

    void finished(bool isBlackWin);

private:

    struct Grid
    {
        int x, y;
        Grid(int _x=0, int _y=0) : x(_x), y(_y)
        {

        }
    };

    ChessType grid[B_HEIGHT][B_WIDTH];
    bool gameFinished;

    int step[B_HEIGHT][B_WIDTH];
    int nextStepNum;

};

#endif // CHESSBOARD_H
