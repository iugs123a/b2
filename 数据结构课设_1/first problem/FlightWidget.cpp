#include "FlightWidget.h"
#include "AddFlightDialog.h"  // 你自己的添加对话框

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QLabel>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QInputDialog>
FlightWidget::FlightWidget(QWidget *parent) : QWidget(parent)
{
    setupUI();
    csvFilePath = "D:\\flight_data_300.csv";
    FlightWidget::loadFromCSV(csvFilePath);

}
void FlightWidget::saveToCSV(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "无法打开文件进行保存:" << filePath;
        return;
    }

    QTextStream out(&file);

    // 写入表头
    out << "航班号,航空公司,起飞时间,降落时间,经停,余票\n";

    // 写入每一条航班信息
    for (const FlightInfo &flight : flightList) {
        out << flight.flightNumber << ","
            << flight.airline << ","
            << flight.departureTime << ","
            << flight.arrivalTime << ","
            << flight.stopover << ","
            << flight.seatsAvailable<< ","
            << flight.fromCity<< ","
            << flight.toCity<<","
            <<flight.price<< "\n";
    }

    file.close();
}

QString normalizeDateTime(const QString &dt) {
    QDateTime datetime = QDateTime::fromString(dt, "yyyy-MM-dd H:mm");
    if (!datetime.isValid()) {
        datetime = QDateTime::fromString(dt, "yyyy-MM-dd HH:mm");
    }
    if (datetime.isValid()) {
        return datetime.toString("yyyy-MM-dd HH:mm");  // 统一格式
    }
    return dt;  // 如果解析失败，保留原始数据
}

void FlightWidget::loadFromCSV(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "无法打开文件读取:" << filePath;
        return;
    }

    QTextStream in(&file);
    flightList.clear();  // 清空原有列表

    bool isFirstLine = true;
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (isFirstLine) {
            isFirstLine = false;
            continue;  // 跳过表头
        }

        QStringList fields = line.split(",");
        if (fields.size() < 9) continue;  // 检查字段数量是否完整

        FlightInfo f;
        f.flightNumber = fields[0];
        f.airline = fields[1];
        f.departureTime = normalizeDateTime(fields[2]);  // ✅ 格式化
        f.arrivalTime = normalizeDateTime(fields[3]);    // ✅ 格式化
        f.stopover = fields[4];
        f.seatsAvailable = fields[5].toInt();
        f.fromCity = fields[6];
        f.toCity = fields[7];
        f.price = fields[8].toDouble();

        flightList.append(f);
    }
    buildIndex();

    file.close();
    updateModelFromList();  // 更新表格
}






