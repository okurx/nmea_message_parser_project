#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QMessageBox>
#include <QString>
#include <QStringList>
#include <QtSerialPort/QSerialPort>
#include <QVector>
#include <iostream>
#include <sstream>
#include <cstddef>
#include <string>
#include <QTimer>
#include <QDateTime>
#include <QSettings>
#include <QPushButton>
#include <QApplication>
using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_start_clicked();
    void on_pushButton_stop_clicked();
    void Read_Data();
    void on_comboBox_choose_id_currentIndexChanged(const QString& arg1);
    void on_checkBox_rtc_stateChanged();
    void on_checkBox_rtd_stateChanged();
    void on_pushButton_Sensor_add_clicked();

private:
    Ui::MainWindow* ui;
    QSerialPort* COMPORT1;
    QSerialPort* COMPORT2;

    void nmeaparser(std::string data);
    QString satellite(QChar data2);
    bool lettertest(QString data3);
    int calculate_csum(std::string data1);
    std::string inttohex(int number);

    QTimer* Timer;
    QList<QString> spinBoxes;

    void setvisible();
    void sclear();
    void defaultsettings();
    void savesettings();
    void mydesign();
    void portsettings();

};
#endif // MAINWINDOW_H
