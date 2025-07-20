#ifndef CREATERANDNUMS_H
#define CREATERANDNUMS_H

#include <stdlib.h>
#include <time.h>
#include <chrono>
#include <iostream>
#include <cstring>
#include <vector>
#include <queue>
#include <map>
#include <QString>
#include <QDebug>
#define SUMNUM 10000  // 随机数的数量
#define MAXNUM 999    // 随机数范围 0 --- MAXNUM

#pragma warning(disable:4996)

using namespace std;
using namespace chrono;

struct HuffmanNode {
    int value;
    int freq;
    HuffmanNode* left;
    HuffmanNode* right;
    HuffmanNode(int v, int f) : value(v), freq(f), left(nullptr), right(nullptr) {}
};

class CreateRandNums {
public:
    int randseed;
    int* intMSG = NULL;
    QString strMSG = "";
    char binaryCode[SUMNUM][24]; // 实际只用于普通编码演示
    QString binaryString = "";   // 存储编码后的总二进制串
    map<int, QString> huffmanCodeMap; // 哈夫曼编码表
    HuffmanNode* huffmanRoot = nullptr;

    CreateRandNums() {
        randseed = -1;
        intMSG = new int[SUMNUM];
    }

    ~CreateRandNums() {
        if (intMSG != NULL) {
            delete[] intMSG;
        }
        qDebug() << "Object is being deleted";
    }

    void AddRandNums() {
        randseed = randseed + 1;
        srand(randseed);
        for (int i = 0; i < SUMNUM; i++) {
            intMSG[i] = (rand() * RAND_MAX % MAXNUM + rand()) % MAXNUM;
        }
    }

    void Transform() {
        strMSG = "";
        for (int i = 0; i < SUMNUM; i++) {
            QString oneMSG = QString::number(intMSG[i]);
            strMSG = strMSG + oneMSG + " ";
        }
    }

    // 1. 普通无压缩二进制编码
    void ToBinaryCode() {
        binaryString.clear();
        for (int i = 0; i < SUMNUM; ++i) {
            QString bin = QString::number(intMSG[i], 2).rightJustified(10, '0'); // 用10位覆盖最大999
            binaryString += bin;
            strcpy(binaryCode[i], bin.toStdString().c_str());
        }
    }

    // 2. 普通二进制解码
    void BinaryCodeToIntArray() {
        int index = 0;
        for (int i = 0; i < binaryString.length(); i += 10) {
            QString bin = binaryString.mid(i, 10);
            intMSG[index++] = bin.toInt(nullptr, 2);
        }
    }

    // 3. 哈夫曼编码
    void ToHaffmanCode() {
        huffmanCodeMap.clear();
        binaryString.clear();

        map<int, int> freq;
        for (int i = 0; i < SUMNUM; i++)
            freq[intMSG[i]]++;

        auto cmp = [](HuffmanNode* a, HuffmanNode* b) { return a->freq > b->freq; };
        priority_queue<HuffmanNode*, vector<HuffmanNode*>, decltype(cmp)> pq(cmp);

        for (auto& kv : freq)
            pq.push(new HuffmanNode(kv.first, kv.second));

        while (pq.size() > 1) {
            HuffmanNode* l = pq.top(); pq.pop();
            HuffmanNode* r = pq.top(); pq.pop();
            HuffmanNode* parent = new HuffmanNode(-1, l->freq + r->freq);
            parent->left = l;
            parent->right = r;
            pq.push(parent);
        }

        huffmanRoot = pq.top();
        function<void(HuffmanNode*, QString)> buildMap = [&](HuffmanNode* node, QString path) {
            if (!node) return;
            if (node->value != -1) {
                huffmanCodeMap[node->value] = path;
                return;
            }
            buildMap(node->left, path + "0");
            buildMap(node->right, path + "1");
        };
        buildMap(huffmanRoot, "");

        for (int i = 0; i < SUMNUM; ++i)
            binaryString += huffmanCodeMap[intMSG[i]];
    }

    // 4. 哈夫曼解码
    void HaffmanCodeToIntArray() {
        int index = 0;
        HuffmanNode* node = huffmanRoot;
        for (int i = 0; i < binaryString.length(); ++i) {
            node = (binaryString[i] == '0') ? node->left : node->right;
            if (node->left == nullptr && node->right == nullptr) {
                intMSG[index++] = node->value;
                node = huffmanRoot;
                if (index >= SUMNUM) break;
            }
        }
    }

    // 5. 二进制串转换为可传输字符串
    void TransformBinaryCode() {
        strMSG = binaryString; // 简单处理，可自行加密压缩
    }
};

#endif // CREATERANDNUMS_H
