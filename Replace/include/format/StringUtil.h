/***************************************************************
 * StringUtil.h
 * ϵͳ��  : ���ù��߹���
 * ��ϵͳ��: ���ù�����
 * ��������      ����           ժҪ
 * ----------    -----------    --------------------------------
 * 2015/07/23    ������			���ο���
 ***************************************************************
 */
#ifndef STRINGUTIL_H_
#define STRINGUTIL_H_
#include "util/util.h"
using std::vector;

/**
 * ������
 * @author  ������
 * @version V2.0.00
 * @since V2.0.00
 */
class StringUtil {

public:
	
	/**
	 * unicode��תΪ����
	 * ��\u52B3\u788C תΪ ��µ 
	 * @author  ������
	 * @version V2.0.00
	 * @since V2.0.00
	 */
	static void stringToUnicode(const std::string& characterStr, std::string& unicodeStr);

};

#endif /*STRINGUTIL_H_*/