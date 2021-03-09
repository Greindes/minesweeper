#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>

class MineFieldWidget;

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTime cloak;
    QTimer * timer;
    QString timeTemplate;
    MineFieldWidget * mineField;


public slots:
    void slotStartGame();
    void slotUpdateTime();
    void slotEndGame(bool isWin);
    void slotResetTimer();
    void minimizeWindowSize();
    void changeGameDifficulty(int w, int h, int bombAmount);
    void refleshGeometry();
private slots:
    void on_action_easy_triggered();
    void on_action_medium_triggered();
    void on_action_hard_triggered();
    void on_action_custom_triggered();
};

#endif // MAINWINDOW_H
