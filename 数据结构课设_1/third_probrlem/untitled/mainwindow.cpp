#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenuBar>
#include <QFileDialog>
#include <QPushButton>
#include "GraphManager.h"
#include <QMessageBox>
#include <QHeaderView>
#include <QStandardPaths>
#include <QDir>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    tabWidget = new QTabWidget(this);
    tabWidget->addTab(createTab1(), "路径验证");
    tabWidget->addTab(createTab2(), "最短路径与全部路径");
    tabWidget->addTab(createTab3(), "绕过城市路径查询");
    tabWidget->addTab(createTab4(), "第 K 短路径");

    // 连接第二个选项卡的查询按钮
    auto tab2 = tabWidget->widget(1);
    if (tab2) {
        QPushButton *queryBtn = tab2->findChild<QPushButton *>("queryBtn");
        if (queryBtn) {
            connect(queryBtn, &QPushButton::clicked, this, &MainWindow::onQueryShortestPaths);
        }
    }

    // 连接第三个选项卡的查询按钮
    auto tab3 = tabWidget->widget(2);
    if (tab3) {
        QPushButton *queryBtn = tab3->findChild<QPushButton *>();
        if (queryBtn) {
            connect(queryBtn, &QPushButton::clicked, this, &MainWindow::onQueryAvoidCityPaths);
        }
    }
    // 连接第三个选项卡的查询按钮
    auto tab4 = tabWidget->widget(3);
    if (tab4) {
        QPushButton *queryBtn = tab4->findChild<QPushButton *>();
        if (queryBtn) {
            connect(queryBtn, &QPushButton::clicked, this, &MainWindow::onQueryAvoidCityPaths);
        }
    }
    setCentralWidget(tabWidget);
    setupMenu();
}
QVector<QStringList> MainWindow::findAllPaths(QString start, QString end, int maxNodes) {
    return graphManager.findAllPaths(start, end, maxNodes);
}

double MainWindow::calculatePathLength(const QStringList &path) {
    return graphManager.calculatePathLength(path);
}

void MainWindow::quickSortPaths(QVector<QStringList> &paths) {
    graphManager.quickSortPaths(paths);
}

MainWindow::~MainWindow() {}

void MainWindow::setupMenu() {
    QMenu *fileMenu = menuBar()->addMenu("文件");

    QAction *loadAction = new QAction("加载城市距离数据...", this);
    connect(loadAction, &QAction::triggered, this, &MainWindow::onLoadData);
    fileMenu->addAction(loadAction);

    QAction *loadAdjacencyAction = new QAction("加载城市邻接数据...", this);
    connect(loadAdjacencyAction, &QAction::triggered, this, &MainWindow::onLoadAdjacencyData);
    fileMenu->addAction(loadAdjacencyAction);

    fileMenu->addAction("保存结果...");
    fileMenu->addSeparator();
    fileMenu->addAction("退出", this, &QMainWindow::close);

    QMenu *algoMenu = menuBar()->addMenu("算法");
    algoMenu->addAction("使用 Dijkstra");
    algoMenu->addAction("使用 Floyd");

    QMenu *aboutMenu = menuBar()->addMenu("关于");
    aboutMenu->addAction("系统说明");
}

QWidget* MainWindow::createTab1() {
    QWidget *tab = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(tab);

    QHBoxLayout *hLayout = new QHBoxLayout;
    QLabel *label = new QLabel("选择省会城市：");
    QComboBox *cityBox = new QComboBox;
    cityBox->setObjectName("provinceCityBox");
    QPushButton *checkButton = new QPushButton("验证");
    connect(checkButton, &QPushButton::clicked, this, &MainWindow::onCheckProvinceDistance);

    hLayout->addWidget(label);
    hLayout->addWidget(cityBox);
    hLayout->addWidget(checkButton);

    QTextEdit *resultEdit = new QTextEdit;
    resultEdit->setReadOnly(true);
    resultEdit->setObjectName("provinceResultEdit");

    layout->addLayout(hLayout);
    layout->addWidget(resultEdit);

    return tab;
}

