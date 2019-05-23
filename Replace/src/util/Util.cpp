/***************************************************************
* Util.cpp
* 系统名  : 常用工具管理
* 子系统名: 常用工具类
* 更改日期      姓名           摘要
* ----------    -----------    --------------------------------
* 2011/04/01    王井锋			初次开发
***************************************************************
*/
#include "util/Util.h"
#include "Poco/StringTokenizer.h"

DBConnectInfo Util::_dbConnectInfo;

void Util::formatList( CString& result, const vector<CString>& toBeFormat, const CString& splitStr)
{
	int size = (int)toBeFormat.size();
	for( int i = 0; i < size; i++ ) {
		result.Append(toBeFormat[i]);
		if( i < size - 1 )
			result.Append(splitStr);
	}
}

std::string Util::formatList(const vector<std::string>& toBeFormat)
{
	std::string result;
	int size = (int)toBeFormat.size();
	for( int i = 0; i < size; i++ ) {
		result.append(toBeFormat[i]);
		if( i < size - 1 )
			result.append(",");
	}

	return result;
}

void Util::parseList( vector<string>& result, const string& toBeParse,const string& toSplit ) {
	Poco::StringTokenizer tokenizer(toBeParse, toSplit, Poco::StringTokenizer::TOK_TRIM);
	for (Poco::StringTokenizer::Iterator it = tokenizer.begin(); it != tokenizer.end(); ++it) {
		result.push_back(*it);
	}
}

void Util::parseListIgnoreEmpty( vector<string>& result, const string& toBeParse,const string& toSplit ) {
	Poco::StringTokenizer tokenizer(toBeParse, toSplit, Poco::StringTokenizer::TOK_IGNORE_EMPTY);
	for (Poco::StringTokenizer::Iterator it = tokenizer.begin(); it != tokenizer.end(); ++it) {
		result.push_back(*it);
	}
}

void Util::parseList( vector<CString>& result, const CString& toBeParse,const CString& toSplit )
{
	CString resToken;  
	int curPos = 0;   

	resToken = toBeParse.Tokenize(toSplit ,curPos); 
	try
	{
		while(resToken != "" && curPos>=0 )   
		{   
			result.push_back(resToken);
			resToken = toBeParse.Tokenize(toSplit, curPos);   
		}
	}catch(CException& ex)
	{
		TCHAR szCause[255];
		ex.GetErrorMessage(szCause,255);
		CString err = szCause;
	}
}

bool Util::findStringFromVector(const CString& str,const vector<CString>& toBeFind)
{
	if(std::find_if(toBeFind.begin(),toBeFind.end(),ElementFinder<CString>(str))!=toBeFind.end())
	{
		return true;
	}
	return false;
}

void Util::SplitString(const CString& str, const CString& seq, CStringArray& arr)
{
	CString resToken;  
	int curPos = 0;   

	resToken = str.Tokenize(seq ,curPos); 
	try
	{
		while(resToken != "" && curPos>=0 )   
		{   
			arr.Add(resToken);
			resToken = str.Tokenize(seq,curPos);   
		}
	}catch(CException& ex)
	{
		TCHAR szCause[255];
		ex.GetErrorMessage(szCause,255);
		CString err = szCause;
	}
}

void Util::SplitStringWithEmpty(const CString& str, const CString& Delimiters, CStringArray& OutputArray)
{
	int     Hit;
	int     EarliestHit;
	int     DelimiterIndex;
	CString sExtract;
	BOOL    MoreTokens = TRUE;
	BOOL    GotToken = FALSE;
	TCHAR   CurrDelimiter;

	CString s = str;

	while (MoreTokens)
	{
		GotToken = FALSE;
		EarliestHit = s.GetLength();

		// Trawl the string looking for the leftmost (earliest) hit in
		// the list of valid separators.
		for (DelimiterIndex = 0; 
			DelimiterIndex < Delimiters.GetLength();
			DelimiterIndex++)
		{
			CurrDelimiter = Delimiters [DelimiterIndex];
			Hit = s.Find (CurrDelimiter);
			if (Hit != -1)
			{
				if (Hit < EarliestHit)
				{
					EarliestHit = Hit;
				}
				GotToken = TRUE;
			}
		}

		if (GotToken)
		{
			sExtract = s.Left(EarliestHit);
			s = s.Right(s.GetLength() - EarliestHit - 1);
			OutputArray.Add(sExtract);
		}
		else
		{
			// Trailing token.
			MoreTokens = FALSE;
			OutputArray.Add(s);
		}
	}
}

