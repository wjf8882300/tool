#if !defined(DBI_5_0_BIND_DEFINE_VAR_SYBASE)
#define DBI_5_0_BIND_DEFINE_VAR_SYBASE
#include "dbi/dbi5.h"

class SybException: public DbException
{
public:
    SybException(int lErrCode, const std::string& strErrMsg);
    ~SybException();

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

struct _csconnection;

class SybSession: public DbSession
{
public:

    SybSession();
    ~SybSession();

    // 连接数据库
    void Connect(const std::string& strUserName,
        const std::string& strPass,
        const std::string& strServer);// throw (CDBIException);

    // 关闭数据库
    void Close();

    // 数据库是否已经连接上
    bool IsConnected()const;

    // 重新连接数据库
    void ReConnect();// throw (CDBIException);

    virtual int Copyin(const std::string& tableName, 
        const std::string& file);

	virtual int Copyout(const std::string& sql,
		const std::string& file);

    // 开始事务
    void Start() ;//throw (CDBIException);

    // 提交事务
    void Commit();// throw (CDBIException);

    // 回滚事务
    void Rollback();// throw (CDBIException);

    DbStatement* createStatement();
    DbProcedure* createProcedure();

private:
    SybSession(const SybSession& src);
    SybSession& operator = (const SybSession& src);
    struct SybSessionImpl* _impl;

    friend class SybTransaction;
    friend class SybStatement;
    friend struct SybTranImpl;
    friend struct SybStmtImpl;
    friend struct SybProImpl;
};


class SybStatement : public DbStatement
{
public:
    SybStatement(const DbSession& sess);
    ~SybStatement();

    // 设置sql语句
    // oracle中占位符使用:1,:2
    // sybase中一率使用?作为占位符
    void operator = (const std::string& str);// throw (CDBIException);

    // 执行sql语句, 如果有需要绑定的变量，之前必须使用Bind进行绑定
    // 如果有输出，之前必须使用Define进行输出绑定
    int Execute(bool bOver = false);// throw (CDBIException);

    // 当有更多的行集返回的时候，可以循环调用此函数获得输出结果
    int Fetch(bool bOver = false);// throw (CDBIException);

    // Define定义输出
    // Bind 绑定输入
    bool IsNull(int i) const; // isNull在执行之后调用
    void SetNull(int i);      // setNull在执行之前绑定之后设置

    void Define(int& valuep);// throw (CDBIException);
    void Bind(const int& valuep);// throw (CDBIException);
    void Define(long& valuep);// throw (CDBIException);
    void Bind(const long& valuep);// throw (CDBIException);

    void Define(std::string& buf);// throw (CDBIException);
    void Bind(const std::string& str, unsigned int maxLen = 0);// throw (CDBIException);
    void Bind(const std::string& str, BindOption opt);// throw (CDBIException);

    void Define(Poco::LocalDateTime& dt);// throw (CDBIException);
    void Bind(const Poco::LocalDateTime& dt) ;//throw (CDBIException);

	void Reset(){}

private:
    SybStatement(const SybStatement&);
    SybStatement& operator = (const SybStatement&);
    struct SybStmtImpl* _impl;
};

// 存储过程类，只支持不返回行集的存储过程
class SybProcedure : public DbProcedure
{
public:
    SybProcedure(const DbSession& sess);
    ~SybProcedure();

    // 设置存储过程的名字
    void SetProName(const std::string& str);// throw (CDBIException);

    // 按位置设定存储过程的参数
    void SetParamIn(const std::string& str);// throw (CDBIException);
    void SetParamIn(const int& str);// throw (CDBIException);
    void SetParamIn(const Poco::LocalDateTime& str);// throw (CDBIException);
    void SetParamOut(std::string& str, unsigned int maxLen = 0);//throw (CDBIException);
    void SetParamOut(int& str);//throw (CDBIException);
    void SetParamOut(Poco::LocalDateTime& str);//throw (CDBIException);

    // 执行存储过程
    // 返回:0 -- 存储执行成功
    //       < 0 -- 存储过程执行失败
    // 异常:执行存储过程的时候产生异常 
    int Execute();// throw (CDBIException);

private:
    SybProcedure(const SybProcedure&);
    SybProcedure& operator = (const SybProcedure&);
    struct SybProImpl* _impl;
};

#if defined(_MSC_VER)
#if !defined(POCO_NO_AUTOMATIC_LIBS)
#pragma comment(lib, "libcs.lib")
#pragma comment(lib, "libct.lib")
#pragma comment(lib, "libblk.lib")
#endif
#endif

#endif //DBI_5_0_BIND_DEFINE_VAR_SYBASE

