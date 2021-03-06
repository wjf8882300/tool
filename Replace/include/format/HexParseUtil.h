/***************************************************************
 * HexParseUtil.h
 * 系统名  : 常用工具管理
 * 子系统名: 常用工具类
 * 更改日期      姓名           摘要
 * ----------    -----------    --------------------------------
 * 2011/04/01    王井锋			初次开发
 ***************************************************************
 */
#ifndef HEXPARSEUTIL_H_
#define HEXPARSEUTIL_H_
#include "util/util.h"
using namespace std;
/**
 * 工具类
 * @author  王井锋
 * @version V2.0.00
 * @since V2.0.00
 */
class HexParseUtil {
public:	
	HexParseUtil();
	void formatHexToBcd(const CString& strInput,CString& strOutput,bool isHighToLower);
	void formatBcdToHex(const CString& strInput,CString& strOutput,bool isHighToLower);
private:
	
private:
	std::vector<CString> sources;
	std::vector<CString> dests;
};


#endif /*HexParseUtil_H_*/