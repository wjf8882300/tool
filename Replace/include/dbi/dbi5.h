#if !defined(DBI_5_0_BIND_DEFINE_VAR_)
#define DBI_5_0_BIND_DEFINE_VAR_
#include <vector>
#include "Poco/LocalDateTime.h"
#include "Poco/RefCountedObject.h"
#include "Poco/AutoPtr.h"

class CSession;
class CStatement;
class CProdure;

class DbSession;
class DbStatement;
class DbProcedure;

class DbException
{
public:
	DbException();
	virtual ~DbException();

	// 是否是连接异常
	virtual bool IsConnectedErr()const = 0;

	// 是否是插入记录时主键重复异常
	virtual bool IsInsertErr()const = 0;

	// 异常消息体
    virtual const std::string& ErrMsg()const = 0;

};

struct DataColumn
{
	enum {
		DATATYPE_UNKNOWN = -1,
		DATATYPE_INT      = 0,
		DATATYPE_STRING   = 1,
		DATATYPE_DATETIME = 2
		
	};
    DataColumn()
    : ind(0)
    , value("")
	, type(1)
	, null(false)
    {
    }
	DataColumn(const std::string& value,const int& type)
	: ind(0)
	, null(false)
	, type(type)
	, value(value)
	{

	}
    ~DataColumn()
    { 
       
    }

	DataColumn& operator =(const DataColumn& dc)
	{
		this->value = dc.value;
		this->ind = dc.ind;
		this->null = dc.null;
		this->type = dc.type;

		return *this;
	}

	short		ind;
	//值
    std::string value;
	//类型 0：字符串 1：整型 2：日期
	int			type;
	//是否可以为0
    bool		null;
};

class DataRow: public Poco::RefCountedObject
{
public:
	DataRow()
	{}

	~DataRow()
	{
		for(std::vector<DataColumn*>::iterator iter = row.begin();iter!=row.end();iter++)
		{
			delete *iter;
		}
	}
public:
	void Add(DataColumn *dc)
	{
		row.push_back(dc);
	}

	DataColumn& operator [](const int& i)
	{
		if(row.size()<i)
			throw Poco::Exception("index of array outsize");
		return *row[i];
	}

	int Count()
	{
		return (int)row.size();
	}
private:
	std::vector<DataColumn*> row;
};


class CDataRow
{
public:
	CDataRow(Poco::AutoPtr<DataRow> dr):_dr(dr)
	{
	}
public:
	void Add(DataColumn* dc)
	{
		_dr->Add(dc);
	}

	DataColumn& operator [](const int& i)
	{
		return (*_dr)[i];
	}

	int Count()
	{
		return _dr->Count();
	}

public:	
	Poco::AutoPtr<DataRow> _dr;
};

class DataTable: public Poco::RefCountedObject
{
public:
	DataTable()
	{}

	~DataTable()
	{

	}
public:
	void Add(CDataRow& dr)
	{
		Rows.push_back(dr);
	}
public:	
	std::vector<CDataRow> Rows;
};


class CDataTable
{
public:
	CDataTable(Poco::AutoPtr<DataTable> dt):_dt(dt),Rows( dt->Rows)
	{
	}
public:

	void Add(CDataRow& dr)
	{
		_dt->Add(dr);
	}

public:	
	Poco::AutoPtr<DataTable> _dt;
	std::vector<CDataRow>& Rows;
};

template<class T>
class AutoReleasePointer {
public:
	 AutoReleasePointer (std::vector<T*>& v) :_v(v) {}
	~AutoReleasePointer() {
		std::vector<T*>::iterator it = _v.begin();
		for( ; it!=_v.end();it++) {
			delete *it;
		}
	}
private:
	std::vector<T*>& _v;
};

class DbSession: public Poco::RefCountedObject
{
public:
	DbSession();
	virtual ~DbSession();

    virtual void SetAutoCommit(bool autoCommit);

	// 连接数据库
	virtual void Connect(const std::string& strUserName,
			const std::string& strPass,
			const std::string& strServer) = 0;

	// 关闭数据库
	virtual void Close() = 0;

	// 数据库是否已经连接上
	virtual bool IsConnected()const = 0;
	
	// 重新连接数据库
	virtual void ReConnect() = 0;
	
    virtual int Copyin(const std::string& tableName, 
        const std::string& file) = 0;

	virtual int Copyout(const std::string& sql,
		const std::string& file) = 0;

