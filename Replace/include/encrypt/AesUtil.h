/***************************************************************
 * AesUtil.h
 * 系统名  : 常用工具管理
 * 子系统名: 常用工具类
 * 更改日期      姓名           摘要
 * ----------    -----------    --------------------------------
 * 2011/04/01    王井锋			初次开发
 ***************************************************************
 */

#ifndef AesUtil_H_
#define AesUtil_H_
#include "util/Util.h"
#include "XEncrypt.h"
using namespace std;

/**
 * 工具类
 * @author  王井锋
 * @version V2.0.00
 * @since V2.0.00
 */
class AesUtil {
public:	
	AesUtil(std::vector<CString>& sr,std::vector<CString>& dt);
	/*
	 * AES加密
	 * @input	输入参数
	 * @key		密钥 
	 * @output	输出参数
	 */
	void Encrypt(CString input,CString key,CString& output);
	/*
	 * AES解密
	 * @input	输入参数
	 * @key		密钥 
	 * @output	输出参数
	 */
	void Decrypt(CString input,CString key,CString& output);
private:
	
private:
	std::vector<CString> &sources;
	std::vector<CString> &dests;
};


#endif /*AesUtil_H_*/