#include "../include/SHA256.h"

using namespace std;


vector<unsigned long> SHA256::resizeBlock(vector<unsigned long> input)
{
    vector<unsigned long> output(16);
    for(int i = 0; i < 64; i = i + 4)
    {
        bitset<32> temp(0);
        // Shift the blocks to their assigned spots and OR them with the original
        // to combine them.
        temp = (unsigned long) input[i] << 24;
        temp |= (unsigned long) input[i + 1] << 16;
        temp |= (unsigned long) input[i + 2] << 8;
        temp |= (unsigned long) input[i + 3];

        output[i/4] = temp.to_ulong();
    }
    return output;
}

string show_as_hex(unsigned long input)
{
    bitset<32> bs(input);
    unsigned long n = bs.to_ulong();
    stringstream sstream;
    sstream << std::hex << std::setw(8) << std::setfill('0') << n;
    string temp;
    sstream >> temp;
    return temp;
}

vector<unsigned long> SHA256::convertToBinary(const string input)
{
    vector<unsigned long> block;
    for (int i = 0; i < input.size(); ++i)
    {
        // Temporary variable to store the 8 bit pattern
        bitset<8> b((unsigned) input.c_str()[i]);
        block.push_back(b.to_ulong());
    }
    return block;
}

vector<unsigned long> SHA256::padTo512bits(vector<unsigned long> block)
{
    unsigned long l = block.size() * 8;
    // Padding is l + 1 + k = 448 mod 512
    // Simplified to: l + 1 + k = 448
    //		  448 - 1 - l = k
    //		  447 - l = k
    // l = length of message in bits
    // k = quantity of zeros to add so new message will be a multiple of 512.
    long k = 447 - l;
    unsigned long t1 = 0x80;
    block.push_back(t1);
    k = k - 7;
    for(int i = 0; i < k / 8; i++)
    {
        block.push_back(0x00000000);
    }

    bitset<64> big_64bit_blob(l);

    string big_64bit_string = big_64bit_blob.to_string();

    bitset<8> temp_string_holder1(big_64bit_string.substr(0,8));

    block.push_back(temp_string_holder1.to_ulong());

    for(int i = 8; i < 63; i=i+8)
    {
        bitset<8> temp_string_holder2(big_64bit_string.substr((unsigned) i,8));
        block.push_back(temp_string_holder2.to_ulong());
    }
    return block;
}

string SHA256::computeHash(const vector<unsigned long> block)
{
    unsigned long k[64] = {
            0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,
            0x923f82a4,0xab1c5ed5,0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,
            0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,0xe49b69c1,0xefbe4786,
            0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
            0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,
            0x06ca6351,0x14292967,0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,
            0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,0xa2bfe8a1,0xa81a664b,
            0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
            0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,
            0x5b9cca4f,0x682e6ff3,0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,
            0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
    };

    unsigned long static H0 = 0x6a09e667;
    unsigned long static H1 = 0xbb67ae85;
    unsigned long static H2 = 0x3c6ef372;
    unsigned long static H3 = 0xa54ff53a;
    unsigned long static H4 = 0x510e527f;
    unsigned long static H5 = 0x9b05688c;
    unsigned long static H6 = 0x1f83d9ab;
    unsigned long static H7 = 0x5be0cd19;

    unsigned long W[64];

    for(int t = 0; t <= 15; t++)
    {
        W[t] = block[t] & 0xFFFFFFFF;
    }

    for(int t = 16; t <= 63; t++)
    {
        W[t] = SSIG1(W[t-2]) + W[t-7] + SSIG0(W[t-15]) + W[t-16];
        W[t] = W[t] & 0xFFFFFFFF;
    }

    unsigned long temp1;
    unsigned long temp2;
    unsigned long a = H0;
    unsigned long b = H1;
    unsigned long c = H2;
    unsigned long d = H3;
    unsigned long e = H4;
    unsigned long f = H5;
    unsigned long g = H6;
    unsigned long h = H7;

    for( int t = 0; t < 64; t++)
    {
        temp1 = h + EP1(e) + CH(e,f,g) + k[t] + W[t];
        temp2 = EP0(a) + MAJ(a,b,c);
        h = g;
        g = f;
        f = e;
        e = (d + temp1) & 0xFFFFFFFF; // Makes sure that we are still using 32 bits.
        d = c;
        c = b;
        b = a;
        a = (temp1 + temp2) & 0xFFFFFFFF; // Makes sure that we are still using 32 bits.
    }

    H0 = (H0 + a) & 0xFFFFFFFF;
    H1 = (H1 + b) & 0xFFFFFFFF;
    H2 = (H2 + c) & 0xFFFFFFFF;
    H3 = (H3 + d) & 0xFFFFFFFF;
    H4 = (H4 + e) & 0xFFFFFFFF;
    H5 = (H5 + f) & 0xFFFFFFFF;
    H6 = (H6 + g) & 0xFFFFFFFF;
    H7 = (H7 + h) & 0xFFFFFFFF;

    return show_as_hex(H0) + show_as_hex(H1) + show_as_hex(H2) +
           show_as_hex(H3) + show_as_hex(H4) + show_as_hex(H5) +
           show_as_hex(H6) + show_as_hex(H7);
}

string SHA256::hashData(const string& input)
{
    try
    {
        if(input.size() > 55)
        {
            throw "The message length should be no more than 55 characters!\n";
        }
    }
    catch(const char* ex)
    {
        cout << ex;
        return "";
    }
    vector<unsigned long> block;
    // First convert it to a vector of strings representing 8 bit variables.
    block = SHA256::convertToBinary(input);

    // Pad it so that the message will be a full 512 bits long.
    block = SHA256::padTo512bits(block);

    // Combine the seperate 8 bit sections into single 32 bit sections.
    block = SHA256::resizeBlock(block);

    string hash = SHA256::computeHash(block);
    return hash;
}