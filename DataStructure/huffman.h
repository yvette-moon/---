/***********************************************************
*
* 文件名称： huffman.h
* 文件标识：无
* 内容摘要：头文件
* 其它说明：无
* 当前版本： V1.0
* 作   者：孙艺之
* 完成日期： 20260225

**********************************************************/

/*********************************************************
* 功能描述：头文件
* 返回值： 0-成功   其他-失败
* 其它说明：消息字段之间用分号(;)分隔
************************************************************/
#include <opencv2/opencv.hpp>
#include<iostream>
#include<map>
#include<string>

using namespace std;
using namespace cv;

struct HuffmanNode{
    int pixel;
    int freq;
    HuffmanNode* left;
    HuffmanNode* right;
    HuffmanNode(int p,int f); // 构造函数：用于创建节点并初始化成员变量
};
// 统计图像中每个像素值出现的频率
// 功能：遍历灰度图像，统计 0~255 每个像素值出现的次数
// 参数：img - 输入灰度图像（OpenCV Mat 矩阵）
// 返回：key 为像素值，value 为该像素值出现次数的表
map<int,int> calcFrequency(const Mat& img);
// 构建 Huffman 树
// 功能：根据像素频率表构建 Huffman 树
// 实现：使用最小堆不断取出频率最小的两个节点进行合并
// 参数：freqMap - 像素值与出现频率的映射表
// 返回：Huffman 树的根节点指针
HuffmanNode* buildHuffmanTree(map<int,int>& freqMap);
// 生成 Huffman 编码表
// 功能：通过递归遍历 Huffman 树，为每个像素值生成对应的 Huffman 编码
// 规则：左子树路径记为 '0'，右子树路径记为 '1'
// huffmanCode - 存储最终编码结果的映射表
void generateHuffmanCode(
    HuffmanNode* root,// root - 当前遍历到的 Huffman 树节点
    string code,// code - 当前路径对应的编码字符串
    map<int,string>& huffmanCode
);
// 图像编码
// 功能：根据 Huffman 编码表将原始图像转换为二进制编码串
// 实现：遍历图像每个像素，将其替换为对应的 Huffman 编码
// 参数：
// img - 输入灰度图像
// huffmanCode - 像素值到 Huffman 编码的映射表
// 返回：编码后的二进制字符串
string encodeImage(
    const Mat& img,
    map<int,string>& huffmanCode
);

void savecode(const string& path,const string& encoded);
// 图像解码
// 功能：根据 Huffman 树将编码后的二进制串恢复为原始图像
// 实现：从根节点开始，根据 0 或 1 在 Huffman 树中向左或向右遍历
// 参数：
// encoded - Huffman 编码字符串
// root - Huffman 树根节点\ rows - 图像行数\ cols - 图像列数
// 返回：解码后的 Mat 图像
Mat decodeImage(
    const string& encoded,
    HuffmanNode* root,
    int rows,
    int cols
);
// 计算压缩率
// 功能：比较编码前后的数据大小，计算压缩效果
// 原始大小：图像像素数 × 8 bit
// 压缩大小：编码字符串长度
// 参数：
// img - 原始图像
// encoded - Huffman 编码结果
void calcCompressionRatio(const Mat& img,const string& encoded);
struct MinHeap {
    HuffmanNode* data[600];
    int size;

    MinHeap();

    void push(HuffmanNode* node);
    HuffmanNode* pop();
    void up(int index);
    void down(int index);
};