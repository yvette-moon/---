/***********************************************************
*
* 文件名称： huffman.cpp
* 文件标识：无
* 内容摘要：函数体
* 其它说明：无
* 当前版本： V1.0
* 作   者：孙艺之
* 完成日期： 20260225

**********************************************************/

/*********************************************************
* 功能描述：实现所有功能
* 返回值： 0-成功   其他-失败
* 其它说明：消息字段之间用分号(;)分隔
************************************************************/
#include "huffman.h"
#include <queue>
#include <fstream>
#include <iostream>

using namespace std;
using namespace cv;

HuffmanNode::HuffmanNode(int p,int f){
    pixel=p;
    freq=f;
    left=nullptr;// 左子节点初始化为空
    right=nullptr;
}
// 最小堆构造函数
MinHeap::MinHeap(){
    size=0;
}
void MinHeap::push(HuffmanNode* node){
    data[size]=node;// 新节点先插入到堆尾
    up(size);// 通过上浮调整堆结构
    size++;
}
HuffmanNode* MinHeap::pop(){
    HuffmanNode* top=data[0];// 堆顶即最小节点
    data[0]=data[size-1];// 用最后一个节点替换堆顶
    size--;
    down(0); // 通过下沉恢复最小堆性质
    return top;
}
void MinHeap::up(int index){
    while(index>0){
        int parent=(index-1)/2;// 计算父节点下标
        if(data[parent]->freq<=data[index]->freq)
        break;
        swap(data[parent],data[index]);// 否则交换节点
        index=parent;
    }
}
void MinHeap::down(int index) {
    while(1){
        int left=index*2+1;
        int right=index*2+2;//找左右孩子
        int smallest=index;
        // 若左孩子存在且更小
        if(left<size&&data[left]->freq < data[smallest]->freq)
        smallest=left;
         // 若右孩子存在且更小
        if(right<size&&data[right]->freq < data[smallest]->freq)
        smallest=right;
        if(smallest==index)
        break;
        swap(data[index],data[smallest]);
        index=smallest;
    }
}
map<int,int> calcFrequency(const Mat& img){
    map<int,int> freq; // 存储像素值及其出现次数
    for(int i=0;i<img.rows;i++){
        for(int j=0;j<img.cols;j++){
            freq[img.at<uchar>(i,j)]++;
        }
    }
    return freq;
}
// 根据像素频率构建 Huffman 树
HuffmanNode* buildHuffmanTree(map<int,int>& freqMap){
    //priority_queue<HuffmanNode*,vector<HuffmanNode*>,Compare> pq;
    
    MinHeap heap;
    for(auto &p:freqMap){
        heap.push(new HuffmanNode(p.first,p.second));
    } // 将每个像素频率构建成一个叶子节点放入堆中
    // 当堆中节点数量大于1时不断合并
    while(heap.size>1){
        HuffmanNode* left=heap.pop();
        HuffmanNode* right=heap.pop();
        HuffmanNode* parent=new HuffmanNode(-1,left->freq+right->freq);

        parent->left=left;
        parent->right=right;
        heap.push(parent); // 合并后的节点重新放入堆
    }
    return heap.pop();
}
// 递归生成 Huffman 编码
void generateHuffmanCode(
    HuffmanNode* root,
    string code,
    map<int,string>& huffmanCode
){
    if(!root)return ;  // 若节点为空直接返回
     // 若为叶子节点
    if(root->pixel!=-1){
        huffmanCode[root->pixel]=code;
    }
    generateHuffmanCode(root->left,code+"0",huffmanCode);
    generateHuffmanCode(root->right,code+"1",huffmanCode);
}
string encodeImage(
    const Mat& img,
    map<int,string>& huffmanCode
){
    string encoded;
    for(int i=0;i<img.rows;i++){
        for(int j=0;j<img.cols;j++){
            // 将每个像素替换为对应 Huffman 编码
           encoded+=huffmanCode[img.at<uchar>(i,j)];
        }
    }
    return encoded;
}
void savecode(const string& path,const string& encoded){
    ofstream out(path);  // 打开输出文件
    out<<encoded; // 写入编码字符串
}
Mat decodeImage(
    const string& encoded,
    HuffmanNode* root,
    int rows,
    int cols
){
    Mat img(rows,cols,CV_8UC1);// 创建解码后的灰度图像
    HuffmanNode* cur=root;// 当前遍历节点
    int index=0;// 编码字符串读取位置

    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
             // 沿 Huffman 树查找直到到达叶子节点
           while(cur->pixel==-1){
            if(encoded[index]=='0')
            cur=cur->left;
            else cur=cur->right;
            index++;// 编码指针向后移动
           }
        img.at<uchar>(i,j)=cur->pixel;
        cur=root;// 回到根节点继续解码
        }
    }
    return img;
}
void calcCompressionRatio(const Mat& img,const string& encoded){
    double original=img.rows*img.cols*8;// 原始图像大小
    double encodedsize=encoded.size();
    cout<<"原始编码长度 : "<<original<<endl;
    cout<<"压缩后编码长度 : "<<encodedsize<<endl;
    cout<<"压缩率 : "<<encodedsize/original<<endl;
}
