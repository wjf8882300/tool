/***************************************************************
* AesUtil.h
* ϵͳ��  : ���ù��߹���
* ��ϵͳ��: ���ù�����
* ��������      ����           ժҪ
* ----------    -----------    --------------------------------
* 2011/04/01    ������			���ο���
***************************************************************
*/
#include "encrypt/MD5Util.h"
#include "encrypt/MD5Engine.h"
MD5Util::MD5Util()
{

}

void MD5Util::Encrypt(const string& input,string& output)
{
	MD5Engine md5;
	md5.update(input.c_str(), (unsigned int)input.length());
	output = Poco::toUpper(DigestEngine::digestToHex(md5.digest()));	
}

