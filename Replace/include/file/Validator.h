/***************************************************************
 * Validator.h
 * 系统名  : 城市一卡通系统
 * 子系统名: 接口子系统_校验
 * 更改日期      姓名           摘要
 * ----------    -----------    --------------------------------
 * 2011/12/02    王井锋			初次开发
 ***************************************************************
 */
#ifndef VALIDATOR_H_
#define VALIDATOR_H_

#include <vector>
#include <string>
#include "Poco/Exception.h"
#include "Poco/RefCountedObject.h"
#include "Poco/format.h"

using std::vector;
using std::string;

/**
 * 校验器
 * @author  王井锋
 * @version V1.0.00
 * @since V1.0.00
 */
template<class T>
class Validator:public Poco::RefCountedObject {
public:
	/**
	 * 构造函数
	 */
	Validator(const string& field);

	/**
	 * 析构函数
	 */
	virtual ~Validator();
	
	/**
	 * 校验
	 * 纯虚函数
	 * @param	toBeValidate 待校验字符串
	 * @throws	Poco::InvalidArgumentException 参数非法异常
	 */
	virtual void validate(const T& toBeValidate) throw (Poco::InvalidArgumentException) =0;
protected:
	/**
	 * 域名
	 */
	string _field;
};

/**
 * 必须字段校验器
 * @author  王井锋
 * @version V1.0.00
 * @since V1.0.00
 */
class RequiredValidator: public Validator<std::string>{
public:
	RequiredValidator(const string& field):Validator(field){}
	/*
	 * 校验
	 * @see Validator::validate(const string&)
	 */
	virtual void validate(const std::string& toBeValidate) throw (Poco::InvalidArgumentException); 
};

/**
 * 长度校验器
 * @author  王井锋
 * @version V1.0.00
 * @since V1.0.00
 */
class LengthValidator: public Validator<std::string>{
public:
	/**
	 * 构造函数
	 * @field	域名
	 * @length	要求字段长度
	 */
	LengthValidator(const string& field,const int& length);
	/*
	 * 校验
	 * @see Validator::validate(const string&)
	 */
	virtual void validate(const std::string& toBeValidate) throw (Poco::InvalidArgumentException);
private:
	/*
	 *	长度
	 */
	int	_length;
};

/**
 * 日期时间校验器
 * @author  王井锋
 * @version V1.0.00
 * @since V1.0.00
 */
class DateTimeValidator: public Validator<std::string>{
public:
	DateTimeValidator(const string& field, const string& format = "%Y-%m-%d %H:%M:%S");
	/*
	 * 校验
	 * @see Validator::validate(const string&)
	 */
	virtual void validate(const std::string& toBeValidate) throw (Poco::InvalidArgumentException);
private:
	/*
	 *	时间格式 YYYY/mm/dd HHMMSS
	 */
	string _format;
};

/**
 * 整数校验器
 * @author  王井锋
 * @version V1.0.00
 * @since V1.0.00
 */
class IntegerValidator: public Validator<std::string>{
public:
	IntegerValidator(const string& field):Validator(field){}
	/*
	 * 校验
	 * @see Validator::validate(const string&)
	 */
	virtual void validate(const std::string& toBeValidate) throw (Poco::InvalidArgumentException);
};

/**
 * 16进制校验器
 * @author  王井锋
 * @version V1.0.00
 * @since V1.0.00
 */
class HexValidator: public Validator<std::string>{
public:
	HexValidator(const string& field):Validator(field){}
	/*
	 * 校验
	 * @see Validator::validate(const string&)
	 */
	virtual void validate(const std::string& toBeValidate) throw (Poco::InvalidArgumentException);
};

/**
 * ASCII字符校验器
 * @author  王井锋
 * @version V1.0.00
 * @since V1.0.00
 */
class AsciiValidator: public Validator<std::string>{
public:
	AsciiValidator(const string& field):Validator(field){}
	/*
	 * 校验
	 * @see Validator::validate(const string&)
	 */
	virtual void validate(const std::string& toBeValidate) throw (Poco::InvalidArgumentException);
};

/**
 * 变长长度校验器
 * @author  王井锋
 * @version V1.0.00
 * @since V1.0.00
 */
class VarLengthValidator: public Validator<std::string>{
public:
	/**
	 * 构造函数
	 * @length	要求字段长度
	 * @field	域名
	 */
	VarLengthValidator(const string& field,const int& length);
	/*
	 * 校验
	 * @see Validator::validate(const string&)
	 */
	virtual void validate(const std::string& toBeValidate) throw (Poco::InvalidArgumentException);
private:
	/*
	 *	长度
	 */
	int	_length;
};

/**
 * 范围校验器
 * @author  王井锋
 * @version V1.0.00
 * @since V1.0.00
 */
class RangeValidator: public Validator<int>{
public:
	/**
	 * 构造函数
	 * @length	要求字段长度
	 * @field	域名
	 */
	RangeValidator(const string& field, const int& min, const int& max);
	/*
	 * 校验
	 * @see Validator::validate(const string&)
	 */
	virtual void validate(const int& toBeValidate) throw (Poco::InvalidArgumentException);
private:
	/*
	 *	最大值
	 */
	int	_max;
	/*
	 *	最小值
	 */
	int	_min;
};

/**
 * 枚举校验器
 * @author  王井锋
 * @version V1.0.00
 * @since V1.0.00
 */
template<class T>
class EnumValidator: public Validator<T> {
public:
	/**
	 * 构造函数
	 * @length	要求字段长度
	 * @field	域名
	 */	
	EnumValidator(const string& field, T* enums, int size) ;

	/*
	 * 校验
	 * @see Validator::validate(const string&)
	 */
	virtual void validate(const T& toBeValidate) throw (Poco::InvalidArgumentException);
private:
	std::vector<T> _szEnums;
};

template<class T>
EnumValidator<T>::EnumValidator(const string& field, T* enums, int size)
:Validator(field)
{
	_szEnums.assign(enums, enums + size);
}

template<class T>
inline void EnumValidator<T>::validate(const T& toBeValidate) 
	throw (Poco::InvalidArgumentException)
{
	vector<T>::iterator it = _szEnums.begin();
	for(; it != _szEnums.end(); it ++)
	{
		if(	toBeValidate == *it)
			break;
	}

	if(it == _szEnums.end())
		throw Poco::InvalidArgumentException( "字段 [" + _field + ":"+ toBeValidate + "] " + "无效。该字段必须是" + Util::formatList(_szEnums) + "中某个值。");
}

#endif /*VALIDATOR_H_*/
