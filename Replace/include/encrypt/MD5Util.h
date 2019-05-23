/***************************************************************
 * AesUtil.h
 * 系统名  : 常用工具管理
 * 子系统名: 常用工具类
 * 更改日期      姓名           摘要
 * ----------    -----------    --------------------------------
 * 2011/04/01    王井锋			初次开发
 ***************************************************************
 */

#ifndef MD5Util_H_
#define MD5Util_H_
#include "util/Util.h"

/**
 * 工具类
 * @author  王井锋
 * @version V2.0.00
 * @since V2.0.00
 */
class MD5Util {
public:	
	MD5Util();
	/*
	 * MD5加密
	 * @input	输入参数
	 * @output	输出参数
	 */
	void Encrypt(const string& input,string& output);
};


#endif /*MD5Util_H_*/