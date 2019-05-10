#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "card_ctrl.h"
extern "C" {
#include "camera.h"
}


#include <QTime>
#include <QTimer>

//pthread_t MainWindow::cardThread = 0;
bool MainWindow::ic_card_state = false;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    ui->radioButton->setChecked(true);
    ui->radioButtonCEL->setChecked(true);

    ledTime = 31;
    ledCurtime = 31;
    closeLedTime = false;
    fixed = false;

    useCamera = false;

    direction = false;
    isAdministrator = false;

    led = new LedLight();

    //调用启动ic卡线程
    startCard();

    if (!ic_card_state) {
        //disableButtons();
    }

    QTimer * timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
    timer->start(1000);
}

MainWindow::~MainWindow()
{
    exitCard();
    delete led;
    delete ui;
}

void MainWindow::timerUpdate()
{
    //第一次刷卡，管理员进入，开启按钮
    //再次刷卡，关闭按钮
    if (ic_card_state && !isAdministrator) {
        isAdministrator = true;
        enableButtons();
    } else if (!ic_card_state && isAdministrator) {
        isAdministrator = false;
        disableButtons();
    }

    //显示系统时间
    QTime time = QTime::currentTime();
    QString curtime = time.toString("hh:mm");
    if (time.second() % 2 == 0) {
        curtime[2] = ' ';
    }
    ui->lcdNumber->display(curtime);

    //显示led倒计时
    if (!this->closeLedTime) {
        this->ledCurtime--;

        if (this->ledCurtime == (this->ledTime - 1)) {
            if (this->direction) {
                //南北
                led->vertical();
                ui->label_2->setText(QString::fromUtf8("current dir:SN"));
            } else {
                //东西
                led->horizotal();
                ui->label_2->setText(QString::fromUtf8("current dir:EW"));
            }
            this->direction = !(this->direction);

        }

        QString ledText = QString::number(this->ledCurtime);
        if (this->ledCurtime < 10) {
            ledText = "0" + ledText;
        }
        if (this->ledCurtime == 0) {
            this->ledCurtime = this->ledTime;
        }
        led->setLedTime(this->ledCurtime);
        ui->lcdNumber2->display(ledText);

    }


}

//由于改变时间后会触发if (this->ledCurtime == (this->ledTime - 1))， 不太合理，所以需要加一条语句
//this->direction = !(this->direction);
void MainWindow::on_radioButton_clicked()
{
    if (this->ledTime != 31) {
        this->ledTime = 31;
        this->ledCurtime = 31;
        this->direction = !(this->direction);
    }
}

void MainWindow::on_radioButton_2_clicked()
{
    if (this->ledTime != 61) {
        this->ledTime = 61;
        this->ledCurtime = 61;
        this->direction = !(this->direction);
    }
}

void MainWindow::on_radioButton_3_clicked()
{
    if (this->ledTime != 91) {
        this->ledTime = 91;
        this->ledCurtime = 91;
        this->direction = !(this->direction);
    }
}

void MainWindow::on_closeTimeButton_clicked()
{
    if (!this->fixed) {
        if (!this->closeLedTime) {
            this->closeLedTime = true;
            ui->closeTimeButton->setText(QString::fromUtf8("open timer"));

        } else {
            this->closeLedTime = false;
            ui->closeTimeButton->setText(QString::fromUtf8("close timer"));
        }
    }


}

void MainWindow::on_radioButtonEW_clicked()
{
    this->closeLedTime = true;
    this->fixed = true;
    this->direction = false;
    led->horizotal();
    ui->label_2->setText(QString::fromUtf8("currrent dir:EW(fixed)"));
}

void MainWindow::on_radioButtonSN_clicked()
{
    this->closeLedTime = true;
    this->fixed = true;
    this->direction = true;
    led->vertical();
    ui->label_2->setText(QString::fromUtf8("current dir:SN(fixed)"));
}

void MainWindow::on_radioButtonCEL_clicked()
{
    this->closeLedTime = false;
    this->fixed = false;
    if (this->direction) {
        ui->label_2->setText(QString::fromUtf8("current dir:SN"));
    } else {
        ui->label_2->setText(QString::fromUtf8("current dir:EW"));
    }

}

void MainWindow::on_cameraButton_clicked()
{
    if (!this->useCamera) {
        this->useCamera = true;
        qDebug("opening camera...");
        if (openCamera() < 0) {
            printf("can't open camera!!!\n");
            this->useCamera = false;
        } else {
            ui->cameraButton->setText(QString::fromUtf8("close camera"));
        }

    } else {
        this->useCamera = false;
        qDebug("closing camera...");
        closeCamera();
        ui->cameraButton->setText(QString::fromUtf8("open camera"));
    }
}

void MainWindow::disableButtons()
{
    ui->radioButton->setDisabled(true);
    ui->radioButton_2->setDisabled(true);
    ui->radioButton_3->setDisabled(true);

    ui->radioButtonEW->setDisabled(true);
    ui->radioButtonSN->setDisabled(true);
    ui->radioButtonCEL->setDisabled(true);

    ui->closeTimeButton->setDisabled(true);
    ui->cameraButton->setDisabled(true);
}

void MainWindow::enableButtons()
{
    ui->radioButton->setDisabled(false);
    ui->radioButton_2->setDisabled(false);
    ui->radioButton_3->setDisabled(false);

    ui->radioButtonEW->setDisabled(false);
    ui->radioButtonSN->setDisabled(false);
    ui->radioButtonCEL->setDisabled(false);

    ui->closeTimeButton->setDisabled(false);
    ui->cameraButton->setDisabled(false);
}
