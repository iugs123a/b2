#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QToolBar>
#include <QAction>
#include "FlightWidget.h"
#include "StatusWidget.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
private:
    QStackedWidget *stackedWidget;
    FlightWidget *flightWidget;
    StatusWidget *statusWidget;

    void createToolBar();
private slots:
    void showFlightWidget();
    void showStatusWidget();

};

#endif // MAINWINDOW_H
