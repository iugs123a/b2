#include "MainWindow.h"
#include "AddFlightDialog.h"
#include <QToolBar>
#include <QStackedWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 初始化页面组件
    flightWidget = new FlightWidget(this);
    statusWidget = new StatusWidget(this);
    // 注入 flightWidget 指针到 statusWidget，确保状态页可以访问航班数据
    statusWidget->setFlightWidget(flightWidget);
    // 初始化堆叠窗口容器
    stackedWidget = new QStackedWidget(this);
    stackedWidget->addWidget(flightWidget);   // 页 0
    stackedWidget->addWidget(statusWidget);   // 页 1

    setCentralWidget(stackedWidget);

    // 创建工具栏
    createToolBar();
}

void MainWindow::createToolBar()
{
    QToolBar *toolBar = addToolBar("导航");

    QAction *flightAction = new QAction("航班管理", this);
    QAction *statusAction = new QAction("状态管理", this);

    // 新增“添加航班”按钮
    QAction *addFlightAction = new QAction("添加航班", this);

    connect(flightAction, &QAction::triggered, this, &MainWindow::showFlightWidget);
    connect(statusAction, &QAction::triggered, this, &MainWindow::showStatusWidget);

    // 连接添加航班动作的槽函数（需要把槽函数声明为public slots或private slots）
    connect(addFlightAction, &QAction::triggered, this, [this]() {
        AddFlightDialog dlg(this);
        if (dlg.exec() == QDialog::Accepted) {
            QStringList flightData = dlg.getFlightData();
            flightWidget->addFlightFromDialog(flightData);
            showFlightWidget();
        }
    });


    toolBar->addAction(flightAction);
    toolBar->addAction(statusAction);
    toolBar->addSeparator();
    toolBar->addAction(addFlightAction);
}

void MainWindow::showFlightWidget()
{
    stackedWidget->setCurrentWidget(flightWidget);
}

void MainWindow::showStatusWidget()
{
    stackedWidget->setCurrentWidget(statusWidget);
}
