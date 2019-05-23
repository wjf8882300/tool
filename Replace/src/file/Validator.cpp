/***************************************************************
 * Validator.cpp
 * ϵͳ��  : ����һ��ͨϵͳ
 * ��ϵͳ��: �ӿ���ϵͳ_У��
 * ��������      ����           ժҪ
 * ----------    -----------    --------------------------------
 * 2011/12/02    ������			���ο���
 ***************************************************************
 */
#include "file/Validator.h"
#include "Poco/String.h"
#include "Poco/DateTimeParser.h"
#include "Poco/Format.h"
#include "Poco/NumberParser.h"
#include "Poco/NumberFormatter.h"

template<class T>
Validator<T>::Validator(const string& field):_field(field) {
}

template<class T>
Validator<T>::~Validator() {
}



void RequiredValidator::validate(const std::string &toBeValidate)
	throw (Poco::InvalidArgumentException)
{
	if(Poco::trim(toBeValidate)=="")
		throw Poco::InvalidArgumentException(
		"�ֶ� [" + _field + ":"+ toBeValidate + "] "+ "�Ǳ����");
}

LengthValidator::LengthValidator(const string& field,const int& length)
:Validator(field),_length(length)
{
}

void LengthValidator::validate(const std::string &toBeValidate)
	throw (Poco::InvalidArgumentException)
{
	if(_length!= toBeValidate.size())
		throw Poco::InvalidArgumentException(
		Poco::format("�ֶ� [" + _field + ":"+ toBeValidate + "] "+"��ʵ����Ϊ %d�� ��Ч����Ϊ %d" ,(int)toBeValidate.size(), _length));
}

DateTimeValidator::DateTimeValidator(const string& field, const string& format)
:Validator(field),_format(format)
{
}

void DateTimeValidator::validate(const std::string &toBeValidate)
	throw (Poco::InvalidArgumentException)
{
	Poco::DateTime dt;
	int tzd;
	try {
		Poco::DateTimeParser::parse(
			_format, toBeValidate, dt, tzd);
	} catch (Poco::SyntaxException&) {
		throw Poco::InvalidArgumentException(
			"�ֶ� [" + _field + ":"+ toBeValidate + "] "+"��ʽ����: " + toBeValidate + 
			". ��Ч��ʽΪ " + _format);
	}
}

void IntegerValidator::validate(const std::string &toBeValidate)
	throw (Poco::InvalidArgumentException)
{
	try {
		int len = toBeValidate.size();
		const char* src = toBeValidate.c_str();
		while(len--)
		{
			if(!Poco::Ascii::isDigit(*src ++))
				throw Poco::SyntaxException();
		}
	} catch(Poco::SyntaxException& ){
		string msg = Poco::format("�ֶ� [" + _field + ":"+ toBeValidate + "] "+"����������:%s",toBeValidate);
		throw Poco::InvalidArgumentException( msg);
	}
}

void HexValidator::validate(const std::string &toBeValidate)
	throw (Poco::InvalidArgumentException)
{
	try {
		int len = toBeValidate.size();
		const char* src = toBeValidate.c_str();
		while(len--)
		{
			if(!Poco::Ascii::isHexDigit(*src ++))
				throw Poco::SyntaxException();
		}
	} catch(Poco::SyntaxException& ){
		string msg = Poco::format("�ֶ� [" + _field + ":"+ toBeValidate + "] "+"������ʮ����������:",toBeValidate);
		throw Poco::InvalidArgumentException( msg);
	}
}

void AsciiValidator::validate(const std::string &toBeValidate)
	throw (Poco::InvalidArgumentException)
{
	try {
		int len = toBeValidate.size();
		const char* src = toBeValidate.c_str();
		while(len--)
		{
			if(!Poco::Ascii::isAscii(*src ++))
				throw Poco::SyntaxException();
		}
	} catch(Poco::SyntaxException& ){
		string msg = Poco::format("�ֶ� [" + _field + ":"+ toBeValidate + "] "+"������ASCII�ַ�:",toBeValidate);
		throw Poco::InvalidArgumentException( msg);
	}
}

VarLengthValidator::VarLengthValidator(const string& field,const int& length)
:Validator(field),_length(length)
{
}

void VarLengthValidator::validate(const std::string &toBeValidate)
	throw (Poco::InvalidArgumentException)
{
	if(_length < toBeValidate.size())
		throw Poco::InvalidArgumentException(
			Poco::format("�ֶ� [" + _field + ":"+ toBeValidate + "] "+"��ʵ����Ϊ %d�� ��Ч����Ϊ 0-%d" ,(int)toBeValidate.size(), _length),"B000P00308",0x00000008);
}

RangeValidator::RangeValidator(const string& field, const int& min, const int& max)
:Validator(field),_max(max),_min(min)
{
	
}

void RangeValidator::validate(const int& toBeValidate) 
	throw (Poco::InvalidArgumentException)
{
	if(toBeValidate > _max || toBeValidate < _min)
		throw Poco::InvalidArgumentException( "�ֶ� [" + _field + ":"+ Poco::NumberFormatter::format(toBeValidate) + "] "+"������Χ : " + Poco::format("%d",toBeValidate) + 
		". ��Ч��ΧΪ" + Poco::format("%d-%d", _min, _max));
}