#include "graphmanager.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <algorithm>
#include <QQueue>
#include <queue>
#include <QStandardPaths>
#include <QDir>
// 1. 首先添加一个完整的数据加载诊断函数
void GraphManager::diagnoseDataLoading() {
    qDebug() << "\n========== 数据加载诊断 ==========";

    // 检查城市列表
    qDebug() << "1. 城市列表检查：";
    qDebug() << "   城市总数：" << cityList.size();
    if (cityList.isEmpty()) {
        qDebug() << "   ❌ 城市列表为空！";
        return;
    } else {
        qDebug() << "   ✅ 城市列表：" << cityList.join(", ");
    }

    // 检查邻接图
    qDebug() << "\n2. 邻接图检查：";
    qDebug() << "   邻接图节点数：" << adjacencyGraph.size();
    if (adjacencyGraph.isEmpty()) {
        qDebug() << "   ❌ 邻接图为空！";
    } else {
        qDebug() << "   ✅ 邻接关系：";
        for (auto it = adjacencyGraph.begin(); it != adjacencyGraph.end(); ++it) {
            qDebug() << "     " << it.key() << " <-> " << it.value().join(", ");
        }
    }

    // 检查距离图
    qDebug() << "\n3. 距离图检查：";
    qDebug() << "   距离图节点数：" << distanceGraph.size();
    if (distanceGraph.isEmpty()) {
        qDebug() << "   ❌ 距离图为空！这是主要问题！";
    } else {
        qDebug() << "   ✅ 距离关系：";
        for (auto it = distanceGraph.begin(); it != distanceGraph.end(); ++it) {
            qDebug() << "     从 " << it.key() << " 出发：";
            for (const Edge& edge : it.value()) {
                qDebug() << "       -> " << edge.to << " (距离: " << edge.weight << ")";
            }
        }
    }

    qDebug() << "==================================\n";
}

// 2. 改进的数据加载函数 - 更宽松的加载策略
bool GraphManager::loadDistanceFromCSV_Fixed(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "无法打开距离文件：" << filePath;
        return false;
    }

    // 清空距离图
    distanceGraph.clear();
    QTextStream in(&file);

    qDebug() << "开始加载距离文件：" << filePath;

    int lineNumber = 0;
    int validLines = 0;
    int skippedLines = 0;

    while (!in.atEnd()) {
        lineNumber++;
        QString line = in.readLine().trimmed();

        qDebug() << "读取第" << lineNumber << "行：" << line;

        if (line.isEmpty()) {
            qDebug() << "  跳过空行";
            continue;
        }

        QStringList parts = line.split(',');
        if (parts.size() < 3) {
            qDebug() << "  ❌ 格式错误：列数不足3列，实际" << parts.size() << "列";
            skippedLines++;
            continue;
        }

        QString from = parts[0].trimmed();
        QString to = parts[1].trimmed();
        QString distStr = parts[2].trimmed();

        // 尝试转换距离
        bool ok;
        int dist = distStr.toInt(&ok);

        if (!ok || dist <= 0) {
            qDebug() << "  ❌ 距离值无效：" << distStr;
            skippedLines++;
            continue;
        }

        qDebug() << "  ✅ 有效数据：" << from << " -> " << to << " 距离：" << dist;

        // 🔧 修改：不依赖邻接图，直接构建距离图
        distanceGraph[from].append({to, dist});
        distanceGraph[to].append({from, dist}); // 无向图

        // 确保城市在列表中
        addCity(from);
        addCity(to);
        validLines++;
    }

    std::sort(cityList.begin(), cityList.end());

    qDebug() << "距离文件加载完成：";
    qDebug() << "  总行数：" << lineNumber;
    qDebug() << "  有效行数：" << validLines;
    qDebug() << "  跳过行数：" << skippedLines;
    qDebug() << "  最终城市数：" << cityList.size();
    qDebug() << "  距离图节点数：" << distanceGraph.size();

    return true;
}

