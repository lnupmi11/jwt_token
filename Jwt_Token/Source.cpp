#include "Include/Base64Encode.h"
#include "Include/Json.h"

using namespace std;

int main()
{
	string url = Base64Encode::DecodeUrl(Base64Encode::EncodeUrl("Bad code"));
	string raw_data = "{\n \"url\": \"" + url + "\"\n}";
	Json data(raw_data);
	cout << raw_data << endl;
//	cout << data << endl;
	return 0;
}
