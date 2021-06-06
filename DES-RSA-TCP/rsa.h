// File: des.h
// Auth: WCH
// Date: 2021/3/14
// Syst: Linux(Ubuntu18.04)

#ifndef RSA_H_
#define RSA_H_
#include <cstring>
#include <iostream>
#include <time.h> 
#include <math.h>
#include "stdlib.h"
using namespace std;

struct Public_key
{
    unsigned int n;
    unsigned int e;
};
struct Secret_key
{
    unsigned int n;
    unsigned int d;
};
struct val
{
    unsigned int p;
    unsigned int q;
    unsigned int euler;
};

unsigned int mod_mul(unsigned int a, unsigned int b, unsigned int m)
{
    return (a % m) * (b % m) % m;
}

unsigned int mod_pow(unsigned int a, unsigned int b, unsigned int m)
{
    unsigned int base = a, pow = b, init = 1;
    while(pow)
    {
        while(!(pow & 1))
        {
            pow = pow >> 1;
            base = mod_mul(base, base, m);
        }
        pow--;
        init = mod_mul(base, init, m);
    }
    return init;
}

long R_Mround(unsigned int &n)
{
    unsigned int a, q, k, v;
    q = n - 1;
    k = 0;
    while(!(q & 1))
    {
        k++;
        q = q >> 1;
    }
    a = 2 + rand() % (n - 3);
    v = mod_pow(a, q, n);

    if (v == 1) return 1;

    for(int i = 0; i < k; i++)
    {
        unsigned int t = 1;
        for(int j = 0; j < i; j++)
        {
            t = t * 2;
        }
        if(mod_pow(a, t * q, n) == n - 1) return 1;
    }

    return 0;
}

long R_M(unsigned int &n, long time)
{
    for(long i = 0; i < time; i++)
    {
        if(!R_Mround(n)) return 0;
    }
    return 1;
}

class Rsa
{
    Public_key pk;
    Secret_key sk;
    val v;

    unsigned int Prime(int bits);    // 生成质数
    unsigned int Euler(unsigned int p, unsigned int q);  // 欧拉函数
    unsigned int Gcd(unsigned int a,unsigned int b);  //最大公约数
    unsigned int E(unsigned int n);   //随机产生e
    unsigned int Euclid(unsigned int e, unsigned int t);//计算 d

   
public:
    void check_key();
    Public_key get_pk();
    Secret_key get_sk();  
    static unsigned int Encode(unsigned int m, Public_key cKey);
    static unsigned int Decode(unsigned int c, Secret_key cKey);
    void init();   // 创建 Rsa 对象时调用，初始化参数
};

unsigned int Rsa::Prime(int bits)
{
    unsigned int basenum;

    do
    {
        basenum = (unsigned int)1 << (bits - 1);
        basenum += rand() % basenum;
        basenum |= 0x1;
    }while(!R_M(basenum, 30));
    
    return basenum;
}

unsigned int Rsa::Euler(unsigned int p, unsigned int q)
{
    unsigned int res;
    res = (p - 1) * (q - 1);
    return res;
}

unsigned int Rsa::Gcd(unsigned int a, unsigned int b)
{
    int t;
    int big, sma;
    if(a > b) {big = a; sma = b;}
    else if(b > a) {big = b; sma = a;}
    else return a;
    while(big % sma)
    {
        t = sma; sma = big % sma; big = t;
    }
    return sma;
}

unsigned int Rsa::E(unsigned int n)
{
    unsigned int a;
    do
    {
        a = 1 + rand() % (n - 1);
    }while (Gcd(n, a) != 1);
    
    return a;
}

unsigned int Rsa::Euclid(unsigned e, unsigned int n)
{
    unsigned long max = 0xffffffffffffffff - n;
    unsigned int i = 1;
    while(true)
    {
        if(((i * n) + 1) % e == 0) return ((i * n) + 1) / e;
        i++;
        unsigned int tmp = (i + 1) * n;
        if(tmp > max) return 0;
    }
    return 0;
}

void Rsa::check_key()
{
    printf("n: %d\td: %d\te: %d\tp: %d\tq: %d\teuler: %d\n", pk.n, sk.d, pk.e, v.p, v.q, v.euler);
}

void Rsa::init()
{
    printf("init begin\r\n");
    srand((unsigned)time(0));
    v.p = Prime(8);
    v.q = Prime(8);
    pk.n = sk.n = v.p * v.q;
    v.euler = Euler(v.p, v.q);
    pk.e = E(v.euler);
    sk.d = Euclid(pk.e, v.euler);
    printf("init end\r\n");
}

Public_key Rsa::get_pk()
{
    return this->pk;
}

Secret_key Rsa::get_sk()
{
    return this->sk;
}

unsigned int Rsa::Encode(unsigned int m, Public_key cKey)
{
    return mod_pow(m, cKey.e, cKey.n);
}

unsigned int Rsa::Decode(unsigned int s, Secret_key cKey)
{
    return mod_pow(s, cKey.d, cKey.n);
}

#endif

