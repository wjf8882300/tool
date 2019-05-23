/***************************************************************
 * HexParseUtil.h
 * ϵͳ��  : ���ù��߹���
 * ��ϵͳ��: ���ù�����
 * ��������      ����           ժҪ
 * ----------    -----------    --------------------------------
 * 2011/04/01    ������			���ο���
 ***************************************************************
 */
#ifndef HEXPARSEUTIL_H_
#define HEXPARSEUTIL_H_
#include "util/util.h"
using namespace std;
/**
 * ������
 * @author  ������
 * @version V2.0.00
 * @since V2.0.00
 */
class HexParseUtil {
public:	
	HexParseUtil();
	void formatHexToBcd(const CString& strInput,CString& strOutput,bool isHighToLower);
	void formatBcdToHex(const CString& strInput,CString& strOutput,bool isHighToLower);
private:
	
private:
	std::vector<CString> sources;
	std::vector<CString> dests;
};


#endif /*HexParseUtil_H_*/