// 3. 改进的邻接图加载函数
bool GraphManager::loadAdjacencyFromCSV_Fixed(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "无法打开邻接文件：" << filePath;
        return false;
    }

    adjacencyGraph.clear();
    QTextStream in(&file);

    qDebug() << "开始加载邻接文件：" << filePath;

    int lineNumber = 0;
    int validLines = 0;

    while (!in.atEnd()) {
        lineNumber++;
        QString line = in.readLine().trimmed();

        qDebug() << "读取第" << lineNumber << "行：" << line;

        if (line.isEmpty()) continue;

        QStringList parts = line.split(',');
        if (parts.size() < 3) {
            qDebug() << "  ❌ 格式错误：列数不足";
            continue;
        }

        QString city1 = parts[0].trimmed();
        QString city2 = parts[1].trimmed();
        QString adjStr = parts[2].trimmed();

        bool ok;
        int adjacent = adjStr.toInt(&ok);

        if (!ok) {
            qDebug() << "  ❌ 邻接值无效：" << adjStr;
            continue;
        }

        qDebug() << "  数据：" << city1 << " <-> " << city2 << " 邻接：" << adjacent;

        if (adjacent == 1) {
            adjacencyGraph[city1].append(city2);
            adjacencyGraph[city2].append(city1);
            qDebug() << "  ✅ 添加邻接关系";
            validLines++;
        }

        addCity(city1);
        addCity(city2);
    }

    std::sort(cityList.begin(), cityList.end());

    qDebug() << "邻接文件加载完成：";
    qDebug() << "  总行数：" << lineNumber;
    qDebug() << "  有效邻接关系：" << validLines;

    return true;
}

// 4. 简化的测试函数
void GraphManager::testConnection(const QString& start, const QString& end) {
    qDebug() << "\n========== 连接测试 ==========";
    qDebug() << "测试路径：" << start << " -> " << end;

    // 检查城市是否存在
    qDebug() << "起点" << start << "是否在城市列表中：" << cityList.contains(start);
    qDebug() << "终点" << end << "是否在城市列表中：" << cityList.contains(end);

    // 检查距离图中是否有这些城市
    qDebug() << "起点" << start << "是否在距离图中：" << distanceGraph.contains(start);
    qDebug() << "终点" << end << "是否在距离图中：" << distanceGraph.contains(end);

    if (distanceGraph.contains(start)) {
        qDebug() << "从" << start << "可以直接到达的城市：";
        for (const Edge& edge : distanceGraph[start]) {
            qDebug() << "  -> " << edge.to << " (距离: " << edge.weight << ")";
        }
    }

    if (distanceGraph.contains(end)) {
        qDebug() << "可以直接到达" << end << "的城市：";
        for (auto it = distanceGraph.begin(); it != distanceGraph.end(); ++it) {
            for (const Edge& edge : it.value()) {
                if (edge.to == end) {
                    qDebug() << "  " << it.key() << " -> " << end << " (距离: " << edge.weight << ")";
                }
            }
        }
    }

    qDebug() << "=============================\n";
}

// 5. 完整的调试和修复流程
void GraphManager::debugAndFix(const QString& adjacencyFile, const QString& distanceFile,
                               const QString& startCity, const QString& endCity) {
    qDebug() << "========== 开始完整调试流程 ==========";

    // 步骤1：加载数据
    qDebug() << "\n步骤1：加载邻接图数据";
    if (!loadAdjacencyFromCSV_Fixed(adjacencyFile)) {
        qDebug() << "❌ 邻接图加载失败！";
        return;
    }

    qDebug() << "\n步骤2：加载距离图数据";
    if (!loadDistanceFromCSV_Fixed(distanceFile)) {
        qDebug() << "❌ 距离图加载失败！";
        return;
    }

    // 步骤2：诊断数据
    qDebug() << "\n步骤3：诊断数据完整性";
    diagnoseDataLoading();

    // 步骤3：测试连接
    qDebug() << "\n步骤4：测试城市连接";
    testConnection(startCity, endCity);

    // 步骤4：尝试算法
    qDebug() << "\n步骤5：测试Dijkstra算法";
    double dijkstraDistance = 0.0;
    QStringList dijkstraPath = dijkstraShortestPath(startCity, endCity, dijkstraDistance);

    if (!dijkstraPath.isEmpty()) {
        qDebug() << "✅ Dijkstra找到路径：" << dijkstraPath.join(" -> ");
        qDebug() << "   总距离：" << dijkstraDistance;
    } else {
        qDebug() << "❌ Dijkstra未找到路径";
    }

    qDebug() << "\n步骤6：测试Floyd算法";
    double floydDistance = 0.0;
    QStringList floydPath = floydShortestPath(startCity, endCity, floydDistance);

    if (!floydPath.isEmpty()) {
        qDebug() << "✅ Floyd找到路径：" << floydPath.join(" -> ");
        qDebug() << "   总距离：" << floydDistance;
    } else {
        qDebug() << "❌ Floyd未找到路径";
    }

    qDebug() << "========== 调试流程结束 ==========\n";
}
void GraphManager::addCity(const QString& city) {
    if (!cityList.contains(city)) {
        cityList.append(city);
    }
}