QWidget* MainWindow::createTab2() {
    QWidget *tab = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(tab);

    // 输入控件区域
    QHBoxLayout *inputLayout = new QHBoxLayout;
    QLabel *startLabel = new QLabel("起点：");
    QComboBox *startBox = new QComboBox;
    startBox->setObjectName("startBox");

    QLabel *endLabel = new QLabel("终点：");
    QComboBox *endBox = new QComboBox;
    endBox->setObjectName("endBox");

    QRadioButton *dijkstraBtn = new QRadioButton("Dijkstra");
    QRadioButton *floydBtn = new QRadioButton("Floyd");
    dijkstraBtn->setObjectName("dijkstraBtn");
    floydBtn->setObjectName("floydBtn");
    dijkstraBtn->setChecked(true);

    QPushButton *queryBtn = new QPushButton("查询");
    queryBtn->setObjectName("queryBtn");

    inputLayout->addWidget(startLabel);
    inputLayout->addWidget(startBox);
    inputLayout->addWidget(endLabel);
    inputLayout->addWidget(endBox);
    inputLayout->addWidget(dijkstraBtn);
    inputLayout->addWidget(floydBtn);
    inputLayout->addWidget(queryBtn);

    // 最短路径结果显示区域
    QLabel *label1 = new QLabel("最短路径结果：");
    QTextEdit *shortestResult = new QTextEdit;
    shortestResult->setReadOnly(true);
    shortestResult->setObjectName("shortestResult");
    shortestResult->setMaximumHeight(120);

    // 所有路径表格显示区域
    QLabel *label2 = new QLabel("所有可行路径（按总长度排序）：");
    QTableWidget *allPathsTable = new QTableWidget;
    allPathsTable->setObjectName("allPathsTable");
    allPathsTable->setColumnCount(3);
    QStringList headers;
    headers << "序号" << "路径" << "总长度";
    allPathsTable->setHorizontalHeaderLabels(headers);
    allPathsTable->horizontalHeader()->setStretchLastSection(true);
    allPathsTable->setAlternatingRowColors(true);
    allPathsTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    layout->addLayout(inputLayout);
    layout->addWidget(label1);
    layout->addWidget(shortestResult);
    layout->addWidget(label2);
    layout->addWidget(allPathsTable);

    return tab;
}

