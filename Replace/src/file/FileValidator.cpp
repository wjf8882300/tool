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
		//�����ļ�ͷ
		if(iter + 1 != _sources.end() && (*iter).GetLength() != (*(iter+1)).GetLength())
		{
			CStringArray heads;
			Util::SplitString(*iter, _T(","), heads);

			if(heads.GetSize() != 8)
			{
				throw Poco::SyntaxException(Poco::format("�м�¼������8���ֶΣ�ʵ��Ϊ%d���ֶ�", heads.GetSize()));
			}

			LengthValidator("���д���", 4).validate(Util::Wcstombs(heads[col ++ ]));
			LengthValidator("�������׼�¼����", 8).validate(Util::Wcstombs(heads[col ++]));
			LengthValidator("���������ܽ��", 8).validate(Util::Wcstombs(heads[col ++]));
			LengthValidator("�Ҽ�¼������", 8).validate(Util::Wcstombs(heads[col ++]));
			LengthValidator("�Ҽ�¼�ܽ��", 8).validate(Util::Wcstombs(heads[col ++]));
			LengthValidator("�������κ�", 6).validate(Util::Wcstombs(heads[col ++]));
			LengthValidator("��������", 8).validate(Util::Wcstombs(heads[col ++]));
			LengthValidator("����ʱ��", 6).validate(Util::Wcstombs(heads[col ++]));

			col = 0;
			std::string cityCodes[] = {"9150","2150","7150","2210"};
			EnumValidator<std::string>("���д���", cityCodes, 4).validate(Util::Wcstombs(heads[col ++]));
			HexValidator("�������׼�¼����").validate(Util::Wcstombs(heads[col ++]));
			HexValidator("���������ܽ��").validate(Util::Wcstombs(heads[col ++]));
			HexValidator("�Ҽ�¼������").validate(Util::Wcstombs(heads[col ++]));
			HexValidator("�Ҽ�¼�ܽ��").validate(Util::Wcstombs(heads[col ++]));
			IntegerValidator("�������κ�").validate(Util::Wcstombs(heads[col ++]));
			DateTimeValidator("��������", "%Y%m%d").validate(Util::Wcstombs(heads[col ++]));
			DateTimeValidator("����ʱ��", "%H%M%S").validate(Util::Wcstombs(heads[col ++]));
			
			coutput.Format(_T("[��%04d            ] | ��ȷ\r\n"), row);
		}
	}
	catch(Poco::InvalidArgumentException& ex)
	{
		coutput += CString(Poco::format("[��%04d��%04d] | ���� | %s\r\n", row, col, ex.message()).c_str());
	}
	catch(Poco::SyntaxException& ex)
	{
		coutput += CString(Poco::format("[��%04d��%04d] | ���� | %s\r\n", row, col, ex.message()).c_str());
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
				throw Poco::SyntaxException(Poco::format("�м�¼������18���ֶΣ�ʵ��Ϊ%d���ֶ�", lines.GetSize()));
			}
			
			col  = 0;
			LengthValidator("��¼����", 1).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("��������", 2).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("�û���Ӧ�����к�", 16).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("Pos�����", 6).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("�ն˻����", 12).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("�ն˽������", 8).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("��������", 8).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("����ʱ��", 6).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("����ǰ���", 8).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("���׽��", 8).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("Ӧ�ս��", 8).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("PSAM���汾��", 2).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("��Ƭ�ѻ��������к�", 8).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("������֤TAC", 8).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("���д���", 4).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("��ҵ����", 2).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("���Ŵ���", 4).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("MAC��", 8).validate(Util::Wcstombs(lines[col ++ ]));

			col = 0;
			std::string recTypes[] = {"0","1"};
			EnumValidator<std::string>("��¼����", recTypes, 2).validate(Util::Wcstombs(lines[col ++ ]));	
			std::string tradeTypes[] = {"05", "06", "07"};
			EnumValidator<std::string>("��������", tradeTypes, 3).validate(Util::Wcstombs(lines[col ++ ]));
			IntegerValidator("�û���Ӧ�����к�").validate(Util::Wcstombs(lines[col ++ ]));
			IntegerValidator("Pos�����").validate(Util::Wcstombs(lines[col ++ ]));
			HexValidator("�ն˻����").validate(Util::Wcstombs(lines[col ++ ]));
			HexValidator("�ն˽������").validate(Util::Wcstombs(lines[col ++ ]));
			DateTimeValidator("��������", "%Y%m%d").validate(Util::Wcstombs(lines[col ++ ]));
			DateTimeValidator("����ʱ��", "%H%M%S").validate(Util::Wcstombs(lines[col ++ ]));
			HexValidator("����ǰ���").validate(Util::Wcstombs(lines[col ++ ]));
			HexValidator("���׽��").validate(Util::Wcstombs(lines[col ++ ]));
			HexValidator("Ӧ�ս��").validate(Util::Wcstombs(lines[col ++ ]));
			HexValidator("PSAM���汾��").validate(Util::Wcstombs(lines[col ++ ]));
			IntegerValidator("��Ƭ�ѻ��������к�").validate(Util::Wcstombs(lines[col ++ ]));
			HexValidator("������֤TAC").validate(Util::Wcstombs(lines[col ++ ]));
			std::string cityCodes[] = {"9150","2150","7150","2210"};
			EnumValidator<std::string>("���д���", cityCodes, 4).validate(Util::Wcstombs(lines[col ++ ]));
			HexValidator("��ҵ����").validate(Util::Wcstombs(lines[col ++ ]));
			HexValidator("���Ŵ���").validate(Util::Wcstombs(lines[col ++ ]));
			HexValidator("MAC��").validate(Util::Wcstombs(lines[col ++ ]));

			CString msg;
			msg.Format(_T("[��%04d            ] | ��ȷ\r\n"), row);
			coutput += msg;
		}
		catch(Poco::InvalidArgumentException& ex)
		{
			coutput += CString(Poco::format("[��%04d��%04d] | ���� | %s\r\n", row, col, ex.message()).c_str());
		}
		catch(Poco::SyntaxException& ex)
		{
			coutput += CString(Poco::format("[��%04d��%04d] | ���� | %s\r\n", row, col, ex.message()).c_str());
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
		//�����ļ�ͷ
		if(iter + 1 != _sources.end() && (*iter).GetLength() != (*(iter+1)).GetLength())
		{
			CStringArray heads;
			Util::SplitString(*iter, _T(","), heads);

			if(heads.GetSize() != 6)
			{
				throw Poco::SyntaxException(Poco::format("�м�¼������6���ֶΣ�ʵ��Ϊ%d���ֶ�", heads.GetSize()));
			}

			LengthValidator("���д���", 4).validate(Util::Wcstombs(heads[col ++ ]));
			VarLengthValidator("���׼�¼����", 8).validate(Util::Wcstombs(heads[col ++]));
			VarLengthValidator("�����ܽ��", 8).validate(Util::Wcstombs(heads[col ++]));
			LengthValidator("�������κ�", 6).validate(Util::Wcstombs(heads[col ++]));
			LengthValidator("��������", 8).validate(Util::Wcstombs(heads[col ++]));
			LengthValidator("����ʱ��", 6).validate(Util::Wcstombs(heads[col ++]));

			col = 0;
			std::string cityCodes[] = {"9150","2150","7150","2210"};
			EnumValidator<std::string>("���д���", cityCodes, 4).validate(Util::Wcstombs(heads[col ++]));
			HexValidator("���׼�¼����").validate(Util::Wcstombs(heads[col ++]));
			HexValidator("�����ܽ��").validate(Util::Wcstombs(heads[col ++]));
			IntegerValidator("�������κ�").validate(Util::Wcstombs(heads[col ++]));
			DateTimeValidator("��������", "%Y%m%d").validate(Util::Wcstombs(heads[col ++]));
			DateTimeValidator("����ʱ��", "%H%M%S").validate(Util::Wcstombs(heads[col ++]));
			
			coutput.Format(_T("[��%04d            ] | ��ȷ\r\n"), row);
		}
	}
	catch(Poco::InvalidArgumentException& ex)
	{
		coutput += CString(Poco::format("[��%04d��%04d] | ���� | %s\r\n", row, col, ex.message()).c_str());
	}
	catch(Poco::SyntaxException& ex)
	{
		coutput += CString(Poco::format("[��%04d��%04d] | ���� | %s\r\n", row, col, ex.message()).c_str());
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
				throw Poco::SyntaxException(Poco::format("�м�¼������16���ֶΣ�ʵ��Ϊ%d���ֶ�", lines.GetSize()));
			}
			
			col  = 0;
			LengthValidator("����״̬", 1).validate(Util::Wcstombs(lines[col ++ ]));
			VarLengthValidator("ICӦ�����к�", 20).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("IC�������к�", 4).validate(Util::Wcstombs(lines[col ++ ]));
			VarLengthValidator("���п���", 19).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("��������", 2).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("������", 2).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("��������", 8).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("����ʱ��", 6).validate(Util::Wcstombs(lines[col ++ ]));
			VarLengthValidator("���׽��", 8).validate(Util::Wcstombs(lines[col ++ ]));
			VarLengthValidator("����ǰ���", 8).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("��ֵ����", 4).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("SAM���", 12).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("POS���", 6).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("����Ա��", 4).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("TAC��֤��", 8).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("������ˮ��", 16).validate(Util::Wcstombs(lines[col ++ ]));

			col = 0;
			std::string recTypes[] = {"1","2","3"};
			EnumValidator<std::string>("����״̬", recTypes, 3).validate(Util::Wcstombs(lines[col ++ ]));
			IntegerValidator("ICӦ�����к�").validate(Util::Wcstombs(lines[col ++ ]));
			HexValidator("IC�������к�").validate(Util::Wcstombs(lines[col ++ ]));
			AsciiValidator("���п���").validate(Util::Wcstombs(lines[col ++ ]));
			std::string tradeTypes[] = {"02"};
			EnumValidator<std::string>("��������", tradeTypes, 1).validate(Util::Wcstombs(lines[col ++ ]));
			HexValidator("������").validate(Util::Wcstombs(lines[col ++ ]));
			DateTimeValidator("��������", "%Y%m%d").validate(Util::Wcstombs(lines[col ++ ]));
			DateTimeValidator("����ʱ��", "%H%M%S").validate(Util::Wcstombs(lines[col ++ ]));
			HexValidator("���׽��").validate(Util::Wcstombs(lines[col ++ ]));
			HexValidator("����ǰ���").validate(Util::Wcstombs(lines[col ++ ]));
			AsciiValidator("��ֵ����").validate(Util::Wcstombs(lines[col ++ ]));
			HexValidator("SAM���").validate(Util::Wcstombs(lines[col ++ ]));
			IntegerValidator("POS���").validate(Util::Wcstombs(lines[col ++ ]));
			AsciiValidator("����Ա��").validate(Util::Wcstombs(lines[col ++ ]));
			HexValidator("TAC��֤��").validate(Util::Wcstombs(lines[col ++ ]));
			HexValidator("������ˮ��").validate(Util::Wcstombs(lines[col ++ ]));

			CString msg;
			msg.Format(_T("[��%04d            ] | ��ȷ\r\n"), row);
			coutput += msg;
		}
		catch(Poco::InvalidArgumentException& ex)
		{
			coutput += CString(Poco::format("[��%04d��%04d] | ���� | %s\r\n", row, col, ex.message()).c_str());
		}
		catch(Poco::SyntaxException& ex)
		{
			coutput += CString(Poco::format("[��%04d��%04d] | ���� | %s\r\n", row, col, ex.message()).c_str());
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
		//�����ļ�ͷ
		if(iter + 1 != _sources.end() && (*iter).GetLength() != (*(iter+1)).GetLength())
		{
			CStringArray heads;
			Util::SplitString(*iter, _T(","), heads);

			if(heads.GetSize() != 6)
			{
				throw Poco::SyntaxException(Poco::format("�м�¼������6���ֶΣ�ʵ��Ϊ%d���ֶ�", heads.GetSize()));
			}

			LengthValidator("���д���", 4).validate(Util::Wcstombs(heads[col ++ ]));
			VarLengthValidator("���׼�¼����", 8).validate(Util::Wcstombs(heads[col ++]));
			VarLengthValidator("�����ܽ��", 8).validate(Util::Wcstombs(heads[col ++]));
			LengthValidator("�������κ�", 6).validate(Util::Wcstombs(heads[col ++]));
			LengthValidator("��������", 8).validate(Util::Wcstombs(heads[col ++]));
			LengthValidator("����ʱ��", 6).validate(Util::Wcstombs(heads[col ++]));

			col = 0;
			std::string cityCodes[] = {"9150","2150","7150","2210"};
			EnumValidator<std::string>("���д���", cityCodes, 4).validate(Util::Wcstombs(heads[col ++]));
			HexValidator("���׼�¼����").validate(Util::Wcstombs(heads[col ++]));
			HexValidator("�����ܽ��").validate(Util::Wcstombs(heads[col ++]));
			IntegerValidator("�������κ�").validate(Util::Wcstombs(heads[col ++]));
			DateTimeValidator("��������", "%Y%m%d").validate(Util::Wcstombs(heads[col ++]));
			DateTimeValidator("����ʱ��", "%H%M%S").validate(Util::Wcstombs(heads[col ++]));
			
			coutput.Format(_T("[��%04d            ] | ��ȷ\r\n"), row);
		}
	}
	catch(Poco::InvalidArgumentException& ex)
	{
		coutput += CString(Poco::format("[��%04d��%04d] | ���� | %s\r\n", row, col, ex.message()).c_str());
	}
	catch(Poco::SyntaxException& ex)
	{
		coutput += CString(Poco::format("[��%04d��%04d] | ���� | %s\r\n", row, col, ex.message()).c_str());
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
				throw Poco::SyntaxException(Poco::format("�м�¼������16���ֶΣ�ʵ��Ϊ%d���ֶ�", lines.GetSize()));
			}
			
			col  = 0;
			LengthValidator("����״̬", 1).validate(Util::Wcstombs(lines[col ++ ]));
			VarLengthValidator("ICӦ�����к�", 20).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("IC�������к�", 4).validate(Util::Wcstombs(lines[col ++ ]));
			VarLengthValidator("���п���", 19).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("��������", 2).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("������", 2).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("��������", 8).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("����ʱ��", 6).validate(Util::Wcstombs(lines[col ++ ]));
			VarLengthValidator("���׽��", 8).validate(Util::Wcstombs(lines[col ++ ]));
			VarLengthValidator("����ǰ���", 8).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("��ֵ����", 4).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("SAM���", 12).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("����Ա��", 4).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("TAC��֤��", 8).validate(Util::Wcstombs(lines[col ++ ]));
			LengthValidator("������ˮ��", 16).validate(Util::Wcstombs(lines[col ++ ]));

			col = 0;
			std::string recTypes[] = {"1","2","3"};
			EnumValidator<std::string>("����״̬", recTypes, 3).validate(Util::Wcstombs(lines[col ++ ]));
			IntegerValidator("ICӦ�����к�").validate(Util::Wcstombs(lines[col ++ ]));
			HexValidator("IC�������к�").validate(Util::Wcstombs(lines[col ++ ]));
			AsciiValidator("���п���").validate(Util::Wcstombs(lines[col ++ ]));
			std::string tradeTypes[] = {"02"};
			EnumValidator<std::string>("��������", tradeTypes, 1).validate(Util::Wcstombs(lines[col ++ ]));
			HexValidator("������").validate(Util::Wcstombs(lines[col ++ ]));
			DateTimeValidator("��������", "%Y%m%d").validate(Util::Wcstombs(lines[col ++ ]));
			DateTimeValidator("����ʱ��", "%H%M%S").validate(Util::Wcstombs(lines[col ++ ]));
			HexValidator("���׽��").validate(Util::Wcstombs(lines[col ++ ]));
			HexValidator("����ǰ���").validate(Util::Wcstombs(lines[col ++ ]));
			AsciiValidator("��ֵ����").validate(Util::Wcstombs(lines[col ++ ]));
			HexValidator("SAM���").validate(Util::Wcstombs(lines[col ++ ]));
			AsciiValidator("����Ա��").validate(Util::Wcstombs(lines[col ++ ]));
			HexValidator("TAC��֤��").validate(Util::Wcstombs(lines[col ++ ]));
			HexValidator("������ˮ��").validate(Util::Wcstombs(lines[col ++ ]));

			CString msg;
			msg.Format(_T("[��%04d            ] | ��ȷ\r\n"), row);
			coutput += msg;
		}
		catch(Poco::InvalidArgumentException& ex)
		{
			coutput += CString(Poco::format("[��%04d��%04d] | ���� | %s\r\n", row, col, ex.message()).c_str());
		}
		catch(Poco::SyntaxException& ex)
		{
			coutput += CString(Poco::format("[��%04d��%04d] | ���� | %s\r\n", row, col, ex.message()).c_str());
		}
	}
}