// 在 loadAdjacencyFromCSV 函数末尾添加调试输出
bool GraphManager::loadAdjacencyFromCSV(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "无法打开邻接文件：" << filePath;
        return false;
    }

    adjacencyGraph.clear();
    QTextStream in(&file);

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        QStringList parts = line.split(',');
        if (parts.size() != 3) continue;

        QString city1 = parts[0].trimmed();
        QString city2 = parts[1].trimmed();
        int adjacent = parts[2].trimmed().toInt();
        // 避免重复添加

        if (adjacent == 1) {
            adjacencyGraph[city1].append(city2);
            adjacencyGraph[city2].append(city1);
        }

        addCity(city1);
        addCity(city2);
    }

    std::sort(cityList.begin(), cityList.end());

    // 🔍 添加调试输出
    qDebug() << "=== 邻接图加载完成 ===";
    qDebug() << "城市总数：" << cityList.size();
    qDebug() << "城市列表：" << cityList;
    qDebug() << "邻接关系数量：" << adjacencyGraph.size();

    for (auto it = adjacencyGraph.begin(); it != adjacencyGraph.end(); ++it) {
        qDebug() << it.key() << "邻接：" << it.value();
    }

    return true;
}

// 在 loadDistanceFromCSV 函数末尾添加调试输出
bool GraphManager::loadDistanceFromCSV(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "无法打开距离文件：" << filePath;
        return false;
    }

    distanceGraph.clear();
    QTextStream in(&file);

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        QStringList parts = line.split(',');
        if (parts.size() != 3) continue;

        QString from = parts[0].trimmed();
        QString to = parts[1].trimmed();
        int dist = parts[2].trimmed().toInt();
        qDebug() << "[调试] 尝试插入边：" << from << "->" << to << ", 距离：" << dist;
        qDebug() << "  - 邻接中是否包含此边？" << adjacencyGraph[from].contains(to);

        // 只处理邻接图中存在的边
        if (adjacencyGraph.contains(from) && adjacencyGraph[from].contains(to)) {
            distanceGraph[from].append({to, dist});
        }
        if (adjacencyGraph.contains(to) && adjacencyGraph[to].contains(from)) {
            distanceGraph[to].append({from, dist});
        }
    }

    // 🔍 添加调试输出
    qDebug() << "=== 距离图加载完成 ===";
    qDebug() << "距离图节点数量：" << distanceGraph.size();

    for (auto it = distanceGraph.begin(); it != distanceGraph.end(); ++it) {
        qDebug() << "从" << it.key() << "出发的边：";
        for (const Edge& edge : it.value()) {
            qDebug() << "  -> " << edge.to << "，距离：" << edge.weight;
        }
    }

    return true;
}

bool GraphManager::bfsPath(const QString &start, const QString &end, QStringList &path) {
    // 判断起点和终点是否在图中
    if (!adjacencyGraph.contains(start) || !adjacencyGraph.contains(end))
        return false;

    QMap<QString, QString> predecessor; // 用于记录每个节点的前驱节点，方便回溯路径
    QSet<QString> visited;              // 记录访问过的节点，避免重复访问
    QQueue<QString> queue;              // BFS队列

    queue.enqueue(start);
    visited.insert(start);

    while (!queue.isEmpty()) {
        QString current = queue.dequeue();
        if (current == end) {
            // 找到终点，回溯路径
            QStringList revPath;
            for (QString at = end; !at.isEmpty(); at = predecessor.value(at)) {
                revPath.prepend(at);
            }
            path = revPath;
            return true;
        }

        // 遍历邻居节点
        for (const QString &neighbor : adjacencyGraph[current]) {
            if (!visited.contains(neighbor)) {
                visited.insert(neighbor);
                predecessor[neighbor] = current;
                queue.enqueue(neighbor);
            }
        }
    }

    // 如果找不到路径返回false
    return false;
}

void GraphManager::dfsAllPaths(const QString& current, const QString& end, QStringList& path,
                               QVector<QStringList>& allPaths, QSet<QString>& visited, int maxNodes) {
    if (path.size() > maxNodes) return;
    if (current == end) {
        allPaths.append(path);
        return;
    }

    visited.insert(current);
    for (const QString& neighbor : adjacencyGraph[current]) {
        if (!visited.contains(neighbor)) {
            path.append(neighbor);
            dfsAllPaths(neighbor, end, path, allPaths, visited, maxNodes);
            path.removeLast();
        }
    }
    visited.remove(current);
}

