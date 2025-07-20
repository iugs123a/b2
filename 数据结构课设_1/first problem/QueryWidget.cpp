#include "QueryWidget.h"
#include <QHBoxLayout>
#include "FlightWidget.h"
QueryWidget::QueryWidget(QWidget *parent) : QWidget(parent)
{
    setupUI();
}

void QueryWidget::setupUI()
{
    fromCityEdit = new QLineEdit(this);
    fromCityEdit->setPlaceholderText("出发城市");

    toCityEdit = new QLineEdit(this);
    toCityEdit->setPlaceholderText("目的城市");

    sortComboBox = new QComboBox(this);
    sortComboBox->addItem("按起飞时间排序");
    sortComboBox->addItem("按价格排序");
    sortComboBox->addItem("按剩余票数排序");

    searchBtn = new QPushButton("查询", this);
    resultTableView = new QTableView(this);

    QHBoxLayout *searchLayout = new QHBoxLayout();
    searchLayout->addWidget(fromCityEdit);
    searchLayout->addWidget(toCityEdit);
    searchLayout->addWidget(sortComboBox);   // 添加排序选择框
    searchLayout->addWidget(searchBtn);

    mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(searchLayout);
    mainLayout->addWidget(resultTableView);

    connect(searchBtn, &QPushButton::clicked, this, &QueryWidget::onSearchClicked);
}

void QueryWidget::onSearchClicked()
{
    // TODO:
    // 1. 读取fromCityEdit->text()和toCityEdit->text()
    // 2. 读取sortComboBox->currentIndex()确定排序方式
    // 3. 查询数据，排序后填充resultTableView
}
