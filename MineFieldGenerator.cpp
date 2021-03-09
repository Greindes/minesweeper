#include "MineFieldGenerator.h"
#include <time.h>
#include <QDebug>

QVector<QVector<bool> > MineFieldGenerator::getField() const
{
    return field;
}

QSet<QPair<int, int> > MineFieldGenerator::getMines() const
{
    return mines;
}


int MineFieldGenerator::getBombAmount() const
{
    return bombAmount;
}


MineFieldGenerator::MineFieldGenerator(const int &h, const int &w, const int &bombs, const QPair<int, int> &start)
{
    if (bombs >= h * w) {
        qDebug() << "Bombs amount is too big, reducing";
        bombAmount = h * w / 20;
    } else
        bombAmount = bombs;
    field = generateMineField(h, w, bombAmount, start);
}

MineFieldGenerator::MineFieldGenerator() : field(QVector<QVector<bool>>()), bombAmount(0)
{

}

//генерирует минное поле с заданными размерами, рядом с 'кликнутой'
//в начале игры ячейкой start бомб не будет
QVector<QVector<bool> > MineFieldGenerator::generateMineField(const int &h, const int &w, int bms, const QPair<int, int> &start)
{
    srand(time(nullptr));
    QVector<QVector<bool>> mineField(h, QVector<bool>(w, false));
    while (bms > 0) {
        QPair<int,int> pos(rand()%h, rand()%w);
        if (mineField[pos.first][pos.second] == false
                && !((pos.first >= start.first - 1 && pos.first <= start.first + 1)
                &&  (pos.second >= start.second - 1 && pos.second <= start.second + 1))) {
            mineField[pos.first][pos.second] = true;
            --bms;
        }
    }
    return mineField;
}

//метод, высчитывающий для каждой ячейки число бомб в соседних ячейках
//(или -1, если в этой ячейке бомба) с возвратом результата
QVector<QVector<int> > MineFieldGenerator::generateCellVals(const QVector<QVector<bool> > &mineField)
{
    if (mineField.empty())
        return {};
    int h = mineField.size(), w = mineField[0].size();
    QVector<QVector<int>> cellVals(h, QVector<int>(w, 0));
    //отмечаем мины
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            cellVals[i][j] = mineField[i][j] ? -1 : 0;
        }
    }
    //считаем число бомб рядом прибавляя единицу к соседним к минам ячейкам
    //если в самой этой ячейке нет бомбы
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {

            if (cellVals[i][j] != -1)
                continue;
            //ячейки сверху от мины
            if (i > 0) {
                if (j > 0 && cellVals[i-1][j-1] != -1)
                    ++cellVals[i-1][j-1];
                if (cellVals[i-1][j] != -1)
                    ++cellVals[i-1][j];
                if (j < w - 1 && cellVals[i-1][j+1] != -1)
                    ++cellVals[i-1][j+1];
            }
            //слева и справа
            if (j > 0 && cellVals[i][j-1] != -1)
                ++cellVals[i][j-1];
            if (j < w - 1 && cellVals[i][j+1] != -1)
                ++cellVals[i][j+1];
            //снизу
            if (i < h - 1) {
                if (j > 0 && cellVals[i+1][j-1] != -1)
                    ++cellVals[i+1][j-1];
                if (cellVals[i+1][j] != -1)
                    ++cellVals[i+1][j];
                if (j < w - 1 && cellVals[i+1][j+1] != -1)
                    ++cellVals[i+1][j+1];
            }
        }
    }
    return cellVals;
}

QVector<QVector<int> > MineFieldGenerator::generateCellVals(const int &h, const int &w, int bms, const QPair<int, int> &start)
{
    return generateCellVals(generateMineField(h, w, bms, start));
}