	virtual DataTable* ExecuteQuery(const std::string& sql,CDataRow& bind) = 0;

    // 开始事务
    virtual void Start()= 0 ;

    // 提交事务
    virtual void Commit() = 0;

    // 回滚事务
    virtual void Rollback() = 0;

    virtual DbStatement* createStatement() = 0;
    virtual DbProcedure* createProcedure() = 0;
};


enum BindOption
{
    Sybase_Numeric
};

class DbStatement:public Poco::RefCountedObject
{
public: 
	DbStatement(const DbSession& sess);
	virtual ~DbStatement();
	
	// 设置sql语句
	// 一率使用?作为占位符
	virtual void operator = (const std::string& str) = 0;
	
	// 执行sql语句, 如果有需要绑定的变量，之前必须使用Bind进行绑定
	// 如果有输出，之前必须使用Define进行输出绑定
	virtual int Execute(bool bOver = false) = 0 ;
	
	// 当有更多的行集返回的时候，可以循环调用此函数获得输出结果
	virtual  int Fetch(bool bOver = false) = 0;
	
	virtual  bool IsNull(int i) const = 0; // isNull在执行之后调用
    virtual void SetNull(int i) = 0;      // setNull在执行之前绑定之后设置

	// Define定义输出
	// Bind 绑定输入
	virtual void Define(int& valuep) = 0;
	virtual void Bind(const int& valuep) = 0;
    virtual void Define(long& valuep) = 0;
    virtual void Bind(const long& valuep) = 0;

    virtual void Define(std::string& buf) = 0;
	virtual void Bind(const std::string& str, unsigned int maxLen = 0) = 0;
    virtual void Bind(const std::string& str, BindOption opt) = 0;

	virtual void Define(Poco::LocalDateTime& dt) = 0;
	virtual void Bind(const Poco::LocalDateTime& dt)  = 0;

	virtual void Reset() = 0;

	virtual void Define(unsigned char** valuep, size_t& len) = 0;
	virtual void Bind(unsigned char** valuep,  size_t& len) = 0;
};

// 存储过程类，只支持不返回行集的存储过程
class DbProcedure:public Poco::RefCountedObject
{
public:
	DbProcedure(const DbSession& sess);
	virtual ~DbProcedure();

	// 设置存储过程的名字
	virtual void SetProName(const std::string& str)=0;
	
	// 按位置设定存储过程的参数
	virtual void SetParamIn(const std::string& str) = 0;
	virtual void SetParamIn(const int& str) = 0;
	virtual void SetParamIn(const Poco::LocalDateTime& str) = 0;
	virtual void SetParamOut(std::string& str, unsigned int maxLen = 0) = 0;
	virtual void SetParamOut(int& str) = 0;
	virtual void SetParamOut(Poco::LocalDateTime& str) = 0;

	virtual void SetParamIn(unsigned char** valuep,  size_t& len) = 0;
	virtual void SetParamOut(unsigned char** valuep, size_t& len) = 0;

	// 执行存储过程
	// 返回:0 -- 存储执行成功
	//       < 0 -- 存储过程执行失败
	// 异常:执行存储过程的时候产生异常 
	virtual int Execute() = 0;
};


class CStatement
{
public:

    CStatement(Poco::AutoPtr<DbStatement> ptrImpl);
    /// Creates the Session.

    CStatement(CSession& sess);

    CStatement(const CStatement&);
    /// Creates a session by copying another one.

    CStatement& operator = (const CStatement&);
    /// Assignment operator.

    ~CStatement();

    DbStatement* impl();
    /// Returns a pointer to the underlying SessionImpl.

    void swap(CStatement& other);
    /// Swaps the session with another one.

    void operator = (const std::string& str);

    // 执行sql语句, 如果有需要绑定的变量，之前必须使用Bind进行绑定
    // 如果有输出，之前必须使用Define进行输出绑定
    int Execute(bool bOver = false) ;

    // 当有更多的行集返回的时候，可以循环调用此函数获得输出结果
    int Fetch(bool bOver = false);

    bool IsNull(int i) const; // isNull在执行之后调用
    void SetNull(int i) ;      // setNull在执行之前绑定之后设置

    // Define定义输出
    // Bind 绑定输入
    void Define(int& valuep);
    void Bind(const int& valuep);
    void Define(long& valuep);
    void Bind(const long& valuep);

    void Define(std::string& buf);
    void Bind(const std::string& str, unsigned int maxLen = 0);
    void Bind(const std::string& str, BindOption opt);

