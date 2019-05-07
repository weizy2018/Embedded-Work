#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <pthread.h>

#include "ledlight.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void on_radioButton_3_clicked();
    void timerUpdate();


    void on_closeTimeButton_clicked();

    void on_radioButtonEW_clicked();

    void on_radioButtonSN_clicked();

    void on_radioButtonCEL_clicked();

    void on_cameraButton_clicked();

public:
    void disableButtons();
    void enableButtons();

private:
    Ui::MainWindow *ui;

private:
    int ledTime;
    int ledCurtime;
    bool closeLedTime;
    bool fixed;
    bool direction;     //false: 东西  true: 南北
    bool useCamera;

public:
    static pthread_t cardThread;
    static bool ic_card_state;

private:
    LedLight * led;
};

#endif // MAINWINDOW_H
