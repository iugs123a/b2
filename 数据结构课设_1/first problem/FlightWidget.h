#ifndef FLIGHTWIDGET_H
#define FLIGHTWIDGET_H

#include <QWidget>
#include <QTableView>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QPushButton>
#include <QLineEdit>
#include <QList>
#include <QQueue>
#include<QComboBox>
struct FlightInfo {
    QString flightNumber;
    QString airline;
    QString departureTime;
    QString arrivalTime;
    QString stopover;
    int seatsAvailable;
    QString fromCity;
    QString toCity;
    double price;
    QQueue<QString> bookingQueue;  // 预约队列，存储等待购票的用户标识
    // FlightWidget.h（或.cpp 中的合适位置）


    FlightInfo() = default;  // 默认构造函数
    FlightInfo(const QString &fNum, const QString &air, const QString &dep,
               const QString &arr, const QString &stop, int seats)
        : flightNumber(fNum), airline(air), departureTime(dep),
        arrivalTime(arr), stopover(stop), seatsAvailable(seats) {}

};

class FlightWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FlightWidget(QWidget *parent = nullptr);
public:
    QList<FlightInfo> getFlightList() const { return flightList; }

public:
    QList<FlightInfo> flightList;
    void loadFromCSV(const QString &filePath);

private:
    QString csvFilePath;  // 保存文件路径
    QHash<QString, QList<FlightInfo>> fromCityMap;
    QHash<QString, QList<FlightInfo>> toCityMap;
    void saveToCSV(const QString &filePath);
    void setupUI();

    void updateModelFromList();
    void onEditFlight();
    void buildIndex();
    QHash<QString, QList<int>> fromCityIndex;  // 出发城市到航班序号的索引
    QHash<QString, QList<int>> toCityIndex;    // 目的城市到航班序号的索引



    QTableView *tableView;
    QStandardItemModel *flightModel;
    QSortFilterProxyModel *filterModel;
    // FlightWidget.h
    QList<FlightInfo>& getFlightListRef();  // 提供引用用于修改
    QPushButton *addBtn;
    QPushButton *editBtn;
    QPushButton *deleteBtn;
    QPushButton *searchBtn;
    QPushButton *resetBtn;
    QLineEdit *searchEdit;
    QPushButton *buyTicketBtn;
    QPushButton *refundTicketBtn;
    QLineEdit *fromCityEdit;
    QLineEdit *toCityEdit;
    QComboBox *sortComboBox;
    QPushButton *advancedSearchBtn;
    // 添加公共函数
public:
    void addFlightFromDialog(const QStringList &flightData);

private slots:
    void onDeleteFlight();
    void onSearchFlight();
    void onResetSearch();
    void onAddFlight();
    // 新增槽函数声明
    void onBuyTicket();
    void onRefundTicket();
    void onAdvancedSearch();
};

#endif // FLIGHTWIDGET_H


