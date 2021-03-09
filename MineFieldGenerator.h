#ifndef MINEFIELDGENERATOR_H
#define MINEFIELDGENERATOR_H

#include <QVector>
#include <QSet>

//класс для генерации игрового поля
class MineFieldGenerator
{
private:
    QVector<QVector<bool>> field;
    QSet<QPair<int, int>> mines;
    int bombAmount;
public:
    MineFieldGenerator(const int& h, const int& w, const int& bms, const QPair<int, int>& start);
    MineFieldGenerator();

    //генерирует минное поле с заданными размерами, в начальной точке бомб быть не может
    static QVector<QVector<bool>> generateMineField(const int& h, const int& w,
                                                    int bms, const QPair<int, int>& start);
    //метод, высчитывающий число бомб в соседних ячейках (или -1, если сама ячейка - бомба)
    static QVector<QVector<int>> generateCellVals(const QVector<QVector<bool>>& mineField);
    static QVector<QVector<int>> generateCellVals(const int& h, const int& w,
                                                  int bms, const QPair<int, int>& start);
    QVector<QVector<bool>> getField() const;
    QSet<QPair<int, int>> getMines() const;
    int getBombAmount() const;
};

#endif // MINEFIELDGENERATOR_H
