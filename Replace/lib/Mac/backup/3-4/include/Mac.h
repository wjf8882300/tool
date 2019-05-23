/************************************************************************/
/* MAC.cpp                                                              */
/* ϵͳ��   :   Mac�㷨	                                                */
/* ��������     ����           ժҪ                                     */
/* ----------   -----------    --------------------------------         */
/* 2010/10/01   ������			���ο���                                */
/* 2010/11/17   ������			����Mac���뷽ʽѡ��                     */
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
	 * 3DES���ܵõ�mac��(������Կȡǰ16�ֽ�)
	 * @keys		��Կ����Կֻȡ16�ֽڣ�
	 * @inputs		����
	 * @output		����
	 * @length		Mac�볤��
	 * @isASCII		�Ƿ�ԭ����ת��ASCII���ַ���
	 */
	void getMac(const std::string& key,const std::string& input,  std::string& output, const int length = 8, bool isPadding = false,bool isASCII = true);
	/*
	 * 3DES���ܵõ�mac��(δ������Կ)
	 * @keys		��Կ����Կֻȡ16�ֽڣ�
	 * @inputs		����
	 * @output		����
	 * @isPadding	8�������Ƿ�λ
	 * @isASCII		�Ƿ�ԭ����ת��ASCII���ַ���
	 */
	void getMac2(const std::string& key,const std::string& input,  std::string& output, const int length = 8, bool isPadding = false,bool isASCII = true);
	/*
	 * 3DES���ܵõ�mac��
	 * @keys		��Կ����Կֻȡ16�ֽڣ�
	 * @inputs		����
	 * @output		����
	 * @isPadding	8�������Ƿ�λ
	 */
	void  encrypt(const char* keys, const char* inputs, uint8* output,bool isPadding = false);

	/*
	 *��16��������תΪ�ַ���
	 *@dest	ת������ַ���
	 *@hex 16����
	 *@len 16���Ƴ���
	 */
	void  convertHexToString(std::string &dest,uint8 hex[],int len);

	/*
	 *���ַ���ת����ʮ������
	 *@Key	ʮ����������
	 *@keys �ַ���
	 */
	void covertStringToHex(uint8 dest[], int len, const char* source);

	/*
	*���ַ���ת����ʮ������ASCII�ַ���
	*@source	ԭ��
	*@dest		Ŀ�괮
	*/
	void ConvertToASCII(const char* source,char* dest);
private:
	/*
	 *�������ַ��������ַ���Ӧ���������֣���"1234"������1��2��3��4
	 *@Key	ʮ����������
	 *@keys �ַ���
	 */
	uint8 getHexChar(uint8 ch);
	/*
	 *������16���ƶ�Ӧ���ַ���
	 *@Key	ʮ����������
	 *@keys �ַ���
	 */
	std::string  convertString(uint8 Key[],int len);
	/*
	 * 3DES���ܵõ�mac��(8�������޲�λ)
	 * @keys		��Կ����Կֻȡ16�ֽڣ�
	 * @inputs		����
	 * @output		����
	 */
	void encrypt1(const char* keys, const char* inputs, uint8* output);
	/*
	 * 3DES���ܵõ�mac��(8�������в�λ)
	 * @keys		��Կ����Կֻȡ16�ֽڣ�
	 * @inputs		����
	 * @output		����
	 */
	void encrypt2(const char* keys, const char* inputs, uint8* output);
	/*
	 *��ʼ��IV
	 */
	uint8 init[8];
};
#endif