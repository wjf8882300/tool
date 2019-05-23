/***************************************************************
 * DBConnectInfo.cpp
 * ϵͳ��  : ����һ��ͨϵͳ
 * ��ϵͳ��: ������ϵͳ(B)_ͨ�ÿ�
 * ��������      ����           ժҪ
 * ----------    -----------    --------------------------------
 * 2007/12/20    ������			���ο���
 ***************************************************************
 */
#include "dbi/DBConnectInfo.h"


DBConnectInfo::DBConnectInfo() {
}

DBConnectInfo::DBConnectInfo(const string& dbName, 
							 const string& userName, const string& password)
: _dbName(dbName), _userName(userName), _password(password) {
}

void DBConnectInfo::set(const string& dbName, 
						const string& userName, const string& password) {
	_dbName = dbName;
	_userName = userName;
	_password = password;
}

const string& DBConnectInfo::getDBName() const {
	return _dbName;
}

const string& DBConnectInfo::getUserName() const {
	return _userName;
}

const string& DBConnectInfo::getPassword() const {
	return _password;
}
