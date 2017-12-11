#include <iostream>
#include <sstream>
#include <bitset>
#include <vector>
#include <iomanip>


#define ROTRIGHT(word,bits) (((word) >> (bits)) | ((word) << (32-(bits))))
#define SSIG0(x) (ROTRIGHT(x,7) ^ ROTRIGHT(x,18) ^ ((x) >> 3))
#define SSIG1(x) (ROTRIGHT(x,17) ^ ROTRIGHT(x,19) ^ ((x) >> 10))
#define CH(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))


#define EP0(x) (ROTRIGHT(x,2) ^ ROTRIGHT(x,13) ^ ROTRIGHT(x,22))
#define EP1(x) (ROTRIGHT(x,6) ^ ROTRIGHT(x,11) ^ ROTRIGHT(x,25))


using namespace std;


// Restriction: only massages less than 55 characters
class SHA256
{
private:
	// Converts the ASCII string to a binary representation.
	static vector<unsigned long> convertToBinary(const string input);

	// Pads the messages to make sure they are a multiple of 512 bits.
	static vector<unsigned long> padTo512bits(vector<unsigned long> block);

	// Changes the n 8 bit segments representing every ASCII character to 32 bit words.
	static vector<unsigned long> resizeBlock(vector<unsigned long> input);

	// The actual hash computing.
	static string computeHash(const vector<unsigned long> block);

public:
	// Hash computing using private functions
	static string hashData(const string& input);

};
