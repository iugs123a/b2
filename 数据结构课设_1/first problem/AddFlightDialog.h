#ifndef ADDFLIGHTDIALOG_H
#define ADDFLIGHTDIALOG_H

#include <QDialog>
#include <QLineEdit>

class AddFlightDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddFlightDialog(QWidget *parent = nullptr);

    void setFlightData(const QStringList &data);  // 新增此函数
    QStringList getFlightData() const;

private:
    QLineEdit *flightNumberEdit;
    QLineEdit *airlineEdit;
    QLineEdit *departureTimeEdit;
    QLineEdit *arrivalTimeEdit;
    QLineEdit *stopoverEdit;
    QLineEdit *ticketLeftEdit;
    QLineEdit *fromCityEdit;
    QLineEdit *toCityEdit;
    QLineEdit *priceEdit;
};

#endif // ADDFLIGHTDIALOG_H
