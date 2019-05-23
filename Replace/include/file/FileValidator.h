#ifndef FILEVALIDATOR_H_
#define FILEVALIDATOR_H_
#include "util/util.h"

class FileValidator
{
public:
	FileValidator(std::vector<CString>& sr,std::vector<CString>& dt);
	~FileValidator();

	/*
	 * 消费数据CPU卡
	 */
	void PurcharseValidate(CString& coutput, CProgressCtrl &cpc);
	/*
	 * 充值数据(移动电信)
	 */
	void LoadValidateDX(CString& coutput, CProgressCtrl &cpc);
	/*
	 * 充值数据(在线充付)
	 */
	void LoadValidateOnline(CString& coutput, CProgressCtrl &cpc);

private:
	std::vector<CString> _sources;
	std::vector<CString> _dests;
};

#endif