// MainWindow.cpp 中需要修改的 createTab3 函数
QWidget* MainWindow::createTab3() {
    QWidget *tab = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(tab);

    QHBoxLayout *inputLayout = new QHBoxLayout;
    QLabel *startLabel = new QLabel("起点：");
    QComboBox *startBox = new QComboBox;
    startBox->setObjectName("avoidStartBox");

    QLabel *endLabel = new QLabel("终点：");
    QComboBox *endBox = new QComboBox;
    endBox->setObjectName("avoidEndBox");

    QLabel *avoidLabel = new QLabel("绕过城市：");
    QComboBox *avoidBox = new QComboBox;
    avoidBox->setObjectName("avoidCityBox");

    QPushButton *queryBtn = new QPushButton("查询");
    queryBtn->setObjectName("avoidQueryBtn");

    inputLayout->addWidget(startLabel);
    inputLayout->addWidget(startBox);
    inputLayout->addWidget(endLabel);
    inputLayout->addWidget(endBox);
    inputLayout->addWidget(avoidLabel);
    inputLayout->addWidget(avoidBox);
    inputLayout->addWidget(queryBtn);

    // 最短路径结果显示区域
    QLabel *shortestLabel = new QLabel("绕过城市的最短路径：");
    QTextEdit *shortestResult = new QTextEdit;
    shortestResult->setReadOnly(true);
    shortestResult->setObjectName("avoidShortestResult");
    shortestResult->setMaximumHeight(120);

    // 所有路径表格显示区域
    QLabel *allPathsLabel = new QLabel("所有绕过该城市的路径（按总长度排序）：");
    QTableWidget *allPathsTable = new QTableWidget;
    allPathsTable->setObjectName("avoidAllPathsTable");
    allPathsTable->setColumnCount(3);
    QStringList headers;
    headers << "序号" << "路径" << "总长度";
    allPathsTable->setHorizontalHeaderLabels(headers);
    allPathsTable->horizontalHeader()->setStretchLastSection(true);
    allPathsTable->setAlternatingRowColors(true);
    allPathsTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    layout->addLayout(inputLayout);
    layout->addWidget(shortestLabel);
    layout->addWidget(shortestResult);
    layout->addWidget(allPathsLabel);
    layout->addWidget(allPathsTable);

    return tab;
}
// MainWindow.cpp 中新增的槽函数实现
void MainWindow::onQueryAvoidCityPaths() {
    QWidget *tab3 = tabWidget->widget(2);
    if (!tab3) return;

    // 获取界面控件
    QComboBox *startBox = tab3->findChild<QComboBox *>("avoidStartBox");
    QComboBox *endBox = tab3->findChild<QComboBox *>("avoidEndBox");
    QComboBox *avoidBox = tab3->findChild<QComboBox *>("avoidCityBox");
    QTextEdit *shortestResult = tab3->findChild<QTextEdit *>("avoidShortestResult");
    QTableWidget *allPathsTable = tab3->findChild<QTableWidget *>("avoidAllPathsTable");

    if (!startBox || !endBox || !avoidBox || !shortestResult || !allPathsTable) {
        return;
    }

    QString start = startBox->currentText();
    QString end = endBox->currentText();
    QString avoidCity = avoidBox->currentText();

    // 检查输入有效性
    if (start.isEmpty() || end.isEmpty() || avoidCity.isEmpty()) {
        shortestResult->setText("请选择起点、终点和要绕过的城市！");
        return;
    }

    if (start == end) {
        shortestResult->setText("起点和终点不能相同！");
        return;
    }

    if (start == avoidCity) {
        shortestResult->setText("起点不能是要绕过的城市！");
        return;
    }

    if (end == avoidCity) {
        shortestResult->setText("终点不能是要绕过的城市！");
        return;
    }

    // 清空之前的结果
    allPathsTable->setRowCount(0);

    // 1. 计算绕过指定城市的最短路径（两种算法）
    double dijkstraDistance = 0.0, floydDistance = 0.0;
    QStringList dijkstraPath = graphManager.dijkstraShortestPathAvoidCity(start, end, avoidCity, dijkstraDistance);
    QStringList floydPath = graphManager.floydShortestPathAvoidCity(start, end, avoidCity, floydDistance);

    // 2. 计算不绕过城市的最短路径（用于对比）
    double normalDijkstraDistance = 0.0;
    QStringList normalDijkstraPath = graphManager.dijkstraShortestPath(start, end, normalDijkstraDistance);

    // 3. 查找所有绕过指定城市的路径
    QVector<QStringList> allAvoidPaths = graphManager.findAllPathsAvoidCity(start, end, avoidCity, 10);

    // 4. 计算每条路径的长度并排序
    QVector<QPair<QStringList, double>> pathsWithDistance;
    for (const auto& path : allAvoidPaths) {
        double distance = graphManager.calculatePathLength(path);
        if (distance > 0) {
            pathsWithDistance.append({path, distance});
        }
    }

    // 按距离排序
    std::sort(pathsWithDistance.begin(), pathsWithDistance.end(),
              [](const QPair<QStringList, double>& a, const QPair<QStringList, double>& b) {
                  return a.second < b.second;
              });

    // 5. 显示最短路径结果
    QString resultText = QString("=== 绕过城市 \"%1\" 的路径查询结果 ===\n\n").arg(avoidCity);

    // 正常路径对比
    if (!normalDijkstraPath.isEmpty()) {
        resultText += QString("正常最短路径（不绕过任何城市）：\n路径：%1\n总距离：%2\n")
                          .arg(normalDijkstraPath.join(" -> "))
                          .arg(normalDijkstraDistance, 0, 'f', 2);

        // 检查正常路径是否经过要绕过的城市
        if (normalDijkstraPath.contains(avoidCity)) {
            resultText += QString("注意：正常最短路径经过了要绕过的城市 \"%1\"\n\n").arg(avoidCity);
        } else {
            resultText += QString("注意：正常最短路径本身就不经过城市 \"%1\"\n\n").arg(avoidCity);
        }
    }

    // 绕过城市的路径结果
    if (!dijkstraPath.isEmpty()) {
        resultText += QString("Dijkstra算法（绕过%1）：\n路径：%2\n总距离：%3\n\n")
                          .arg(avoidCity)
                          .arg(dijkstraPath.join(" -> "))
                          .arg(dijkstraDistance, 0, 'f', 2);
    } else {
        resultText += QString("Dijkstra算法：绕过%1后未找到路径\n\n").arg(avoidCity);
    }

    if (!floydPath.isEmpty()) {
        resultText += QString("Floyd算法（绕过%1）：\n路径：%2\n总距离：%3\n\n")
                          .arg(avoidCity)
                          .arg(floydPath.join(" -> "))
                          .arg(floydDistance, 0, 'f', 2);
    } else {
        resultText += QString("Floyd算法：绕过%1后未找到路径\n\n").arg(avoidCity);
    }

    // 算法结果比较
    if (!dijkstraPath.isEmpty() && !floydPath.isEmpty()) {
        if (qAbs(dijkstraDistance - floydDistance) < 0.001) {
            resultText += "✓ 两种算法得到相同的绕过路径距离\n";
            if (dijkstraPath == floydPath) {
                resultText += "✓ 两种算法得到相同的绕过路径\n";
            } else {
                resultText += "! 两种算法得到不同的绕过路径（但距离相同）\n";
            }
        } else {
            resultText += "✗ 两种算法得到不同的绕过路径结果！\n";
        }
    }

    // 距离增加分析
    if (!dijkstraPath.isEmpty() && !normalDijkstraPath.isEmpty() && normalDijkstraPath.contains(avoidCity)) {
        double extraDistance = dijkstraDistance - normalDijkstraDistance;
        double extraPercent = (extraDistance / normalDijkstraDistance) * 100;
        resultText += QString("\n绕过城市后的距离增加：%1 (增加了 %2%)")
                          .arg(extraDistance, 0, 'f', 2)
                          .arg(extraPercent, 0, 'f', 1);
    }

    resultText += QString("\n\n共找到 %1 条绕过城市的有效路径").arg(pathsWithDistance.size());

    shortestResult->setText(resultText);

    // 6. 显示所有绕过路径到表格中
    allPathsTable->setRowCount(pathsWithDistance.size());

    for (int i = 0; i < pathsWithDistance.size(); ++i) {
        const QStringList& path = pathsWithDistance[i].first;
        double distance = pathsWithDistance[i].second;

        // 序号
        QTableWidgetItem *indexItem = new QTableWidgetItem(QString::number(i + 1));
        indexItem->setTextAlignment(Qt::AlignCenter);
        allPathsTable->setItem(i, 0, indexItem);

        // 路径
        QString pathStr = path.join(" -> ");
        QTableWidgetItem *pathItem = new QTableWidgetItem(pathStr);

        // 标记特殊路径
        if (!dijkstraPath.isEmpty() && qAbs(distance - dijkstraDistance) < 0.001) {
            pathItem->setBackground(QColor(144, 238, 144)); // 浅绿色 - Dijkstra结果
            pathItem->setToolTip("与Dijkstra算法结果一致");
        }
        if (!floydPath.isEmpty() && qAbs(distance - floydDistance) < 0.001) {
            if (pathItem->background().color() == QColor(144, 238, 144)) {
                pathItem->setBackground(QColor(255, 215, 0)); // 金色 - 两种算法都一致
                pathItem->setToolTip("与Dijkstra和Floyd算法结果都一致");
            } else {
                pathItem->setBackground(QColor(173, 216, 230)); // 浅蓝色 - Floyd结果
                pathItem->setToolTip("与Floyd算法结果一致");
            }
        }

        allPathsTable->setItem(i, 1, pathItem);

        // 总长度
        QTableWidgetItem *distanceItem = new QTableWidgetItem(QString::number(distance, 'f', 2));
        distanceItem->setTextAlignment(Qt::AlignCenter);
        allPathsTable->setItem(i, 2, distanceItem);
    }

    // 调整列宽
    allPathsTable->resizeColumnsToContents();
    allPathsTable->horizontalHeader()->setStretchLastSection(true);

    // 7. 保存结果到文件
    saveAvoidCityResultsToFile(start, end, avoidCity,
                               dijkstraPath, dijkstraDistance,
                               floydPath, floydDistance,
                               normalDijkstraPath, normalDijkstraDistance,
                               pathsWithDistance);
}
// 保存绕过城市查询结果的函数
void MainWindow::saveAvoidCityResultsToFile(const QString& start, const QString& end, const QString& avoidCity,
                                            const QStringList& dijkstraPath, double dijkstraDistance,
                                            const QStringList& floydPath, double floydDistance,
                                            const QStringList& normalPath, double normalDistance,
                                            const QVector<QPair<QStringList, double>>& sortedPaths) {

    QString documentsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString fileName = QString("绕过城市路径查询_%1到%2绕过%3_%4.txt")
                           .arg(start, end, avoidCity, QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"));
    QString filePath = QDir(documentsPath).filePath(fileName);

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "保存失败", "无法创建输出文件：" + filePath);
        return;
    }

    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);

    // 文件头信息
    out << "========================================\n";
    out << "绕过城市路径查询结果报告\n";
    out << "========================================\n";
    out << "起点: " << start << "\n";
    out << "终点: " << end << "\n";
    out << "绕过城市: " << avoidCity << "\n";
    out << "查询时间: " << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "\n";
    out << "========================================\n\n";

    // 正常路径对比
    out << "1. 正常最短路径（不绕过任何城市）:\n";
    if (!normalPath.isEmpty()) {
        out << "   路径: " << normalPath.join(" -> ") << "\n";
        out << "   总距离: " << normalDistance << "\n";
        if (normalPath.contains(avoidCity)) {
            out << "   注意: 此路径经过了要绕过的城市 \"" << avoidCity << "\"\n";
        } else {
            out << "   注意: 此路径本身就不经过城市 \"" << avoidCity << "\"\n";
        }
    } else {
        out << "   未找到路径\n";
    }
    out << "\n";

    // 绕过城市的算法结果
    out << "2. 绕过城市的算法结果:\n";
    out << "   Dijkstra算法结果:\n";
    if (!dijkstraPath.isEmpty()) {
        out << "     路径: " << dijkstraPath.join(" -> ") << "\n";
        out << "     总距离: " << dijkstraDistance << "\n";
    } else {
        out << "     未找到路径\n";
    }

    out << "   Floyd算法结果:\n";
    if (!floydPath.isEmpty()) {
        out << "     路径: " << floydPath.join(" -> ") << "\n";
        out << "     总距离: " << floydDistance << "\n";
    } else {
        out << "     未找到路径\n";
    }
    out << "\n";

    // 距离增加分析
    out << "3. 绕过城市影响分析:\n";
    if (!dijkstraPath.isEmpty() && !normalPath.isEmpty() && normalPath.contains(avoidCity)) {
        double extraDistance = dijkstraDistance - normalDistance;
        double extraPercent = (extraDistance / normalDistance) * 100;
        out << "   绕过城市后距离增加: " << extraDistance << " (" << QString::number(extraPercent, 'f', 1) << "%)\n";
        out << "   绕过前距离: " << normalDistance << "\n";
        out << "   绕过后距离: " << dijkstraDistance << "\n";
    } else if (!normalPath.isEmpty() && !normalPath.contains(avoidCity)) {
        out << "   正常路径本身就不经过要绕过的城市，无需绕过\n";
    } else {
        out << "   无法进行对比分析\n";
    }
    out << "\n";

    // 所有绕过路径结果
    out << "4. 所有绕过城市的可行路径（按距离排序）:\n";
    out << "   总计找到 " << sortedPaths.size() << " 条有效路径\n\n";

    for (int i = 0; i < sortedPaths.size(); ++i) {
        const QStringList& path = sortedPaths[i].first;
        double distance = sortedPaths[i].second;

        out << QString("   路径 %1: ").arg(i + 1, 2);
        out << path.join(" -> ");
        out << QString(" (距离: %1)").arg(distance, 0, 'f', 2);

        // 标记特殊路径
        QStringList tags;
        if (i == 0) {
            tags << "最短绕过路径";
        }
        if (!dijkstraPath.isEmpty() && qAbs(distance - dijkstraDistance) < 0.001) {
            tags << "Dijkstra结果";
        }
        if (!floydPath.isEmpty() && qAbs(distance - floydDistance) < 0.001) {
            tags << "Floyd结果";
        }

        if (!tags.isEmpty()) {
            out << " [" << tags.join(", ") << "]";
        }

        out << "\n";
    }

    out << "\n========================================\n";
    out << "报告生成完成\n";
    out << "========================================\n";

    file.close();

    QMessageBox::information(this, "保存成功",
                             QString("绕过城市查询结果已保存到文件:\n%1").arg(filePath));
}

