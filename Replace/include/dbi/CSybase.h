#if !defined(DBI_5_0_BIND_DEFINE_VAR_SYBASE)
#define DBI_5_0_BIND_DEFINE_VAR_SYBASE
#include "dbi/dbi5.h"

class SybException: public DbException
{
public:
    SybException(int lErrCode, const std::string& strErrMsg);
    ~SybException();

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

struct _csconnection;

class SybSession: public DbSession
{
public:

    SybSession();
    ~SybSession();

    // �������ݿ�
    void Connect(const std::string& strUserName,
        const std::string& strPass,
        const std::string& strServer);// throw (CDBIException);

    // �ر����ݿ�
    void Close();

    // ���ݿ��Ƿ��Ѿ�������
    bool IsConnected()const;

    // �����������ݿ�
    void ReConnect();// throw (CDBIException);

    virtual int Copyin(const std::string& tableName, 
        const std::string& file);

	virtual int Copyout(const std::string& sql,
		const std::string& file);

    // ��ʼ����
    void Start() ;//throw (CDBIException);

    // �ύ����
    void Commit();// throw (CDBIException);

    // �ع�����
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

    // ����sql���
    // oracle��ռλ��ʹ��:1,:2
    // sybase��һ��ʹ��?��Ϊռλ��
    void operator = (const std::string& str);// throw (CDBIException);

    // ִ��sql���, �������Ҫ�󶨵ı�����֮ǰ����ʹ��Bind���а�
    // ����������֮ǰ����ʹ��Define���������
    int Execute(bool bOver = false);// throw (CDBIException);

    // ���и�����м����ص�ʱ�򣬿���ѭ�����ô˺������������
    int Fetch(bool bOver = false);// throw (CDBIException);

    // Define�������
    // Bind ������
    bool IsNull(int i) const; // isNull��ִ��֮�����
    void SetNull(int i);      // setNull��ִ��֮ǰ��֮������

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

// �洢�����ֻ֧࣬�ֲ������м��Ĵ洢����
class SybProcedure : public DbProcedure
{
public:
    SybProcedure(const DbSession& sess);
    ~SybProcedure();

    // ���ô洢���̵�����
    void SetProName(const std::string& str);// throw (CDBIException);

    // ��λ���趨�洢���̵Ĳ���
    void SetParamIn(const std::string& str);// throw (CDBIException);
    void SetParamIn(const int& str);// throw (CDBIException);
    void SetParamIn(const Poco::LocalDateTime& str);// throw (CDBIException);
    void SetParamOut(std::string& str, unsigned int maxLen = 0);//throw (CDBIException);
    void SetParamOut(int& str);//throw (CDBIException);
    void SetParamOut(Poco::LocalDateTime& str);//throw (CDBIException);

    // ִ�д洢����
    // ����:0 -- �洢ִ�гɹ�
    //       < 0 -- �洢����ִ��ʧ��
    // �쳣:ִ�д洢���̵�ʱ������쳣 
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