QVector<QStringList> GraphManager::findAllPaths(QString start, QString end, int maxNodes) {
    QVector<QStringList> allPaths;
    QStringList currentPath{start};
    QSet<QString> visited;
    dfsAllPaths(start, end, currentPath, allPaths, visited, maxNodes);
    return allPaths;
}

double GraphManager::calculatePathLength(const QStringList& path) {
    double length = 0.0;
    for (int i = 0; i < path.size() - 1; ++i) {
        const QString &from = path[i];
        const QString &to = path[i + 1];
        bool found = false;

        for (const Edge &edge : distanceGraph[from]) {
            if (edge.to == to) {
                length += edge.weight;
                found = true;
                break;
            }
        }

        if (!found) {
            // 没找到边，说明路径非法或数据不完整
            qWarning() << "Edge not found from" << from << "to" << to;
            return -1; // 或者 throw std::runtime_error("Invalid path");
        }
    }
    return length;
}

// 快速排序路径（按照路径总长度升序）
void GraphManager::quickSortPaths(QVector<QStringList>& paths) {
    std::sort(paths.begin(), paths.end(), [this](const QStringList& a, const QStringList& b) {
        return calculatePathLength(a) < calculatePathLength(b);
    });
}

// 在算法函数开始处添加调试输出
QStringList GraphManager::dijkstraShortestPath(const QString &start, const QString &end, double &totalDistance) {
    totalDistance = 0.0;

    // 🔍 添加调试输出
    qDebug() << "=== Dijkstra算法调试 ===";
    qDebug() << "起点：" << start << "，终点：" << end;
    qDebug() << "距离图是否包含起点：" << distanceGraph.contains(start);
    qDebug() << "距离图是否包含终点：" << distanceGraph.contains(end);

    if (distanceGraph.contains(start)) {
        qDebug() << "起点" << start << "的邻居：";
        for (const Edge& edge : distanceGraph[start]) {
            qDebug() << "  -> " << edge.to << "，距离：" << edge.weight;
        }
    }

    if (!distanceGraph.contains(start) || !distanceGraph.contains(end)) {
        qDebug() << "起点或终点不在距离图中！";
        return {};
    }

    // 原有的Dijkstra算法代码...
    QMap<QString, double> dist;
    QMap<QString, QString> prev;
    QSet<QString> visited;

    for (const QString &city : cityList)
        dist[city] = std::numeric_limits<double>::max();
    dist[start] = 0;

    auto cmp = [&](const QString &a, const QString &b) { return dist[a] > dist[b]; };
    std::priority_queue<QString, QVector<QString>, decltype(cmp)> pq(cmp);
    pq.push(start);

    while (!pq.empty()) {
        QString u = pq.top();
        pq.pop();
        if (visited.contains(u)) continue;
        visited.insert(u);

        qDebug() << "处理节点：" << u << "，当前距离：" << dist[u];

        for (const Edge &edge : distanceGraph[u]) {
            const QString &v = edge.to;
            int weight = edge.weight;
            qDebug() << "  检查边：" << u << "->" << v << "，权重：" << weight;

            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                prev[v] = u;
                pq.push(v);
                qDebug() << "    更新距离：" << v << "=" << dist[v];
            }
        }
    }

    qDebug() << "最终距离：" << dist[end];

    if (dist[end] == std::numeric_limits<double>::max()) {
        qDebug() << "未找到路径！";
        return {};
    }

    totalDistance = dist[end];
    QStringList path;
    for (QString at = end; !at.isEmpty(); at = prev.value(at, "")) {
        path.prepend(at);
        if (at == start) break;
    }

    qDebug() << "找到路径：" << path;
    return (path.first() == start) ? path : QStringList();
}

