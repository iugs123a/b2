#include "StatusWidget.h"
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QGroupBox>
#include <QTextEdit>
#include <QPushButton>
#include <QListView>
#include <QVBoxLayout>
#include <QMessageBox>
#include "FlightWidget.h"
StatusWidget::StatusWidget(QWidget *parent) : QWidget(parent)
{
    setupUI();

    statusModel = new QStringListModel(this);
    statusListView->setModel(statusModel);

    connect(refreshBtn, &QPushButton::clicked, this, &StatusWidget::onRefreshClicked);

    connect(updateStatusBtn, &QPushButton::clicked, this, &StatusWidget::onUpdateStatusClicked);
    connect(recommendBtn, &QPushButton::clicked, this, &StatusWidget::onRecommendClicked);

    onRefreshClicked();
}

void StatusWidget::setupUI()
{
    // 航班状态列表与刷新按钮 横向布局
    statusListView = new QListView(this);
    refreshBtn = new QPushButton("刷新状态", this);
    QHBoxLayout *statusLayout = new QHBoxLayout();
    statusLayout->addWidget(statusListView, 1);   // 列表扩展占位
    statusLayout->addWidget(refreshBtn);

    // 航班状态更新区域
    flightIDEdit = new QLineEdit(this);
    statusEdit = new QLineEdit(this);
    updateStatusBtn = new QPushButton("更新状态", this);
    QFormLayout *updateForm = new QFormLayout();
    updateForm->addRow("航班号:", flightIDEdit);
    updateForm->addRow("新状态:", statusEdit);
    updateForm->addWidget(updateStatusBtn);

    QGroupBox *updateGroup = new QGroupBox("航班状态更新", this);
    updateGroup->setLayout(updateForm);

    // 转机推荐区域
    fromCityEdit = new QLineEdit(this);
    toCityEdit = new QLineEdit(this);
    recommendBtn = new QPushButton("推荐转机方案", this);
    recommendResult = new QTextEdit(this);
    recommendResult->setReadOnly(true);

    QFormLayout *recommendForm = new QFormLayout();
    recommendForm->addRow("出发城市:", fromCityEdit);
    recommendForm->addRow("目的城市:", toCityEdit);
    recommendForm->addWidget(recommendBtn);
    recommendForm->addRow("推荐方案:", recommendResult);

    QGroupBox *recommendGroup = new QGroupBox("转机推荐", this);
    recommendGroup->setLayout(recommendForm);

    // 主布局，竖直排列各区域
    mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(statusLayout);
    mainLayout->addWidget(updateGroup);
    mainLayout->addWidget(recommendGroup);

    setLayout(mainLayout);
}
// 新增的更新状态槽函数
void StatusWidget::onUpdateStatusClicked()
{
    QString flightID = flightIDEdit->text().trimmed();
    QString newStatus = statusEdit->text().trimmed();

    if (flightID.isEmpty() || newStatus.isEmpty()) {
        // 可以弹个消息框提醒用户
        QMessageBox::warning(this, "输入错误", "请输入航班号和新的状态！");
        return;
    }

    // 查找对应航班号，格式假设是 "航班号 - 状态"
    bool found = false;
    for (int i = 0; i < flightStatusList.size(); ++i) {
        if (flightStatusList[i].startsWith(flightID + " -")) {
            flightStatusList[i] = flightID + " - " + newStatus;
            found = true;
            break;
        }
    }

    if (!found) {
        QMessageBox::information(this, "未找到航班", "未找到对应的航班号，将添加新航班状态。");
        flightStatusList.append(flightID + " - " + newStatus);
    }

    // 过滤掉空字符串，防止空数据导致崩溃
    QStringList filteredList;
    for (const QString &str : flightStatusList) {
        if (!str.isEmpty())
            filteredList.append(str);
    }
    flightStatusList = filteredList;

    statusModel->setStringList(flightStatusList);
}
void StatusWidget::setFlightWidget(FlightWidget *widget) {
    flightWidget = widget;
}
// 辅助函数：将 "HH:mm" 格式的时间转换成分钟数
int timeStringToMinutes(const QString &timeStr) {
    QStringList parts = timeStr.split(":");
    if (parts.size() != 2) return -1;  // 格式错误返回-1
    bool ok1 = false, ok2 = false;
    int hour = parts[0].toInt(&ok1);
    int minute = parts[1].toInt(&ok2);
    if (!ok1 || !ok2) return -1;
    return hour * 60 + minute;
}

