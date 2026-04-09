/***********************************************************
*
* 文件名称： main.cpp
* 文件标识：无
* 内容摘要：主程序
* 其它说明：无
* 当前版本： V1.0
* 作   者：孙艺之
* 完成日期： 20260225

**********************************************************/

/*********************************************************
* 功能描述：主程序
* 返回值： 0-成功   其他-失败
* 其它说明：消息字段之间用分号(;)分隔
************************************************************/
#include <opencv2/opencv.hpp>
#include<iostream>
#include<map>
#include<string>
#include "huffman.h"

using namespace std;
using namespace cv;

int main(){
    system("chcp 65001");
    Mat img=imread("data/longpaopao.png",IMREAD_GRAYSCALE);

    if(img.empty()){
      cout<<"图片加载失败"<<endl;
      return -1;
    }

    map<int,int> freqMap=calcFrequency(img);//统计频率

    HuffmanNode* root=buildHuffmanTree(freqMap);//建树

    map<int,string> huffmanCode;
    generateHuffmanCode(root,"",huffmanCode);//生成编码
    string encoded ;
    encoded=encodeImage(img,huffmanCode);//图像编码
    Mat decoded;
     decoded=decodeImage(encoded,root,img.rows,img.cols);//解码
    while(1){
        cout<<"1:输出哈夫曼编码表"<<endl;
        cout<<"2:保存编码结果"<<endl;
        cout<<"3:图像解码"<<endl;
        cout<<"4:计算压缩率"<<endl;
        cout<<"0:结束程序"<<endl;
        int n=-1;
        cin>>n;
     if(n==1){   
    cout<<"哈夫曼表 : "<<endl;
    for(auto &p:huffmanCode)
    cout<<p.first<<" : "<<p.second<<endl;
     }  
        
 
    if(n==2){
    savecode("output/encoded.txt",encoded);
    }  
       
    if(n==3){
    imwrite("output/decodelongpaopao.png",decoded);
    imshow("decoded",decoded);
    waitKey(0);
    }
    if(n==4){
    calcCompressionRatio(img,encoded);}
    cout<<"Finish"<<endl;
   
    if(n==0)
    break;
    }
    return 0;

}