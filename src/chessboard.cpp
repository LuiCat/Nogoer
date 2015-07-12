#include "chessboard.h"

#include <cstring>
#include <QQueue>

ChessBoard::ChessBoard(QObject *parent)
    :QObject(parent)
    ,gameFinished(false)
    ,nextStepNum(1)
{
    memset(grid, 0, sizeof(grid));
    memset(step, 0, sizeof(step));
}

void ChessBoard::reset()
{
    memset(grid, 0, sizeof(grid));
    memset(step, 0, sizeof(step));
    gameFinished=false;
    nextStepNum=1;
}

ChessBoard::ChessType ChessBoard::getGrid(int x, int y) const
{
    if(x<0||x>=B_HEIGHT||y<0||y>=B_WIDTH)
        return invalid;
    return grid[x][y];
}

int ChessBoard::getStep(int x, int y) const
{
    if(x<0||x>=B_HEIGHT||y<0||y>=B_WIDTH)
        return 0;
    return step[x][y];
}

void ChessBoard::setGrid(int x, int y, ChessBoard::ChessType chess, int stepNum)
{
    if(x<0||x>B_HEIGHT||y<0||y>B_WIDTH)
        return;
    grid[x][y]=chess;
    step[x][y]=stepNum;
}

int ChessBoard::countLife(int x, int y) const
{
    bool mark[B_HEIGHT][B_WIDTH];
    memset(mark, 0, sizeof(mark));

    if(!isChess(x, y))
        return false;

    int i,tx,ty,res=0;
    ChessType t;
    ChessType chess=getGrid(x, y);
    QQueue<Grid> q;

    q.push_back(Grid(x, y));
    mark[x][y]=true;
    while(!q.empty())
    {
        const Grid& g=q.front();
        for(i=0;i<4;++i)
        {
            tx=g.x+dx[i];
            ty=g.y+dy[i];
            t=getGrid(tx, ty);
            if(t==invalid||mark[tx][ty])
                continue;
            mark[tx][ty]=true;
            if(t==chess)
            {
                q.push_back(Grid(tx, ty));
            }
            else if(t==empty)
            {
                ++res;
            }
        }
        q.pop_front();
    }

    return res;
}

bool ChessBoard::checkMove(int x, int y, bool isBlack) const
{
    if(!isEmpty(x, y))
        return false;
    const ChessType oppoChess=(isBlack?white:black);
    bool flagKO=false,flagSC=true;
    int tx,ty,i;
    ChessType t;
    for(i=0;i<4;++i)
    {
        tx=x+dx[i];
        ty=y+dy[i];
        t=getGrid(tx, ty);
        if(t==invalid)
            continue;
        if(t==oppoChess)
        {
            if(countLife(tx, ty)==1)
            {
                flagKO=true;
                break;
            }
        }
        else if(flagSC&&(t==empty||countLife(tx, ty)>1))
        {
            flagSC=false;
        }
    }
    return !(flagKO||flagSC);
}

bool ChessBoard::isFinished() const
{
    return gameFinished;
}

bool ChessBoard::checkFinished(bool checkBlack)
{
    int i, j;

    for(i=0;i<B_HEIGHT;++i)
        for(j=0;j<B_WIDTH;++j)
            if(isEmpty(i, j)&&checkMove(i, j, checkBlack))
                return false;

    return true;
}

bool ChessBoard::doChess(int x, int y, bool isBlack)
{
    if(!isEmpty(x, y)||gameFinished)
        return false;
    setGrid(x, y, isBlack?black:white, nextStepNum);
    ++nextStepNum;
    return true;
}