void FlightWidget::setupUI()
{
    // 现有控件
    tableView = new QTableView(this);

    addBtn = new QPushButton("添加航班", this);
    editBtn = new QPushButton("编辑航班", this);
    deleteBtn = new QPushButton("删除航班", this);

    buyTicketBtn = new QPushButton("购票", this);
    refundTicketBtn = new QPushButton("退票", this);

    QLabel *searchLabel = new QLabel("查询航班号：", this);
    searchEdit = new QLineEdit(this);
    searchBtn = new QPushButton("查询", this);
    resetBtn = new QPushButton("重置", this);

    QHBoxLayout *searchLayout = new QHBoxLayout();
    searchLayout->addWidget(searchLabel);
    searchLayout->addWidget(searchEdit);
    searchLayout->addWidget(searchBtn);
    searchLayout->addWidget(resetBtn);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(searchLayout);
    mainLayout->addWidget(tableView);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(addBtn);
    btnLayout->addWidget(editBtn);
    btnLayout->addWidget(deleteBtn);
    btnLayout->addWidget(buyTicketBtn);
    btnLayout->addWidget(refundTicketBtn);
    mainLayout->addLayout(btnLayout);

    flightModel = new QStandardItemModel(this);
    flightModel->setHorizontalHeaderLabels(QStringList() << "航班号" << "航空公司" << "起飞时间" << "降落时间" << "经停" << "余票" << "出发城市" << "目的城市" << "票价");

    filterModel = new QSortFilterProxyModel(this);
    filterModel->setSourceModel(flightModel);
    filterModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    filterModel->setFilterKeyColumn(0); // 按航班号过滤

    tableView->setModel(filterModel);
    // 🔽 新增控件：出发城市、目的城市、排序方式
    fromCityEdit = new QLineEdit(this);
    fromCityEdit->setPlaceholderText("出发城市");

    toCityEdit = new QLineEdit(this);
    toCityEdit->setPlaceholderText("目的城市");

    sortComboBox = new QComboBox(this);
    sortComboBox->addItem("按起飞时间排序");
    sortComboBox->addItem("按价格排序");
    sortComboBox->addItem("按剩余票数排序");

    advancedSearchBtn = new QPushButton("高级查询", this);  // 新按钮

    QHBoxLayout *advancedSearchLayout = new QHBoxLayout();
    advancedSearchLayout->addWidget(fromCityEdit);
    advancedSearchLayout->addWidget(toCityEdit);
    advancedSearchLayout->addWidget(sortComboBox);
    advancedSearchLayout->addWidget(advancedSearchBtn);

    // 加入主布局
    mainLayout->addLayout(advancedSearchLayout);
    // 信号槽连接
    connect(searchBtn, &QPushButton::clicked, this, &FlightWidget::onSearchFlight);
    connect(resetBtn, &QPushButton::clicked, this, &FlightWidget::onResetSearch);
    connect(addBtn, &QPushButton::clicked, this, &FlightWidget::onAddFlight);
    connect(editBtn, &QPushButton::clicked, this, &FlightWidget::onEditFlight);
    connect(deleteBtn, &QPushButton::clicked, this, &FlightWidget::onDeleteFlight);

    connect(buyTicketBtn, &QPushButton::clicked, this, &FlightWidget::onBuyTicket);
    connect(refundTicketBtn, &QPushButton::clicked, this, &FlightWidget::onRefundTicket);
    connect(advancedSearchBtn, &QPushButton::clicked, this, &FlightWidget::onAdvancedSearch);
    QPushButton *showAllBtn = new QPushButton("显示全部航班", this);
    advancedSearchLayout->addWidget(showAllBtn);
    connect(showAllBtn, &QPushButton::clicked, this, [this]() {
        updateModelFromList();  // 恢复完整数据
    });


}
//利用的是哈希表实现高效搜索结构
//fromCityIndex 存储了每个出发城市对应的航班在 flightList 中的索引列表。
//toCityIndex 存储了每个目的城市对应的航班在 flightList 中的索引列表。
void FlightWidget::buildIndex() {
    fromCityIndex.clear();
    toCityIndex.clear();

    for (int i = 0; i < flightList.size(); ++i) {
        QString fromKey = flightList[i].fromCity.toLower();  // 统一小写方便查询
        QString toKey = flightList[i].toCity.toLower();

        fromCityIndex[fromKey].append(i);
        toCityIndex[toKey].append(i);
    }
}
//全局的快速排序
void quickSort(QList<FlightInfo> &list, int left, int right, int sortIndex) {
    if (left >= right) return;

    int i = left, j = right;
    FlightInfo pivot = list[left];

    while (i < j) {
        while (i < j) {
            bool cmp = false;
            switch (sortIndex) {
            case 0: {
                QTime t1 = QTime::fromString(list[j].departureTime, "H:mm");
                if (!t1.isValid()) t1 = QTime::fromString(list[j].departureTime, "HH:mm");
                QTime tPivot = QTime::fromString(pivot.departureTime, "H:mm");
                if (!tPivot.isValid()) tPivot = QTime::fromString(pivot.departureTime, "HH:mm");
                cmp = t1 >= tPivot;
                break;
            }
            case 1:
                cmp = list[j].price >= pivot.price;
                break;
            case 2:
                cmp = list[j].seatsAvailable <= pivot.seatsAvailable;
                break;
            }
            if (cmp) j--;
            else break;
        }
        if (i < j) list[i++] = list[j];

        while (i < j) {
            bool cmp = false;
            switch (sortIndex) {
            case 0: {
                QTime t1 = QTime::fromString(list[i].departureTime, "H:mm");
                if (!t1.isValid()) t1 = QTime::fromString(list[i].departureTime, "HH:mm");
                QTime tPivot = QTime::fromString(pivot.departureTime, "H:mm");
                if (!tPivot.isValid()) tPivot = QTime::fromString(pivot.departureTime, "HH:mm");
                cmp = t1 <= tPivot;
                break;
            }
            case 1:
                cmp = list[i].price <= pivot.price;
                break;
            case 2:
                cmp = list[i].seatsAvailable >= pivot.seatsAvailable;
                break;
            }
            if (cmp) i++;
            else break;
        }
        if (i < j) list[j--] = list[i];
    }
    list[i] = pivot;

    quickSort(list, left, i - 1, sortIndex);
    quickSort(list, i + 1, right, sortIndex);
}

