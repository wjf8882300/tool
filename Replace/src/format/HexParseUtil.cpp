/***************************************************************
 * HexParseUtil.h
 * 系统名  : 常用工具管理
 * 子系统名: 常用工具类
 * 更改日期      姓名           摘要
 * ----------    -----------    --------------------------------
 * 2011/04/01    王井锋			初次开发
 ***************************************************************
 */
#include "format/HexParseUtil.h"
HexParseUtil::HexParseUtil()
{
}

void HexParseUtil::formatHexToBcd(const CString& strInput,CString& strOutput,bool isHighToLower)
{
	CString tmp;
	//先转成8位
	tmp.Format(_T("%08s"),strInput);
	//需高低位转换
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

