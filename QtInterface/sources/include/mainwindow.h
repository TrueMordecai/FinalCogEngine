       #ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QWidget>
#include <serialport.h>
#include <myplot.h>
#include <QElapsedTimer>
#include <QDebug>
#include <QTimer>
#include <QLabel>
#include <QLCDNumber>
#include <QPushButton>
#include <fstream>
#include <QGridLayout>
#include <QRadioButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();
    void write_csv(std::string filename, std::vector<std::pair<std::string, std::vector<int> > > dataset);
    void read_csv(std::string filename);
    void InitInterface();
    void traitementMainWindow();

private slots:
    void on_lcdNumber_overflow();

private:
    SerialPort *mySerialPort;
    QTimer *m_displayTimer;
    QTimer *m_display2;
    QElapsedTimer timer= QElapsedTimer();
    QElapsedTimer m_timer;
    bool showMatplotlib=false;
    bool buttonPushed=false;
    QLabel *labelCapt1;
    QLabel *labelCapt2;
    QLabel * labelCapt3;
    QLCDNumber *lcd;
    QLCDNumber *lcd2;
    QLCDNumber *lcd3;
    QFrame *alerte1;
    QFrame *alerte2;
    bool isRed = false;
    bool isGreen = true;
    bool isOrange = false;
    bool isViolet = false;
    bool scenarioCSV = false;
    int numberPush = 0;
};
#endif // MAINWINDOW_H
