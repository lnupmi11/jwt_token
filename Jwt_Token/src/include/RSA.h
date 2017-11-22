#ifndef JWT_TOKEN_RSA_H
#define JWT_TOKEN_RSA_H

#include <iostream>
#include <cmath>
#include <string>
#include <vector>


using namespace std;


bool is_prime(long int digit);


class Rsa
{
private:
    static int lastPosEncoded;
    // data that is needed in both encoding and decoding
    static vector<int> bridge;

    static int findExponent();

    static int getPrivateKey(int param);

    static int countModulus();

    static int countEilerFunc();

public:

    static string encodeUrl(string str);

    static string decodeUrl(string str);
};



#endif
