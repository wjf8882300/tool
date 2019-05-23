/***************************************************************
 * Util.h
 * 系统名  : 常用工具管理
 * 子系统名: 常用工具类
 * 更改日期      姓名           摘要
 * ----------    -----------    --------------------------------
 * 2011/04/01    王井锋			初次开发
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
 * 工具类
 * @author  王井锋
 * @version V2.0.00
 * @since V2.0.00
 */
class Util {
public:	
	Util();
	/**
	 * 将列表中各元素转换成字符串，并拼装在一起，使用逗号分隔
	 * @param result		输出参数，拼装结果
	 * @param toBeFormat	待拼装列表
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
	 * 宽字符转换为普通字符
	 * @param cinput		源字符
	 * @param input			目标字符
	 */
	static string Wcstombs(const CString& cinput);

	/**
	 * 普通字符转换为宽字符
	 * @param input         源字符
	 * @return				目标字符
	 */
	static CString Mbstowcs(const std::string& input);

	static int HexToBcd(char ch);

	/**
	 * UTF8码转GBK码
	 * @param	UTF8码
	 * @return  GBK码
	 */
	static string Utf8ToGBK(const string& strUtf8);
	/**
	 * GBK码转UTF8码
	 * @param	GBK码
	 * @return  UTF8码
	 */
	static string Util::GBKToUtf8(const string& strGBK);

	/**
	 * 获取数据库连接
	 */
	static DBConnectInfo& getDBConnectInfo() { return _dbConnectInfo;}

	/**
	 * 忽略大小写比较
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