    void Define(Poco::LocalDateTime& dt);
    void Bind(const Poco::LocalDateTime& dt);

	void Reset();

	// data在内部分配空间，需在外部释放
	void Define(unsigned char** valuep, size_t& len);
	void Bind(unsigned char** valuep,  size_t& len);
private:
    CStatement();

    Poco::AutoPtr<DbStatement> _ptrImpl;
};


inline DbStatement* CStatement::impl()
{
    return _ptrImpl;
}

inline void swap(CStatement& s1, CStatement& s2)
{
    s1.swap(s2);
}

namespace std
{
    template<>
    inline void swap<CStatement>(CStatement& s1, 
        CStatement& s2)
        /// Full template specalization of std:::swap for Session
    {
        s1.swap(s2);
    }
}

inline void CStatement::operator = (const std::string& str)
{
    _ptrImpl->operator =(str);
}

// 执行sql语句, 如果有需要绑定的变量，之前必须使用Bind进行绑定
// 如果有输出，之前必须使用Define进行输出绑定
inline int CStatement::Execute(bool bOver) 
{
    return _ptrImpl->Execute(bOver);
}

// 当有更多的行集返回的时候，可以循环调用此函数获得输出结果
inline int CStatement::Fetch(bool bOver)
{
    return _ptrImpl->Fetch(bOver);
}

inline bool CStatement::IsNull(int i) const
{
    return _ptrImpl->IsNull(i);
}

inline void CStatement::SetNull(int i)
{
    return _ptrImpl->SetNull(i);
}

inline void CStatement::Define(int& valuep)
{
     _ptrImpl->Define(valuep);
}
inline void CStatement::Bind(const int& valuep)
{
     _ptrImpl->Bind(valuep);
}
inline void CStatement::Define(long& valuep)
{
     _ptrImpl->Define(valuep);
}
inline void CStatement::Bind(const long& valuep)
{
     _ptrImpl->Bind(valuep);
}

inline void CStatement::Define(std::string& buf)
{
     _ptrImpl->Define(buf);
}

inline void CStatement::Bind(const std::string& str, unsigned int maxLen)
{
     _ptrImpl->Bind(str, maxLen);
}
inline void CStatement::Bind(const std::string& str, BindOption opt)
{
     _ptrImpl->Bind(str, opt);
}

inline void CStatement::Define(Poco::LocalDateTime& dt)
{
     _ptrImpl->Define(dt);
}
inline void CStatement::Bind(const Poco::LocalDateTime& dt)
{
     _ptrImpl->Bind(dt);
}

inline void CStatement::Reset()
{
	_ptrImpl->Reset();
}

inline void CStatement::Define(unsigned char** valuep, size_t& len)
{
	_ptrImpl->Define(valuep, len);
}

inline void CStatement::Bind(unsigned char** valuep,  size_t& len)
{
	_ptrImpl->Bind(valuep, len);
}


class CProcedure
{
public:

    CProcedure(Poco::AutoPtr<DbProcedure> ptrImpl);
    /// Creates the Session.

    CProcedure(CSession& sess);

    CProcedure(const CProcedure&);
    /// Creates a session by copying another one.

    CProcedure& operator = (const CProcedure&);
    /// Assignment operator.

    ~CProcedure();

    DbProcedure* impl();
    /// Returns a pointer to the underlying SessionImpl.

    void swap(CProcedure& other);
    /// Swaps the session with another one.

    // 设置存储过程的名字
    void SetProName(const std::string& str);

    // 按位置设定存储过程的参数
    void SetParamIn(const std::string& str);
    void SetParamIn(const int& str);
    void SetParamIn(const Poco::LocalDateTime& str);
    void SetParamOut(std::string& str, unsigned int maxLen = 0);
    void SetParamOut(int& str);
    void SetParamOut(Poco::LocalDateTime& str);

	void SetParamIn(unsigned char** valuep,  size_t& len);
	void SetParamOut(unsigned char** valuep, size_t& len);

    // 执行存储过程
    // 返回:0 -- 存储执行成功
    //       < 0 -- 存储过程执行失败
    // 异常:执行存储过程的时候产生异常 
    int Execute();

private:
    CProcedure();

    Poco::AutoPtr<DbProcedure> _ptrImpl;
};

inline void CProcedure::SetProName(const std::string& str)
{
    _ptrImpl->SetProName(str);
}

