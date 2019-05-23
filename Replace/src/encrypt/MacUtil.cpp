/***************************************************************
 * MacUtil.cpp
 * 系统名  : 常用工具管理
 * 子系统名: 常用工具类
 * 更改日期      姓名           摘要
 * ----------    -----------    --------------------------------
 * 2011/04/01    王井锋			初次开发
 ***************************************************************
 */
#include "encrypt/MacUtil.h"
MacUtil::MacUtil(std::vector<CString>& sr,std::vector<CString>& dt):sources(sr),dests(dt)
{
}

void MacUtil::MacCalculate(const CString& cinput,CString& output,bool isPadding,bool isAscii,bool isVerse,bool isSub)//
{
	std::string input = Util::Wcstombs(cinput);
	

	std::string sstr;
	/*if(isVerse)
		getMac3(input,input,sstr,16,isPadding,isAscii);
	else
		getMac(input,input,sstr,16,isPadding,isAscii);*/
	if(isSub)
	{
		sstr=sstr.substr(0,8);
	}

	output = CString(sstr.c_str());
	output.MakeUpper();
}

void MacUtil::MacValidate(CString& coutput,bool isPadding,bool isAscii,bool isVerse,bool isSub,CProgressCtrl &cpc)//
{
	const int buf_size=1000;

	CString cinput;
	int count = 1;
	cpc.SetRange(0,(short)sources.size());
	cpc.SetStep(1);
	for(vector<CString>::iterator iter =sources.begin();iter!=sources.end();iter++)
	{
		cinput = *iter;
		CString maccode = cinput.Right(8);
		cinput.Replace(_T(","),_T(""));
		cinput.Replace(maccode,_T(""));
		//TCHAR buf[buf_size];
		//wcscpy_s(buf, buf_size, cinput);
		//size_t cn = 0;
		//wcstombs_s(&cn,inputs,buf_size,buf,buf_size);
		//std::string input(inputs);
		std::string input = Util::Wcstombs(cinput);
		
		std::string sstr;
		/*if(isVerse)
			getMac3(input,input,sstr,16,isPadding,isAscii);
		else
			getMac(input,input,sstr,16,isPadding,isAscii);*/
		
		sstr=sstr.substr(0,8);
		CString output(sstr.c_str());
		output.MakeUpper();
		CString mes;
		if(maccode==output)
		{
			mes.Format(_T("第%08d行 OK\r\n"),count++);
		}
		else
		{
			mes.Format(_T("第%08d行 ERROR %s\r\n"),count++,output);
		}
		coutput += mes;
		cpc.StepIt();
	}
}