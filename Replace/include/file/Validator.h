/***************************************************************
 * Validator.h
 * ϵͳ��  : ����һ��ͨϵͳ
 * ��ϵͳ��: �ӿ���ϵͳ_У��
 * ��������      ����           ժҪ
 * ----------    -----------    --------------------------------
 * 2011/12/02    ������			���ο���
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
 * У����
 * @author  ������
 * @version V1.0.00
 * @since V1.0.00
 */
template<class T>
class Validator:public Poco::RefCountedObject {
public:
	/**
	 * ���캯��
	 */
	Validator(const string& field);

	/**
	 * ��������
	 */
	virtual ~Validator();
	
	/**
	 * У��
	 * ���麯��
	 * @param	toBeValidate ��У���ַ���
	 * @throws	Poco::InvalidArgumentException �����Ƿ��쳣
	 */
	virtual void validate(const T& toBeValidate) throw (Poco::InvalidArgumentException) =0;
protected:
	/**
	 * ����
	 */
	string _field;
};

/**
 * �����ֶ�У����
 * @author  ������
 * @version V1.0.00
 * @since V1.0.00
 */
class RequiredValidator: public Validator<std::string>{
public:
	RequiredValidator(const string& field):Validator(field){}
	/*
	 * У��
	 * @see Validator::validate(const string&)
	 */
	virtual void validate(const std::string& toBeValidate) throw (Poco::InvalidArgumentException); 
};

/**
 * ����У����
 * @author  ������
 * @version V1.0.00
 * @since V1.0.00
 */
class LengthValidator: public Validator<std::string>{
public:
	/**
	 * ���캯��
	 * @field	����
	 * @length	Ҫ���ֶγ���
	 */
	LengthValidator(const string& field,const int& length);
	/*
	 * У��
	 * @see Validator::validate(const string&)
	 */
	virtual void validate(const std::string& toBeValidate) throw (Poco::InvalidArgumentException);
private:
	/*
	 *	����
	 */
	int	_length;
};

/**
 * ����ʱ��У����
 * @author  ������
 * @version V1.0.00
 * @since V1.0.00
 */
class DateTimeValidator: public Validator<std::string>{
public:
	DateTimeValidator(const string& field, const string& format = "%Y-%m-%d %H:%M:%S");
	/*
	 * У��
	 * @see Validator::validate(const string&)
	 */
	virtual void validate(const std::string& toBeValidate) throw (Poco::InvalidArgumentException);
private:
	/*
	 *	ʱ���ʽ YYYY/mm/dd HHMMSS
	 */
	string _format;
};

/**
 * ����У����
 * @author  ������
 * @version V1.0.00
 * @since V1.0.00
 */
class IntegerValidator: public Validator<std::string>{
public:
	IntegerValidator(const string& field):Validator(field){}
	/*
	 * У��
	 * @see Validator::validate(const string&)
	 */
	virtual void validate(const std::string& toBeValidate) throw (Poco::InvalidArgumentException);
};

/**
 * 16����У����
 * @author  ������
 * @version V1.0.00
 * @since V1.0.00
 */
class HexValidator: public Validator<std::string>{
public:
	HexValidator(const string& field):Validator(field){}
	/*
	 * У��
	 * @see Validator::validate(const string&)
	 */
	virtual void validate(const std::string& toBeValidate) throw (Poco::InvalidArgumentException);
};

/**
 * ASCII�ַ�У����
 * @author  ������
 * @version V1.0.00
 * @since V1.0.00
 */
class AsciiValidator: public Validator<std::string>{
public:
	AsciiValidator(const string& field):Validator(field){}
	/*
	 * У��
	 * @see Validator::validate(const string&)
	 */
	virtual void validate(const std::string& toBeValidate) throw (Poco::InvalidArgumentException);
};

/**
 * �䳤����У����
 * @author  ������
 * @version V1.0.00
 * @since V1.0.00
 */
class VarLengthValidator: public Validator<std::string>{
public:
	/**
	 * ���캯��
	 * @length	Ҫ���ֶγ���
	 * @field	����
	 */
	VarLengthValidator(const string& field,const int& length);
	/*
	 * У��
	 * @see Validator::validate(const string&)
	 */
	virtual void validate(const std::string& toBeValidate) throw (Poco::InvalidArgumentException);
private:
	/*
	 *	����
	 */
	int	_length;
};

/**
 * ��ΧУ����
 * @author  ������
 * @version V1.0.00
 * @since V1.0.00
 */
class RangeValidator: public Validator<int>{
public:
	/**
	 * ���캯��
	 * @length	Ҫ���ֶγ���
	 * @field	����
	 */
	RangeValidator(const string& field, const int& min, const int& max);
	/*
	 * У��
	 * @see Validator::validate(const string&)
	 */
	virtual void validate(const int& toBeValidate) throw (Poco::InvalidArgumentException);
private:
	/*
	 *	���ֵ
	 */
	int	_max;
	/*
	 *	��Сֵ
	 */
	int	_min;
};

/**
 * ö��У����
 * @author  ������
 * @version V1.0.00
 * @since V1.0.00
 */
template<class T>
class EnumValidator: public Validator<T> {
public:
	/**
	 * ���캯��
	 * @length	Ҫ���ֶγ���
	 * @field	����
	 */	
	EnumValidator(const string& field, T* enums, int size) ;

	/*
	 * У��
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
		throw Poco::InvalidArgumentException( "�ֶ� [" + _field + ":"+ toBeValidate + "] " + "��Ч�����ֶα�����" + Util::formatList(_szEnums) + "��ĳ��ֵ��");
}

#endif /*VALIDATOR_H_*/
