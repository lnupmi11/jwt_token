#pragma once
#ifndef SHA256_H
#define SHA256_H

#include <string>
#include <vector>
#include <stdint.h>


using std::string;

#define SHA256_BLOCK_SIZE			64
#define SHA256_DIGEST_SIZE			32
#define SHA256_PAD_LENGTH			56
#define SHA256_DIGEST_STRING_SIZE	(SHA256_DIGEST_SIZE * 2 + 1)

typedef uint8_t		HashByte;
typedef uint32_t	HashWord;
typedef uint64_t	HashLong;

static const HashWord InitialHash[8] =
{
	0x6a09e667UL,
	0xbb67ae85UL,
	0x3c6ef372UL,
	0xa54ff53aUL,
	0x510e527fUL,
	0x9b05688cUL,
	0x1f83d9abUL,
	0x5be0cd19UL
};

static const HashWord TableRounds[64] =
{
	0x428a2f98UL, 0x71374491UL, 0xb5c0fbcfUL, 0xe9b5dba5UL, 0x3956c25bUL, 0x59f111f1UL, 0x923f82a4UL, 0xab1c5ed5UL,
	0xd807aa98UL, 0x12835b01UL, 0x243185beUL, 0x550c7dc3UL, 0x72be5d74UL, 0x80deb1feUL, 0x9bdc06a7UL, 0xc19bf174UL,
	0xe49b69c1UL, 0xefbe4786UL, 0x0fc19dc6UL, 0x240ca1ccUL, 0x2de92c6fUL, 0x4a7484aaUL, 0x5cb0a9dcUL, 0x76f988daUL,
	0x983e5152UL, 0xa831c66dUL, 0xb00327c8UL, 0xbf597fc7UL, 0xc6e00bf3UL, 0xd5a79147UL, 0x06ca6351UL, 0x14292967UL,
	0x27b70a85UL, 0x2e1b2138UL, 0x4d2c6dfcUL, 0x53380d13UL, 0x650a7354UL, 0x766a0abbUL, 0x81c2c92eUL, 0x92722c85UL,
	0xa2bfe8a1UL, 0xa81a664bUL, 0xc24b8b70UL, 0xc76c51a3UL, 0xd192e819UL, 0xd6990624UL, 0xf40e3585UL, 0x106aa070UL,
	0x19a4c116UL, 0x1e376c08UL, 0x2748774cUL, 0x34b0bcb5UL, 0x391c0cb3UL, 0x4ed8aa4aUL, 0x5b9cca4fUL, 0x682e6ff3UL,
	0x748f82eeUL, 0x78a5636fUL, 0x84c87814UL, 0x8cc70208UL, 0x90befffaUL, 0xa4506cebUL, 0xbef9a3f7UL, 0xc67178f2UL
};


class Sha256
{
private:

	uint32_t m_MessageSize;

	HashWord m_Hash[8];
	string m_HashString;

	inline HashWord MakeBigEndianWord(HashWord in);
	inline HashLong MakeBigEndianLong(HashLong in);

	void Rotate(HashWord* chunk);
	void Finalize(HashWord* chunk, unsigned int size);

	void GenerateString();

public:
	Sha256();
	~Sha256();

	void ComputeHash(HashByte* const Data, int DataSizeInBytes);
	inline std::string GetHashString() { return m_HashString; }

	bool operator == (const Sha256 &rhs);
	bool operator == (const std::string &rhs);
	bool operator == (const char* rhs);
	friend bool operator == (const std::string &lhs, const Sha256 &rhs);
	friend bool operator == (const char* lhs, const Sha256 &rhs);
	bool operator != (const Sha256 &rhs);
	bool operator != (const std::string &rhs);
	bool operator != (const char* rhs);
	friend bool operator != (const std::string &lhs, const Sha256 &rhs);
	friend bool operator != (const char* lhs, const Sha256 &rhs);
};

#endif
