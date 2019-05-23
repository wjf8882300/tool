#include "file/FileValidator.h"
#include "file/Validator.h"

FileValidator::FileValidator(std::vector<CString>& sr,std::vector<CString>& dt):_sources(sr),_dests(dt)
{

}

FileValidator::~FileValidator()
{

}

void FileValidator::PurcharseValidate(CString& coutput, CProgressCtrl &cpc)
{
	int row = 1;
	int col = 0;
	cpc.SetRange(0,(short)_sources.size());
	cpc.SetStep(1);
	vector<CString>::iterator iter = _sources.begin();

	try
	{
		//存在文件头
		if(iter + 1 != _sources.end() && (*iter).GetLength() != (*(iter+1)).GetLength())
		{
			CStringArray heads;
			Util::SplitString(*iter, _T(","), heads);

			if(heads.GetSize() != 8)
			{
				throw Poco::SyntaxException(Poco::format("行记录必须是8个字段，实际为%d个字段", heads.GetSize()));
			}

			LengthValidator("城市代码", 4).validate(Util::Wcstombs(heads[col ++ ]));
			LengthValidator("正常交易记录条数", 8).validate(Util::Wcstombs(heads[col ++]));
			LengthValidator("正常交易总金额", 8).validate(Util::Wcstombs(heads[col ++]));
			LengthValidator("灰记录总条数", 8).validate(Util::Wcstombs(heads[col ++]));
			LengthValidator("灰记录总金额", 8).validate(Util::Wcstombs(heads[col ++]));
			LengthValidator("数据批次号", 6).validate(Util::Wcstombs(heads[col ++]));
			LengthValidator("传送日期", 8).validate(Util::Wcstombs(heads[col ++]));
			LengthValidator("传送时间", 6).validate(Util::Wcstombs(heads[col ++]));

			col = 0;
			std::string cityCodes[] = {"9150","2150","7150","2210"};
			EnumValidator<std::string>("城市代码", cityCodes, 4).validate(Util::Wcstombs(heads[col ++]));
			HexValidator("正常交易记录条数").validate(Util::Wcstombs(heads[col ++]));
			HexValidator("正常交易总金额").validate(Util::Wcstombs(heads[col ++]));
			HexValidator("灰记录总条数").validate(Util::Wcstombs(heads[col ++]));
			HexValidator("灰记录总金额").validate(Util::Wcstombs(heads[col ++]));
			IntegerValidator("数据批次号").validate(Util::Wcstombs(heads[col ++]));
			DateTimeValidator("传送日期", "%Y%m%d").validate(Util::Wcstombs(heads[col ++]));
			DateTimeValidator("传送时间", "%H%M%S").validate(Util::Wcstombs(heads[col ++]));
			
			coutput.Format(_T("[行%04d            ] | 正确\r\n"), row);
		}
	}
	catch(Poco::InvalidArgumentException& ex)
	{
		coutput += CString(Poco::format("[行%04d列%04d] | 错误 | %s\r\n", row, col, ex.message()).c_str());
	}
	catch(Poco::SyntaxException& ex)
	{
		coutput += CString(Poco::format("[行%04d列%04d] | 错误 | %s\r\n", row, col, ex.message()).c_str());
	}

	iter ++;
	row++;
	cpc.StepIt();

	for(;iter!=_sources.end();iter++,row++,cpc.StepIt())
	{
		CStringArray lines;
		Util::SplitString(*iter, _T(","), lines);
		
		try
		{
			if(lines.GetSize() != 18)
			{
				throw Poco::SyntaxException(Poco::format("行记录必须是18个字段，实际为%d个字段", lines.GetSize()));
			}
			
			col  = 0;
			LengthValidator("记录类型", 1).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("交易类型", 2).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("用户卡应用序列号", 16).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("Pos机编号", 6).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("终端机编号", 12).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("终端交易序号", 8).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("交易日期", 8).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("交易时间", 6).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("交易前余额", 8).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("交易金额", 8).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("应收金额", 8).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("PSAM卡版本号", 2).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("卡片脱机交易序列号", 8).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("交易认证TAC", 8).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("城市代码", 4).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("行业代码", 2).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("部门代码", 4).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("MAC码", 8).validate(Util::Wcstombs(lines[col ++ ]));

			col = 0;
			std::string recTypes[] = {"0","1"};
			EnumValidator<std::string>("记录类型", recTypes, 2).validate(Util::Wcstombs(lines[col ++ ]));	
			std::string tradeTypes[] = {"05", "06", "07"};
			EnumValidator<std::string>("交易类型", tradeTypes, 3).validate(Util::Wcstombs(lines[col ++ ]));
			IntegerValidator("用户卡应用序列号").validate(Util::Wcstombs(lines[col ++ ]));
			IntegerValidator("Pos机编号").validate(Util::Wcstombs(lines[col ++ ]));
			HexValidator("终端机编号").validate(Util::Wcstombs(lines[col ++ ]));
			HexValidator("终端交易序号").validate(Util::Wcstombs(lines[col ++ ]));
			DateTimeValidator("交易日期", "%Y%m%d").validate(Util::Wcstombs(lines[col ++ ]));
			DateTimeValidator("交易时间", "%H%M%S").validate(Util::Wcstombs(lines[col ++ ]));
			HexValidator("交易前余额").validate(Util::Wcstombs(lines[col ++ ]));
			HexValidator("交易金额").validate(Util::Wcstombs(lines[col ++ ]));
			HexValidator("应收金额").validate(Util::Wcstombs(lines[col ++ ]));
			HexValidator("PSAM卡版本号").validate(Util::Wcstombs(lines[col ++ ]));
			IntegerValidator("卡片脱机交易序列号").validate(Util::Wcstombs(lines[col ++ ]));
			HexValidator("交易认证TAC").validate(Util::Wcstombs(lines[col ++ ]));
			std::string cityCodes[] = {"9150","2150","7150","2210"};
			EnumValidator<std::string>("城市代码", cityCodes, 4).validate(Util::Wcstombs(lines[col ++ ]));
			HexValidator("行业代码").validate(Util::Wcstombs(lines[col ++ ]));
			HexValidator("部门代码").validate(Util::Wcstombs(lines[col ++ ]));
			HexValidator("MAC码").validate(Util::Wcstombs(lines[col ++ ]));

			CString msg;
			msg.Format(_T("[行%04d            ] | 正确\r\n"), row);
			coutput += msg;
		}
		catch(Poco::InvalidArgumentException& ex)
		{
			coutput += CString(Poco::format("[行%04d列%04d] | 错误 | %s\r\n", row, col, ex.message()).c_str());
		}
		catch(Poco::SyntaxException& ex)
		{
			coutput += CString(Poco::format("[行%04d列%04d] | 错误 | %s\r\n", row, col, ex.message()).c_str());
		}
	}
}