void FlightWidget::onAdvancedSearch()
{
    QString from = fromCityEdit->text().trimmed().toLower();
    QString to = toCityEdit->text().trimmed().toLower();
    int sortIndex = sortComboBox->currentIndex();

    QList<int> fromList;
    QList<int> toList;

    if (!from.isEmpty()) {
        if (fromCityIndex.contains(from)) {
            fromList = fromCityIndex.value(from);
        } else {
            fromList.clear();
        }
    } else {
        fromList.reserve(flightList.size());
        for (int i = 0; i < flightList.size(); ++i) fromList.append(i);
    }

    if (!to.isEmpty()) {
        if (toCityIndex.contains(to)) {
            toList = toCityIndex.value(to);
        } else {
            toList.clear();
        }
    } else {
        toList.reserve(flightList.size());
        for (int i = 0; i < flightList.size(); ++i) toList.append(i);
    }

    QSet<int> fromSet = QSet<int>(fromList.begin(), fromList.end());

    QList<int> filteredIdx;
    for (int idx : toList) {
        if (fromSet.contains(idx)) filteredIdx.append(idx);
    }

    QList<FlightInfo> filtered;
    for (int idx : filteredIdx) {
        filtered.append(flightList[idx]);
    }

    if (!filtered.isEmpty())
        quickSort(filtered, 0, filtered.size() - 1, sortIndex);


    flightModel->removeRows(0, flightModel->rowCount());

    for (const FlightInfo &flight : filtered) {
        QList<QStandardItem *> row;
        row << new QStandardItem(flight.flightNumber)
            << new QStandardItem(flight.airline)
            << new QStandardItem(flight.departureTime)
            << new QStandardItem(flight.arrivalTime)
            << new QStandardItem(flight.stopover)
            << new QStandardItem(QString::number(flight.seatsAvailable))
            << new QStandardItem(flight.fromCity)
            << new QStandardItem(flight.toCity)
            << new QStandardItem(QString::number(flight.price));
        flightModel->appendRow(row);
    }
}


void FlightWidget::onBuyTicket()
{
    QModelIndex index = tableView->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "提示", "请先选择一个航班！");
        return;
    }

    int row = filterModel->mapToSource(index).row();
    FlightInfo &flight = flightList[row];

    if (flight.seatsAvailable > 0) {
        // 有票，直接购票成功
        flight.seatsAvailable -= 1;
        updateModelFromList();
        saveToCSV(csvFilePath);
        QMessageBox::information(this, "购票成功", QString("航班 %1 购票成功，剩余余票：%2")
                                                       .arg(flight.flightNumber)
                                                       .arg(flight.seatsAvailable));
    } else {
        // 无票，加入预约队列
        bool ok;
        QString userName = QInputDialog::getText(this, "余票不足", "请输入您的姓名进行预约：", QLineEdit::Normal, "", &ok);
        if (ok && !userName.trimmed().isEmpty()) {
            flight.bookingQueue.enqueue(userName.trimmed());
            QMessageBox::information(this, "预约成功", QString("航班 %1 余票不足，已为您预约。").arg(flight.flightNumber));
        } else {
            QMessageBox::information(this, "预约取消", "预约已取消。");
        }
    }
}
void FlightWidget::onRefundTicket()
{
    QModelIndex index = tableView->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "提示", "请先选择一个航班！");
        return;
    }

    int row = filterModel->mapToSource(index).row();
    FlightInfo &flight = flightList[row];

    if (!flight.bookingQueue.isEmpty()) {
        // 预约队列非空，优先给预约用户补票
        QString nextUser = flight.bookingQueue.dequeue();

        // 此处可以根据需求实现通知用户等操作，这里简单用消息框表示
        QMessageBox::information(this, "补票通知",
                                 QString("尊敬的 %1，您预约的航班 %2 有票补上了！请尽快购票。")
                                     .arg(nextUser)
                                     .arg(flight.flightNumber));
        // 余票不增加，因为补票直接送出了一张票
    } else {
        // 无预约用户，正常退票
        flight.seatsAvailable += 1;
    }

    updateModelFromList();
    saveToCSV(csvFilePath);

    QMessageBox::information(this, "退票成功", QString("航班 %1 退票成功，当前余票：%2")
                                                   .arg(flight.flightNumber)
                                                   .arg(flight.seatsAvailable));
}

