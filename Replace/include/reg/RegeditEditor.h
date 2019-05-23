/***************************************************************
 * RegeditEditor.h
 * 系统名  : 常用工具管理
 * 子系统名: 常用工具类
 * 更改日期      姓名           摘要
 * ----------    -----------    --------------------------------
 * 2011/04/01    王井锋			初次开发
 ***************************************************************
 */
#ifndef RegeditEditor_H_
#define RegeditEditor_H_
#include "util/util.h"
using namespace std;
/**
 * 工具类
 * @author  王井锋
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