QStringList GraphManager::floydShortestPath(const QString &start, const QString &end, double &totalDistance) {
    totalDistance = 0.0;
    int n = cityList.size();
    if (!cityList.contains(start) || !cityList.contains(end)) return {};

    // 初始化
    QMap<QString, QMap<QString, double>> dist;
    QMap<QString, QMap<QString, QString>> next;

    for (const QString &u : cityList) {
        for (const QString &v : cityList) {
            dist[u][v] = (u == v) ? 0 : std::numeric_limits<double>::max();
        }
    }

    for (const auto &u : distanceGraph.keys()) {
        for (const Edge &edge : distanceGraph[u]) {
            dist[u][edge.to] = edge.weight;
            next[u][edge.to] = edge.to;
        }
    }

    // Floyd 主过程
    for (const QString &k : cityList) {
        for (const QString &i : cityList) {
            for (const QString &j : cityList) {
                if (dist[i][k] < std::numeric_limits<double>::max() &&
                    dist[k][j] < std::numeric_limits<double>::max() &&
                    dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    next[i][j] = next[i][k];
                }
            }
        }
    }

    if (next[start].value(end).isEmpty()) return {};

    QStringList path;
    QString u = start;
    while (u != end) {
        path << u;
        u = next[u][end];
        if (u.isEmpty()) return {};
    }
    path << end;
    totalDistance = dist[start][end];
    return path;
}

// 新增：综合路径查询和比较分析
PathAnalysisResult GraphManager::analyzeAllPaths(const QString &start, const QString &end, int maxNodes) {
    PathAnalysisResult result;
    result.startCity = start;
    result.endCity = end;

    // 检查起点和终点是否存在
    if (!cityList.contains(start) || !cityList.contains(end)) {
        result.isValid = false;
        result.errorMessage = QString("起点 '%1' 或终点 '%2' 不存在于图中").arg(start, end);
        return result;
    }

    // 1. 使用Dijkstra算法找最短路径
    double dijkstraDistance = 0.0;
    result.dijkstraPath = dijkstraShortestPath(start, end, dijkstraDistance);
    result.dijkstraDistance = dijkstraDistance;

    // 2. 使用Floyd算法找最短路径
    double floydDistance = 0.0;
    result.floydPath = floydShortestPath(start, end, floydDistance);
    result.floydDistance = floydDistance;

    // 3. 找所有可行路径
    result.allPaths = findAllPaths(start, end, maxNodes);

    // 4. 计算每条路径的长度并排序
    QVector<QPair<QStringList, double>> pathsWithDistance;
    for (const auto& path : result.allPaths) {
        double distance = calculatePathLength(path);
        if (distance > 0) { // 只保留有效路径
            pathsWithDistance.append({path, distance});
        }
    }

    // 按距离排序
    std::sort(pathsWithDistance.begin(), pathsWithDistance.end(),
              [](const QPair<QStringList, double>& a, const QPair<QStringList, double>& b) {
                  return a.second < b.second;
              });

    // 提取排序后的路径
    result.sortedPaths.clear();
    result.pathDistances.clear();
    for (const auto& pair : pathsWithDistance) {
        result.sortedPaths.append(pair.first);
        result.pathDistances.append(pair.second);
    }

    result.isValid = true;
    return result;
}