QWidget* MainWindow::createTab4() {
    QWidget *tab = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(tab);

    QHBoxLayout *inputLayout = new QHBoxLayout;
    QLabel *startLabel = new QLabel("起点：");
    QComboBox *startBox = new QComboBox;
    startBox->setObjectName("kStartBox");

    QLabel *endLabel = new QLabel("终点：");
    QComboBox *endBox = new QComboBox;
    endBox->setObjectName("kEndBox");

    QLabel *kLabel = new QLabel("K值：");
    QSpinBox *kSpin = new QSpinBox;
    kSpin->setObjectName("kSpin");
    kSpin->setMinimum(1);
    kSpin->setMaximum(100);

    QPushButton *queryBtn = new QPushButton("查询");

    inputLayout->addWidget(startLabel);
    inputLayout->addWidget(startBox);
    inputLayout->addWidget(endLabel);
    inputLayout->addWidget(endBox);
    inputLayout->addWidget(kLabel);
    inputLayout->addWidget(kSpin);
    inputLayout->addWidget(queryBtn);

    QTableWidget *resultTable = new QTableWidget;
    resultTable->setObjectName("kResultTable");

    layout->addLayout(inputLayout);
    layout->addWidget(resultTable);

    // 信号连接
    connect(queryBtn, &QPushButton::clicked, this, &MainWindow::onQueryKthShortestPath);

    return tab;
}
void MainWindow::onQueryKthShortestPath() {
    QComboBox *startBox = findChild<QComboBox*>("kStartBox");
    QComboBox *endBox = findChild<QComboBox*>("kEndBox");
    QSpinBox *kSpin = findChild<QSpinBox*>("kSpin");
    QTableWidget *resultTable = findChild<QTableWidget*>("kResultTable");

    if (!startBox || !endBox || !kSpin || !resultTable) return;

    QString start = startBox->currentText();
    QString end = endBox->currentText();
    int K = kSpin->value();

    // 清空旧表格
    resultTable->clear();
    resultTable->setRowCount(0);
    resultTable->setColumnCount(2);
    resultTable->setHorizontalHeaderLabels({"路径", "总长度"});

    // 调用 GraphManager 的 K 最短路径算法
    std::vector<Path> paths = graphManager.yenKShortestPaths(start, end, K);

    for (int i = 0; i < paths.size(); ++i) {
        const Path& p = paths[i];
        QString pathStr = p.nodes.join(" -> ");
        resultTable->insertRow(i);
        resultTable->setItem(i, 0, new QTableWidgetItem(pathStr));
        resultTable->setItem(i, 1, new QTableWidgetItem(QString::number(p.totalDistance)));
    }
}


