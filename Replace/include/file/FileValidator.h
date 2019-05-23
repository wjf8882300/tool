#ifndef FILEVALIDATOR_H_
#define FILEVALIDATOR_H_
#include "util/util.h"

class FileValidator
{
public:
	FileValidator(std::vector<CString>& sr,std::vector<CString>& dt);
	~FileValidator();

	/*
	 * ��������CPU��
	 */
	void PurcharseValidate(CString& coutput, CProgressCtrl &cpc);
	/*
	 * ��ֵ����(�ƶ�����)
	 */
	void LoadValidateDX(CString& coutput, CProgressCtrl &cpc);
	/*
	 * ��ֵ����(���߳丶)
	 */
	void LoadValidateOnline(CString& coutput, CProgressCtrl &cpc);

private:
	std::vector<CString> _sources;
	std::vector<CString> _dests;
};

#endif