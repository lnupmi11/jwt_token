#include "../include/RSA.h"


#define FIRST_PRIME 37
#define SECOND_PRIME 53
#define SHIFT 96


using namespace std;


int Rsa::lastPosEncoded = 0;

vector<int> Rsa::bridge;


bool is_prime(long int digit)
{
    int value;
    value = (int) sqrt(digit);
    for (int index = 2; index <= value; index++)
    {
        if (digit % index == 0)
        {
            return false;
        }
    }
    return true;
}

int Rsa::findExponent()
{
    int e = 0;
    for (int index = 2; index < Rsa::countEilerFunc(); index++)
    {
        if (Rsa::countEilerFunc() % index == 0)
        {
            continue;
        }
        if (is_prime(index) && index != FIRST_PRIME && index != SECOND_PRIME)
        {
            e = index;
            if (getPrivateKey(e) > 0)
            {
                break;
            }
        }
    }
    return e;
}

int Rsa::getPrivateKey(int param)
{
    int var = 1;
    while (true)
    {
        var = var + Rsa::countEilerFunc();
        if (var % param == 0)
        {
            return (var / param);
        }
    }
}

int Rsa::countModulus()
{
    return FIRST_PRIME * SECOND_PRIME;
}

int Rsa::countEilerFunc()
{
    return (FIRST_PRIME - 1) * (SECOND_PRIME - 1);
}

string Rsa::encodeUrl(string str)
{
    string result;
    bridge.resize(str.size());
    findExponent();
    int ct, var, length;
    int index = 0;
    length = (int) str.size();
    while (index != length)
    {
        str[index] = str[index] - (char) SHIFT;
        var = 1;
        for (int counter = 0; counter < Rsa::findExponent(); counter++)
        {
            var = var * str[index];
            var = var % Rsa::countModulus();
        }
        bridge[index] = var;
        ct = var + SHIFT;
        result += (char) ct;
        index++;
    }
    lastPosEncoded = index;
    return result;
}

string Rsa::decodeUrl(string str)
{
    int digit, value;
    int index = 0;
    string data;
    while (lastPosEncoded != index)
    {
        value = bridge[index];
        int var = 1;
        for (int counter = 0; counter < getPrivateKey(findExponent()); counter++)
        {
            var = var * value;
            var = var % Rsa::countModulus();
        }
        digit = var + SHIFT;
        str[index] = (char) digit;
        index++;
    }
    return str;
}