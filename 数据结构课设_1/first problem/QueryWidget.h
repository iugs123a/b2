#ifndef QUERYWIDGET_H
#define QUERYWIDGET_H
#include "FlightWidget.h"
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTableView>
#include <QComboBox>
#include <QVBoxLayout>

class QueryWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QueryWidget(QWidget *parent = nullptr);

private:
    QLineEdit *fromCityEdit;
    QLineEdit *toCityEdit;
    QComboBox *sortComboBox;      // 新增排序方式选择框
    QPushButton *searchBtn;
    QTableView *resultTableView;
    QVBoxLayout *mainLayout;

    void setupUI();

private slots:
    void onSearchClicked();

};

#endif // QUERYWIDGET_H
