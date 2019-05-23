/***************************************************************
 * DBConnectInfo.h
 * 系统名  : 城市一卡通系统
 * 子系统名: 结算子系统(B)_通用库
 * 更改日期      姓名           摘要
 * ----------    -----------    --------------------------------
 * 2007/12/10    王井锋			初次开发
 ***************************************************************
 */
#ifndef DBCONNECTINFO_H_
#define DBCONNECTINFO_H_

#include <string>

using std::string;

/**
 * 数据库连接信息
 * @author  王井锋
 * @version V2.0.00
 * @since V2.0.00
 */
class DBConnectInfo {
public:
	
	/**
	 * 构造函数
	 */
	DBConnectInfo();

	/**
	 * 构造函数
	 * @param dbName	数据库名
	 * @param userName	用户名
	 * @param password	密码
	 */
	DBConnectInfo(const string& dbName, const string& userName, const string& password);
	
	/**
	 * 获取数据库名
	 * @return 数据库名
	 */
	const string& getDBName() const;

	/**
	 * 获取用户名
	 * @return 用户名
	 */
	const string& getUserName() const;
	
	/**
	 * 获取密码
	 * @return 密码
	 */
	const string& getPassword() const;
	
	/**
	 * 设置值
	 * @param 数据库名
	 * @param 用户名
	 * @param 密码
	 */
	void set(const string& dbName, const string& userName, const string& password);

private:
	/**
	 * 数据库名
	 */
	string _dbName;

	/**
	 * 用户名
	 */
	string _userName;

	/**
	 * 密码
	 */
	string _password;
};

#endif /*DBCONNECTINFO_H_*/
