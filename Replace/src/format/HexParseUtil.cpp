/***************************************************************
 * HexParseUtil.h
 * ϵͳ��  : ���ù��߹���
 * ��ϵͳ��: ���ù�����
 * ��������      ����           ժҪ
 * ----------    -----------    --------------------------------
 * 2011/04/01    ������			���ο���
 ***************************************************************
 */
#include "format/HexParseUtil.h"
HexParseUtil::HexParseUtil()
{
}

void HexParseUtil::formatHexToBcd(const CString& strInput,CString& strOutput,bool isHighToLower)
{
	CString tmp;
	//��ת��8λ
	tmp.Format(_T("%08s"),strInput);
	//��ߵ�λת��
	if(isHighToLower)
	{
		CString chead4  = tmp.Mid(0,4).Mid(2,2)+tmp.Mid(0,4).Mid(0,2);
		CString ctrail4 = tmp.Mid(4,4).Mid(2,2)+tmp.Mid(4,4).Mid(0,2);
		tmp  = ctrail4 + chead4;
	}
	TCHAR* stopStr = _T("\0");
	long lg = 0L;
	CString hexStr("");
#ifdef UNICODE
	lg = wcstol(tmp.GetString(),&stopStr,16);
	TCHAR hexSize[256];
	swprintf(hexSize,_T("%d"),lg);
	hexStr = CString(hexSize).MakeUpper();
#else
	lg = atol(tmp.GetString());
	ltoa(lg,stopStr,10);
	hexStr = CString(stopStr).MakeUpper();
#endif
	//long lg = wcstol(tmp.GetString(),&stopStr,16);
	/*TCHAR hexSize[256];
	swprintf(hexSize,_T("%d"),lg);
	CString hexStr = CString(hexSize).MakeUpper();*/
	strOutput.Format(_T("%08s"),hexStr);
}

void HexParseUtil::formatBcdToHex(const CString& strInput,CString& strOutput,bool isHighToLower)
{
	TCHAR* stopStr = _T("\0");
	long lg = 0L;
	CString hexStr("");
#ifdef UNICODE
	lg = wcstol(strInput.GetString(),&stopStr,10);
	TCHAR hexSize[256];
	swprintf(hexSize,_T("%x"),lg);
	hexStr = CString(hexSize).MakeUpper();
#else
	lg = atol(strInput.GetString());
	ltoa(lg,stopStr,16);
	hexStr = CString(stopStr).MakeUpper();
#endif
	strOutput.Format(_T("%08s"),hexStr);
	if(isHighToLower)
	{
		CString chead4  = strOutput.Mid(0,4).Mid(2,2)+strOutput.Mid(0,4).Mid(0,2);
		CString ctrail4 = strOutput.Mid(4,4).Mid(2,2)+strOutput.Mid(4,4).Mid(0,2);
		strOutput = ctrail4 + chead4;
	}
}