void MainWindow::onQueryShortestPaths() {
    QWidget *tab2 = tabWidget->widget(1);
    if (!tab2) return;

    // 获取界面控件
    QComboBox *startBox = tab2->findChild<QComboBox *>("startBox");
    QComboBox *endBox = tab2->findChild<QComboBox *>("endBox");
    QTextEdit *shortestResult = tab2->findChild<QTextEdit *>("shortestResult");
    QRadioButton *dijkstraBtn = tab2->findChild<QRadioButton *>("dijkstraBtn");
    QRadioButton *floydBtn = tab2->findChild<QRadioButton *>("floydBtn");
    QTableWidget *allPathsTable = tab2->findChild<QTableWidget *>("allPathsTable");

    if (!startBox || !endBox || !shortestResult || !dijkstraBtn || !floydBtn || !allPathsTable)
        return;

    QString start = startBox->currentText();
    QString end = endBox->currentText();

    // 检查输入有效性
    if (start.isEmpty() || end.isEmpty()) {
        shortestResult->setText("请选择起点和终点！");
        return;
    }

    if (start == end) {
        shortestResult->setText("起点和终点不能相同！");
        return;
    }

    // 清空之前的结果
    allPathsTable->setRowCount(0);

    // 1. 计算两种算法的最短路径
    double dijkstraDistance = 0.0, floydDistance = 0.0;
    QStringList dijkstraPath = graphManager.dijkstraShortestPath(start, end, dijkstraDistance);
    QStringList floydPath = graphManager.floydShortestPath(start, end, floydDistance);

    // 2. 查找所有可行路径（限制最多经过10个节点）
    QVector<QStringList> allPaths = graphManager.findAllPaths(start, end, 10);

    // 3. 计算每条路径的长度并排序
    QVector<QPair<QStringList, double>> pathsWithDistance;
    for (const auto& path : allPaths) {
        double distance = graphManager.calculatePathLength(path);
        if (distance > 0) { // 只保留有效路径
            pathsWithDistance.append({path, distance});
        }
    }

    // 按距离排序（快速排序）
    std::sort(pathsWithDistance.begin(), pathsWithDistance.end(),
              [](const QPair<QStringList, double>& a, const QPair<QStringList, double>& b) {
                  return a.second < b.second;
              });

    // 4. 显示最短路径结果
    QString resultText = "=== 最短路径算法结果比较 ===\n\n";

    if (!dijkstraPath.isEmpty()) {
        resultText += QString("Dijkstra算法结果：\n路径：%1\n总距离：%2\n\n")
                          .arg(dijkstraPath.join(" -> "))
                          .arg(dijkstraDistance, 0, 'f', 2);
    } else {
        resultText += "Dijkstra算法：未找到路径\n\n";
    }

    if (!floydPath.isEmpty()) {
        resultText += QString("Floyd算法结果：\n路径：%1\n总距离：%2\n\n")
                          .arg(floydPath.join(" -> "))
                          .arg(floydDistance, 0, 'f', 2);
    } else {
        resultText += "Floyd算法：未找到路径\n\n";
    }

    // 算法结果比较
    if (!dijkstraPath.isEmpty() && !floydPath.isEmpty()) {
        if (qAbs(dijkstraDistance - floydDistance) < 0.001) {
            resultText += "✓ 两种算法得到相同的最短距离\n";
            if (dijkstraPath == floydPath) {
                resultText += "✓ 两种算法得到相同的最短路径\n";
            } else {
                resultText += "! 两种算法得到不同的最短路径（但距离相同）\n";
            }
        } else {
            resultText += "✗ 两种算法得到不同的结果！\n";
        }
    }

    resultText += QString("\n共找到 %1 条有效路径").arg(pathsWithDistance.size());

    // 显示当前选择的算法结果
    if (dijkstraBtn->isChecked() && !dijkstraPath.isEmpty()) {
        resultText += QString("\n\n当前选择：Dijkstra算法\n路径：%1\n总距离：%2")
                          .arg(dijkstraPath.join(" -> "))
                          .arg(dijkstraDistance, 0, 'f', 2);
    } else if (floydBtn->isChecked() && !floydPath.isEmpty()) {
        resultText += QString("\n\n当前选择：Floyd算法\n路径：%1\n总距离：%2")
                          .arg(floydPath.join(" -> "))
                          .arg(floydDistance, 0, 'f', 2);
    }

    shortestResult->setText(resultText);

    // 5. 显示所有路径到表格中
    allPathsTable->setRowCount(pathsWithDistance.size());

    for (int i = 0; i < pathsWithDistance.size(); ++i) {
        const QStringList& path = pathsWithDistance[i].first;
        double distance = pathsWithDistance[i].second;

        // 序号
        QTableWidgetItem *indexItem = new QTableWidgetItem(QString::number(i + 1));
        indexItem->setTextAlignment(Qt::AlignCenter);
        allPathsTable->setItem(i, 0, indexItem);

        // 路径
        QString pathStr = path.join(" -> ");
        QTableWidgetItem *pathItem = new QTableWidgetItem(pathStr);

        // 标记特殊路径
        if (!dijkstraPath.isEmpty() && qAbs(distance - dijkstraDistance) < 0.001) {
            pathItem->setBackground(QColor(144, 238, 144)); // 浅绿色 - Dijkstra最短路径
            pathItem->setToolTip("与Dijkstra算法结果一致");
        }
        if (!floydPath.isEmpty() && qAbs(distance - floydDistance) < 0.001) {
            if (pathItem->background().color() == QColor(144, 238, 144)) {
                pathItem->setBackground(QColor(255, 215, 0)); // 金色 - 两种算法都一致
                pathItem->setToolTip("与Dijkstra和Floyd算法结果都一致");
            } else {
                pathItem->setBackground(QColor(173, 216, 230)); // 浅蓝色 - Floyd最短路径
                pathItem->setToolTip("与Floyd算法结果一致");
            }
        }

        allPathsTable->setItem(i, 1, pathItem);

        // 总长度
        QTableWidgetItem *distanceItem = new QTableWidgetItem(QString::number(distance, 'f', 2));
        distanceItem->setTextAlignment(Qt::AlignCenter);
        allPathsTable->setItem(i, 2, distanceItem);
    }

    // 调整列宽
    allPathsTable->resizeColumnsToContents();
    allPathsTable->horizontalHeader()->setStretchLastSection(true);

    // 6. 输出结果到文件
    saveResultsToFile(start, end, dijkstraPath, dijkstraDistance,
                      floydPath, floydDistance, pathsWithDistance);
}

