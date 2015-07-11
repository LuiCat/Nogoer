#include "chessengine.h"

#include <QDebug>
#include <QtWidgets/QMessageBox>
ChessEngine::ChessEngine(QObject *parent)
    :QObject(parent)
    ,process(0)
    ,engineType(0)
    ,first(true)
{

}

bool ChessEngine::isAvailable()
{
    return process!=0;
}

bool ChessEngine::loadEngine(QString path)
{
    execPath=path;
    return reloadEngine();
}

bool ChessEngine::reloadEngine()
{
    unloadEngine();
    process=new QProcess(this);
    connect(process, SIGNAL(readyRead()), this, SLOT(dealLine()));
    connect(process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(dealError(QProcess::ProcessError)));
    connect(process, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(dealExit(int,QProcess::ExitStatus)));
    process->start(execPath, QStringList());
    if(isAvailable()&&!process->isOpen())
    {
        QMessageBox::critical(dynamic_cast<QWidget*>(parent()->parent()), "", "Failed when openning pipe");
        unloadEngine();
        return false;
    }
    engineName="Engine";
    nameRead=false;
    writeLine("name?");
    emit nameChanged();
    //engineType=first?2:1;
    return true;
}

void ChessEngine::unloadEngine()
{
    if(process)
    {
        disconnect(process, SIGNAL(readyRead()), this, SLOT(dealLine()));
        disconnect(process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(dealError(QProcess::ProcessError)));
        disconnect(process, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(dealExit(int,QProcess::ExitStatus)));
        process->deleteLater();
        process->waitForFinished(500);
        delete process;
        process=0;
    }
    emit nameChanged();
}

void ChessEngine::writeLine(const QByteArray &line)
{
    if(!isAvailable())return;
    process->write(line);
    process->write("\n");
    //qDebug()<<line;
}

QString ChessEngine::getName()
{
    if(!isAvailable())
        return "Manual";
    return engineName;
}

void ChessEngine::setMoveOrder(bool isFirst)
{
    first=isFirst;
}

void ChessEngine::start()
{
    if(!isAvailable())
        return;
    writeLine("new");
    writeLine(first?"first":"second");
}

void ChessEngine::writeMove(int move)
{
    QByteArray l;
    switch(engineType)
    {
    case 1:
        l="F=AA\nOK";
        l[0]=(first?'S':'F');
        l[2]='A'+idx[move];
        l[3]='A'+idy[move];
        writeLine(l);
        break;
    case 2:
        writeLine(QString("#%0").arg(move).toLatin1());
        break;
    default:
        l="move AA";
        l[5]='A'+idx[move];
        l[6]='A'+idy[move];
        writeLine(l);
        break;
    }
}

void ChessEngine::dealLine()
{
    if(!isAvailable())return;
    while(process->canReadLine())
    {
        QByteArray line=process->readLine();
        if(line.endsWith('\n'))
            line.chop(1);
        if(line.endsWith('\r'))
            line.chop(1);
        qDebug()<<line;
        if(!nameRead)
        {
            engineType=0;
            if(line.length()>0)
            {
                if(line.startsWith("name "))
                    engineName=line.mid(5);
                else
                    engineName=line;
                //qDebug()<<engineName;
                engineType=0;
                if(engineName.startsWith("DOGE", Qt::CaseInsensitive))
                {
                    engineType=1;
                }
                else if(engineName.startsWith("Too Young", Qt::CaseInsensitive))
                {
                    engineType=1;
                }
                else if(engineName.startsWith("Dotsole", Qt::CaseInsensitive))
                {
                    engineType=2;
                }
            }
            else
            {
                engineName="Engine";
            }
            nameRead=true;
            emit nameChanged();
            return;
        }
        int move=-1,x,y;
        bool flag;
        switch(engineType)
        {
        case 1:
            if(line[1]=='=')
            {
                x=line[2]-'A';
                y=line[3]-'A';
                if(x>=0&&x<11&&y>=0&&y<11)
                move=xyid[x][y];
                process->write("OK\n");
                emit moveChess(move);
            }
            break;
        case 2:
            x=0;
            while(x=line.indexOf('#',x),x!=-1)
            {
                ++x;
                y=line.indexOf(' ',x);
                move=line.mid(x,y-x).toInt(&flag);
                emit moveChess(move);
            }
        default:
            if(line.startsWith("move "))
            {
                x=line[5]-'A';
                y=line[6]-'A';
                if(x>=0&&x<11&&y>=0&&y<11)
                move=xyid[x][y];
                emit moveChess(move);
            }
            break;
        }
    }
}

void ChessEngine::dealError(QProcess::ProcessError e)
{
    QMessageBox::critical(dynamic_cast<QWidget*>(parent()->parent()), "", QString("Failed in loading engine, error code: %1").arg(e));
    unloadEngine();
}

void ChessEngine::dealExit(int exitCode, QProcess::ExitStatus exitStatus)
{
    bool b=(exitStatus==QProcess::CrashExit);
    if(b)QMessageBox::critical(dynamic_cast<QWidget*>(parent()->parent()), "", QString("Engine crashed with exit code: %1").arg(exitCode));
    emit engineExited(b);
}

