// File: des.h
// Auth: WCH
// Date: 2021/3/14
// Syst: Linux(Ubuntu18.04)

#ifndef DES_H_
#define DES_H_
#include<cstring>
#include <bitset>
#include <iostream>
#include <math.h>
using namespace std;
//初始置换IP
static int IP[64] = {
    58,50,42,34,26,18,10,2,60,52,44,36,28,20,12,4,
    62,54,46,38,30,22,14,6,64,56,48,40,32,24,16,8,
    57,49,41,33,25,17,9,1,59,51,43,35,27,19,11,3,
    61,53,45,37,29,21,13,5,63,55,47,39,31,23,15,7 
};
//逆IP
static int IPreverse[64] = {
    40,8,48,16,56,24,64,32, 39,7,47,15,55,23,63,31,
    38,6,46,14,54,22,62,30, 37,5,45,13,53,21,61,29,
    36,4,44,12,52,20,60,28, 35,3,43,11,51,19,59,27,
    34,2,42,10,50,18,58,26, 33,1,41,9,49,17,57,25
};
//变换矩阵P和扩展矩阵E
static int P[32] = {
    16,7,20,21,29,12,28,17,1,15,23,26,5,18,31,10,
    2,8,24,14,32,27,3,9,19,13,30,6,22,11,4,25
};
static int E[48] = {
    32,1,2,3,4,5,4,5,6,7,8,9,
	8,9,10,11,12,13,12,13,14,15,16,17,
	16,17,18,19,20,21,20,21,22,23,24,25,
	24,25,26,27,28,29,28,29,30,31,32,1
};
//S盒
static int S[8][4][16] = {
    14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
    0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
    4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
    15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13,
    15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
    3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
    0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
    13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9,
    10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
    13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
    13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
    1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12,
    7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
    13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
    10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
    3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14,
    2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
    14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
    4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
    11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3,
    12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
    10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
    9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
    4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13,
    4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
    13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
    1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
    6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12,
    13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
    1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
    7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
    2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11
};
//等分密钥以及密钥移位,密钥选取
static int PC1[56] = {
    57, 49, 41, 33, 25, 17,  9,  1, 58, 50, 42, 34, 26, 18,
	10,  2, 59, 51, 43, 35, 27, 19, 11,  3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,  7, 62, 54, 46, 38, 30, 22,
	14,  6, 61, 53, 45, 37, 29, 21, 13,  5, 28, 20, 12,  4
};  
static int LShift[16] = {
    1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1
};
static int PC2[48] = {
    14, 17, 11, 24,  1,  5,  3, 28, 15,  6, 21, 10,
	23, 19, 12,  4, 26,  8, 16,  7, 27, 20, 13,  2,
	41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48,
	44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32
};

class Des
{
    private:
        bitset<64> key; //64位密钥
        bitset<48> subKey[16];//16轮的子密钥

        bitset<64> strToBit(string s);//8字节大小字符数组转换成二进制
        string bitToStr(bitset<64> src); //64位转换为8个字符
        void generateKeys(); //生成16个子密钥
        bitset<28> leftShift(bitset<28> k, int shift);//循环移位操作
        bitset<32> func(bitset<32> R, bitset<48> k);//生成子密钥过程中的f函数
        bitset<64> encode_bit(bitset<64> m); //64位加密
        bitset<64> decode_bit(bitset<64> c);//64位解密
    
    public:
        string encode(string m); //流数据加密
        string decode(string c); //流数据解密
        bool setKey(string key); //实例化对象，设置密钥

};


bool Des::setKey(string key)
{
    //先判断是否为64位即8个字节
    if(sizeof(key) != 8)
        return false;
    else
        this->key = strToBit(key);
    generateKeys(); //生成16个子密钥
    return true;
}
bitset<64> Des:: strToBit(string s)
 {
    bitset<64> bits;
	for(int i=0; i<8; i++)
    {
		for(int j=0; j<8; j++)
		{
            bits[56 - 8 * i + j] = ((s[i]>>j) & 1);
        }
    }
	return bits;
 }
