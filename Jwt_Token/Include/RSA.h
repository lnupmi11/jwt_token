#ifndef JWT_TOKEN_RSA_H
#define JWT_TOKEN_RSA_H

#include <iostream>
#include <cmath>
#include <string>


#define FIRST_PRIME  37
#define SECOND_PRIME  53


using namespace std;

int gcd(int a, int h);


class RSA
{
private:

    bool isPrime(int digit);

    int countModulus();
    // should be prime, must be < countEilerFunc()
    int findExponent();

    int generatePrivateKey(int digit);

    int countEilerFunc();

public:

    string EncodeUrl(const string& data);

    string DecodeUrl(const string& data);

};
#endif
