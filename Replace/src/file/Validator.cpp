/***************************************************************
 * Validator.cpp
 * 系统名  : 城市一卡通系统
 * 子系统名: 接口子系统_校验
 * 更改日期      姓名           摘要
 * ----------    -----------    --------------------------------
 * 2011/12/02    王井锋			初次开发
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
		"字段 [" + _field + ":"+ toBeValidate + "] "+ "是必须项。");
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
		Poco::format("字段 [" + _field + ":"+ toBeValidate + "] "+"真实长度为 %d。 有效长度为 %d" ,(int)toBeValidate.size(), _length));
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
			"字段 [" + _field + ":"+ toBeValidate + "] "+"格式错误: " + toBeValidate + 
			". 有效格式为 " + _format);
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
		string msg = Poco::format("字段 [" + _field + ":"+ toBeValidate + "] "+"包含非数字:%s",toBeValidate);
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
		string msg = Poco::format("字段 [" + _field + ":"+ toBeValidate + "] "+"包含非十六进制数字:",toBeValidate);
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
		string msg = Poco::format("字段 [" + _field + ":"+ toBeValidate + "] "+"包含非ASCII字符:",toBeValidate);
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
			Poco::format("字段 [" + _field + ":"+ toBeValidate + "] "+"真实长度为 %d。 有效长度为 0-%d" ,(int)toBeValidate.size(), _length),"B000P00308",0x00000008);
}

RangeValidator::RangeValidator(const string& field, const int& min, const int& max)
:Validator(field),_max(max),_min(min)
{
	
}

void RangeValidator::validate(const int& toBeValidate) 
	throw (Poco::InvalidArgumentException)
{
	if(toBeValidate > _max || toBeValidate < _min)
		throw Poco::InvalidArgumentException( "字段 [" + _field + ":"+ Poco::NumberFormatter::format(toBeValidate) + "] "+"超出范围 : " + Poco::format("%d",toBeValidate) + 
		". 有效范围为" + Poco::format("%d-%d", _min, _max));
}