/***************************************************************
 * DESUtil.h
 * ϵͳ��  : ���ù��߹���
 * ��ϵͳ��: ���ù�����
 * ��������      ����           ժҪ
 * ----------    -----------    --------------------------------
 * 2011/04/01    ������			���ο���
 ***************************************************************
 */
#ifndef __DESUtil_H 
#define __DESUtil_H  
#include "util/Util.h"

//#include "des.h"
//#include "hex.h"
//#include "files.h"
//using namespace CryptoPP;
//typedef auto_ptr<BlockTransformation> apbt;


class DESUtil { 
public: 
		DESUtil(std::vector<CString>& sr,std::vector<CString>& dt);

		/*
		 * DES����
		 * @input	�������
		 * @key		��Կ 
		 * @output	�������
		 */
		string Encrypt(const string& input,const string& key);
		/*
		 * DES����
		 * @input	�������
		 * @key		��Կ 
		 * @output	�������
		 */
		string Decrypt(const string& input,const string& key);
        // Encrypt/decrypt the data in "data", according to the "key". 
        // Caller is responsible for confirming the buffer size of "data" 
        // points to is 8*"blocks" bytes. 
        // The data encrypted/decrypted is stored in data. 
        // The return code is 1:success, other:failed. 
//protected:
//		
//		SecByteBlock HexDecodeString(const char *hex);
private:
	std::vector<CString> sources;
	std::vector<CString> dests;
}; 
#endif 