/***************************************************************
 * SQLFormat.h
 * 系统名  : 常用工具管理
 * 子系统名: 常用工具类
 * 更改日期      姓名           摘要
 * ----------    -----------    --------------------------------
 * 2011/04/01    王井锋			初次开发
 ***************************************************************
 */
#ifndef SQLFORMAT_H_
#define SQLFORMAT_H_
#include "util/util.h"
using std::vector;

/**
 * 工具类
 * @author  王井锋
 * @version V2.0.00
 * @since V2.0.00
 */
class SQLFormat {
public:	
	/**
	 * 构造函数
	 * @param sql		待格式化SQL
	 * @param isQt		是否添加引号
	 */
	SQLFormat(CString &sql,bool isQt);
	/**
	 * 格式化SQL语句
	 * @strOut		格式字符串
	 */
	void parseSql(CString& strOut);
	/**
	 * 格式化SQL语句
	 * @strOut		格式字符串
	 */
	bool formatSql(CString& strOut);
private:
	/**
	 * 初始化关键字
	 */
	void Initlize();
	/**
	 * 格式化关键字
	 * @param vcstr		关键字集合
	 * @param isBefore	格式化位置是否在前插入换行符
	 */
	void formatKey(const vector<CString>& vcstr,bool isBefore);

	/**
	 * 格式化括号
	 */
	void judgeQuotes();

	/**
	 * 格式化长度
	 */
	void formatLength(CString& strOut);

	/**
	 * tab替换为空格键
	 */
	void formatTab();

	/**
	 * 替换逗号
	 */
	void formatComma();

	/**
	 * 缩进
	 */
	void formatIndentation();

	/**
	 * 格式化空格
	 */
	void formatSpace();

	/**
	 * 格式化长度
	 */
	bool formatColumn(CString& strOut);

	/**
	 * 获取列
	 */
	int findColumn(CString& strOut, const CString& strInput);

	bool judgeColumn(const CString& strInput, CString& strOutput);
private:
	//前置关键字
	vector<CString> vbefore;
	//后置关键字
	vector<CString> vbehind;
	//函数
	vector<CString> vfunction;

	//待格式化串
	CString strSql;
	//是否加双引号
	bool isQuotes;
	//换行符
	CString sepChangeLine;
	//空格符
	CString sepSpace	 ;
	//引号
	CString sepQuotes    ;
	//tab符
	CString sepTab		 ;
};


#endif /*SQLFormat_H_*/