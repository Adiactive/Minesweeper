#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    gameRow = game->getRow();
    gameCol = game->getCol();
    setFixedSize(gameCol * blockSize + offsetX * 2, gameRow * blockSize + offsetY * 2 + spaceY);
    timer = new QTimer(this);
    timer->start(1000);
    clock = new QTime(0,0,0);
    connect(timer,SIGNAL(timeout()),this,SLOT(updateTime()));
    connect(ui->actionEasy,SIGNAL(triggered(bool)),this,SLOT(startEasyGame()));
    connect(ui->actionMedium,SIGNAL(triggered(bool)),this,SLOT(startMediumGame()));
    connect(ui->actionHard,SIGNAL(triggered(bool)),this,SLOT(startHardGame()));
    connect(ui->actionQuit,SIGNAL(triggered(bool)),this,SLOT(close()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::restart(int theRow, int theCol, int theTotalBomb)
{
    delete game;
    game = new block(theRow,theCol,theTotalBomb);
    gameRow = theRow;gameCol = theCol;
    update();
    loseFlag = 0;
    winFlag = 0 ;
    ui->label_2->setVisible(true);
    clock->setHMS(0,0,0);
}

void MainWindow::updateTime()
{
    *clock = clock->addSecs(1);
    ui->label_2->setText(clock->toString("hh:mm:ss"));
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPixmap pngBlockDouble(":/images/block_double.png");
    QPixmap pngBlockNone(":/images/block_none.png");
    QPixmap pngRedFlag(":/images/redFlag.png");
    QPixmap pngBomb(":/images/bomb.png");
    QPixmap pngOne(":/images/1.png");QPixmap pngTwo(":/images/2.png");QPixmap pngThr(":/images/3.png");
    QPixmap pngFor(":/images/4.png");QPixmap pngFiv(":/images/5.png");QPixmap pngSix(":/images/6.png");
    QPixmap pngSvn(":/images/7.png");QPixmap pngEight(":/images/8.png");
    pngBlockDouble = pngBlockDouble.scaled(QSize(blockSize,blockSize));
    pngBlockNone = pngBlockNone.scaled(QSize(blockSize,blockSize));
    pngRedFlag = pngRedFlag.scaled(QSize(blockSize,blockSize));
    pngBomb = pngBomb.scaled(QSize(blockSize,blockSize));
    pngOne = pngOne.scaled(QSize(blockSize,blockSize));
    pngTwo = pngTwo.scaled(QSize(blockSize,blockSize));
    pngThr = pngThr.scaled(QSize(blockSize,blockSize));
    pngFor = pngFor.scaled(QSize(blockSize,blockSize));
    pngFiv = pngFiv.scaled(QSize(blockSize,blockSize));
    pngSix = pngSix.scaled(QSize(blockSize,blockSize));
    pngSvn = pngSvn.scaled(QSize(blockSize,blockSize));
    pngEight = pngEight.scaled(QSize(blockSize,blockSize));

    if (game->checkGame() == WIN && !winFlag){
        QString str = "你赢了！\n";
        str += "共用时：";
        str += clock->toString("hh:mm:ss");
        QMessageBox::about(this,"warning",str);
        winFlag = 1;
    }
    for(int i=0;i<gameRow;++i)
    {
        for(int j=0;j<gameCol;++j)
        {
            unit &curUnit = game->map[i][j];
            switch(curUnit.CurrentState)
            {
            case UNDIGGED:
                if(game->checkGame()==LOSE&&curUnit.isBomb==1)
                    painter.drawPixmap(j*blockSize+offsetX,i*blockSize+offsetY+spaceY,pngBomb);
                else
                    painter.drawPixmap(j*blockSize+offsetX,i*blockSize+offsetY+spaceY,pngBlockDouble);
                break;
            case DIGGED:
            {
                if(curUnit.isBomb==0)
                {
                    switch(curUnit.bombNum)
                    {
                    case 0:
                        painter.drawPixmap(j*blockSize+offsetX,i*blockSize+offsetY+spaceY,pngBlockNone);
                        break;
                    case 1:
                        painter.drawPixmap(j*blockSize+offsetX,i*blockSize+offsetY+spaceY,pngOne);
                        break;
                    case 2:
                        painter.drawPixmap(j*blockSize+offsetX,i*blockSize+offsetY+spaceY,pngTwo);
                        break;
                    case 3:
                        painter.drawPixmap(j*blockSize+offsetX,i*blockSize+offsetY+spaceY,pngThr);
                        break;
                    case 4:
                        painter.drawPixmap(j*blockSize+offsetX,i*blockSize+offsetY+spaceY,pngFor);
                        break;
                    case 5:
                        painter.drawPixmap(j*blockSize+offsetX,i*blockSize+offsetY+spaceY,pngFiv);
                        break;
                    case 6:
                        painter.drawPixmap(j*blockSize+offsetX,i*blockSize+offsetY+spaceY,pngSix);
                        break;
                    case 7:
                        painter.drawPixmap(j*blockSize+offsetX,i*blockSize+offsetY+spaceY,pngSvn);
                        break;
                    case 8:
                        painter.drawPixmap(j*blockSize+offsetX,i*blockSize+offsetY+spaceY,pngEight);
                        break;
                    }
                }
                else
                {
                    if (!loseFlag){
                        for(int i=0;i<gameRow;++i)
                        {
                            for(int j=0;j<gameCol;++j)
                            {
                                unit &curUnit = game->map[i][j];
                                if (curUnit.CurrentState != REDFLAG && curUnit.isBomb)
                                    game->restBomb++;
                            }
                        }
                        qDebug() << "you lose" << endl;
                        QString str = "你输了！\n";
                        str += "共用时：";
                        str += clock->toString("hh:mm:ss");
                        str += "\n剩余雷数：";
                        str += QString::number(game->restBomb-1);
                        QMessageBox::about(this,"warning",str);
                        painter.drawPixmap(j*blockSize+offsetX,i*blockSize+offsetY+spaceY,pngBomb);
                        loseFlag = 1;
                        ui->label_2->setVisible(false);
                    }
                }
            }
                break;
            case REDFLAG:
                painter.drawPixmap(j*blockSize+offsetX,i*blockSize+offsetY+spaceY,pngRedFlag);
                break;
            default:
                break;
            }
        }
    }

}

void MainWindow::mousePressEvent(QMouseEvent *event)
{

    if(game->checkGame()==PLAYING)
    {
        int px = event->x() - offsetX;int py = event->y() - offsetY - spaceY;
        int row = py/blockSize; int col = px/blockSize;
        switch(event->button())
        {
        case Qt::LeftButton:
            game->dig(row,col);
            update();
            break;
        case Qt::RightButton:
            game->mark(row,col);
            update();
            break;
        default:
            break;
        }
    }
}

void MainWindow::startEasyGame()
{
    restart(10,10,15);
    setFixedSize(gameCol * blockSize + offsetX * 2, gameRow * blockSize + offsetY * 2 + spaceY);
}

void MainWindow::startMediumGame()
{
    restart(20,20,60);
    setFixedSize(gameCol * blockSize + offsetX * 2, gameRow * blockSize + offsetY * 2 + spaceY);
}

void MainWindow::startHardGame()
{
    restart(30,30,135);
    setFixedSize(gameCol * blockSize + offsetX * 2, gameRow * blockSize + offsetY * 2 + spaceY);
}




