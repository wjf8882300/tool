/***************************************************************
 * MacUtil.h
 * ϵͳ��  : ���ù��߹���
 * ��ϵͳ��: ���ù�����
 * ��������      ����           ժҪ
 * ----------    -----------    --------------------------------
 * 2011/04/01    ������			���ο���
 ***************************************************************
 */
#ifndef MacUtil_H_
#define MacUtil_H_
#include "util/util.h"
#include "StdMac.h"
/**
 * ������
 * @author  ������
 * @version V2.0.00
 * @since V2.0.00
 */
class MacUtil {
public:	
	MacUtil(std::vector<CString>& sr,std::vector<CString>& dt);
	/*
	 * MACУ��
	 * @input	�������
	 * @key		��Կ 
	 * @output	�������
	 */
	void MacValidate(CString& coutput,bool isPadding,bool isAscii,bool isVerse,bool isSub,CProgressCtrl &cpc);
	/*
	 * MAC����
	 * @input	�������
	 * @key		��Կ 
	 * @output	�������
	 */
	void MacCalculate(const CString& cinput,CString& output,bool isPadding,bool isAscii,bool isVerse,bool isSub);
private:
	
private:
	std::vector<CString> sources;
	std::vector<CString> dests;
};


#endif /*MacUtil_H_*/