// 新增：输出结果到文件
bool GraphManager::exportPathAnalysisToFile(const PathAnalysisResult &result, const QString &fileName) {
    QString documentsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString filePath = QDir(documentsPath).filePath(fileName);

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "无法创建输出文件：" << filePath;
        return false;
    }

    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);

    // 文件头信息
    out << "========================================\n";
    out << "路径分析报告\n";
    out << "========================================\n";
    out << "起点: " << result.startCity << "\n";
    out << "终点: " << result.endCity << "\n";
    out << "分析时间: " << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "\n";
    out << "========================================\n\n";

    if (!result.isValid) {
        out << "错误: " << result.errorMessage << "\n";
        return true;
    }

    // Dijkstra算法结果
    out << "1. Dijkstra算法结果:\n";
    if (!result.dijkstraPath.isEmpty()) {
        out << "   路径: " << result.dijkstraPath.join(" -> ") << "\n";
        out << "   总距离: " << result.dijkstraDistance << "\n";
    } else {
        out << "   未找到路径\n";
    }
    out << "\n";

    // Floyd算法结果
    out << "2. Floyd算法结果:\n";
    if (!result.floydPath.isEmpty()) {
        out << "   路径: " << result.floydPath.join(" -> ") << "\n";
        out << "   总距离: " << result.floydDistance << "\n";
    } else {
        out << "   未找到路径\n";
    }
    out << "\n";

    // 算法结果比较
    out << "3. 算法结果比较:\n";
    if (!result.dijkstraPath.isEmpty() && !result.floydPath.isEmpty()) {
        if (qAbs(result.dijkstraDistance - result.floydDistance) < 0.001) {
            out << "   ✓ 两种算法得到相同的最短距离: " << result.dijkstraDistance << "\n";
            if (result.dijkstraPath == result.floydPath) {
                out << "   ✓ 两种算法得到相同的最短路径\n";
            } else {
                out << "   ! 两种算法得到不同的最短路径（但距离相同）\n";
            }
        } else {
            out << "   ✗ 两种算法得到不同的结果！\n";
            out << "     Dijkstra距离: " << result.dijkstraDistance << "\n";
            out << "     Floyd距离: " << result.floydDistance << "\n";
        }
    }
    out << "\n";

    // 所有路径结果（按距离排序）
    out << "4. 所有可行路径（按距离升序排列）:\n";
    out << "   总计找到 " << result.sortedPaths.size() << " 条路径\n\n";

    for (int i = 0; i < result.sortedPaths.size(); ++i) {
        out << QString("   路径 %1: ").arg(i + 1, 2);
        out << result.sortedPaths[i].join(" -> ");
        out << QString(" (距离: %1)").arg(result.pathDistances[i]);

        // 标记是否为最短路径
        if (i == 0) {
            out << " [最短路径]";
        }

        // 比较与算法结果
        if (!result.dijkstraPath.isEmpty() &&
            qAbs(result.pathDistances[i] - result.dijkstraDistance) < 0.001) {
            out << " [与Dijkstra结果一致]";
        }
        if (!result.floydPath.isEmpty() &&
            qAbs(result.pathDistances[i] - result.floydDistance) < 0.001) {
            out << " [与Floyd结果一致]";
        }

        out << "\n";
    }

    // 统计信息
    out << "\n5. 统计信息:\n";
    if (!result.sortedPaths.isEmpty()) {
        out << "   最短距离: " << result.pathDistances.first() << "\n";
        out << "   最长距离: " << result.pathDistances.last() << "\n";

        double avgDistance = 0.0;
        for (double dist : result.pathDistances) {
            avgDistance += dist;
        }
        avgDistance /= result.pathDistances.size();
        out << "   平均距离: " << QString::number(avgDistance, 'f', 2) << "\n";

        // 路径长度分布
        QMap<int, int> lengthDistribution;
        for (const auto& path : result.sortedPaths) {
            lengthDistribution[path.size()]++;
        }
        out << "   路径长度分布:\n";
        for (auto it = lengthDistribution.begin(); it != lengthDistribution.end(); ++it) {
            out << QString("     %1个节点: %2条路径\n").arg(it.key()).arg(it.value());
        }
    }

    out << "\n========================================\n";
    out << "报告生成完成\n";
    out << "文件保存位置: " << filePath << "\n";
    out << "========================================\n";

    qDebug() << "路径分析报告已保存到:" << filePath;
    return true;
}

// 新增：快速查询两点间最短路径（可选择算法）
ShortestPathResult GraphManager::findShortestPath(const QString &start, const QString &end,
                                                  ShortestPathAlgorithm algorithm) {
    ShortestPathResult result;
    result.startCity = start;
    result.endCity = end;
    result.algorithm = algorithm;

    if (!cityList.contains(start) || !cityList.contains(end)) {
        result.isValid = false;
        result.errorMessage = QString("起点 '%1' 或终点 '%2' 不存在于图中").arg(start, end);
        return result;
    }

    double distance = 0.0;
    QStringList path;

    switch (algorithm) {
    case ShortestPathAlgorithm::Dijkstra:
        path = dijkstraShortestPath(start, end, distance);
        result.algorithmName = "Dijkstra";
        break;
    case ShortestPathAlgorithm::Floyd:
        path = floydShortestPath(start, end, distance);
        result.algorithmName = "Floyd-Warshall";
        break;
    }

    result.path = path;
    result.distance = distance;
    result.isValid = !path.isEmpty();

    if (!result.isValid) {
        result.errorMessage = QString("使用%1算法未找到从 '%2' 到 '%3' 的路径")
                                  .arg(result.algorithmName, start, end);
    }

    return result;
}






// GraphManager.cpp 中的实现

