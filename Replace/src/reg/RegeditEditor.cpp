/***************************************************************
 * RegeditEditor.h
 * 系统名  : 常用工具管理
 * 子系统名: 常用工具类
 * 更改日期      姓名           摘要
 * ----------    -----------    --------------------------------
 * 2011/04/01    王井锋			初次开发
 ***************************************************************
 */
#include "reg/RegeditEditor.h"
RegeditEditor::RegeditEditor()
{
}

void RegeditEditor::FormatError()
{
	DWORD dwError = GetLastError();
	HLOCAL hlocal = NULL;
	//格式化错误消息
	BOOL fOK = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_ALLOCATE_BUFFER,NULL,
		dwError,MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED),
		(LPTSTR)&hlocal,0,NULL);

	AfxMessageBox((PCTSTR)LocalLock(hlocal));

	LocalFree(hlocal);
}

bool RegeditEditor::UpdateRegedit(const CString& subKey,const CString& valueName,const CString& value)
{
	HKEY hKeyIn =  HKEY_CURRENT_USER, hKeyOut; 
	if(  ERROR_SUCCESS != RegOpenKeyEx(hKeyIn, subKey, 0, 
		KEY_WRITE,&hKeyOut)) 
	{ 
		FormatError();
		return false; 
	} 

	DWORD type = REG_SZ;

	if(ERROR_SUCCESS != RegSetValueEx(hKeyOut,valueName,NULL,type,CStringToLPBYTE(value),value.GetLength()+1))
	{
		FormatError();
		return false;
	}

	return true;
}

bool RegeditEditor::UpdateRegedit(const CString& subKey,const CString& valueName,const int& value)
{
	HKEY hKeyIn =  HKEY_CURRENT_USER, hKeyOut; 
	if(  ERROR_SUCCESS != RegOpenKeyEx(hKeyIn, subKey, 0, 
		KEY_WRITE,&hKeyOut)) 
	{ 
		FormatError();
		return false; 
	} 

	DWORD type = REG_DWORD;
	//LPBYTE lpb = CStringToLPBYTE(value);
	if(ERROR_SUCCESS != RegSetValueEx(hKeyOut,valueName,NULL,type,(BYTE*)&value,sizeof(DWORD)))
	{
		FormatError();
		return false;
	}

	return true;
}



LPBYTE RegeditEditor::CStringToLPBYTE(const CString& str)
{
	LPBYTE lpb = new BYTE[str.GetLength()+1];
	for(int i =0 ;i<str.GetLength();i++)
	{
		lpb[i] = (BYTE)str[i];
	}
	lpb[str.GetLength()] = 0;
	return lpb;
}
