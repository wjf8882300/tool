#ifndef _MAC_H
#define _MAC_H
#include "3des.h"
#include <memory>
#include <string>
#include <algorithm>

class Mac
{
public:
	Mac();
	static void getMac(const std::string& key,const std::string& input,  std::string& output, const int length = 8);
	/*
	 *3DES加密得到mac码
	 *@keys	密钥（密钥只取16字节）
	 *@inputs 明文
	 *@output 密文
	 */
	void  encrypt(const char* keys, const char* inputs, uint8* output);

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
	 *初始量IV
	 */
	uint8 init[8];
};
#endif