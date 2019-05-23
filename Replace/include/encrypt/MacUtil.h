/***************************************************************
 * MacUtil.h
 * 系统名  : 常用工具管理
 * 子系统名: 常用工具类
 * 更改日期      姓名           摘要
 * ----------    -----------    --------------------------------
 * 2011/04/01    王井锋			初次开发
 ***************************************************************
 */
#ifndef MacUtil_H_
#define MacUtil_H_
#include "util/util.h"
#include "StdMac.h"
/**
 * 工具类
 * @author  王井锋
 * @version V2.0.00
 * @since V2.0.00
 */
class MacUtil {
public:	
	MacUtil(std::vector<CString>& sr,std::vector<CString>& dt);
	/*
	 * MAC校验
	 * @input	输入参数
	 * @key		密钥 
	 * @output	输出参数
	 */
	void MacValidate(CString& coutput,bool isPadding,bool isAscii,bool isVerse,bool isSub,CProgressCtrl &cpc);
	/*
	 * MAC计算
	 * @input	输入参数
	 * @key		密钥 
	 * @output	输出参数
	 */
	void MacCalculate(const CString& cinput,CString& output,bool isPadding,bool isAscii,bool isVerse,bool isSub);
private:
	
private:
	std::vector<CString> sources;
	std::vector<CString> dests;
};


#endif /*MacUtil_H_*/