void FileValidator::LoadValidateDX(CString& coutput, CProgressCtrl &cpc)
{
	int row = 1;
	int col = 0;
	cpc.SetRange(0,(short)_sources.size());
	cpc.SetStep(1);
	vector<CString>::iterator iter = _sources.begin();

	try
	{
		//存在文件头
		if(iter + 1 != _sources.end() && (*iter).GetLength() != (*(iter+1)).GetLength())
		{
			CStringArray heads;
			Util::SplitString(*iter, _T(","), heads);

			if(heads.GetSize() != 6)
			{
				throw Poco::SyntaxException(Poco::format("行记录必须是6个字段，实际为%d个字段", heads.GetSize()));
			}

			LengthValidator("城市代码", 4).validate(Util::Wcstombs(heads[col ++ ]));
			VarLengthValidator("交易记录条数", 8).validate(Util::Wcstombs(heads[col ++]));
			VarLengthValidator("交易总金额", 8).validate(Util::Wcstombs(heads[col ++]));
			LengthValidator("数据批次号", 6).validate(Util::Wcstombs(heads[col ++]));
			LengthValidator("传送日期", 8).validate(Util::Wcstombs(heads[col ++]));
			LengthValidator("传送时间", 6).validate(Util::Wcstombs(heads[col ++]));

			col = 0;
			std::string cityCodes[] = {"9150","2150","7150","2210"};
			EnumValidator<std::string>("城市代码", cityCodes, 4).validate(Util::Wcstombs(heads[col ++]));
			HexValidator("交易记录条数").validate(Util::Wcstombs(heads[col ++]));
			HexValidator("交易总金额").validate(Util::Wcstombs(heads[col ++]));
			IntegerValidator("数据批次号").validate(Util::Wcstombs(heads[col ++]));
			DateTimeValidator("传送日期", "%Y%m%d").validate(Util::Wcstombs(heads[col ++]));
			DateTimeValidator("传送时间", "%H%M%S").validate(Util::Wcstombs(heads[col ++]));
			
			coutput.Format(_T("[行%04d            ] | 正确\r\n"), row);
		}
	}
	catch(Poco::InvalidArgumentException& ex)
	{
		coutput += CString(Poco::format("[行%04d列%04d] | 错误 | %s\r\n", row, col, ex.message()).c_str());
	}
	catch(Poco::SyntaxException& ex)
	{
		coutput += CString(Poco::format("[行%04d列%04d] | 错误 | %s\r\n", row, col, ex.message()).c_str());
	}

	iter ++;
	row++;
	cpc.StepIt();

	for(;iter!=_sources.end();iter++,row++,cpc.StepIt())
	{
		CStringArray lines;
		Util::SplitString(*iter, _T(","), lines);
		
		try
		{
			if(lines.GetSize() != 16)
			{
				throw Poco::SyntaxException(Poco::format("行记录必须是16个字段，实际为%d个字段", lines.GetSize()));
			}
			
			col  = 0;
			LengthValidator("交易状态", 1).validate(Util::Wcstombs(lines[col ++ ]));
			VarLengthValidator("IC应用序列号", 20).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("IC交易序列号", 4).validate(Util::Wcstombs(lines[col ++ ]));
			VarLengthValidator("银行卡号", 19).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("交易类型", 2).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("卡类型", 2).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("交易日期", 8).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("交易时间", 6).validate(Util::Wcstombs(lines[col ++ ]));
			VarLengthValidator("交易金额", 8).validate(Util::Wcstombs(lines[col ++ ]));
			VarLengthValidator("交易前余额", 8).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("充值点编号", 4).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("SAM编号", 12).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("POS编号", 6).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("操作员号", 4).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("TAC认证码", 8).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("交易流水号", 16).validate(Util::Wcstombs(lines[col ++ ]));

			col = 0;
			std::string recTypes[] = {"1","2","3"};
			EnumValidator<std::string>("交易状态", recTypes, 3).validate(Util::Wcstombs(lines[col ++ ]));
			IntegerValidator("IC应用序列号").validate(Util::Wcstombs(lines[col ++ ]));
			HexValidator("IC交易序列号").validate(Util::Wcstombs(lines[col ++ ]));
			AsciiValidator("银行卡号").validate(Util::Wcstombs(lines[col ++ ]));
			std::string tradeTypes[] = {"02"};
			EnumValidator<std::string>("交易类型", tradeTypes, 1).validate(Util::Wcstombs(lines[col ++ ]));
			HexValidator("卡类型").validate(Util::Wcstombs(lines[col ++ ]));
			DateTimeValidator("交易日期", "%Y%m%d").validate(Util::Wcstombs(lines[col ++ ]));
			DateTimeValidator("交易时间", "%H%M%S").validate(Util::Wcstombs(lines[col ++ ]));
			HexValidator("交易金额").validate(Util::Wcstombs(lines[col ++ ]));
			HexValidator("交易前余额").validate(Util::Wcstombs(lines[col ++ ]));
			AsciiValidator("充值点编号").validate(Util::Wcstombs(lines[col ++ ]));
			HexValidator("SAM编号").validate(Util::Wcstombs(lines[col ++ ]));
			IntegerValidator("POS编号").validate(Util::Wcstombs(lines[col ++ ]));
			AsciiValidator("操作员号").validate(Util::Wcstombs(lines[col ++ ]));
			HexValidator("TAC认证码").validate(Util::Wcstombs(lines[col ++ ]));
			HexValidator("交易流水号").validate(Util::Wcstombs(lines[col ++ ]));

			CString msg;
			msg.Format(_T("[行%04d            ] | 正确\r\n"), row);
			coutput += msg;
		}
		catch(Poco::InvalidArgumentException& ex)
		{
			coutput += CString(Poco::format("[行%04d列%04d] | 错误 | %s\r\n", row, col, ex.message()).c_str());
		}
		catch(Poco::SyntaxException& ex)
		{
			coutput += CString(Poco::format("[行%04d列%04d] | 错误 | %s\r\n", row, col, ex.message()).c_str());
		}
	}
}

