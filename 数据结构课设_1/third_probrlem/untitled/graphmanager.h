#ifndef GRAPHMANAGER_H
#define GRAPHMANAGER_H

#include <QString>
#include <QStringList>
#include <QVector>
#include <QMap>
#include <QSet>
#include <limits>
#include <QDateTime>
struct Path {
    QStringList nodes;
    double totalDistance;

    bool operator<(const Path &other) const {
        return totalDistance > other.totalDistance; // 最小堆使用
    }
};

// 边的结构体
struct Edge {
    QString to;
    int weight;
};

// 最短路径算法枚举
enum class ShortestPathAlgorithm {
    Dijkstra,
    Floyd
};

// 最短路径查询结果结构体
struct ShortestPathResult {
    QString startCity;
    QString endCity;
    QStringList path;
    double distance = 0.0;
    bool isValid = false;
    QString errorMessage;
    ShortestPathAlgorithm algorithm;
    QString algorithmName;
};

// 路径分析结果结构体
struct PathAnalysisResult {
    QString startCity;
    QString endCity;
    bool isValid = false;
    QString errorMessage;

    // Dijkstra算法结果
    QStringList dijkstraPath;
    double dijkstraDistance = 0.0;

    // Floyd算法结果
    QStringList floydPath;
    double floydDistance = 0.0;

    // 所有路径
    QVector<QStringList> allPaths;

    // 排序后的路径和对应距离
    QVector<QStringList> sortedPaths;
    QVector<double> pathDistances;
};

class GraphManager
{
public:

    GraphManager() = default;
    // 在 GraphManager 类的 public 部分添加
    void diagnoseDataLoading();
    bool loadDistanceFromCSV_Fixed(const QString &filePath);
    bool loadAdjacencyFromCSV_Fixed(const QString &filePath);
    void testConnection(const QString& start, const QString& end);
    void debugAndFix(const QString& adjacencyFile, const QString& distanceFile,
                     const QString& startCity, const QString& endCity);
    // 原有功能
    void addCity(const QString& city);
    bool loadAdjacencyFromCSV(const QString &filePath);
    bool loadDistanceFromCSV(const QString &filePath);
    bool bfsPath(const QString &start, const QString &end, QStringList &path);
    QVector<QStringList> findAllPaths(QString start, QString end, int maxNodes = 10);
    double calculatePathLength(const QStringList& path);
    void quickSortPaths(QVector<QStringList>& paths);
    QStringList dijkstraShortestPath(const QString &start, const QString &end, double &totalDistance);
    QStringList floydShortestPath(const QString &start, const QString &end, double &totalDistance);
 std::vector<Path> yenKShortestPaths(const QString &start, const QString &end, int K);//求第k短路径函数
    // 新增功能
    /**
     * @brief 综合分析两点间的所有路径
     * @param start 起点城市
     * @param end 终点城市
     * @param maxNodes 路径最大节点数（默认10）
     * @return PathAnalysisResult 包含所有分析结果的结构体
     */
    PathAnalysisResult analyzeAllPaths(const QString &start, const QString &end, int maxNodes = 10);

    /**
     * @brief 导出路径分析结果到文件
     * @param result 分析结果
     * @param fileName 输出文件名（默认保存到Documents文件夹）
     * @return bool 是否成功保存
     */
    bool exportPathAnalysisToFile(const PathAnalysisResult &result, const QString &fileName = "path_analysis.txt");

    /**
     * @brief 查找两点间最短路径（可选择算法）
     * @param start 起点城市
     * @param end 终点城市
     * @param algorithm 使用的算法
     * @return ShortestPathResult 最短路径查询结果
     */
    ShortestPathResult findShortestPath(const QString &start, const QString &end,
                                        ShortestPathAlgorithm algorithm = ShortestPathAlgorithm::Dijkstra);

    // Getter方法
    const QMap<QString, QVector<Edge>>& getDistanceGraph() const;
    const QMap<QString, QVector<QString>>& getAdjacencyGraph() const;
    const QStringList& getCityList() const;

    // 清理方法
    void clear();
public:
    // 绕过指定城市的最短路径查询（基于Dijkstra算法）
    QStringList dijkstraShortestPathAvoidCity(const QString& start, const QString& end,
                                              const QString& avoidCity, double& distance);

    // 绕过指定城市的最短路径查询（基于Floyd算法）
    QStringList floydShortestPathAvoidCity(const QString& start, const QString& end,
                                           const QString& avoidCity, double& distance);

    // 查找绕过指定城市的所有路径
    QVector<QStringList> findAllPathsAvoidCity(const QString& start, const QString& end,
                                               const QString& avoidCity, int maxNodes = 10);
private:
    QMap<QString, QMap<QString, double>> adjacencyMatrix;

private:
    void findAllPathsAvoidCityHelper(const QString& current, const QString& end,
                                     const QString& avoidCity, QStringList& currentPath,
                                     QSet<QString>& visited, QVector<QStringList>& allPaths,
                                     int maxNodes);
    QMap<QString, QVector<Edge>> distanceGraph;      // 距离图（带权重）
    QMap<QString, QVector<QString>> adjacencyGraph; // 邻接图（无权重）
    QStringList cityList;                           // 城市列表

    // DFS递归查找所有路径的辅助函数
    void dfsAllPaths(const QString& current, const QString& end, QStringList& path,
                     QVector<QStringList>& allPaths, QSet<QString>& visited, int maxNodes);
};

#endif // GRAPHMANAGER_H