void MainWindow::saveResultsToFile(const QString& start, const QString& end,
                                   const QStringList& dijkstraPath, double dijkstraDistance,
                                   const QStringList& floydPath, double floydDistance,
                                   const QVector<QPair<QStringList, double>>& sortedPaths) {

    QString documentsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString fileName = QString("路径查询结果_%1到%2_%3.txt")
                           .arg(start, end, QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"));
    QString filePath = QDir(documentsPath).filePath(fileName);

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "保存失败", "无法创建输出文件：" + filePath);
        return;
    }

    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);

    // 文件头信息
    out << "========================================\n";
    out << "城市路径查询结果报告\n";
    out << "========================================\n";
    out << "起点: " << start << "\n";
    out << "终点: " << end << "\n";
    out << "查询时间: " << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "\n";
    out << "========================================\n\n";

    // Dijkstra算法结果
    out << "1. Dijkstra算法结果:\n";
    if (!dijkstraPath.isEmpty()) {
        out << "   路径: " << dijkstraPath.join(" -> ") << "\n";
        out << "   总距离: " << dijkstraDistance << "\n";
    } else {
        out << "   未找到路径\n";
    }
    out << "\n";

    // Floyd算法结果
    out << "2. Floyd算法结果:\n";
    if (!floydPath.isEmpty()) {
        out << "   路径: " << floydPath.join(" -> ") << "\n";
        out << "   总距离: " << floydDistance << "\n";
    } else {
        out << "   未找到路径\n";
    }
    out << "\n";

    // 算法结果比较
    out << "3. 算法结果比较:\n";
    if (!dijkstraPath.isEmpty() && !floydPath.isEmpty()) {
        if (qAbs(dijkstraDistance - floydDistance) < 0.001) {
            out << "   ✓ 两种算法得到相同的最短距离: " << dijkstraDistance << "\n";
            if (dijkstraPath == floydPath) {
                out << "   ✓ 两种算法得到相同的最短路径\n";
            } else {
                out << "   ! 两种算法得到不同的最短路径（但距离相同）\n";
                out << "     Dijkstra路径: " << dijkstraPath.join(" -> ") << "\n";
                out << "     Floyd路径: " << floydPath.join(" -> ") << "\n";
            }
        } else {
            out << "   ✗ 两种算法得到不同的结果！\n";
            out << "     Dijkstra距离: " << dijkstraDistance << "\n";
            out << "     Floyd距离: " << floydDistance << "\n";
        }
    } else if (dijkstraPath.isEmpty() && floydPath.isEmpty()) {
        out << "   两种算法都未找到路径\n";
    } else {
        out << "   只有一种算法找到了路径\n";
    }
    out << "\n";

    // 所有路径结果（按距离排序）
    out << "4. 所有可行路径（快速排序后，按距离升序排列）:\n";
    out << "   总计找到 " << sortedPaths.size() << " 条有效路径\n";
    out << "   （限制：最多经过10个节点）\n\n";

    for (int i = 0; i < sortedPaths.size(); ++i) {
        const QStringList& path = sortedPaths[i].first;
        double distance = sortedPaths[i].second;

        out << QString("   路径 %1: ").arg(i + 1, 2);
        out << path.join(" -> ");
        out << QString(" (距离: %1)").arg(distance, 0, 'f', 2);

        // 标记特殊路径
        QStringList tags;
        if (i == 0) {
            tags << "最短路径";
        }
        if (!dijkstraPath.isEmpty() && qAbs(distance - dijkstraDistance) < 0.001) {
            tags << "Dijkstra结果";
        }
        if (!floydPath.isEmpty() && qAbs(distance - floydDistance) < 0.001) {
            tags << "Floyd结果";
        }

        if (!tags.isEmpty()) {
            out << " [" << tags.join(", ") << "]";
        }

        out << "\n";
    }

    // 统计信息
    if (!sortedPaths.isEmpty()) {
        out << "\n5. 统计信息:\n";
        out << "   最短距离: " << sortedPaths.first().second << "\n";
        out << "   最长距离: " << sortedPaths.last().second << "\n";

        double avgDistance = 0.0;
        for (const auto& pair : sortedPaths) {
            avgDistance += pair.second;
        }
        avgDistance /= sortedPaths.size();
        out << "   平均距离: " << QString::number(avgDistance, 'f', 2) << "\n";

        // 路径长度分布
        QMap<int, int> lengthDistribution;
        for (const auto& pair : sortedPaths) {
            lengthDistribution[pair.first.size()]++;
        }
        out << "   路径节点数分布:\n";
        for (auto it = lengthDistribution.begin(); it != lengthDistribution.end(); ++it) {
            out << QString("     %1个节点: %2条路径\n").arg(it.key()).arg(it.value());
        }
    }

    out << "\n========================================\n";
    out << "报告生成完成\n";
    out << "文件保存位置: " << filePath << "\n";
    out << "========================================\n";

    file.close();

    // 显示保存成功消息
    QMessageBox::information(this, "保存成功",
                             QString("查询结果已保存到文件:\n%1").arg(filePath));
}

