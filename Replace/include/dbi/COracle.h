#if !defined(CORACLE_DBI5_HEADER)
#define CORACLE_DBI5_HEADER

#include "dbi/dbi5.h"

class OraException: public DbException
{
public:
    OraException(int lErrCode, const std::string& strErrMsg);
    ~OraException();

    // �Ƿ��������쳣
    bool IsConnectedErr()const;

    // �Ƿ��ǲ����¼ʱ�����ظ��쳣
    bool IsInsertErr()const;

    // �쳣��Ϣ��
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

    virtual int Copyin(const std::string& tableName, 
        const std::string& file);

	virtual int Copyout(const std::string& sql,
		const std::string& file);

	virtual DataTable* OraSession::ExecuteQuery(const std::string& sql,CDataRow& bind);
    // ��ʼ����
    void Start() ;//throw (CDBIException);

    // �ύ����
    void Commit();

    // �ع�����
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

    // ����sql���
    // oracle��ռλ��ʹ��:1,:2
    // sybase��һ��ʹ��?��Ϊռλ��
    void operator = (const std::string& str);

    // ִ��sql���, �������Ҫ�󶨵ı�����֮ǰ����ʹ��Bind���а�
    // ����������֮ǰ����ʹ��Define���������
    int Execute(bool bOver = false);

    // ���и�����м����ص�ʱ�򣬿���ѭ�����ô˺������������
    int Fetch(bool bOver = false);

    // Define�������
    // Bind ������
    bool IsNull(int i) const; // isNull��ִ��֮�����
    void SetNull(int i);      // setNull��ִ��֮ǰ��֮������

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

// �洢�����ֻ֧࣬�ֲ������м��Ĵ洢����
class OraProcedure : public DbProcedure
{
public:
    OraProcedure(const DbSession& sess);
    ~OraProcedure();

    // ���ô洢���̵�����
    void SetProName(const std::string& str);

    // ��λ���趨�洢���̵Ĳ���
    void SetParamIn(const std::string& str);
    void SetParamIn(const int& str);
    void SetParamIn(const Poco::LocalDateTime& str);
    void SetParamOut(std::string& str, unsigned int maxLen = 0);//throw (CDBIException);
    void SetParamOut(int& str);//throw (CDBIException);
    void SetParamOut(Poco::LocalDateTime& str);//throw (CDBIException);

	void SetParamIn(unsigned char** valuep,  size_t& len);
	void SetParamOut(unsigned char** valuep, size_t& len);

    // ִ�д洢����
    // ����:0 -- �洢ִ�гɹ�
    //       < 0 -- �洢����ִ��ʧ��
    // �쳣:ִ�д洢���̵�ʱ������쳣 
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


