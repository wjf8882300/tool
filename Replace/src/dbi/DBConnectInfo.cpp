/***************************************************************
 * DBConnectInfo.cpp
 * 系统名  : 城市一卡通系统
 * 子系统名: 结算子系统(B)_通用库
 * 更改日期      姓名           摘要
 * ----------    -----------    --------------------------------
 * 2007/12/20    王井锋			初次开发
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