void FlightWidget::onEditFlight()
{
    QModelIndex index = tableView->currentIndex();
    if (!index.isValid()) return;

    int row = filterModel->mapToSource(index).row();
    const FlightInfo &flight = flightList[row];

    QStringList flightData;
    flightData << flight.flightNumber
               << flight.airline
               << flight.departureTime
               << flight.arrivalTime
               << flight.stopover
               << QString::number(flight.seatsAvailable)
               << flight.fromCity
               << flight.toCity
               << QString::number(flight.price);

    AddFlightDialog dialog(this);
    dialog.setFlightData(flightData);

    if (dialog.exec() == QDialog::Accepted) {
        QStringList newData = dialog.getFlightData();

        // ✅ 构造一个新的 FlightInfo 对象
        FlightInfo updatedFlight;
        updatedFlight.flightNumber = newData[0];
        updatedFlight.airline = newData[1];
        updatedFlight.departureTime = newData[2];
        updatedFlight.arrivalTime = newData[3];
        updatedFlight.stopover = newData[4];
        updatedFlight.seatsAvailable = newData[5].toInt();
        updatedFlight.fromCity = newData[6];
        updatedFlight.toCity = newData[7];
        updatedFlight.price = newData[8].toDouble();

        // ✅ 替换原来的航班信息
        flightList[row] = updatedFlight;

        updateModelFromList();
        saveToCSV(csvFilePath);  // 写回文件
    }
}


void FlightWidget::onDeleteFlight()
{
    QModelIndex index = tableView->currentIndex();
    if (!index.isValid()) return;

    int row = filterModel->mapToSource(index).row();

    // 确认是否删除
    int ret = QMessageBox::question(this, "确认删除", "确定要删除该航班信息吗？",
                                    QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::No) return;

    flightList.removeAt(row);
    updateModelFromList();
    saveToCSV(csvFilePath);  // 写回文件
}



void FlightWidget::updateModelFromList()
{
    flightModel->clear();
    flightModel->setHorizontalHeaderLabels(QStringList()
                                           << "航班号" << "航空公司" << "起飞时间" << "降落时间" << "经停"
                                           << "余票" << "出发城市" << "目的城市" << "票价");

    for (const FlightInfo &f : qAsConst(flightList)) {
        QList<QStandardItem *> items;
        items.append(new QStandardItem(f.flightNumber));
        items.append(new QStandardItem(f.airline));
        items.append(new QStandardItem(f.departureTime));
        items.append(new QStandardItem(f.arrivalTime));
        items.append(new QStandardItem(f.stopover));
        items.append(new QStandardItem(QString::number(f.seatsAvailable)));
        items.append(new QStandardItem(f.fromCity));
        items.append(new QStandardItem(f.toCity));
        items.append(new QStandardItem(QString::number(f.price)));
        flightModel->appendRow(items);
    }
}

void FlightWidget::onSearchFlight()
{
    QString keyword = searchEdit->text().trimmed();

    if (keyword.isEmpty()) {
        QMessageBox::warning(this, "提示", "请输入航班号！");
        return;
    }

    filterModel->setFilterFixedString(keyword);
}

void FlightWidget::onResetSearch()
{
    searchEdit->clear();
    filterModel->setFilterFixedString("");
}

void FlightWidget::onAddFlight()
{
    AddFlightDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QStringList flightData = dialog.getFlightData();

        for (const QString &text : flightData) {
            if (text.isEmpty()) {
                QMessageBox::warning(this, "提示", "所有字段不能为空！");
                return;
            }
        }

        FlightInfo f;
        f.flightNumber = flightData[0];
        f.airline = flightData[1];
        f.departureTime = flightData[2];
        f.arrivalTime = flightData[3];
        f.stopover = flightData[4];
        f.seatsAvailable = flightData[5].toInt();
        f.fromCity = flightData[6];
        f.toCity = flightData[7];
        f.price = flightData[8].toDouble();

        flightList.append(f);

        updateModelFromList();
        saveToCSV(csvFilePath);

        QMessageBox::information(this, "提示", "航班添加成功！");
    }
}
void FlightWidget::addFlightFromDialog(const QStringList &flightData)
{
    for (const QString &text : flightData) {
        if (text.isEmpty()) {
            QMessageBox::warning(this, "提示", "所有字段不能为空！");
            return;
        }
    }

    FlightInfo f;
    f.flightNumber = flightData[0];
    f.airline = flightData[1];
    f.departureTime = flightData[2];
    f.arrivalTime = flightData[3];
    f.stopover = flightData[4];
    f.seatsAvailable = flightData[5].toInt();
    f.fromCity = flightData[6];
    f.toCity = flightData[7];
    f.price = flightData[8].toDouble();

    flightList.append(f);
    updateModelFromList();
    saveToCSV(csvFilePath);

    QMessageBox::information(this, "提示", "航班添加成功！");
}