QStringList GraphManager::dijkstraShortestPathAvoidCity(const QString& start, const QString& end,
                                                        const QString& avoidCity, double& distance) {
    distance = 0.0;

    if (!adjacencyMatrix.contains(start) || !adjacencyMatrix.contains(end)) {
        return QStringList();
    }

    if (start == avoidCity || end == avoidCity) {
        return QStringList(); // 起点或终点不能是要绕过的城市
    }

    QMap<QString, double> distances;
    QMap<QString, QString> previous;
    QSet<QString> visited;

    // 初始化距离
    for (auto it = adjacencyMatrix.begin(); it != adjacencyMatrix.end(); ++it) {
        distances[it.key()] = std::numeric_limits<double>::infinity();
    }
    distances[start] = 0.0;

    while (true) {
        QString current;
        double minDistance = std::numeric_limits<double>::infinity();

        // 找到未访问节点中距离最小的节点（排除要绕过的城市）
        for (auto it = distances.begin(); it != distances.end(); ++it) {
            if (!visited.contains(it.key()) && it.value() < minDistance && it.key() != avoidCity) {
                minDistance = it.value();
                current = it.key();
            }
        }

        if (current.isEmpty() || minDistance == std::numeric_limits<double>::infinity()) {
            break;
        }

        visited.insert(current);

        if (current == end) {
            break;
        }

        // 更新邻居节点的距离（排除要绕过的城市）
        if (adjacencyMatrix.contains(current)) {
            for (auto it = adjacencyMatrix[current].begin(); it != adjacencyMatrix[current].end(); ++it) {
                QString neighbor = it.key();
                double weight = it.value();

                if (neighbor == avoidCity || visited.contains(neighbor)) {
                    continue;
                }

                double newDistance = distances[current] + weight;
                if (newDistance < distances[neighbor]) {
                    distances[neighbor] = newDistance;
                    previous[neighbor] = current;
                }
            }
        }
    }

    // 重构路径
    if (distances[end] == std::numeric_limits<double>::infinity()) {
        return QStringList();
    }

    QStringList path;
    QString current = end;
    while (!current.isEmpty()) {
        path.prepend(current);
        current = previous.value(current);
    }

    distance = distances[end];
    return path;
}

QStringList GraphManager::floydShortestPathAvoidCity(const QString& start, const QString& end,
                                                     const QString& avoidCity, double& distance) {
    distance = 0.0;

    if (!adjacencyMatrix.contains(start) || !adjacencyMatrix.contains(end)) {
        return QStringList();
    }

    if (start == avoidCity || end == avoidCity) {
        return QStringList();
    }

    QStringList cities = getCityList();
    cities.removeAll(avoidCity); // 移除要绕过的城市

    int n = cities.size();
    QMap<QString, int> cityIndex;
    for (int i = 0; i < n; ++i) {
        cityIndex[cities[i]] = i;
    }

    // 初始化距离矩阵和路径矩阵
    QVector<QVector<double>> dist(n, QVector<double>(n, std::numeric_limits<double>::infinity()));
    QVector<QVector<int>> next(n, QVector<int>(n, -1));

    // 初始化直接连接的距离
    for (int i = 0; i < n; ++i) {
        dist[i][i] = 0.0;
        for (int j = 0; j < n; ++j) {
            if (i != j && adjacencyMatrix.contains(cities[i]) &&
                adjacencyMatrix[cities[i]].contains(cities[j])) {
                dist[i][j] = adjacencyMatrix[cities[i]][cities[j]];
                next[i][j] = j;
            }
        }
    }

    // Floyd-Warshall 算法
    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    next[i][j] = next[i][k];
                }
            }
        }
    }

    int startIdx = cityIndex.value(start, -1);
    int endIdx = cityIndex.value(end, -1);

    if (startIdx == -1 || endIdx == -1 || dist[startIdx][endIdx] == std::numeric_limits<double>::infinity()) {
        return QStringList();
    }

    // 重构路径
    QStringList path;
    int current = startIdx;
    while (current != endIdx) {
        path.append(cities[current]);
        current = next[current][endIdx];
        if (current == -1) {
            return QStringList();
        }
    }
    path.append(cities[endIdx]);

    distance = dist[startIdx][endIdx];
    return path;
}

QVector<QStringList> GraphManager::findAllPathsAvoidCity(const QString& start, const QString& end,
                                                         const QString& avoidCity, int maxNodes) {
    QVector<QStringList> allPaths;
    QStringList currentPath;
    QSet<QString> visited;

    if (start == avoidCity || end == avoidCity) {
        return allPaths;
    }

    currentPath.append(start);
    visited.insert(start);

    findAllPathsAvoidCityHelper(start, end, avoidCity, currentPath, visited, allPaths, maxNodes);

    return allPaths;
}

