#include "AddFlightDialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

AddFlightDialog::AddFlightDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("添加/编辑航班");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    flightNumberEdit = new QLineEdit;
    airlineEdit = new QLineEdit;
    departureTimeEdit = new QLineEdit;
    arrivalTimeEdit = new QLineEdit;
    stopoverEdit = new QLineEdit;
    ticketLeftEdit = new QLineEdit;
    fromCityEdit = new QLineEdit;
    toCityEdit = new QLineEdit;
    priceEdit = new QLineEdit;

    mainLayout->addWidget(new QLabel("航班号:"));
    mainLayout->addWidget(flightNumberEdit);
    mainLayout->addWidget(new QLabel("航空公司:"));
    mainLayout->addWidget(airlineEdit);
    mainLayout->addWidget(new QLabel("出发时间:"));
    mainLayout->addWidget(departureTimeEdit);
    mainLayout->addWidget(new QLabel("到达时间:"));
    mainLayout->addWidget(arrivalTimeEdit);
    mainLayout->addWidget(new QLabel("经停信息:"));
    mainLayout->addWidget(stopoverEdit);
    mainLayout->addWidget(new QLabel("余票数量:"));
    mainLayout->addWidget(ticketLeftEdit);
    mainLayout->addWidget(new QLabel("出发城市:"));
    mainLayout->addWidget(fromCityEdit);
    mainLayout->addWidget(new QLabel("到达城市:"));
    mainLayout->addWidget(toCityEdit);
    mainLayout->addWidget(new QLabel("票价:"));
    mainLayout->addWidget(priceEdit);

    QPushButton *okButton = new QPushButton("确认");
    QPushButton *cancelButton = new QPushButton("取消");

    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);

    mainLayout->addWidget(okButton);
    mainLayout->addWidget(cancelButton);
}

void AddFlightDialog::setFlightData(const QStringList &data)
{
    if (data.size() >= 9) {  // 你数据大小需要匹配实际字段数
        flightNumberEdit->setText(data[0]);
        airlineEdit->setText(data[1]);
        departureTimeEdit->setText(data[2]);
        arrivalTimeEdit->setText(data[3]);
        stopoverEdit->setText(data[4]);
        ticketLeftEdit->setText(data[5]);
        fromCityEdit->setText(data[6]);
        toCityEdit->setText(data[7]);
        priceEdit->setText(data[8]);
    }
}

QStringList AddFlightDialog::getFlightData() const
{
    return {
        flightNumberEdit->text(),
        airlineEdit->text(),
        departureTimeEdit->text(),
        arrivalTimeEdit->text(),
        stopoverEdit->text(),
        ticketLeftEdit->text(),
        fromCityEdit->text(),
        toCityEdit->text(),
        priceEdit->text()
    };
}
