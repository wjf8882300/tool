#if !defined(CORACLE_DBI5_HEADER)
#define CORACLE_DBI5_HEADER

#include "dbi/dbi5.h"

class OraException: public DbException
{
public:
    OraException(int lErrCode, const std::string& strErrMsg);
    ~OraException();

    // 是否是连接异常
    bool IsConnectedErr()const;

    // 是否是插入记录时主键重复异常
    bool IsInsertErr()const;

    // 异常消息体
    const std::string& ErrMsg()const;

private:
    int m_lErrCode;
    std::string m_str;
};

class OraSession: public DbSession
{
public:
    OraSession();
    ~OraSession();

    void SetAutoCommit(bool autoCommit);

    // 连接数据库
    void Connect(const std::string& strUserName,
        const std::string& strPass,
        const std::string& strServer);

    // 关闭数据库
    void Close();

    // 数据库是否已经连接上
    bool IsConnected()const;

    // 重新连接数据库
    void ReConnect();

    virtual int Copyin(const std::string& tableName, 
        const std::string& file);

	virtual int Copyout(const std::string& sql,
		const std::string& file);

	virtual DataTable* OraSession::ExecuteQuery(const std::string& sql,CDataRow& bind);
    // 开始事务
    void Start() ;//throw (CDBIException);

    // 提交事务
    void Commit();

    // 回滚事务
    void Rollback();

    DbStatement* createStatement();
    DbProcedure* createProcedure();

private:
    OraSession(const OraSession& src);
    OraSession& operator = (const OraSession& src);
    struct OraSessionImpl* _impl;

    friend class OraTransaction;
    friend class OraStatement;
    friend struct OraTranImpl;
    friend struct OraProImpl;
    friend struct OraStmtImpl;
};


class OraStatement : public DbStatement
{
public:
    OraStatement(const DbSession& sess);
    ~OraStatement();

    // 设置sql语句
    // oracle中占位符使用:1,:2
    // sybase中一率使用?作为占位符
    void operator = (const std::string& str);

    // 执行sql语句, 如果有需要绑定的变量，之前必须使用Bind进行绑定
    // 如果有输出，之前必须使用Define进行输出绑定
    int Execute(bool bOver = false);

    // 当有更多的行集返回的时候，可以循环调用此函数获得输出结果
    int Fetch(bool bOver = false);

    // Define定义输出
    // Bind 绑定输入
    bool IsNull(int i) const; // isNull在执行之后调用
    void SetNull(int i);      // setNull在执行之前绑定之后设置

    void Define(int& valuep);
    void Bind(const int& valuep);
    void Define(long& valuep);
    void Bind(const long& valuep);

    void Define(std::string& buf);
    void Bind(const std::string& str, unsigned int maxLen = 0);
    void Bind(const std::string& str, BindOption opt);

    void Define(Poco::LocalDateTime& dt);
    void Bind(const Poco::LocalDateTime& dt) ;

	void Reset();

	void Define(unsigned char** data, unsigned int & length);
	void Bind(unsigned char** data, unsigned int & length);

private:
    OraStatement(const OraStatement&);
    OraStatement& operator = (const OraStatement&);
    struct OraStmtImpl* _impl;

    friend class OraProcedure;
    friend struct OraProImpl;
    friend class OraSession;
};

// 存储过程类，只支持不返回行集的存储过程
class OraProcedure : public DbProcedure
{
public:
    OraProcedure(const DbSession& sess);
    ~OraProcedure();

    // 设置存储过程的名字
    void SetProName(const std::string& str);

    // 按位置设定存储过程的参数
    void SetParamIn(const std::string& str);
    void SetParamIn(const int& str);
    void SetParamIn(const Poco::LocalDateTime& str);
    void SetParamOut(std::string& str, unsigned int maxLen = 0);//throw (CDBIException);
    void SetParamOut(int& str);//throw (CDBIException);
    void SetParamOut(Poco::LocalDateTime& str);//throw (CDBIException);

	void SetParamIn(unsigned char** valuep,  size_t& len);
	void SetParamOut(unsigned char** valuep, size_t& len);

    // 执行存储过程
    // 返回:0 -- 存储执行成功
    //       < 0 -- 存储过程执行失败
    // 异常:执行存储过程的时候产生异常 
    int Execute();

private:
    OraProcedure(const OraProcedure&);
    OraProcedure& operator = (const OraProcedure&);
    struct OraProImpl* _impl;
};

#if defined(_MSC_VER)
#if !defined(POCO_NO_AUTOMATIC_LIBS)
#pragma comment(lib, "oci.lib")
#endif
#endif

#endif // CORACLE_DBI5_HEADER


