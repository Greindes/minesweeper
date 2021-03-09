#include "MineFieldWidget.h"
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include "MineFieldGenerator.h"

#include <QDebug>

#include <QGuiApplication>
#include <QScreen>


//размер стороны клетки/кнопки
static const int SIDE_SIZE = 16;
//дистанция между левым верхним углом двух соседних клеток/кнопок
static const int CELL_SIZE = SIDE_SIZE + 2;

static const int SIDES[8][2] = {{-1, -1}, {-1, 0 }, {-1, 1}, {0, -1},
                                {0 , 1 }, {1 , -1}, {1 , 0}, {1, 1}};


MineFieldWidget::MineFieldWidget(int width, int height, int bombAmount, QWidget *pwgt /*=nullptr*/)
    : QGraphicsView(pwgt), gameState(OFF), w(width), h(height),
      bombs(bombAmount), flagsLeft(bombAmount)
{
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setScene(new QGraphicsScene(this));
    scene()->setItemIndexMethod(QGraphicsScene::NoIndex);
    drawScene();
    setStyleSheet("background-color: #d0d0d0; border: 1px solid black");
    cellsLeftToOpen = h * w - bombs;
}

//изменяет сложность игры, размер игрового поля ограничен разрешением экрана
//и не может быть меньше 7х7
void MineFieldWidget::changeDifficulty(int width, int height, int bombAmount)
{
    QSize screenSize = QGuiApplication::primaryScreen()->availableSize();
    width = std::max(width, 7);
    width = std::min(width, (screenSize.width() - 6) / CELL_SIZE);
    height = std::max(height, 7);
    height = std::min(height, (screenSize.height() - 80) / CELL_SIZE);
    bombAmount = std::min(width * height - 1, bombAmount);
    bombAmount = std::max(bombAmount, 2);
    bombs = bombAmount;
    //перерисовка поля выполняется только если изменились его размеры
    if (w != width || h != height) {
        w = width;
        h = height;
        drawScene();
    }
    resetGame();
}


int MineFieldWidget::getWidth() const
{
    return w;
}

int MineFieldWidget::getHeight() const
{
    return h;
}

int MineFieldWidget::getBombs() const
{
    return bombs;
}

//метод для отрисовки игрового поля
void MineFieldWidget::drawScene()
{
    scene()->clear();
    scene()->setSceneRect(0, 0, w * CELL_SIZE + 6, h * CELL_SIZE + 6);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setMinimumSize(w * CELL_SIZE + 6, h * CELL_SIZE + 6);
    setMaximumSize(w * CELL_SIZE + 6, h * CELL_SIZE + 6);

    cellIcons = Matrix<icon>(h, QVector<icon>(w, NO_ICON));
    rects = Matrix<QGraphicsRectItem *>(h, QVector<QGraphicsRectItem*>(w, nullptr));

    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j)
            rects[i][j] = scene()->addRect(QRect(j*CELL_SIZE + 2, i*CELL_SIZE + 2, SIDE_SIZE, SIDE_SIZE));
    }
    //emit sizeChanged();
}