// 辅助递归函数（需要在 GraphManager.h 的 private 区域声明）
void GraphManager::findAllPathsAvoidCityHelper(const QString& current, const QString& end,
                                               const QString& avoidCity, QStringList& currentPath,
                                               QSet<QString>& visited, QVector<QStringList>& allPaths,
                                               int maxNodes) {
    if (currentPath.size() > maxNodes) {
        return;
    }

    if (current == end) {
        allPaths.append(currentPath);
        return;
    }

    if (adjacencyMatrix.contains(current)) {
        for (auto it = adjacencyMatrix[current].begin(); it != adjacencyMatrix[current].end(); ++it) {
            QString neighbor = it.key();

            if (neighbor == avoidCity || visited.contains(neighbor)) {
                continue;
            }

            currentPath.append(neighbor);
            visited.insert(neighbor);

            findAllPathsAvoidCityHelper(neighbor, end, avoidCity, currentPath, visited, allPaths, maxNodes);

            currentPath.removeLast();
            visited.remove(neighbor);
        }
    }
}
//Yen's K 最短路径算法的实现（代码未完整） ...
    // 这是一个用于查找 K 条最短路径的复杂算法。
    // 概述：
    // 1. 找到第一条最短路径 (Dijkstra)。
    // 2. 迭代 K 次，在每次迭代中：
    //    a. 确定“根路径”和“分支节点”。
    //    b. 暂时移除根路径上的边，并在分支节点处重新计算从分支节点到终点的最短路径。
    //    c. 将根路径和新的分支路径合并，形成候选路径。
    //    d. 从候选路径中选择最短且不重复的路径。
std::vector<Path> GraphManager::yenKShortestPaths(const QString &start, const QString &end, int K) {
    std::vector<Path> result;
    QSet<QString> bannedEdges;
    QSet<QString> bannedNodes;

    double firstDist = 0.0;
    QStringList firstPath = dijkstraShortestPath(start, end, firstDist);
    if (firstPath.isEmpty()) return result;

    result.push_back({firstPath, firstDist});

    using Candidate = std::pair<QStringList, double>;
    std::priority_queue<Path> candidates;

    for (int k = 1; k < K; ++k) {
        const Path &prevPath = result[k - 1];

        for (int i = 0; i < prevPath.nodes.size() - 1; ++i) {
            QString spurNode = prevPath.nodes[i];
            QStringList rootPath = prevPath.nodes.mid(0, i + 1);

            QMap<QString, QList<Edge>> originalGraph = distanceGraph;

            // 禁用 rootPath 的重复路径
            for (const Path &p : result) {
                if (p.nodes.size() > i && p.nodes.mid(0, i + 1) == rootPath) {
                    QString from = p.nodes[i];
                    QString to = p.nodes[i + 1];

                    auto &edges = distanceGraph[from];
                    edges.erase(std::remove_if(edges.begin(), edges.end(),
                                               [&](const Edge &e) { return e.to == to; }),
                                edges.end());
                }
            }

            // 禁用 rootPath 中的所有节点
            for (int j = 0; j < i; ++j) {
                distanceGraph.remove(prevPath.nodes[j]);
            }

            double spurDist = 0.0;
            QStringList spurPath = dijkstraShortestPath(spurNode, end, spurDist);

            if (!spurPath.isEmpty()) {
                spurPath.pop_front(); // 移除重复的spurNode
                QStringList totalPath = rootPath + spurPath;
                double rootDist = 0.0;
                for (int m = 0; m < rootPath.size() - 1; ++m) {
                    for (const Edge &e : originalGraph[rootPath[m]]) {
                        if (e.to == rootPath[m + 1]) {
                            rootDist += e.weight;
                            break;
                        }
                    }
                }
                double totalDist = rootDist + spurDist;
                candidates.push({totalPath, totalDist});
            }

            // 恢复原图
            distanceGraph = originalGraph;
        }

        // 若没有更多候选路径
        if (candidates.empty()) break;

        // 取最短候选路径加入结果
        while (!candidates.empty()) {
            Path next = candidates.top();
            candidates.pop();

            bool exists = false;
            for (const auto &p : result) {
                if (p.nodes == next.nodes) {
                    exists = true;
                    break;
                }
            }

            if (!exists) {
                result.push_back(next);
                break;
            }
        }
    }

    return result;
}

const QMap<QString, QVector<Edge>>& GraphManager::getDistanceGraph() const {
    return distanceGraph;
}

const QMap<QString, QVector<QString>>& GraphManager::getAdjacencyGraph() const {
    return adjacencyGraph;
}

const QStringList& GraphManager::getCityList() const {
    return cityList;
}

void GraphManager::clear() {
    distanceGraph.clear();
    adjacencyGraph.clear();
    cityList.clear();
}
