/***************************************************************
 * AesUtil.h
 * ϵͳ��  : ���ù��߹���
 * ��ϵͳ��: ���ù�����
 * ��������      ����           ժҪ
 * ----------    -----------    --------------------------------
 * 2011/04/01    ������			���ο���
 ***************************************************************
 */

#ifndef AesUtil_H_
#define AesUtil_H_
#include "util/Util.h"
#include "XEncrypt.h"
using namespace std;

/**
 * ������
 * @author  ������
 * @version V2.0.00
 * @since V2.0.00
 */
class AesUtil {
public:	
	AesUtil(std::vector<CString>& sr,std::vector<CString>& dt);
	/*
	 * AES����
	 * @input	�������
	 * @key		��Կ 
	 * @output	�������
	 */
	void Encrypt(CString input,CString key,CString& output);
	/*
	 * AES����
	 * @input	�������
	 * @key		��Կ 
	 * @output	�������
	 */
	void Decrypt(CString input,CString key,CString& output);
private:
	
private:
	std::vector<CString> &sources;
	std::vector<CString> &dests;
};


#endif /*AesUtil_H_*/