void FileValidator::LoadValidateOnline(CString& coutput, CProgressCtrl &cpc)
{
	int row = 1;
	int col = 0;
	cpc.SetRange(0,(short)_sources.size());
	cpc.SetStep(1);
	vector<CString>::iterator iter = _sources.begin();

	try
	{
		//存在文件头
		if(iter + 1 != _sources.end() && (*iter).GetLength() != (*(iter+1)).GetLength())
		{
			CStringArray heads;
			Util::SplitString(*iter, _T(","), heads);

			if(heads.GetSize() != 6)
			{
				throw Poco::SyntaxException(Poco::format("行记录必须是6个字段，实际为%d个字段", heads.GetSize()));
			}

			LengthValidator("城市代码", 4).validate(Util::Wcstombs(heads[col ++ ]));
			VarLengthValidator("交易记录条数", 8).validate(Util::Wcstombs(heads[col ++]));
			VarLengthValidator("交易总金额", 8).validate(Util::Wcstombs(heads[col ++]));
			LengthValidator("数据批次号", 6).validate(Util::Wcstombs(heads[col ++]));
			LengthValidator("传送日期", 8).validate(Util::Wcstombs(heads[col ++]));
			LengthValidator("传送时间", 6).validate(Util::Wcstombs(heads[col ++]));

			col = 0;
			std::string cityCodes[] = {"9150","2150","7150","2210"};
			EnumValidator<std::string>("城市代码", cityCodes, 4).validate(Util::Wcstombs(heads[col ++]));
			HexValidator("交易记录条数").validate(Util::Wcstombs(heads[col ++]));
			HexValidator("交易总金额").validate(Util::Wcstombs(heads[col ++]));
			IntegerValidator("数据批次号").validate(Util::Wcstombs(heads[col ++]));
			DateTimeValidator("传送日期", "%Y%m%d").validate(Util::Wcstombs(heads[col ++]));
			DateTimeValidator("传送时间", "%H%M%S").validate(Util::Wcstombs(heads[col ++]));
			
			coutput.Format(_T("[行%04d            ] | 正确\r\n"), row);
		}
	}
	catch(Poco::InvalidArgumentException& ex)
	{
		coutput += CString(Poco::format("[行%04d列%04d] | 错误 | %s\r\n", row, col, ex.message()).c_str());
	}
	catch(Poco::SyntaxException& ex)
	{
		coutput += CString(Poco::format("[行%04d列%04d] | 错误 | %s\r\n", row, col, ex.message()).c_str());
	}

	iter ++;
	row++;
	cpc.StepIt();

	for(;iter!=_sources.end();iter++,row++,cpc.StepIt())
	{
		CStringArray lines;
		Util::SplitString(*iter, _T(","), lines);
		
		try
		{
			if(lines.GetSize() != 15)
			{
				throw Poco::SyntaxException(Poco::format("行记录必须是16个字段，实际为%d个字段", lines.GetSize()));
			}
			
			col  = 0;
			LengthValidator("交易状态", 1).validate(Util::Wcstombs(lines[col ++ ]));
			VarLengthValidator("IC应用序列号", 20).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("IC交易序列号", 4).validate(Util::Wcstombs(lines[col ++ ]));
			VarLengthValidator("银行卡号", 19).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("交易类型", 2).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("卡类型", 2).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("交易日期", 8).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("交易时间", 6).validate(Util::Wcstombs(lines[col ++ ]));
			VarLengthValidator("交易金额", 8).validate(Util::Wcstombs(lines[col ++ ]));
			VarLengthValidator("交易前余额", 8).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("充值点编号", 4).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("SAM编号", 12).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("操作员号", 4).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("TAC认证码", 8).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("交易流水号", 16).validate(Util::Wcstombs(lines[col ++ ]));

			col = 0;
			std::string recTypes[] = {"1","2","3"};
			EnumValidator<std::string>("交易状态", recTypes, 3).validate(Util::Wcstombs(lines[col ++ ]));
			IntegerValidator("IC应用序列号").validate(Util::Wcstombs(lines[col ++ ]));
			HexValidator("IC交易序列号").validate(Util::Wcstombs(lines[col ++ ]));
			AsciiValidator("银行卡号").validate(Util::Wcstombs(lines[col ++ ]));
			std::string tradeTypes[] = {"02"};
			EnumValidator<std::string>("交易类型", tradeTypes, 1).validate(Util::Wcstombs(lines[col ++ ]));
			HexValidator("卡类型").validate(Util::Wcstombs(lines[col ++ ]));
			DateTimeValidator("交易日期", "%Y%m%d").validate(Util::Wcstombs(lines[col ++ ]));
			DateTimeValidator("交易时间", "%H%M%S").validate(Util::Wcstombs(lines[col ++ ]));
			HexValidator("交易金额").validate(Util::Wcstombs(lines[col ++ ]));
			HexValidator("交易前余额").validate(Util::Wcstombs(lines[col ++ ]));
			AsciiValidator("充值点编号").validate(Util::Wcstombs(lines[col ++ ]));
			HexValidator("SAM编号").validate(Util::Wcstombs(lines[col ++ ]));
			AsciiValidator("操作员号").validate(Util::Wcstombs(lines[col ++ ]));
			HexValidator("TAC认证码").validate(Util::Wcstombs(lines[col ++ ]));
			HexValidator("交易流水号").validate(Util::Wcstombs(lines[col ++ ]));

			CString msg;
			msg.Format(_T("[行%04d            ] | 正确\r\n"), row);
			coutput += msg;
		}
		catch(Poco::InvalidArgumentException& ex)
		{
			coutput += CString(Poco::format("[行%04d列%04d] | 错误 | %s\r\n", row, col, ex.message()).c_str());
		}
		catch(Poco::SyntaxException& ex)
		{
			coutput += CString(Poco::format("[行%04d列%04d] | 错误 | %s\r\n", row, col, ex.message()).c_str());
		}
	}
}