#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QComboBox>
#include <QTextEdit>
#include <QTableWidget>
#include <QSpinBox>
#include <QRadioButton>
#include "graphmanager.h"  // 正确包含 graphmanager，而不是 Graph

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QTabWidget *tabWidget;

    GraphManager graphManager;  // ✅ 正确使用 GraphManager 管理图结构

    void setupMenu();
    QWidget* createTab1();
    QWidget* createTab2();
    QWidget* createTab3();
    QWidget* createTab4();

    // 你还可以声明槽函数用于响应菜单或按钮点击等
private slots:
    void onLoadData();  // 添加槽函数声明
    void updateCityComboBoxes();
    void onLoadAdjacencyData();  // ← 添加这个
    void onCheckProvinceDistance() ;
    void onQueryShortestPaths() ;
    // GraphManager.h 中声明
    QVector<QStringList> findAllPaths(QString start, QString end, int maxNodes);
    void quickSortPaths(QVector<QStringList>& paths);
    double calculatePathLength(const QStringList& path);
    void saveResultsToFile(const QString& start, const QString& end,
                                       const QStringList& dijkstraPath, double dijkstraDistance,
                                       const QStringList& floydPath, double floydDistance,
                           const QVector<QPair<QStringList, double>>& sortedPaths) ;
    void saveAvoidCityResultsToFile(const QString& start, const QString& end, const QString& avoidCity,
                                                const QStringList& dijkstraPath, double dijkstraDistance,
                                                const QStringList& floydPath, double floydDistance,
                                                const QStringList& normalPath, double normalDistance,
                                    const QVector<QPair<QStringList, double>>& sortedPaths) ;
private slots:
    void onQueryAvoidCityPaths(); // 绕过城市路径查询
private slots:
    void onQueryKthShortestPath();  // 查询第K短路径

};

#endif // MAINWINDOW_H
