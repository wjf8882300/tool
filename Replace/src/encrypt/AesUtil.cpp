/***************************************************************
* AesUtil.h
* 系统名  : 常用工具管理
* 子系统名: 常用工具类
* 更改日期      姓名           摘要
* ----------    -----------    --------------------------------
* 2011/04/01    王井锋			初次开发
***************************************************************
*/
#include "encrypt/AesUtil.h"
AesUtil::AesUtil(std::vector<CString>& sr,std::vector<CString>& dt):sources(sr),dests(dt)
{

}

void AesUtil::Encrypt(CString cinput,CString ckey,CString& coutput)
{
	std::string input = Util::Wcstombs(cinput);
	std::string szkey = Util::Wcstombs(ckey);
	
	std::string szOutput;

	int inputlen = 0;
	int outputlen = 0;
	char* output = NULL;

	const char* szInput = input.c_str();
	const char* key = szkey.c_str();

	inputlen = (int)strlen(szInput);
	outputlen = XEncrypt::GetEncryptSize(inputlen) + 1;
	output = new char[outputlen];
	output[outputlen-1] = '\0';
	if(XEncrypt::Encrypt(szInput,-1,output,outputlen,key))
	{
		szOutput = string(output);
	}	

	if(output != NULL)
	{
		delete[] output;
		output = NULL;
	}

	coutput = CString(szOutput.c_str());
}

void AesUtil::Decrypt(CString cinput,CString ckey,CString& coutput)
{
	std::string input = Util::Wcstombs(cinput);
	std::string szkey = Util::Wcstombs(ckey);
	std::string szOutput;

	int inputlen = 0;
	int outputlen = 0;
	char* output = NULL;

	const char* szInput = input.c_str();
	const char* key = szkey.c_str();

	inputlen = (int)strlen(szInput);
	outputlen = (inputlen>>1) + 1;
	output = new char[outputlen];
	output[outputlen-1] = '\0';
	if(XEncrypt::Decrypt(szInput,-1,output,outputlen,key))
	{
		szOutput = string(output);
	}

	if(output != NULL)
	{
		delete[] output;
		output = NULL;
	}

	coutput = CString(szOutput.c_str());
}