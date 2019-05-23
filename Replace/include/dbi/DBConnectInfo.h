/***************************************************************
 * DBConnectInfo.h
 * ϵͳ��  : ����һ��ͨϵͳ
 * ��ϵͳ��: ������ϵͳ(B)_ͨ�ÿ�
 * ��������      ����           ժҪ
 * ----------    -----------    --------------------------------
 * 2007/12/10    ������			���ο���
 ***************************************************************
 */
#ifndef DBCONNECTINFO_H_
#define DBCONNECTINFO_H_

#include <string>

using std::string;

/**
 * ���ݿ�������Ϣ
 * @author  ������
 * @version V2.0.00
 * @since V2.0.00
 */
class DBConnectInfo {
public:
	
	/**
	 * ���캯��
	 */
	DBConnectInfo();

	/**
	 * ���캯��
	 * @param dbName	���ݿ���
	 * @param userName	�û���
	 * @param password	����
	 */
	DBConnectInfo(const string& dbName, const string& userName, const string& password);
	
	/**
	 * ��ȡ���ݿ���
	 * @return ���ݿ���
	 */
	const string& getDBName() const;

	/**
	 * ��ȡ�û���
	 * @return �û���
	 */
	const string& getUserName() const;
	
	/**
	 * ��ȡ����
	 * @return ����
	 */
	const string& getPassword() const;
	
	/**
	 * ����ֵ
	 * @param ���ݿ���
	 * @param �û���
	 * @param ����
	 */
	void set(const string& dbName, const string& userName, const string& password);

private:
	/**
	 * ���ݿ���
	 */
	string _dbName;

	/**
	 * �û���
	 */
	string _userName;

	/**
	 * ����
	 */
	string _password;
};

#endif /*DBCONNECTINFO_H_*/
