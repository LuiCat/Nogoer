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

ChessEngine::ChessEngine(const QString& pathExec, QObject* parent)
    :ChessEngine(parent)
{
    loadEngine(pathExec);
}

ChessEngine::~ChessEngine()
{
    unloadEngine();
}

bool ChessEngine::isAvailable()
{
    return process!=0;
}

bool ChessEngine::loadEngine(const QString& path)
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
    writeLine("name");
    emit nameChanged(engineName);
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
    emit nameChanged(getName());
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
    writeLine(first?"newblack":"newwhite");
}

void ChessEngine::writeMove(int x, int y)
{
    if(x<0||x>8||y<0||y>8)
        return;
    QByteArray l;
    switch(engineType)
    {
    default:
        l="move:00";
        l[5]='0'+x;
        l[6]='0'+y;
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
        qDebug()<<QString::fromLocal8Bit(line);
        if(!nameRead)
        {
            engineType=0;
            if(line.length()>0)
            {
                if(line.startsWith("name "))
                    engineName=QString::fromLocal8Bit(line.mid(5));
                else
                    engineName=QString::fromLocal8Bit(line);
                //qDebug()<<engineName;
                engineType=0;
            }
            else
            {
                engineName="Engine";
            }
            nameRead=true;
            emit nameChanged(engineName);
            return;
        }
        int x,y;
        switch(engineType)
        {
        default:
            if(line.startsWith("move"))
            {
                x=line[5]-'0';
                y=line[6]-'0';
                if(x<0||x>8||y<0||y>8)
                    break;
                emit moveChess(x, y);
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

