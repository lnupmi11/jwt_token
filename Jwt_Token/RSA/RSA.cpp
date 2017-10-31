#include "../Include/RSA.h"


int gcd(int a, int h)
{
    int temp;
    while (true)
    {
        temp = a % h;
        if(temp == 0)
        {
            return h;
        }
        a = h;
        h = temp;
    }
}

bool RSA::isPrime(int digit)
{
    int i;
    for (i = 2; i <= sqrt(digit); i++)
    {
        if (digit % i == 0)
        {
            return false;
        }
    }
    return true;
}

int RSA::countModulus()
{
    return FIRST_PRIME * SECOND_PRIME;
}

int RSA::findExponent()
{
    int e = 2;
    while (e < countEilerFunc())
    {
        // e must be co-prime to countEilerFunc() and
        // smaller than countEilerFunc().
        if(gcd(e, countEilerFunc()) == 1)
        {
            break;
        }
        else
        {
            e++;
        }
    }
}

int RSA::countEilerFunc()
{
    return (FIRST_PRIME - 1) * (SECOND_PRIME - 1);
}

int RSA::generatePrivateKey(int digit)
{
    return (digit * countEilerFunc() + 1) / findExponent();
}

string RSA::EncodeUrl(const string &data)
{
    //TODO
}

string RSA::DecodeUrl(const string &data)
{
    //TODO
}