void MainWindow::onCheckProvinceDistance() {
    QWidget *tab1 = tabWidget->widget(0);
    if (!tab1) return;

    QComboBox *cityBox = tab1->findChild<QComboBox *>("provinceCityBox");
    QTextEdit *resultEdit = tab1->findChild<QTextEdit *>("provinceResultEdit");

    if (!cityBox || !resultEdit) return;

    QString startCity = cityBox->currentText();
    QString endCity = "武汉";

    if (startCity == endCity) {
        resultEdit->setText("起点和终点均为武汉，无需验证。");
        return;
    }

    QStringList path;
    bool found = graphManager.bfsPath(startCity, endCity, path);



    if (!found) {
        resultEdit->setText(QString("从 %1 到 %2 无可达路径。").arg(startCity, endCity));
        return;
    }

    int middleCount = path.size() > 2 ? path.size() - 2 : 0;

    if (middleCount <= 2) {
        resultEdit->setText(QString("验证成立：从 %1 到武汉的路径中，中间省会城市不超过2个。\n路径为：%2")
                                .arg(startCity, path.join(" -> ")));
    } else {
        resultEdit->setText(QString("验证不成立：从 %1 到武汉的路径中，中间省会城市超过2个。\n路径为：%2")
                                .arg(startCity, path.join(" -> ")));
    }
}

