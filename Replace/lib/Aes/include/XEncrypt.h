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

	//! ����һ��Key.
	//! \param szKey �������ɵ�Key������������Ӧ��С��XEncrypt::KEY_SIZE + 1.
	XENCRYPT_DLLEXPORT void __stdcall GenerateKey(char *szKey);

	//! ����һ���ı�.
	//! \param szInput ��Ҫ���ܵ��ı�
	//! \param nInSize ��Ҫ���ܵ��ı����ȣ�����ı���\0��β������ָ��-1��
	//! \param szOutput ���ܺ���ı�
	//! \param nOutSize ������ܺ��ı��Ļ��������ȣ�ͨ��GetEncryptSize��ȡ����Ļ��������ȣ�
	//! \param szKey ��Կ������ΪXEncrypt::KEY_SIZE���˲���Ϊ�ս�ʹ��Ĭ����Կ��
	XENCRYPT_DLLEXPORT bool __stdcall Encrypt(
		const char *szInput, 
		int nInSize,
		char *szOutput, 
		int nOutSize,
		const char *szKey = NULL);

	//! ������ܺ���ı�����.
	//! \param nInSize ����ǰ���ı�����.
	XENCRYPT_DLLEXPORT int __stdcall GetEncryptSize(int nInSize);

	//! ����һ���ı�.
	//! \param szInput ��Ҫ���ܵ��ı�
	//! \param nInSize ��Ҫ���ܵ��ı����ȣ�����ı���\0��β������ָ��-1��
	//! \param szOutput ���ܺ���ı�
	//! \param nOutSize ������ܺ��ı��Ļ��������ȣ��������ҪnInSize/2��
	//! \param szKey ��Կ������ΪXEncrypt::KEY_SIZE���˲���Ϊ�ս�ʹ��Ĭ����Կ��
	XENCRYPT_DLLEXPORT bool __stdcall Decrypt(
		const char *szInput, 
		int nInSize,
		char *szOutput, 
		int nOutSize,
		const char *szKey = NULL);

	/*
	 * AES�ӽ���
	 * @type		AES���� 0Ϊ���� 1Ϊ����
	 * @szInput		�������
	 * @key			��Կ
	 * @szOutput	�������
	 */
	XENCRYPT_DLLEXPORT bool __stdcall AES(int type, const char* szInput,const char* key, char* szOutput);
}