inline void CProcedure::SetParamIn(const std::string& str)
{
    _ptrImpl->SetParamIn(str);
}
inline void CProcedure::SetParamIn(const int& str)
{
    _ptrImpl->SetParamIn(str);
}
inline void CProcedure::SetParamIn(const Poco::LocalDateTime& str)
{
    _ptrImpl->SetParamIn(str);
}

inline void CProcedure::SetParamOut(std::string& str, unsigned int maxLen)
{
    _ptrImpl->SetParamOut(str, maxLen);
}
inline void CProcedure::SetParamOut(int& str)
{
    _ptrImpl->SetParamOut(str);
}
inline void CProcedure::SetParamOut(Poco::LocalDateTime& str)
{
    _ptrImpl->SetParamOut(str);
}

inline void CProcedure::SetParamIn(unsigned char** valuep,  size_t& len)
{
    _ptrImpl->SetParamIn(valuep, len);
}

inline void CProcedure::SetParamOut(unsigned char** valuep, size_t& len)
{
    _ptrImpl->SetParamOut(valuep, len);
}

inline int CProcedure::Execute()
{
    return _ptrImpl->Execute();
}

inline DbProcedure* CProcedure::impl()
{
    return _ptrImpl;
}

inline void swap(CProcedure& s1, CProcedure& s2)
{
    s1.swap(s2);
}

namespace std
{
    template<>
    inline void swap<CProcedure>(CProcedure& s1, 
        CProcedure& s2)
        /// Full template specalization of std:::swap for Session
    {
        s1.swap(s2);
    }
}

class CSession
{
public:
    CSession(Poco::AutoPtr<DbSession> ptrImpl);
    /// Creates the Session.

    // CSession(const std::string& connector);

    CSession(const CSession&);
    /// Creates a session by copying another one.

    CSession& operator = (const CSession&);
    /// Assignment operator.

    ~CSession();

    void SetAutoCommit(bool autoCommit);

    /// Destroys the Session.
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

    int Copyin(const std::string& tableName, 
        const std::string& file);
 
	int Copyout(const std::string& sql,
		const std::string& file);

	DataTable* ExecuteQuery(const std::string& sql,CDataRow& bind);

    // 开始事务
    void Start();
    // 提交事务
    void Commit();
    // 回滚事务
    void Rollback();

    CStatement createStatement();
    CProcedure createProcedure();

    DbSession* impl();
    /// Returns a pointer to the underlying SessionImpl.

    void swap(CSession& other);
    /// Swaps the session with another one.

private:
    CSession();

    Poco::AutoPtr<DbSession> _ptrImpl;
};

inline void CSession::SetAutoCommit(bool autoCommit)
{
    _ptrImpl->SetAutoCommit(autoCommit);
}

inline void CSession::Connect(const std::string& strUserName,
                              const std::string& strPass,
                              const std::string& strServer)
{
    return _ptrImpl->Connect(strUserName, strPass, strServer);
}

inline void CSession::Close()
{
    return _ptrImpl->Close();
}

inline bool CSession::IsConnected()const
{
    return _ptrImpl->IsConnected();
}

inline void CSession::ReConnect()
{
    return _ptrImpl->ReConnect();
}

inline CStatement CSession::createStatement()
{
    return CStatement(_ptrImpl->createStatement());
}
inline CProcedure CSession::createProcedure()
{
    return CProcedure(_ptrImpl->createProcedure());
}


inline int CSession::Copyin(const std::string& tableName, 
                            const std::string& file)
{
    return _ptrImpl->Copyin(tableName, file);
}

inline int CSession::Copyout(const std::string& sql,
		const std::string& file)
{
    return _ptrImpl->Copyout(sql, file);
}

inline DataTable* CSession::ExecuteQuery(const std::string& sql,CDataRow& bind)
{
	return _ptrImpl->ExecuteQuery(sql,bind);
}

inline DbSession* CSession::impl()
{
    return _ptrImpl;
}
inline void CSession::Start()
{
    _ptrImpl->Start();
}
// 提交事务
inline void CSession::Commit()
{
    _ptrImpl->Commit(); 
}
// 回滚事务
inline void CSession::Rollback()
{
    _ptrImpl->Rollback();
}

inline void swap(CSession& s1, CSession& s2)
{
    s1.swap(s2);
}

namespace std
{
    template<>
    inline void swap<CSession>(CSession& s1, 
        CSession& s2)
        /// Full template specalization of std:::swap for Session
    {
        s1.swap(s2);
    }
}

#endif //DBI_5_0_BIND_DEFINE_VAR_

