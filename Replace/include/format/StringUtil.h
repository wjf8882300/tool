/***************************************************************
 * StringUtil.h
 * 系统名  : 常用工具管理
 * 子系统名: 常用工具类
 * 更改日期      姓名           摘要
 * ----------    -----------    --------------------------------
 * 2015/07/23    王井锋			初次开发
 ***************************************************************
 */
#ifndef STRINGUTIL_H_
#define STRINGUTIL_H_
#include "util/util.h"
using std::vector;

/**
 * 工具类
 * @author  王井锋
 * @version V2.0.00
 * @since V2.0.00
 */
class StringUtil {

public:
	
	/**
	 * unicode码转为中文
	 * 如\u52B3\u788C 转为 劳碌 
	 * @author  王井锋
	 * @version V2.0.00
	 * @since V2.0.00
	 */
	static void stringToUnicode(const std::string& characterStr, std::string& unicodeStr);

};

#endif /*STRINGUTIL_H_*/