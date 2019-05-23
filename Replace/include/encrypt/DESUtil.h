/***************************************************************
 * DESUtil.h
 * 系统名  : 常用工具管理
 * 子系统名: 常用工具类
 * 更改日期      姓名           摘要
 * ----------    -----------    --------------------------------
 * 2011/04/01    王井锋			初次开发
 ***************************************************************
 */
#ifndef __DESUtil_H 
#define __DESUtil_H  
#include "util/Util.h"

//#include "des.h"
//#include "hex.h"
//#include "files.h"
//using namespace CryptoPP;
//typedef auto_ptr<BlockTransformation> apbt;


class DESUtil { 
public: 
		DESUtil(std::vector<CString>& sr,std::vector<CString>& dt);

		/*
		 * DES加密
		 * @input	输入参数
		 * @key		密钥 
		 * @output	输出参数
		 */
		string Encrypt(const string& input,const string& key);
		/*
		 * DES解密
		 * @input	输入参数
		 * @key		密钥 
		 * @output	输出参数
		 */
		string Decrypt(const string& input,const string& key);
        // Encrypt/decrypt the data in "data", according to the "key". 
        // Caller is responsible for confirming the buffer size of "data" 
        // points to is 8*"blocks" bytes. 
        // The data encrypted/decrypted is stored in data. 
        // The return code is 1:success, other:failed. 
//protected:
//		
//		SecByteBlock HexDecodeString(const char *hex);
private:
	std::vector<CString> sources;
	std::vector<CString> dests;
}; 
#endif 