/***************************************************************
 * DESUtil.h
 * ϵͳ��  : ���ù��߹���
 * ��ϵͳ��: ���ù�����
 * ��������      ����           ժҪ
 * ----------    -----------    --------------------------------
 * 2011/04/01    ������			���ο���
 ***************************************************************
 */
#include "encrypt/DESUtil.h"
#include "encrypt/3des.h"
#include "encrypt/DesEnc.h"

DESUtil::DESUtil(std::vector<CString>& sr,std::vector<CString>& dt):sources(sr),dests(dt)
{

}

string DESUtil::Encrypt(const string& szinput,const string& szkey)
{
	std::ostringstream ss;
	char szOutput[16];
	char szInput[16];
	for(int i = 0; i < (int)szinput.size(); i += 16)
	{
		memset(	szOutput, 0 , sizeof(szOutput));
		memcpy( szInput, szinput.c_str() + i, sizeof(szInput));
		StrEnDES(const_cast<char*>(szkey.c_str()), szInput, szOutput);
		ss.write(szOutput, sizeof(szOutput));
	}
	return Poco::toUpper(ss.str());
	
}

string DESUtil::Decrypt(const string& szinput,const string& szkey)
{
	std::ostringstream ss;
	char szOutput[16];
	for(int i = 0; i < (int)szinput.size(); i += 16)
	{
		memset(	szOutput, 0 , sizeof(szOutput));
		StrDeDES(const_cast<char*>(szkey.c_str()), const_cast<char*>(szinput.c_str() + i), szOutput);
		ss.write(szOutput, sizeof(szOutput));
	}
	return Poco::toUpper(ss.str());
}

