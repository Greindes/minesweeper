#ifndef MINEFIELDWIDGET_H
#define MINEFIELDWIDGET_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QHash>

//основной класс, отвечающий за ход игры
//и отображение игрового поля при помощи графической сцены
class MineFieldWidget : public QGraphicsView
{
    Q_OBJECT
private:

    enum states { OFF, PLAYING, FINISHED};
    enum icon  { NO_ICON, FLAG, OTHER  };

    states gameState;
    //QGraphicsScene * scene;
    int w, h, bombs, cellsLeftToOpen, flagsLeft;

    template<typename T>
    using Matrix = QVector<QVector<T>>;
    Matrix<int>   mineField;
    Matrix<icon>  cellIcons;
    Matrix<QGraphicsRectItem *> rects;
    QHash<QPair<int, int>, QGraphicsPixmapItem *> flags;

    void drawScene();
    //Отображает значение в заданной ячейке поля
    bool showCell(int i, int j, bool isRightCLick = false, bool ignoreFlag = false);
    //Отображает все бомбы
    void showAllBombs();
    //начинает игру (заполняет mineField)
    void startGame(int i, int j);
    void endGame();
protected:
    virtual void mouseReleaseEvent(QMouseEvent * ev) override;

public:
    MineFieldWidget(int width, int height, int bombs, QWidget * pwgt = nullptr);

    int getWidth() const;

    int getHeight() const;

    int getBombs() const;

signals:
    void gameStarted();
    void gameFinished(bool isWin);
    void flagUsed(int left);

public slots:
    void resetGame();
    void changeDifficulty(int width, int height, int bombAmount);
};

#endif // MINEFIELDWIDGET_H
