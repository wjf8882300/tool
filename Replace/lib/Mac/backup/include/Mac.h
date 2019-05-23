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
	 *3DES���ܵõ�mac��
	 *@keys	��Կ����Կֻȡ16�ֽڣ�
	 *@inputs ����
	 *@output ����
	 */
	void  encrypt(const char* keys, const char* inputs, uint8* output);

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
	 *��ʼ��IV
	 */
	uint8 init[8];
};
#endif