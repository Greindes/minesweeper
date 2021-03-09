#include "MainWindow.h"
#include "ui_mainwindow.h"
#include <MineFieldWidget.h>
#include <QTimer>
#include <QTime>
#include "CustomDifficulyDialog.h"

#include <QGuiApplication>
#include <QScreen>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), timer(nullptr), timeTemplate("%1")
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowCloseButtonHint);
    ui->resetButton->setIcon(QIcon(":/icons/playing.png"));
    mineField = new MineFieldWidget(30, 16, 100, this);
    timer = new QTimer(this);
    ui->lcdBombs->display("100");
    ui->lcdTime->display("000");
    ui->centralWidget->layout()->addWidget(mineField);
    connect(mineField, SIGNAL(gameStarted()), SLOT(slotStartGame()));
    connect(mineField, SIGNAL(gameFinished(bool)), SLOT(slotEndGame(bool)));
    connect(ui->resetButton, SIGNAL(clicked()), mineField, SLOT(resetGame()));
    connect(ui->resetButton, SIGNAL(clicked()), SLOT(slotResetTimer()));
    connect(mineField, SIGNAL(flagUsed(int)), ui->lcdBombs, SLOT(display(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::slotStartGame()
{
    ui->resetButton->setIcon(QIcon(":/icons/playing.png"));
    cloak.start();
    connect(timer, SIGNAL(timeout()), SLOT(slotUpdateTime()));
    timer->start(200);
    slotUpdateTime();
}

void MainWindow::slotUpdateTime()
{
    ui->lcdTime->display(timeTemplate.arg(cloak.elapsed() / 1000, 3, 10, QChar('0')));
}

void MainWindow::slotEndGame(bool isWin)
{
    //ui->lcdTime->display("000");
    timer->stop();
    if (isWin)
        ui->resetButton->setIcon(QIcon(":/icons/win.png"));
    else
        ui->resetButton->setIcon(QIcon(":/icons/lost.png"));
}

void MainWindow::slotResetTimer()
{
    ui->resetButton->setIcon(QIcon(":/icons/playing.png"));
    timer->stop();
    ui->lcdTime->display("000");
}

//подгоняет размер окна под размер содержимого
void MainWindow::minimizeWindowSize()
{
    resize(minimumSizeHint());
}

void MainWindow::on_action_easy_triggered()
{
    changeGameDifficulty(9, 9, 10);
}

void MainWindow::on_action_medium_triggered()
{
    changeGameDifficulty(16, 16, 40);
}

void MainWindow::on_action_hard_triggered()
{
    changeGameDifficulty(30, 16, 99);
}

void MainWindow::changeGameDifficulty(int w, int h, int bombAmount)
{
    mineField->changeDifficulty(w, h, bombAmount);
    slotResetTimer();
    QTimer::singleShot(50, this, SLOT(refleshGeometry()));
    //альтернатива:
    /*for (int i = 0; i < 10; ++i)
        QApplication::processEvents();
    minimizeWindowSize();*/
}

void MainWindow::refleshGeometry()
{
    minimizeWindowSize();
    QSize screenSize = QGuiApplication::primaryScreen()->availableSize();
    int xPos = std::min(x(), screenSize.width() - frameGeometry().width());
    int yPos = std::min(y(), screenSize.height() - frameGeometry().height());
    move(xPos, yPos);


}

void MainWindow::on_action_custom_triggered()
{
    CustomDifficultyDialog * dlg = new CustomDifficultyDialog(mineField->getWidth(),
                                                              mineField->getHeight(),
                                                              mineField->getBombs());
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    connect(dlg, SIGNAL(difMade(int, int, int)), SLOT(changeGameDifficulty(int, int, int)));
    dlg->open();
}
