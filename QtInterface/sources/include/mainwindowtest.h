#ifndef MAINWINDOWTEST_H
#define MAINWINDOWTEST_H
#include <Python.h>
#include <QMainWindow>
#include <matplotlibcpp.h>
#include <serialport.h>
#include <myplot.h>
#include <QElapsedTimer>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindowTest; }
QT_END_NAMESPACE

class MainWindowTest : public QMainWindow
{
    Q_OBJECT

public:
    MainWindowTest(QWidget *parent = nullptr);

private:
    Ui::MainWindowTest *ui;

};
#endif // MAINWINDOWTEST_H
