#include "server.h"
#include "ui_server.h"
#include <QDebug>
#include <QList>
#include <QNetworkInterface>
#include "createrandnums.h"
#include <algorithm>
#include <unordered_map>
std::vector<int> allData;  // 保存所有历史数据

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Server)
{
    ui->setupUi(this);

    init();
	msgs = new CreateRandNums();	//生成处理随机数的类对象
}
//服务器端的析构函数
Widget::~Widget()
{
    delete ui;
    if (msgs != NULL) {
		delete msgs;
	}
}
void Widget::init()
{
    TcpServer = new QTcpServer(this);
    TcpSocket = new QTcpSocket(this);
    newListen();
    connect(TcpServer,SIGNAL(newConnection()),this,SLOT(mconnect()));      //建立连接
    connect(TcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),
                this,SLOT(displayError(QAbstractSocket::SocketError)));

}
//检查端口是否被占用
void Widget::newListen()
{
    if(!TcpServer->listen(QHostAddress::Any,8080))
    {
        qDebug()<<TcpServer->errorString();
        close();
        return;
    }
}
//发送数据
void Widget::on_pushButton_send_clicked()
{
    QString str = ui->textEdit_write->toPlainText();
    TcpSocket->write(str.toUtf8().data());
    ui->textEdit_read->append(QString("host:%1").arg(str));
    ui->textEdit_write->clear();
}
//断开连接
void Widget::on_pushButton_close_clicked()
{
    TcpSocket->disconnectFromHost();
    TcpSocket->close();
}

void Widget::mconnect()
{
    qDebug()<<"connect";
    TcpSocket = TcpServer->nextPendingConnection();      //获取当前连接的socket
    connect(TcpSocket,SIGNAL(readyRead()),this,SLOT(RcvData()));
    QString ip = TcpSocket->peerAddress().toString();

    quint16 port = TcpSocket->peerPort();
//    ui->textEdit_read->setText(QString::fromLocal8Bit("[%1:%2]:connected!").arg(ip).arg(port));
    ui->textEdit_read->setText(QString("[%1:%2]:connected").arg(ip).arg(port));
}

void Widget::displayError(QAbstractSocket::SocketError)
{
    qDebug()<<TcpSocket->errorString();
        TcpSocket->close();
}

/*
接收数据，请修改该函数以便实现利用类对象msgs对传输的二进制字符串解码，
请调用msgs->BinaryCodeToIntArray()或者msgs->HaffmanCodeToIntArray()，将二进制字符串解码到msgs->intMSG整形数组中，并基于最大堆或者其他算法动态寻找最大的20个数 / 出现次数最多的20个数
*/
// 获取最大20个数，使用最大堆（大顶堆），直接用 STL partial_sort
std::vector<int> getMax20(const std::vector<int>& data) {
    std::vector<int> res = data;
    if (res.size() > 20)
        std::partial_sort(res.begin(), res.begin() + 20, res.end(), std::greater<int>());
    else
        std::sort(res.begin(), res.end(), std::greater<int>());
    if (res.size() > 20)
        res.resize(20);
    return res;
}

// 获取出现次数最多的20个数，利用哈希表 + 最小堆
std::vector<std::pair<int,int>> getTop20Freq(const std::vector<int>& data) {
    std::unordered_map<int,int> freq;
    for (int v : data) freq[v]++;

    // 用最小堆维护频率最高的20个数，堆顶为最小频率
    auto cmp = [](const std::pair<int,int>& a, const std::pair<int,int>& b) { return a.second > b.second; };
    std::priority_queue<std::pair<int,int>, std::vector<std::pair<int,int>>, decltype(cmp)> minHeap(cmp);

    for (auto& kv : freq) {
        if (minHeap.size() < 20) {
            minHeap.push(kv);
        } else if (kv.second > minHeap.top().second) {
            minHeap.pop();
            minHeap.push(kv);
        }
    }

    std::vector<std::pair<int,int>> res;
    while(!minHeap.empty()) {
        res.push_back(minHeap.top());
        minHeap.pop();
    }
    std::reverse(res.begin(), res.end()); // 频率高的排前面
    return res;
}
void Widget::RcvData()
{
    QByteArray rawData = TcpSocket->readAll();

    // 假设收到的是二进制字符串，转换为QString，赋给msgs->binaryString
    // 注意这里收到的是QString编码，如果是纯二进制，按协议转换
    msgs->binaryString = QString::fromUtf8(rawData);

    // 根据实际编码调用解码函数，演示普通二进制解码：
    msgs->BinaryCodeToIntArray();
    // 或哈夫曼解码
    // msgs->HaffmanCodeToIntArray();

    // 把新接收的整数加入历史数据
    for (int i = 0; i < SUMNUM; i++) {
        allData.push_back(msgs->intMSG[i]);
    }

    // 最大20个数
    std::vector<int> max20 = getMax20(allData);

    // 出现次数最多的20个数
    std::vector<std::pair<int,int>> top20Freq = getTop20Freq(allData);

    // 朴素算法：遍历allData找最大20个数（不排序，只简单选择20个最大）
    std::vector<int> naiveMax20;
    for (int i = 0; i < 20; i++) naiveMax20.push_back(INT_MIN);
    for (int v : allData) {
        int minIndex = 0;
        int minVal = naiveMax20[0];
        for (int i = 1; i < 20; i++) {
            if (naiveMax20[i] < minVal) {
                minIndex = i;
                minVal = naiveMax20[i];
            }
        }
        if (v > minVal) naiveMax20[minIndex] = v;
    }
    std::sort(naiveMax20.begin(), naiveMax20.end(), std::greater<int>());

    // 显示结果
    ui->textEdit_read->append(QString("最大20个数（堆）:"));
    QString max20str;
    for (int v : max20) max20str += QString::number(v) + " ";
    ui->textEdit_read->append(max20str);

    ui->textEdit_read->append(QString("最大20个数（朴素）:"));
    QString naiveStr;
    for (int v : naiveMax20) naiveStr += QString::number(v) + " ";
    ui->textEdit_read->append(naiveStr);

    ui->textEdit_read->append(QString("出现次数最多的20个数（数 频）:"));
    QString freqStr;
    for (auto& p : top20Freq) {
        freqStr += QString("%1:%2 ").arg(p.first).arg(p.second);
    }
    ui->textEdit_read->append(freqStr);
}

