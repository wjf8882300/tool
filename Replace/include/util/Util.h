/***************************************************************
 * Util.h
 * ϵͳ��  : ���ù��߹���
 * ��ϵͳ��: ���ù�����
 * ��������      ����           ժҪ
 * ----------    -----------    --------------------------------
 * 2011/04/01    ������			���ο���
 ***************************************************************
 */
#ifndef UTIL_H_
#define UTIL_H_
#include "util/stdafx.h"
//#include <afx.h>
#include <vector>
#include <algorithm>
#include "deelx.h"
#include <stack>
#include <fstream>
#include <sstream>
#include <string>
#include "dbi/DBConnectInfo.h"
using namespace std;

/**
 * ������
 * @author  ������
 * @version V2.0.00
 * @since V2.0.00
 */
class Util {
public:	
	Util();
	/**
	 * ���б��и�Ԫ��ת�����ַ�������ƴװ��һ��ʹ�ö��ŷָ�
	 * @param result		���������ƴװ���
	 * @param toBeFormat	��ƴװ�б�
	 */
	static void formatList( CString& result, const vector<CString>& toBeFormat, const CString& splitStr = _T(",") );

	static std::string formatList(const vector<std::string>& toBeFormat);

	static void parseList( vector<string>& result, const string& toBeParse,const string& toSplit );

	static void parseListIgnoreEmpty( vector<string>& result, const string& toBeParse,const string& toSplit );

	static void parseList( vector<CString>& result, const CString& toBeParse,const CString& toSplit );
	
	static bool findStringFromVector(const CString& str,const vector<CString>& toBeFind);

	static void SplitString(const CString& str, const CString& seq, CStringArray& toBeFormat);

	static void SplitStringWithEmpty(const CString& str, const CString& seq, CStringArray& toBeFormat);

	static CString formatList(const CStringArray& toBeFormat);

	/**
	 * ���ַ�ת��Ϊ��ͨ�ַ�
	 * @param cinput		Դ�ַ�
	 * @param input			Ŀ���ַ�
	 */
	static string Wcstombs(const CString& cinput);

	/**
	 * ��ͨ�ַ�ת��Ϊ���ַ�
	 * @param input         Դ�ַ�
	 * @return				Ŀ���ַ�
	 */
	static CString Mbstowcs(const std::string& input);

	static int HexToBcd(char ch);

	/**
	 * UTF8��תGBK��
	 * @param	UTF8��
	 * @return  GBK��
	 */
	static string Utf8ToGBK(const string& strUtf8);
	/**
	 * GBK��תUTF8��
	 * @param	GBK��
	 * @return  UTF8��
	 */
	static string Util::GBKToUtf8(const string& strGBK);

	/**
	 * ��ȡ���ݿ�����
	 */
	static DBConnectInfo& getDBConnectInfo() { return _dbConnectInfo;}

	/**
	 * ���Դ�Сд�Ƚ�
	 */
	static bool compareIgnorCase(const CString& a, const CString& b);
private:
	static DBConnectInfo _dbConnectInfo;
};

template<class ElementType>
class ElementFinder
{
public:
	ElementFinder(const ElementType &elem):elementType(elem){}
	bool operator()(const ElementType &elem)
	{
		return elementType == elem;
	}
private:
	const ElementType &elementType;
};

namespace Poco
{

	int toUpper(int ch);
	string toUpper(const string& str);
}

#endif /*UTIL_H_*/