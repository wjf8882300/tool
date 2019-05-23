/************************************************************************/
/* MAC.cpp                                                              */
/* 系统名   :   Mac算法	                                                */
/* 更改日期     姓名           摘要                                     */
/* ----------   -----------    --------------------------------         */
/* 2010/10/01   王井锋			初次开发                                */
/* 2010/11/17   王井锋			增加Mac补齐方式选择                     */
/************************************************************************/
#ifndef _MAC_H
#define _MAC_H
#include "3des.h"
#include <memory>
#include <string>
#include <algorithm>
#pragma warning(disable: 4996)
class Mac
{
public:
	Mac();
	/*
	 * 3DES加密得到mac码(处理密钥取前16字节)
	 * @keys		密钥（密钥只取16字节）
	 * @inputs		明文
	 * @output		密文
	 * @length		Mac码长度
	 * @isASCII		是否将原串先转成ASCII码字符串
	 */
	void getMac(const std::string& key,const std::string& input,  std::string& output, const int length = 8, bool isPadding = false,bool isASCII = true);
	/*
	 * 3DES加密得到mac码(未处理密钥)
	 * @keys		密钥（密钥只取16字节）
	 * @inputs		明文
	 * @output		密文
	 * @isPadding	8整数倍是否补位
	 * @isASCII		是否将原串先转成ASCII码字符串
	 */
	void getMac2(const std::string& key,const std::string& input,  std::string& output, const int length = 8, bool isPadding = false,bool isASCII = true);
	/*
	 * 3DES加密得到mac码
	 * @keys		密钥（密钥只取16字节）
	 * @inputs		明文
	 * @output		密文
	 * @isPadding	8整数倍是否补位
	 */
	void  encrypt(const char* keys, const char* inputs, uint8* output,bool isPadding = false);

	/*
	 *将16进制数字转为字符串
	 *@dest	转换后的字符串
	 *@hex 16进制
	 *@len 16进制长度
	 */
	void  convertHexToString(std::string &dest,uint8 hex[],int len);

	/*
	 *把字符串转化成十六进制
	 *@Key	十六进制数组
	 *@keys 字符串
	 */
	void covertStringToHex(uint8 dest[], int len, const char* source);

	/*
	*把字符串转化成十六进制ASCII字符串
	*@source	原串
	*@dest		目标串
	*/
	void ConvertToASCII(const char* source,char* dest);
private:
	/*
	 *返回与字符串数字字符对应的整形数字，如"1234"，返回1，2，3，4
	 *@Key	十六进制数组
	 *@keys 字符串
	 */
	uint8 getHexChar(uint8 ch);
	/*
	 *返回与16进制对应的字符串
	 *@Key	十六进制数组
	 *@keys 字符串
	 */
	std::string  convertString(uint8 Key[],int len);
	/*
	 * 3DES加密得到mac码(8整数倍无补位)
	 * @keys		密钥（密钥只取16字节）
	 * @inputs		明文
	 * @output		密文
	 */
	void encrypt1(const char* keys, const char* inputs, uint8* output);
	/*
	 * 3DES加密得到mac码(8整数倍有补位)
	 * @keys		密钥（密钥只取16字节）
	 * @inputs		明文
	 * @output		密文
	 */
	void encrypt2(const char* keys, const char* inputs, uint8* output);
	/*
	 *初始量IV
	 */
	uint8 init[8];
};
#endif