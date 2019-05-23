/***************************************************************
 * FileUtil.h
 * ϵͳ��  : ���ù��߹���
 * ��ϵͳ��: ���ù�����
 * ��������      ����           ժҪ
 * ----------    -----------    --------------------------------
 * 2011/04/01    ������			���ο���
 ***************************************************************
 */
#ifndef FileUtil_H_
#define FileUtil_H_
#include "util/util.h"

enum OPERATION{ OPERATION_UPDATE,OPERATION_DELETE};

/**
 * ������
 * @author  ������
 * @version V2.0.00
 * @since V2.0.00
 */
class FileUtil {
public:	
	FileUtil(std::vector<CString>& sr,std::vector<CString>& dt);

	void AccessFile(const CString &path,const OPERATION oper,const bool fileNameRegex,const CString &ext,int& replaceFileCount);

	bool ReplaceFileText(const CString &path);

	void ReadFileContent(const CString &path);

	bool RemoveFile(CString path);

private:
	
private:
	std::vector<CString> &sources;
	std::vector<CString> &dests;
};


#endif /*FileUtil_H_*/