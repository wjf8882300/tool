/***************************************************************
 * AesUtil.h
 * ϵͳ��  : ���ù��߹���
 * ��ϵͳ��: ���ù�����
 * ��������      ����           ժҪ
 * ----------    -----------    --------------------------------
 * 2011/04/01    ������			���ο���
 ***************************************************************
 */

#ifndef MD5Util_H_
#define MD5Util_H_
#include "util/Util.h"

/**
 * ������
 * @author  ������
 * @version V2.0.00
 * @since V2.0.00
 */
class MD5Util {
public:	
	MD5Util();
	/*
	 * MD5����
	 * @input	�������
	 * @output	�������
	 */
	void Encrypt(const string& input,string& output);
};


#endif /*MD5Util_H_*/