string Des:: bitToStr(bitset<64> src)
{
    string res;
    for(int i=0; i<8; ++i)
    {
        int value = 0;
        for(int j=0; j<8; ++j)
        {
            value += src[56 - 8 * i + j] * pow(2,j);
        }
       res += char(value);
    }
    return res;
}
//左右28位的循环移位操作函数
bitset<28> Des::leftShift(bitset<28> k, int shift)
{
    bitset<28> temp = k;
	for(int i = 27; i >= 0; i--)
	{
		if(i-shift<0)
			k[i] = temp[i-shift+28];
		else
			k[i] = temp[i-shift];
	}
	return k;
}
void Des::generateKeys()
{
    bitset<56> new_res;
    //第一步去掉奇偶校验位，64位密钥根据PC1表置换为56位
    for(int i = 0; i < 56; i++)
    {
        new_res[55 - i] = key[64 - PC1[i]];
    }
    //第二步 16次循环，56位分为左右各28位，分别进行循环移位，拼接之后再按PC2表进行置换得到子密钥
    bitset<28> left;
    bitset<28> right;
    for(int i = 0; i < 16; i++)
    {
        for(int j = 0; j < 28; j++)
        {
            left[27 - j] = new_res[55 - j];
        }
         for(int j = 0; j < 28; j++)
        {
            right[j] = new_res[j];
        }
        //下一步对左右两部分进行循环左移
        left = leftShift(left,LShift[i]);
        right = leftShift(right,LShift[i]);

        //拼接在一起
        for(int j = 0; j < 28; j++)
        {
            new_res[55 - j] = left[27 - j]; 
        }
         for(int j = 0; j < 28; j++)
        {
            new_res[j] = right[j]; 
        }
        //根据PC2表置换，去掉某些位，得到48位子密钥
        bitset<48> real_key;
        for(int j = 0; j < 48; j++)
        {
            real_key[j] = new_res[56 - PC2[j]];
        }
        this->subKey[i] = real_key;
    }
}
//右边的32位与某一密钥k进行f运算
bitset<32> Des:: func(bitset<32> R, bitset<48> k)
{
    //第一步，将32位数据根据扩展表扩展为48位
    bitset<48> expand;
    for(int i = 0; i < 48; i++)
    {
        expand[47 - i] = R[32 - E[i]];
    }
    //第二步，两者异或作为S盒的输入
    expand = expand ^ k;
    //第三步，将expand分为8组，每组6bit，查找S盒置换
    //6位中高低2位作为行数，中间4位作为列数，查找对应值即为替换后的值
    bitset<32> res; //S盒选择压缩后的结果
    int h = 0;
    for(int i = 0; i < 48; i += 6)
    {
        int row = expand[47 - i]*2 + expand[42 - i];
        int col = expand[47 - i - 1]*8 + expand[47 - i - 2]*4 + expand[47- i - 3]*2 + expand[47 - i - 4];
        int value = S[i / 6][row][col];
        bitset<4> temp = value;
        res[31 - h] = temp[3];
        res[31 - h - 1] = temp[2];
        res[31 - h - 2] = temp[1];
        res[31 - h - 3] = temp[0];
        h += 4;
    }
    //最后一步，将32位数进行P置换
    bitset<32> real_res;
    for(int i = 0; i < 32; i++)
    {
        real_res[31 - i] = res[32 - P[i]];
    }
    return real_res;
}
//将64位明文加密得到64位密文
bitset<64> Des:: encode_bit(bitset<64> plain)
{
    bitset<64> first_ip_displace;
    //第一步是初始置换IP，位数是从左往右的，最左为第一位
    for(int i = 0; i < 64;i++)
    {
        first_ip_displace[63 - i] = plain[64 - IP[i]];
    }
    //第二步，将64位明文分成左右各32位

    bitset<32> left;
    bitset<32> right;
    for(int i = 32; i < 64;i++)
    {
        left[i-32] = first_ip_displace[i];
    }
    for(int i = 0; i < 32;i++)
    {
        right[i] = first_ip_displace[i];
    }
    //第三步 进行16轮迭代
    bitset<32> new_left;
    for(int i = 0; i < 16;i++)
    {
        //left为上一步的右边，右边是下一轮的左边和旧的右边异或的结果
        new_left = right;
        right = left ^ func(right,subKey[i]);
        left = new_left;
    }
    bitset<64> new_res;
    //第四步 合并left和right，上一步结束时left和right是相反的
    //所以，此时应该left是低32位，right是高32位
    for(int i = 0;i < 32;i++)
    {
        new_res[i] = left[i];
    }
    for(int i = 32;i < 64;i++)
    {
        new_res[i] = right[i - 32];
    }
    //最后一步，逆初始置换y=IP-1(L16R16)
    bitset<64> cypher;
    for(int i = 0; i < 64; i++)
    {
        cypher[63 - i] = new_res[64 - IPreverse[i]];
    }
    //得到该64位的密文，返回
    return cypher;
}
//将64位明文加密得到64位密文
bitset<64> Des:: decode_bit(bitset<64> cypher)
{
    bitset<64> first_ip_displace;
    //第一步是初始置换IP，位数是从左往右的，最左为第一位
    for(int i = 0; i < 64;i++)
    {
        first_ip_displace[63 - i] = cypher[64 - IP[i]];
    }
    //第二步，将64位明文分成左右各32位

    bitset<32> left;
    bitset<32> right;
    for(int i = 32; i < 64;i++)
    {
        left[i-32] = first_ip_displace[i];
    }
    for(int i = 0; i < 32;i++)
    {
        right[i] = first_ip_displace[i];
    }
    //第三步 进行16轮迭代，解密时逆序应用子密钥
    bitset<32> old_left,new_left;
    for(int i = 0; i < 16;i++)
    {
        new_left = right;
        right = left ^ func(right,subKey[15 - i]);
        left = new_left;
    }
    bitset<64> new_res;
    //第四步 合并left和right，上一步结束时left和right是相反的
    //所以，此时应该left是低32位，right是高32位
    for(int i = 0;i < 32;i++)
    {
        new_res[i] = left[i];
    }
    for(int i = 32;i < 64;i++)
    {
        new_res[i] = right[i - 32];
    }
    //最后一步，逆初始置换y=IP-1(L16R16)
    bitset<64> plain;
    for(int i = 0; i < 64; i++)
    {
        plain[63 - i] = new_res[64 - IPreverse[i]];
    }
    //得到该64位的明文，返回
    return plain;
}
string Des::encode(string plain)
{
    int size = plain.size(); //总的长度
    int round = size / 8;

    int i = 0;
    int h = 0;
    string res; //加密后的结果
    while(i < round) //8字节
    {
        //分成8个字节单元的组
        string s;
        for(int j = 0; j < 8; j++)
        {
            s += plain[i * 8 + j];
        }
        bitset<64> temp = strToBit(s);
        bitset<64> cypher = encode_bit(temp);
        res += bitToStr(cypher);
        h += 8;
        i++;
    }
    //剩余不足8字节,补 凑成8字节
    if(h < size)
    {
        string temp1;
        for(int i = h; i < h+8; i++)
        {
            if(i<size)temp1 += plain[i];
            else temp1 +=" ";
        }
        bitset<64> temp2 = strToBit(temp1);
        bitset<64> cypher = encode_bit(temp2);
        res += bitToStr(cypher);
    }
    return res;
}
string Des::decode(string cypher)
{
    int size = cypher.size(); //总的长度
    int round = size / 8;
    //cout<<"Dpart"<<endl;
    //cout<<"密文长度"<<size<<"字节"<<endl;
    int i = 0;
    int h = 0;
    string res; //加密后的结果
    while(i < round) //8字节
    {
        //分成8个字节单元的组
        string s;
        for(int j = 0; j < 8; j++)
        {
            s += cypher[i * 8 + j];
        }
        bitset<64> temp = strToBit(s);
        bitset<64> plain = decode_bit(temp);
        res += bitToStr(plain);
        h += 8;
        i++;
    }
    //cout<<h<<endl;
    //剩余不足8字节,补0凑成8字节
    if(h < size)
    {
        string temp1;
        for(int i = h; i < h+8; i++)
        {
            if(i<size)temp1 += cypher[i];
            else temp1 +=" ";
        }
        bitset<64> temp2 = strToBit(temp1);
        bitset<64> plain = decode_bit(temp2);
        res += bitToStr(plain);
      //  cout<<"temp1:"<<temp1<<" length: "<<temp1.size()<<endl;
    }
    //cout<<"Decode:"<<res<<endl;
    return res;
}

#endif