CString Util::formatList(const CStringArray& toBeFormat)
{
	CString str;
	for(int i = 0; i < toBeFormat.GetSize(); i ++)
	{
		str.Append(toBeFormat[i]);
		if(i != toBeFormat.GetSize() - 1)
			str.Append(_T(","));
	}
	return str;
}

std::string Util::Wcstombs(const CString& cinput)
{
	string input;
#ifdef UNICODE
	int sizeA = WideCharToMultiByte(CP_ACP, 0, cinput.GetString(), -1, NULL, 0, NULL, NULL);
	char* inputs = new char[sizeA+1];
	ZeroMemory(inputs,sizeof(char)*(sizeA+1));
	WideCharToMultiByte(CP_ACP,0,cinput.GetString(),-1,inputs,sizeA +1 ,NULL,NULL);
	input = std::string(inputs);
	delete[] inputs;
#else
	input = std::string(cinput.GetString());
#endif
	return input;
}

CString Util::Mbstowcs(const std::string& input)
{
	CString cinput;
#ifdef UNICODE
	int sizeW = MultiByteToWideChar(CP_ACP, 0, input.c_str(), -1, NULL, 0);
	wchar_t* inputs = new wchar_t[sizeW+1];
	ZeroMemory(inputs,sizeof(char)*(sizeW+1));
	MultiByteToWideChar(CP_ACP,0,input.c_str(),-1,inputs,sizeW +1 );
	cinput = CString(inputs);
	delete inputs;
#else
	input = CString(input.c_str());
#endif
	return cinput;
}

int Util::HexToBcd(char ch)
{
	if((ch >='0')&&(ch<='9'))
		return ch - 0x30;
	else if((ch>='A')&&(ch<='F'))
		return ch-'A'+10;
	else if((ch>='a')&&(ch<='f'))
		return ch-'a'+10;        
	else
		return -1;
}

string Util::Utf8ToGBK(const string& strUtf8)
{
	const char* s = strUtf8.c_str();
	int len=MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)s, -1, NULL,0); 
	unsigned short * wszGBK = new unsigned short[len+1];       
	memset(wszGBK, 0, len * 2 + 2); 
	MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)s, -1, (LPWSTR)wszGBK, len);
	len = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wszGBK, -1, NULL, 0, NULL, NULL);
	char *szGBK = new char[len + 1]; 
	memset(szGBK, 0, len + 1); 
	WideCharToMultiByte (CP_ACP, 0, (LPCWSTR)wszGBK, -1, (LPSTR)szGBK, len, NULL,NULL);
	string output(szGBK);
	delete[] wszGBK;
	delete[] szGBK;
	return output;
}

string Util::GBKToUtf8(const string& strGBK)
{ 
	const char* s = strGBK.c_str();
	int len=MultiByteToWideChar(CP_ACP, 0, (LPCSTR)s, -1, NULL,0); 
	unsigned short * wszUtf8 = new unsigned short[len+1]; 
	memset(wszUtf8, 0, len * 2 + 2); 
	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)s, -1, (LPWSTR)wszUtf8, len);
	len = WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)wszUtf8, -1, NULL, 0, NULL, NULL);
	char *szUtf8 = new char[len + 1]; 
	memset(szUtf8, 0, len + 1); 
	WideCharToMultiByte (CP_UTF8, 0, (LPCWSTR)wszUtf8, -1, (LPSTR)szUtf8, len, NULL,NULL);
	string output(szUtf8);
	delete[] wszUtf8;
	delete[] szUtf8;
	return output; 
}

bool Util::compareIgnorCase(const CString& a, const CString& b)
{
	return CString(a).MakeUpper() == CString(b).MakeUpper();
}

namespace Poco
{

	int toUpper(int ch)
	{
		if (islower(ch))
			return ch - 32;
		else
			return ch;
	}

	string toUpper(const string& str)
		/// Returns a copy of str containing all upper-case characters.
	{
		string::const_iterator it  = str.begin();
		string::const_iterator end = str.end();

		string result;
		result.reserve(str.size());
		while (it != end) result += toUpper(*it++);
		return result;
	}

}