#pragma once
#ifndef JWT_Error_H
#define JWT_Error_H

#include <exception>
#include <stdexcept>
#include <string>

using namespace std;

/*
Designate that we failed to validate and parse the token.
*/
class ErrorOfInvalidToken : public runtime_error
{
public:
	explicit ErrorOfInvalidToken(string message) : runtime_error(message) {}
};

/*
Designate that the token is not properly encoded. The token cannot be parsed.
*/
class ErrorOfTokenFormat : public ErrorOfInvalidToken
{
public:
	explicit ErrorOfTokenFormat(string message) : ErrorOfInvalidToken(message) {}
};

/*
Designate that the token is not properly signed.
*/
class ErrorOfInvalidSignature : public ErrorOfInvalidToken
{
public:
	explicit ErrorOfInvalidSignature(string message) : ErrorOfInvalidToken(message) {}
};

/*
Designate that we are unable to construct the validator.
*/
class ErrorOfInvalidValidator : public logic_error
{
public:
	explicit ErrorOfInvalidValidator(string message) : logic_error(message) {}
};

#endif // JWT_Error_H