void StatusWidget::onRecommendClicked() {
    QString from = fromCityEdit->text().trimmed();
    QString to = toCityEdit->text().trimmed();

    if (!flightWidget) {
        QMessageBox::warning(this, "错误", "未连接航班数据！");
        return;
    }

    if (from.isEmpty() || to.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "请输入出发城市和目的城市！");
        return;
    }

    QList<FlightInfo> flightList = flightWidget->getFlightList(); // 需要实现此方法获取航班列表

    QList<const FlightInfo*> directFlights;
    QList<QPair<const FlightInfo*, const FlightInfo*>> transferOptions;

    // 查找直飞航班
    for (const auto &f : flightList) {
        if (f.fromCity == from && f.toCity == to)
            directFlights.append(&f);
    }

    if (!directFlights.isEmpty()) {
        recommendResult->setText("存在直飞航班，请优先选择直飞：\n");
        for (const auto *f : directFlights) {
            QString stopover = f->stopover.isEmpty() ? "无" : f->stopover;
            recommendResult->append(f->flightNumber + " (" + f->airline + ") " +
                                    f->fromCity + " → " + f->toCity + " 经停：" + stopover + "\n" +
                                    f->departureTime + " - " + f->arrivalTime +
                                    " 票价：" + QString::number(f->price));
        }
        return;
    }

    // 查找转机航班，要求转机时间 >= 30分钟
    const int minTransferMinutes = 30;

    for (const auto &f1 : flightList) {
        if (f1.fromCity != from)
            continue;

        for (const auto &f2 : flightList) {
            if (f2.toCity != to)
                continue;

            if (f1.toCity == f2.fromCity) {
                int f1Arrival = timeStringToMinutes(f1.arrivalTime);
                int f2Departure = timeStringToMinutes(f2.departureTime);

                // 判断转机时间是否合理
                if (f1Arrival >= 0 && f2Departure >= 0 && (f2Departure - f1Arrival) >= minTransferMinutes) {
                    transferOptions.append(qMakePair(&f1, &f2));
                }
            }
        }
    }

    if (transferOptions.isEmpty()) {
        recommendResult->setText("没有找到合适的转机方案。");
        return;
    }

    // 按票价总和排序（可扩展为综合评分）
    std::sort(transferOptions.begin(), transferOptions.end(),
              [](const QPair<const FlightInfo*, const FlightInfo*> &a,
                 const QPair<const FlightInfo*, const FlightInfo*> &b) {
                  return (a.first->price + a.second->price) < (b.first->price + b.second->price);
              });

    recommendResult->setText("推荐转机方案如下：\n");
    int shown = 0;
    for (const auto &pair : transferOptions) {
        recommendResult->append(QString("方案%1：").arg(++shown));
        recommendResult->append("第一段: " + pair.first->flightNumber + " " +
                                pair.first->fromCity + " → " + pair.first->toCity + " " +
                                pair.first->departureTime + " - " + pair.first->arrivalTime +
                                " ¥" + QString::number(pair.first->price));
        recommendResult->append("第二段: " + pair.second->flightNumber + " " +
                                pair.second->fromCity + " → " + pair.second->toCity + " " +
                                pair.second->departureTime + " - " + pair.second->arrivalTime +
                                " ¥" + QString::number(pair.second->price));
        recommendResult->append("------------------------");
        if (shown >= 3) break; // 只展示前三个最优方案
    }
}


void StatusWidget::onRefreshClicked()
{
    // 过滤空字符串，防止崩溃
    QStringList filteredList;
    for (const QString &str : flightStatusList) {
        if (!str.isEmpty())
            filteredList.append(str);
    }
    statusModel->setStringList(filteredList);
}
