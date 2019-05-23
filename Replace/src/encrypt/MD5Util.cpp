/***************************************************************
* AesUtil.h
* 系统名  : 常用工具管理
* 子系统名: 常用工具类
* 更改日期      姓名           摘要
* ----------    -----------    --------------------------------
* 2011/04/01    王井锋			初次开发
***************************************************************
*/
#include "encrypt/MD5Util.h"
#include "encrypt/MD5Engine.h"
MD5Util::MD5Util()
{

}

void MD5Util::Encrypt(const string& input,string& output)
{
	MD5Engine md5;
	md5.update(input.c_str(), (unsigned int)input.length());
	output = Poco::toUpper(DigestEngine::digestToHex(md5.digest()));	
}