//Отображает значение в заданной ячейке поля (клик игрока)
bool MineFieldWidget::showCell(int i, int j, bool isRightCLick /*= false*/,
                               bool ignoreFlag /*= false*/)
{
    if (i < 0 || i >= h || j < 0 || j >= w || cellIcons[i][j] == OTHER)
        return true;
    //убирание флага
    if (cellIcons[i][j] == FLAG) {
        if (!isRightCLick && !ignoreFlag)
            return true;
        cellIcons[i][j] = NO_ICON;
        if (flags.count({i, j})) {
            scene()->removeItem(flags[{i, j}]);
            flags.remove({i, j});
        }
        emit flagUsed(++flagsLeft);
        if (ignoreFlag)
            showCell(i, j);
    } else {
        cellIcons[i][j] = isRightCLick ? FLAG : OTHER;
        //установка флага
        if (isRightCLick) {
            emit flagUsed(--flagsLeft);
            flags[{i, j}] = scene()->addPixmap(QPixmap(":/icons/flag.png"));
            flags[{i, j}]->setPos(j * CELL_SIZE + 2, i * CELL_SIZE + 2);
        }
        //отображение соотвествующего значения из mineField
        else {
            if (mineField[i][j] == 0) {
                rects[i][j]->setBrush(QBrush(Qt::white));
                for (int k = 0; k < 8; ++k)
                    showCell(i + SIDES[k][0], j + SIDES[k][1], false, true);
            } else if (mineField[i][j] > 0) {
                //устанавливаем число
                auto * p = scene()->addPixmap(QPixmap(QString(":/icons/%1.png").arg(mineField[i][j])));
                p->setPos(j * CELL_SIZE + 2, i * CELL_SIZE + 2);
            } else {
                //показываем бомбу
                auto * p = scene()->addPixmap(QPixmap(":/icons/bomb.png"));
                p->setPos(j * CELL_SIZE + 2, i * CELL_SIZE + 2);
                if (gameState == PLAYING)
                    rects[i][j]->setBrush(Qt::red);
                return false;
            }
            --cellsLeftToOpen;
        }
    }

    return cellsLeftToOpen > 0;
}

//отображает все бомбы - поражение
void MineFieldWidget::showAllBombs()
{
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            if (mineField[i][j] == -1 && cellIcons[i][j] == NO_ICON)
                showCell(i, j);
            else if (mineField[i][j] != -1 && cellIcons[i][j] == FLAG) {
                auto * p = scene()->addPixmap(QPixmap(":/icons/bomb.png"));
                p->setPos(j * CELL_SIZE + 2, i * CELL_SIZE + 2);
                scene()->addLine(QLineF(rects[i][j]->rect().topLeft(), rects[i][j]->rect().bottomRight()), QPen(Qt::red, 1));
                scene()->addLine(QLineF(rects[i][j]->rect().bottomLeft(), rects[i][j]->rect().topRight()), QPen(Qt::red, 1));
            }
        }
    }
}


void MineFieldWidget::startGame(int i, int j)
{
    mineField = MineFieldGenerator::generateCellVals(h, w, bombs, {i, j});
    cellsLeftToOpen = h * w - bombs;
    gameState = PLAYING;
    //сигнал о начале игры (для запуска счетчика времени игры)
    emit gameStarted();
}

void MineFieldWidget::endGame()
{
    gameState = FINISHED;
    bool isWin = cellsLeftToOpen <= 0;
    if (!isWin)
        showAllBombs();
    emit gameFinished(isWin);
}

//открывает указанную ячейку по отпусканию пользователем кнопки мыши
//если игра ещё не закончена
void MineFieldWidget::mouseReleaseEvent(QMouseEvent *ev)
{
    if (gameState == FINISHED)
        return;
    if (ev->button() == Qt::LeftButton || ev->button() == Qt::RightButton) {
        //определяет индекс ячейки из координат клика и размеров ячеек
        auto xy = ev->pos();
        int i = (xy.y() - 2) / CELL_SIZE;
        int j = (xy.x() - 2) / CELL_SIZE;
        bool isRight = ev->button() == Qt::RightButton;
        if (gameState == OFF && !isRight)
            startGame(i, j);
        if (!showCell(i, j, isRight))
            endGame();
    }
}



void MineFieldWidget::resetGame()
{
    //отчистка ячеек
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            rects[i][j]->setBrush(QBrush());
            cellIcons[i][j] = NO_ICON;
        }
    }
    for (QGraphicsItem * item : scene()->items()) {
        QGraphicsPixmapItem * pi = qgraphicsitem_cast<QGraphicsPixmapItem *> (item);
        if (pi)
            scene()->removeItem(pi);
        else {
            QGraphicsLineItem * li = qgraphicsitem_cast<QGraphicsLineItem *> (item);
            if (li)
                scene()->removeItem(li);
        }
    }
    flags.clear();
    flagsLeft = bombs;
    emit flagUsed(flagsLeft);
    gameState = OFF;

}



