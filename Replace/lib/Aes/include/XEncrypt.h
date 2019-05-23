#pragma once
#include <string>
#include "WTypes.h"

using namespace std;
#define XENCRYPT_DLL

#ifdef XENCRYPT_DLL
	#define XENCRYPT_DLLEXPORT extern "C" __declspec(dllexport)
#else
	#define XENCRYPT_DLLEXPORT extern "C" __declspec(dllimport)
#endif

namespace XEncrypt
{
	const unsigned int KEY_SIZE= 32;

	//! 生成一个Key.
	//! \param szKey 保存生成的Key，缓冲区长度应不小于XEncrypt::KEY_SIZE + 1.
	XENCRYPT_DLLEXPORT void __stdcall GenerateKey(char *szKey);

	//! 加密一段文本.
	//! \param szInput 需要加密的文本
	//! \param nInSize 需要加密的文本长度（如果文本以\0结尾，可以指定-1）
	//! \param szOutput 加密后的文本
	//! \param nOutSize 保存加密后文本的缓冲区长度（通过GetEncryptSize获取所需的缓冲区长度）
	//! \param szKey 密钥，长度为XEncrypt::KEY_SIZE。此参数为空将使用默认密钥。
	XENCRYPT_DLLEXPORT bool __stdcall Encrypt(
		const char *szInput, 
		int nInSize,
		char *szOutput, 
		int nOutSize,
		const char *szKey = NULL);

	//! 计算加密后的文本长度.
	//! \param nInSize 加密前的文本长度.
	XENCRYPT_DLLEXPORT int __stdcall GetEncryptSize(int nInSize);

	//! 解密一段文本.
	//! \param szInput 需要解密的文本
	//! \param nInSize 需要解密的文本长度（如果文本以\0结尾，可以指定-1）
	//! \param szOutput 解密后的文本
	//! \param nOutSize 保存解密后文本的缓冲区长度（最长可能需要nInSize/2）
	//! \param szKey 密钥，长度为XEncrypt::KEY_SIZE。此参数为空将使用默认密钥。
	XENCRYPT_DLLEXPORT bool __stdcall Decrypt(
		const char *szInput, 
		int nInSize,
		char *szOutput, 
		int nOutSize,
		const char *szKey = NULL);

	/*
	 * AES加解密
	 * @type		AES类型 0为加密 1为解密
	 * @szInput		输入参数
	 * @key			密钥
	 * @szOutput	输出参数
	 */
	XENCRYPT_DLLEXPORT bool __stdcall AES(int type, const char* szInput,const char* key, char* szOutput);
}
