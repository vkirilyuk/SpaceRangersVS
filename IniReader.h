#ifndef INIREADER_H
#define INIREADER_H

#include <string>
using namespace std;

class CIniReader
{
public:
	CIniReader(const char* szFileName); 
	int ReadInteger(char* szSection, char* szKey, int iDefaultValue);
	float ReadFloat(char* szSection, char* szKey, float fltDefaultValue);
	bool ReadBoolean(char* szSection, char* szKey, bool bolDefaultValue);
	string ReadString(char* szSection, char* szKey, const string defaultValue);
private:
	char m_szFileName[255];
};
#endif//INIREADER_H