void MainWindow::updateCityComboBoxes() {
    QStringList cities = graphManager.getCityList();

    // 路径验证tab
    auto tab1 = tabWidget->widget(0);
    if (tab1) {
        QComboBox *cityBox = tab1->findChild<QComboBox *>("provinceCityBox");
        if (cityBox) {
            cityBox->clear();
            cityBox->addItems(cities);
        }
    }

    // 最短路径tab
    auto tab2 = tabWidget->widget(1);
    if (tab2) {
        QComboBox *startBox = tab2->findChild<QComboBox *>("startBox");
        QComboBox *endBox = tab2->findChild<QComboBox *>("endBox");
        if (startBox && endBox) {
            startBox->clear();
            startBox->addItems(cities);
            endBox->clear();
            endBox->addItems(cities);
        }
    }

    // 绕过城市路径查询tab
    auto tab3 = tabWidget->widget(2);
    if (tab3) {
        QComboBox *startBox = tab3->findChild<QComboBox *>("avoidStartBox");
        QComboBox *endBox = tab3->findChild<QComboBox *>("avoidEndBox");
        QComboBox *avoidBox = tab3->findChild<QComboBox *>("avoidCityBox");
        if (startBox && endBox && avoidBox) {
            startBox->clear();
            startBox->addItems(cities);
            endBox->clear();
            endBox->addItems(cities);
            avoidBox->clear();
            avoidBox->addItems(cities);
        }
    }

    // 第K短路径tab
    auto tab4 = tabWidget->widget(3);
    if (tab4) {
        QComboBox *startBox = tab4->findChild<QComboBox *>("kStartBox");
        QComboBox *endBox = tab4->findChild<QComboBox *>("kEndBox");
        if (startBox && endBox) {
            startBox->clear();
            startBox->addItems(cities);
            endBox->clear();
            endBox->addItems(cities);
        }
    }
}

void MainWindow::onLoadData() {
    QString filePath = QFileDialog::getOpenFileName(this, "选择CSV文件", "", "CSV Files (*.csv)");
    if (filePath.isEmpty())
        return;

    bool success = graphManager.loadDistanceFromCSV(filePath);
    if (success) {
        QMessageBox::information(this, "加载成功", "城市数据加载成功！");
        updateCityComboBoxes();
    } else {
        QMessageBox::warning(this, "加载失败", "无法加载城市数据，请检查文件格式。");
    }
}

void MainWindow::onLoadAdjacencyData() {
    QString filePath = QFileDialog::getOpenFileName(this, "选择邻接数据CSV文件", "", "CSV Files (*.csv)");
    if (filePath.isEmpty())
        return;

    bool success = graphManager.loadAdjacencyFromCSV(filePath);
    if (success) {
        QMessageBox::information(this, "加载成功", "邻接数据加载成功！");
        updateCityComboBoxes();
    } else {
        QMessageBox::warning(this, "加载失败", "无法加载邻接数据，请检查文件格式。");
    }
}
