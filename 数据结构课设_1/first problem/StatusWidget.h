#ifndef STATUSWIDGET_H
#define STATUSWIDGET_H

#include <QWidget>
#include <QStringListModel>
#include <QListView>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QStringListModel>
#include <QWidget>
#include "FlightWidget.h"
class FlightWidget;
class StatusWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StatusWidget(QWidget *parent = nullptr);
    void setFlightWidget(FlightWidget *widget);  // 注入航班控件
private slots:
    void onRefreshClicked();
    void onUpdateStatusClicked();  // 新增槽函数
private slots:
    void onRecommendClicked();  // ← 加上这行！

private:
    void setupUI();
    FlightWidget *flightWidget; // 用于访问航班信息
    // UI控件
    QStringListModel *statusModel;
    QListView *statusListView;
    QPushButton *refreshBtn;

    QLineEdit *flightIDEdit;
    QLineEdit *statusEdit;
    QPushButton *updateStatusBtn;

    QLineEdit *fromCityEdit;
    QLineEdit *toCityEdit;
    QPushButton *recommendBtn;
    QTextEdit *recommendResult;

    QVBoxLayout *mainLayout;

    QStringList flightStatusList;  // 存储当前状态列表
};
#endif // STATUSWIDGET_H
