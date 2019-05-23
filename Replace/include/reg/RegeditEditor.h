/***************************************************************
 * RegeditEditor.h
 * ϵͳ��  : ���ù��߹���
 * ��ϵͳ��: ���ù�����
 * ��������      ����           ժҪ
 * ----------    -----------    --------------------------------
 * 2011/04/01    ������			���ο���
 ***************************************************************
 */
#ifndef RegeditEditor_H_
#define RegeditEditor_H_
#include "util/util.h"
using namespace std;
/**
 * ������
 * @author  ������
 * @version V2.0.00
 * @since V2.0.00
 */
class RegeditEditor {
public:	
	RegeditEditor();
	bool UpdateRegedit(const CString& subKey,const CString& valueName,const CString& value);
	bool UpdateRegedit(const CString& subKey,const CString& valueName,const int& value);
private:
	LPBYTE CStringToLPBYTE(const CString& str);
	LPBYTE CStringToLPBYTE(const int& str);
	void FormatError();
private:
	std::vector<CString> sources;
	std::vector<CString> dests;
};


#endif /*RegeditEditor_H_*/