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

	// �Ƿ��������쳣
	virtual bool IsConnectedErr()const = 0;

	// �Ƿ��ǲ����¼ʱ�����ظ��쳣
	virtual bool IsInsertErr()const = 0;

	// �쳣��Ϣ��
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
	//ֵ
    std::string value;
	//���� 0���ַ��� 1������ 2������
	int			type;
	//�Ƿ����Ϊ0
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

	// �������ݿ�
	virtual void Connect(const std::string& strUserName,
			const std::string& strPass,
			const std::string& strServer) = 0;

	// �ر����ݿ�
	virtual void Close() = 0;

	// ���ݿ��Ƿ��Ѿ�������
	virtual bool IsConnected()const = 0;
	
	// �����������ݿ�
	virtual void ReConnect() = 0;
	
    virtual int Copyin(const std::string& tableName, 
        const std::string& file) = 0;

	virtual int Copyout(const std::string& sql,
		const std::string& file) = 0;

	virtual DataTable* ExecuteQuery(const std::string& sql,CDataRow& bind) = 0;

    // ��ʼ����
    virtual void Start()= 0 ;

    // �ύ����
    virtual void Commit() = 0;

    // �ع�����
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
	
	// ����sql���
	// һ��ʹ��?��Ϊռλ��
	virtual void operator = (const std::string& str) = 0;
	
	// ִ��sql���, �������Ҫ�󶨵ı�����֮ǰ����ʹ��Bind���а�
	// ����������֮ǰ����ʹ��Define���������
	virtual int Execute(bool bOver = false) = 0 ;
	
	// ���и�����м����ص�ʱ�򣬿���ѭ�����ô˺������������
	virtual  int Fetch(bool bOver = false) = 0;
	
	virtual  bool IsNull(int i) const = 0; // isNull��ִ��֮�����
    virtual void SetNull(int i) = 0;      // setNull��ִ��֮ǰ��֮������

	// Define�������
	// Bind ������
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

// �洢�����ֻ֧࣬�ֲ������м��Ĵ洢����
class DbProcedure:public Poco::RefCountedObject
{
public:
	DbProcedure(const DbSession& sess);
	virtual ~DbProcedure();

	// ���ô洢���̵�����
	virtual void SetProName(const std::string& str)=0;
	
	// ��λ���趨�洢���̵Ĳ���
	virtual void SetParamIn(const std::string& str) = 0;
	virtual void SetParamIn(const int& str) = 0;
	virtual void SetParamIn(const Poco::LocalDateTime& str) = 0;
	virtual void SetParamOut(std::string& str, unsigned int maxLen = 0) = 0;
	virtual void SetParamOut(int& str) = 0;
	virtual void SetParamOut(Poco::LocalDateTime& str) = 0;

	virtual void SetParamIn(unsigned char** valuep,  size_t& len) = 0;
	virtual void SetParamOut(unsigned char** valuep, size_t& len) = 0;

	// ִ�д洢����
	// ����:0 -- �洢ִ�гɹ�
	//       < 0 -- �洢����ִ��ʧ��
	// �쳣:ִ�д洢���̵�ʱ������쳣 
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

    // ִ��sql���, �������Ҫ�󶨵ı�����֮ǰ����ʹ��Bind���а�
    // ����������֮ǰ����ʹ��Define���������
    int Execute(bool bOver = false) ;

    // ���и�����м����ص�ʱ�򣬿���ѭ�����ô˺������������
    int Fetch(bool bOver = false);

    bool IsNull(int i) const; // isNull��ִ��֮�����
    void SetNull(int i) ;      // setNull��ִ��֮ǰ��֮������

    // Define�������
    // Bind ������
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

	// data���ڲ�����ռ䣬�����ⲿ�ͷ�
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

// ִ��sql���, �������Ҫ�󶨵ı�����֮ǰ����ʹ��Bind���а�
// ����������֮ǰ����ʹ��Define���������
inline int CStatement::Execute(bool bOver) 
{
    return _ptrImpl->Execute(bOver);
}

// ���и�����м����ص�ʱ�򣬿���ѭ�����ô˺������������
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

    // ���ô洢���̵�����
    void SetProName(const std::string& str);

    // ��λ���趨�洢���̵Ĳ���
    void SetParamIn(const std::string& str);
    void SetParamIn(const int& str);
    void SetParamIn(const Poco::LocalDateTime& str);
    void SetParamOut(std::string& str, unsigned int maxLen = 0);
    void SetParamOut(int& str);
    void SetParamOut(Poco::LocalDateTime& str);

	void SetParamIn(unsigned char** valuep,  size_t& len);
	void SetParamOut(unsigned char** valuep, size_t& len);

    // ִ�д洢����
    // ����:0 -- �洢ִ�гɹ�
    //       < 0 -- �洢����ִ��ʧ��
    // �쳣:ִ�д洢���̵�ʱ������쳣 
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
    // �������ݿ�
    void Connect(const std::string& strUserName,
        const std::string& strPass,
        const std::string& strServer);

    // �ر����ݿ�
    void Close();

    // ���ݿ��Ƿ��Ѿ�������
    bool IsConnected()const;

    // �����������ݿ�
    void ReConnect();

    int Copyin(const std::string& tableName, 
        const std::string& file);
 
	int Copyout(const std::string& sql,
		const std::string& file);

	DataTable* ExecuteQuery(const std::string& sql,CDataRow& bind);

    // ��ʼ����
    void Start();
    // �ύ����
    void Commit();
    // �ع�����
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
// �ύ����
inline void CSession::Commit()
{
    _ptrImpl->Commit(); 
}
// �ع�����
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

