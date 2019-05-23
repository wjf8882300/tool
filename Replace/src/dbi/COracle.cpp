#include "dbi/COracle.h"
#include "Poco/String.h"
#include <stdio.h>
#include "oci.h"
#include <deque>
#include <sstream>
#include <iostream>
#include "Poco/Format.h"
#include "Poco/FileStream.h"
#include "Poco/NumberParser.h"
#include "Poco/DateTimeParser.h"

#define MAXLOBLEN 102400

void split(std::vector<std::string>& container, const std::string& source, const std::string& sep)
{
	container.clear();

	std::string::size_type lastPos = 0;
	std::string::size_type currPos = 0;

	currPos = source.find_first_of(sep);

	while(currPos != std::string::npos)
	{
		container.push_back(source.substr(lastPos, currPos - lastPos));
		lastPos = currPos + 1;
		currPos = source.find_first_of(sep, lastPos);
	}

	if (lastPos < source.length())
	{
		container.push_back(source.substr(lastPos));
	}
}

OraException::OraException(int lErrCode, const std::string& strErrMsg)
:m_lErrCode(lErrCode), m_str(strErrMsg)
{
}

OraException::~OraException()
{
}

// 是不是连接错误
bool OraException::IsConnectedErr()const 
{
	switch(m_lErrCode) {
	case 1017:
	case 12154:
	case 24309:
	case 1034:
	case 27101:
	case 1033:
	case 1089:
	case 1092:
	case 3113: // ORA-03113: end-of-file on communication channel
	case 3114:
	case 24324:
	case 12541:
	case 12170: // ORA-12170: TNS:Connect timeout occurred
	case 12560: // ORA-12560: TNS:protocol adapter error
	case 257:// ORA-00257: archiver error. Connect internal only, until freed.
		return true;
	default:
		break;
	}
	return false;
}

// 是否查表错误
bool OraException::IsInsertErr()const 
{
	return m_lErrCode == 1;
}

// 返回错误原因	
const std::string& OraException::ErrMsg()const
{
	return m_str;
}


// CHECK macro，用来检查每次oci调用是否成功，如果失败，抛出异常

#define CHECK(errhp,Func)\
{\
	text errbuf[512];\
	errbuf[0] = 0;\
	sb4 errcode;\
	sword sres = (Func);\
	if (sres != OCI_SUCCESS)\
{\
	OCIErrorGet ((dvoid *) errhp, (ub4) 1, (text *) NULL, &errcode,\
	errbuf, (ub4) sizeof(errbuf), (ub4) OCI_HTYPE_ERROR);\
	throw OraException((int)errcode, (const char*)errbuf);\
}\
}



struct OraSessionImpl {
	OraSessionImpl();
	~OraSessionImpl();
	OCISession* authp; 	// oci会话句柄
	OCIServer* srvhp;  	// oci服务句柄
	OCISvcCtx* svchp;	// oci上下文句柄
	OCIEnv* envhp;		// oci环境句柄
	OCIError* errhp;	// oci错误句柄
	std::string m_strUser; // 用户名
	std::string m_strPass;	// 用户密码
	std::string m_strServ;	// 服务名
	bool m_bConnected;	// 是否已经连接上服务器
	bool _autoCommit;
	bool _autoCommitForTran; // 事物开始时，设置autoCommit为false，提交/回滚后恢复

	void log_on(const std::string& username, const std::string& password);
	void init_handles(int init_mode);
	void free_handles();
	void attach_server(const std::string& server);
	void close();
	void Start();
	void Rollback();
	void Commit();

};

OraSessionImpl::OraSessionImpl()
:authp(0)
,srvhp(0)
,svchp(0)
,envhp(0)
,errhp(0)
,m_bConnected(false)
, _autoCommit(false)
{
	init_handles(OCI_THREADED);
}

OraSessionImpl::~OraSessionImpl()
{
	close();
	free_handles();
}

OraSession::OraSession()
:_impl(new OraSessionImpl)
{
}

OraSession::~OraSession()
{
	delete _impl;
}

bool OraSession::IsConnected()const
{
	return _impl->m_bConnected;
}

// 连接数据库
void OraSession::Connect(const std::string& strUserName,
						 const std::string& strPass,
						 const std::string& strServer)
{
	// 连接服务器
	_impl->attach_server(strServer);
	// 登录
	_impl->log_on(strUserName, strPass);
}

// 重新连接数据库
void OraSession::ReConnect()
{
	//	if( !m_impl->m_bConnected ) {
	_impl->close();
	_impl->attach_server(_impl->m_strServ);
	_impl->log_on(_impl->m_strUser,_impl->m_strPass);
	//	}
}

void OraSession::Close()
{
	_impl->close();
}


#define CHECK(errhp,Func)\
{\
	text errbuf[512];\
	errbuf[0] = 0;\
	sb4 errcode;\
	sword sres = (Func);\
	if (sres != OCI_SUCCESS)\
{\
	OCIErrorGet ((dvoid *) errhp, (ub4) 1, (text *) NULL, &errcode,\
	errbuf, (ub4) sizeof(errbuf), (ub4) OCI_HTYPE_ERROR);\
	throw OraException((int)errcode, (const char*)errbuf);\
}\
}


void OraSessionImpl::init_handles(int init_mode)
{
	// 初始化OCI过程 
	CHECK(errhp,OCIInitialize((ub4)init_mode, 0, 0, 0, 0 ));

	/* Inititialize the OCI Environment */
	CHECK(errhp,OCIEnvInit((OCIEnv **) &envhp, (ub4) OCI_DEFAULT,
		(size_t) 0, (dvoid **) 0 ));

	/* Allocate an error handle */
	CHECK(errhp,OCIHandleAlloc((dvoid *) envhp, (dvoid **) &errhp,
		(ub4) OCI_HTYPE_ERROR, (size_t) 0, (dvoid **) 0));

	/* Allocate a server handle */
	CHECK(errhp,OCIHandleAlloc((dvoid *) envhp, (dvoid **) &srvhp,
		(ub4) OCI_HTYPE_SERVER, (size_t) 0, (dvoid **) 0));

	/* Allocate a service handle */
	CHECK(errhp,OCIHandleAlloc((dvoid *) envhp, (dvoid **) &svchp,
		(ub4) OCI_HTYPE_SVCCTX, (size_t) 0, (dvoid **) 0));


	/* Allocate a authentication handle */
	CHECK(errhp,OCIHandleAlloc((dvoid *) envhp, (dvoid **) &authp,
		(ub4) OCI_HTYPE_SESSION, (size_t) 0, (dvoid **) 0));

}

void OraSessionImpl::attach_server(const std::string& server)
{
	m_strServ = server;

	CHECK(errhp,OCIServerAttach(srvhp, errhp, (unsigned char *) server.c_str(),
		(sb4) server.length(), (ub4) OCI_DEFAULT));


	/* Set the server handle in the service handle */
	CHECK(errhp,OCIAttrSet((dvoid *) svchp, (ub4) OCI_HTYPE_SVCCTX,
		(dvoid *) srvhp, (ub4) 0, (ub4) OCI_ATTR_SERVER, errhp));

}


void OraSessionImpl::log_on(const std::string& username, const std::string& password)
{
	m_strUser = username;
	m_strPass = password;  

	/* Set attributes in the authentication handle */
	CHECK(errhp,OCIAttrSet((dvoid *) authp, (ub4) OCI_HTYPE_SESSION,
		(dvoid *) username.c_str(), (ub4) username.length(),
		(ub4) OCI_ATTR_USERNAME, errhp));
	CHECK(errhp,OCIAttrSet((dvoid *) authp, (ub4) OCI_HTYPE_SESSION,
		(dvoid *) password.c_str(), (ub4) password.length(),
		(ub4) OCI_ATTR_PASSWORD, errhp));


	CHECK(errhp,OCISessionBegin(svchp, errhp, authp, OCI_CRED_RDBMS, OCI_DEFAULT));

	/* Set the authentication handle in the Service handle */
	CHECK(errhp,OCIAttrSet((dvoid *) svchp, (ub4) OCI_HTYPE_SVCCTX,
		(dvoid *) authp, (ub4) 0, (ub4) OCI_ATTR_SESSION, errhp));

	m_bConnected = true;
}

void OraSessionImpl::close()
{
	// 退出登录
	if (m_bConnected) {
		m_bConnected = false;
		CHECK(errhp,OCISessionEnd(svchp, errhp, authp, OCI_DEFAULT));
		CHECK(errhp,OCIServerDetach ( srvhp, errhp, OCI_DEFAULT)); 
	}
}

void OraSessionImpl::free_handles()
{
	if(srvhp){
		OCIHandleFree((dvoid *)srvhp,(ub4)OCI_HTYPE_SERVER);
		srvhp = 0;
	}
	if(svchp) {
		OCIHandleFree((dvoid *)svchp,(ub4)OCI_HTYPE_SVCCTX);
		svchp = 0;
	}
	if(authp) {
		OCIHandleFree((dvoid *)authp,(ub4)OCI_HTYPE_SESSION);
		authp = 0;
	}
	if(errhp) {
		OCIHandleFree((dvoid *)errhp,(ub4)OCI_HTYPE_ERROR);
		errhp = 0;
	}
	if(envhp) {
		OCIHandleFree((dvoid *)envhp,(ub4)OCI_HTYPE_ENV);
		envhp = 0;
	}
}

// 开始事务
void OraSession::Start()
{
	_impl->Start();
}
void OraSessionImpl::Start()
{
	_autoCommitForTran = _autoCommit;
	_autoCommit = false;
}

// 提交事务
void OraSession::Commit()
{
	_impl->Commit();
}

void OraSessionImpl::Commit()
{
	_autoCommit = _autoCommitForTran;

	CHECK(errhp, OCITransCommit(svchp, errhp, (ub4)0));
}

// 回滚事务
void OraSession::Rollback()
{
	_impl->Rollback();
}

void OraSessionImpl::Rollback()
{
	_autoCommit = _autoCommitForTran;

	CHECK(errhp,OCITransRollback(svchp, errhp, (ub4)0));
}

DbStatement* OraSession::createStatement()
{
	return new OraStatement(*this);
}

DbProcedure* OraSession::createProcedure()
{
	return new OraProcedure(*this);
}

struct DelayBindData
{
	ub4          position;
	dvoid        *valuep;
	sb4          value_sz;
	ub2          dty;
	dvoid        *indp;
};
struct ParamData
{
	enum Dir
	{
		DirDefine,
		DirBind,
	};

	enum Type
	{
		TypeString,
		TypeDate,
		TypeInt,
		TypeLong,
		TypeBlob,
	};

	enum InOut
	{
		IO_IN,
		IO_OUT,
	};

	short _pos;
	short _dir; 
	short _type;
	short _io;
	char  _buf[256];
	void* _ref; // string*
	ub4 * _len;
	OCILobLocator* _lobl;

	ParamData():_io(IO_IN),_lobl(NULL){}
};

struct OraColumnInfo
{
	int	_type;
	int	_maxlen;
	bool _null;
};

struct OraColData
{
	OraColData()
		: ind(0)
		, val(0)
		, vallen(0)
	{
	}
	~OraColData()
	{ 
		if (val != 0)
		{ 
			delete []val; 
			val = 0;
		}
	}
	short		ind;
	char*		val;
	unsigned short	vallen;
};

struct OraStmtImpl
{
	OraStmtImpl(const DbSession& Sess);
	~OraStmtImpl();
	struct OCIStmt* stmthp;
	OraSessionImpl* _sessImpl;
	const OraSession& _sess;
	std::string sqlstmt;
	bool plsqlBlock; // 是否是pl sql块
	bool _delayBind;  // 是否推迟绑定
	std::deque<DelayBindData> _delayBinds;
	struct OCIDefine* m_pDefine;
	struct OCIBind* m_pBind;
	unsigned int m_RowsFetched;
	unsigned int m_TotalRowsFetched;

	std::deque<short> _definenull, _bindnull;
	std::deque<ParamData> _params;

	void prepareData(); // 执行之前特殊数据到buf的转换
	void postData();    // 执行之后buf到特殊数据（string等）的转换

	void delayBind();

	void setSql(const std::string& sql);
	int Execute(bool bOver);
	int Fetch(bool bOver);

	void Define(int& valuep);
	void Bind(const int& valuep);
	void Define(long& valuep);
	void Bind(const long& valuep);

	void Define(std::string& buf);
	void Bind(const std::string& str, unsigned int maxLen = 0,ParamData::InOut io = ParamData::IO_IN);
	void Bind(const std::string& str, BindOption opt);

	void Define(Poco::LocalDateTime& dt);
	void Bind(const Poco::LocalDateTime& dt,ParamData::InOut io = ParamData::IO_IN) ;

	void clean_up();
	void clean_blob();

	int prepareColInfo(std::vector<OraColumnInfo>& colVec,
		std::vector<OraColData>& colData);

	void Bind(int pos, char* buf, int size, short* is_null);
	void Reset();

	void Define(void** valuep, size_t& len);
	void Bind(void** valuep,  size_t& len, ParamData::InOut io = ParamData::IO_IN);

};

void OraStmtImpl::Bind(int pos, char* buf, int size, short* is_null)
{
	CHECK(_sessImpl->errhp, OCIBindByPos(stmthp, &m_pBind, _sessImpl->errhp, (ub4)pos, 
		(dvoid*)buf, (sb4)size, (ub2)SQLT_STR, (is_null) ? is_null: 0,0,0,0,0, OCI_DEFAULT));
}

OraStmtImpl::OraStmtImpl(const DbSession& sess)
: stmthp(0)
, _delayBind(false)
, _sess(dynamic_cast<const OraSession&>(sess))
, m_pDefine(0)
, m_pBind(0)
, m_RowsFetched(0)
, m_TotalRowsFetched(0)
{
	_sessImpl = _sess._impl;
}

OraStmtImpl::~OraStmtImpl()
{
	clean_up();
}

void OraStatement::Reset()
{
	_impl->Reset();
}


OraStatement::OraStatement(const DbSession& sess)
: DbStatement(sess)
, _impl(new OraStmtImpl(sess))
{
}

OraStatement::~OraStatement()
{
	delete _impl;
}

void OraStmtImpl::clean_up()
{
	if(stmthp) {
		OCIHandleFree(stmthp,OCI_HTYPE_STMT);
		stmthp = 0;
	}

	m_RowsFetched = 0;
	m_TotalRowsFetched = 0;
}

void OraStmtImpl::setSql( const std::string& str )
{
	if (!_delayBind)
	{
		clean_blob();
		_definenull.clear();
		_bindnull.clear();
		_params.clear();
	}
	if(sqlstmt.length() > 0)
	{
		clean_up();
	}

	CHECK(_sessImpl->errhp,OCIHandleAlloc((dvoid *)_sessImpl->envhp, 
		(dvoid **)&stmthp, OCI_HTYPE_STMT, 0, 0));

	CHECK(_sessImpl->errhp, OCIStmtPrepare(stmthp, _sessImpl->errhp,
		(unsigned char *)str.c_str(), (short)str.length(), OCI_NTV_SYNTAX, 0));

	sqlstmt = Poco::toLower(str);
	plsqlBlock =sqlstmt.find("begin") != std::string::npos 
		&& sqlstmt.find("end;") != std::string::npos;
}

int OraStmtImpl::prepareColInfo(std::vector<OraColumnInfo>& colVec,
								std::vector<OraColData>& colData)
{
	CHECK(_sessImpl->errhp, OCIStmtExecute(_sessImpl->svchp, stmthp, _sessImpl->errhp, 
		(ub4)1,  (ub4)0, (OCISnapshot *) 0, (OCISnapshot *) 0, (ub4) OCI_DESCRIBE_ONLY));

	ub4 colnum = 0;
	ub1 bIsNull;

	CHECK(_sessImpl->errhp, OCIAttrGet(stmthp, OCI_HTYPE_STMT, (dvoid *)&colnum, 
		(ub4 *)0, OCI_ATTR_PARAM_COUNT, _sessImpl->errhp));

	OCIParam *parmdp; 

	colVec.resize(colnum);
	colData.resize(colnum, OraColData());

	for (ub4 i = 0; i < colnum; i++) 
	{
		CHECK(_sessImpl->errhp, OCIParamGet ((dvoid *)stmthp, OCI_HTYPE_STMT, _sessImpl->errhp, 
			(dvoid**)&parmdp, i + 1)); 

		CHECK(_sessImpl->errhp, OCIAttrGet((dvoid*) parmdp, (ub4)OCI_DTYPE_PARAM,
			(dvoid*) &bIsNull, (ub4 *) 0, (ub4) OCI_ATTR_IS_NULL, _sessImpl->errhp));

		colVec[i]._null = (bIsNull != 0);

		CHECK(_sessImpl->errhp, OCIAttrGet((dvoid*) parmdp, (ub4) OCI_DTYPE_PARAM,
			(dvoid*) &colVec[i]._type, (ub4 *) 0, (ub4) OCI_ATTR_DATA_TYPE, _sessImpl->errhp));

		CHECK(_sessImpl->errhp, OCIAttrGet((dvoid*) parmdp, (ub4) OCI_DTYPE_PARAM,
			(dvoid*) &colVec[i]._maxlen, (ub4 *) 0, (ub4) OCI_ATTR_DATA_SIZE, _sessImpl->errhp));

		// eg:2003-10-12 13:14:15
		colVec[i]._maxlen = colVec[i]._type == SQLT_DAT ? 20 : colVec[i]._maxlen + 1;

		colData[i].val = new char[colVec[i]._maxlen + 1];
	}

	for (ub4 i = 0; i < colnum; i++) 
	{
		CHECK(_sessImpl->errhp, OCIDefineByPos(stmthp, &m_pDefine, _sessImpl->errhp, 
			i + 1, colData[i].val, (sb4)colVec[i]._maxlen, (ub2)SQLT_STR, 
			&colData[i].ind,0,0,OCI_DEFAULT));
	}

	return (int)colnum;
}



int OraStmtImpl::Execute( bool bOver )
{
	m_RowsFetched = 0;
	m_TotalRowsFetched = 0;

	sb4 status = OCIStmtExecute(_sessImpl->svchp, stmthp, _sessImpl->errhp, 
		(ub4)1, (ub4) 0, (CONST OCISnapshot *) NULL, (OCISnapshot *) NULL, 
		_sessImpl->_autoCommit ? OCI_COMMIT_ON_SUCCESS : OCI_DEFAULT);

	// Check to see if we were bringing back gobbets of data (=> not all slots filled)
	ub4 size = sizeof(ub4);
	CHECK(_sessImpl->errhp, OCIAttrGet ( (dvoid *) stmthp,(ub4) OCI_HTYPE_STMT,
		(dvoid*)&m_RowsFetched, &size, (ub4) OCI_ATTR_ROW_COUNT,_sessImpl->errhp));

	if(status != OCI_NO_DATA)
	{
		CHECK(_sessImpl->errhp, status);
		m_TotalRowsFetched += m_RowsFetched;
	}

	return m_RowsFetched;
}

void OraStmtImpl::Reset()
{
	clean_blob();
	this->_params.clear();
	this->_definenull.clear();
	this->_bindnull.clear();
	this->_delayBinds.clear();
}


int OraStmtImpl::Fetch( bool bOver )
{
	// Has all data been retrieved? (=> can't fetch any more data)
	if(m_RowsFetched < 1)
	{
		m_RowsFetched = 0;
		return 0;
	}

	m_RowsFetched = 0;

	sb4 status = OCIStmtFetch(stmthp, _sessImpl->errhp, (ub4)1, (ub4) OCI_FETCH_NEXT,
		(ub4) OCI_DEFAULT);

	if ( status == OCI_NO_DATA )
	{
		// Check to see if we were bringing back gobbets of data (=> not all slots filled)
		int size = sizeof(ub4);
		m_RowsFetched = 0;
		CHECK(_sessImpl->errhp, OCIAttrGet((dvoid *)stmthp,(ub4)OCI_HTYPE_STMT,
			(dvoid*)&m_RowsFetched, (ub4*)&size,(ub4)OCI_ATTR_ROW_COUNT,
			_sessImpl->errhp));
		// If same => no more rows
		if(m_RowsFetched == m_TotalRowsFetched)
		{
			m_RowsFetched = 0;
		}
		else // Otherwise => more data
		{
			m_RowsFetched = m_RowsFetched - m_TotalRowsFetched;
		}
	}
	else
	{  
		CHECK(_sessImpl->errhp, status);
		m_RowsFetched = 1;
		m_TotalRowsFetched += 1;
	}

	return m_RowsFetched;
}
// 设定sql语句
void OraStatement::operator = (const std::string& str) 
{
	_impl->setSql(str);
}

// 执行sql语句
int OraStatement::Execute(bool bOver)
{
	_impl->delayBind();
	_impl->prepareData();
	int ret = _impl->Execute(bOver);
	if (ret > 0)
	{
		_impl->postData();
	}

	return ret;
}

// 获取返回数据
int OraStatement::Fetch(bool bOver)
{
	_impl->prepareData();
	int ret = _impl->Fetch(bOver);
	if (ret > 0)
	{
		_impl->postData();
	}

	return ret;
}

void OraStatement::Define(int& valuep)
{
	_impl->Define(valuep);
}

void OraStmtImpl::Define(int& valuep)
{
	_definenull.push_back(0);

	_params.push_back(ParamData());

	ParamData& pd = _params.back();
	pd._pos = (short)_definenull.size();
	pd._dir = ParamData::DirDefine; // define
	pd._type = ParamData::TypeInt;
	pd._ref = (void*)&valuep;


	CHECK(_sessImpl->errhp, OCIDefineByPos(stmthp, &m_pDefine, _sessImpl->errhp, 
		(ub4)_definenull.size(), &valuep, (sb4)sizeof(ub4), (ub2)SQLT_INT, 
		&(_definenull.back()),0,0,OCI_DEFAULT));
}

void OraStatement::Bind(const int& valuep)
{
	_impl->Bind(valuep);
}

void OraStmtImpl::Bind(const int& valuep)
{
	_bindnull.push_back(0);

	if (_delayBind)
	{
		_delayBinds.push_back(DelayBindData());
		DelayBindData& dbd = _delayBinds.back();
		dbd.position = (ub4)_bindnull.size();
		dbd.valuep   = (dvoid*)&valuep;
		dbd.value_sz = (sb4)sizeof(ub4);
		dbd.dty      = (ub2)SQLT_INT;
		dbd.indp     = &(_bindnull.back());
	}
	else
	{
		CHECK(_sessImpl->errhp, OCIBindByPos(stmthp, &m_pBind, _sessImpl->errhp, 
			(ub4)_bindnull.size(), (dvoid*)&valuep, (sb4)sizeof(ub4), (ub2)SQLT_INT, 
			&(_bindnull.back()),0,0,0,0, OCI_DEFAULT));
	}
}

void OraStatement::Define(long& valuep)
{
	_impl->Define(valuep);
}

void OraStmtImpl::Define(long& valuep)
{
	_definenull.push_back(0);

	_params.push_back(ParamData());

	ParamData& pd = _params.back();
	pd._pos = (short)_definenull.size();
	pd._dir = ParamData::DirDefine; // define
	pd._type = ParamData::TypeLong;
	pd._ref = (void*)&valuep;

	CHECK(_sessImpl->errhp, OCIDefineByPos(stmthp, &m_pDefine, _sessImpl->errhp, 
		(ub4)_definenull.size(), &valuep, (sb4)sizeof(eb4), (ub2)SQLT_INT, 
		&(_definenull.back()),0,0,OCI_DEFAULT));
}

void OraStatement::Bind(const long& valuep)
{
	_impl->Bind(valuep);
}

void OraStmtImpl::Bind(const long& valuep)
{
	_bindnull.push_back(0);

	if (_delayBind)
	{
		_delayBinds.push_back(DelayBindData());
		DelayBindData& dbd = _delayBinds.back();
		dbd.position = (ub4)_bindnull.size();
		dbd.valuep   = (dvoid*)&valuep;
		dbd.value_sz = (sb4)sizeof(ub4);
		dbd.dty      = (ub2)SQLT_INT;
		dbd.indp     = &(_bindnull.back());
	}
	else
	{
		CHECK(_sessImpl->errhp, OCIBindByPos(stmthp, &m_pBind, _sessImpl->errhp, 
			(ub4)_bindnull.size(), (dvoid*)&valuep, (sb4)sizeof(ub4), (ub2)SQLT_INT, 
			&(_bindnull.back()),0,0,0,0, OCI_DEFAULT));
	}
}

void OraStatement::Define(std::string& buf)
{
	_impl->Define(buf);
}

void OraStmtImpl::Define(std::string& buf)
{
	_definenull.push_back(0);

	_params.push_back(ParamData());

	ParamData& pd = _params.back();
	pd._pos = (short)_definenull.size();
	pd._dir = ParamData::DirDefine; // define
	pd._type = ParamData::TypeString; // string
	pd._ref = (void*)&buf;

	CHECK(_sessImpl->errhp, OCIDefineByPos(stmthp, &m_pDefine, _sessImpl->errhp, 
		(ub4)pd._pos, pd._buf, sizeof(pd._buf), (ub2)SQLT_STR, 
		&(_definenull.back()),0,0,OCI_DEFAULT));
}

void OraStatement::Bind(const std::string& str, unsigned int maxLen)
{
	_impl->Bind(str, maxLen);
}

void OraStmtImpl::Bind(const std::string& str, unsigned int maxLen, ParamData::InOut io)
{
	_bindnull.push_back(0);

	_params.push_back(ParamData());

	ParamData& pd = _params.back();
	pd._pos = (short)_bindnull.size();
	pd._dir = ParamData::DirBind; // define
	pd._type = ParamData::TypeString; // string
	pd._ref = (void*)&str;
	pd._io  = io;

	if (_delayBind)
	{
		_delayBinds.push_back(DelayBindData());
		DelayBindData& dbd = _delayBinds.back();
		dbd.position = (ub4)_bindnull.size();
		dbd.valuep   = (dvoid*)pd._buf;
		dbd.value_sz = maxLen == 0 ? (sb4)sizeof(pd._buf) : maxLen + 1;
		dbd.dty      = (ub2)SQLT_STR;
		dbd.indp     = &(_bindnull.back());
	}
	else
	{
		// 2008年7月26日，修改SQLT_STR到SQLT_AVC，支持char[n]的blank-padded比较
		// The default datatype for all other character variables in Pro*C/C++ is CHARZ; 
		// see "CHARZ ". Briefly, this means that you must null-terminate the string on input, 
		// and it is both blank-padded and null-terminated on output.
		CHECK(_sessImpl->errhp, OCIBindByPos(stmthp, &m_pBind, _sessImpl->errhp, 
			(ub4)pd._pos, (dvoid*)pd._buf, (sb4)sizeof(pd._buf), (ub2)SQLT_AVC, 
			&_bindnull.back(),0,0,0,0, OCI_DEFAULT));
	}
}

void OraStatement::Bind(const std::string& str, BindOption opt)
{
	_impl->Bind(str, opt);
}

void OraStmtImpl::Bind(const std::string& str, BindOption opt)
{
	Bind(str);
}

void OraStatement::Define(Poco::LocalDateTime& buf)
{
	_impl->Define(buf);
}

void OraStmtImpl::Define(Poco::LocalDateTime& dt)
{
	_definenull.push_back(0);
	_params.push_back(ParamData());

	ParamData& pd = _params.back();
	pd._pos = (short)_definenull.size();
	pd._dir = ParamData::DirDefine; // define
	pd._type = ParamData::TypeDate; // string
	pd._ref = (void*)&dt;

	CHECK(_sessImpl->errhp, OCIDefineByPos(stmthp, &m_pDefine, _sessImpl->errhp, 
		(ub4)pd._pos, pd._buf, sizeof(pd._buf), (ub2)SQLT_ODT, 
		&(_definenull.back()),0,0,OCI_DEFAULT));

}
void OraStmtImpl::Bind(const Poco::LocalDateTime& dt, ParamData::InOut io)
{
	_bindnull.push_back(0);
	_params.push_back(ParamData());

	ParamData& pd = _params.back();
	pd._pos = (short)_bindnull.size();
	pd._dir = ParamData::DirBind; // define
	pd._type = ParamData::TypeDate; // string
	pd._ref = (void*)&dt;
	pd._io  = io;

	if (_delayBind)
	{
		_delayBinds.push_back(DelayBindData());
		DelayBindData& dbd = _delayBinds.back();
		dbd.position = (ub4)_bindnull.size();
		dbd.valuep   = (dvoid*)pd._buf;
		dbd.value_sz = (sb4)sizeof(pd._buf);
		dbd.dty      = (ub2)SQLT_ODT;
		dbd.indp     = &(_bindnull.back());
	}
	else
	{
		CHECK(_sessImpl->errhp, OCIBindByPos(stmthp, &m_pBind, _sessImpl->errhp, 
			(ub4)pd._pos, (dvoid*)pd._buf, (sb4)sizeof(pd._buf), (ub2)SQLT_ODT, 
			&_bindnull.back(),0,0,0,0, OCI_DEFAULT));
	}
}

void OraStatement::Bind(const Poco::LocalDateTime& dt)
{
	_impl->Bind(dt);
}

bool OraStatement::IsNull( int i ) const
{
	return _impl->_definenull[i] == -1;
}

void OraStatement::SetNull( int i )
{
	_impl->_bindnull[i] = -1;
}

void OraStmtImpl::prepareData()
{
	std::deque<ParamData>::iterator it = _params.begin();

	for (; it != _params.end(); ++it)
	{
		ParamData& pd = *it;

		if (pd._dir == ParamData::DirBind) // bind
		{
			if (_bindnull[pd._pos - 1] == -1)
			{
				continue;
			}

			switch (pd._type)
			{
			case ParamData::TypeString: // string
				{
					std::string& str = *(std::string*)pd._ref;
					strncpy(pd._buf, str.c_str(), sizeof(pd._buf) - 1);
					pd._buf[sizeof(pd._buf)-1] = 0;
				}
				break;
			case ParamData::TypeDate:
				{
					Poco::LocalDateTime& ldt = *(Poco::LocalDateTime*)pd._ref;
					OCIDate* od = (OCIDate*)pd._buf;

					OCIDateSetDate(od, ldt.year(), ldt.month(), ldt.day());
					OCIDateSetTime(od, ldt.hour(), ldt.minute(), ldt.second());
				}
				break;
			case ParamData::TypeBlob:
				{
					if(pd._io == ParamData::IO_OUT)continue;
					unsigned char** data = (unsigned char**)pd._ref;
					unsigned char*  val = *data;
					ub4 length = *(pd._len);

					ub4   offset = 1;
					ub4   loblen = 0;
					ub1   bufp[MAXLOBLEN];
					ub4   amtp = length;
					ub1   piece;
					sword retval;
					ub4   len = 0;
					ub4   nbytes;
					ub4   remainder = length;
					
					CHECK( _sessImpl->errhp, OCILobGetLength(_sessImpl->svchp, _sessImpl->errhp, pd._lobl, &loblen));

					if (length > MAXLOBLEN)
						nbytes = MAXLOBLEN;
					else
						nbytes = length;

					memset(bufp,0,MAXLOBLEN);
					memcpy(bufp, val, nbytes);

					remainder -= nbytes;

					if (remainder == 0)       /* exactly one piece in the file */
					{
						CHECK( _sessImpl->errhp,OCILobWrite(_sessImpl->svchp, _sessImpl->errhp, pd._lobl, &amtp, offset, (dvoid *) bufp,
							(ub4) nbytes, OCI_ONE_PIECE, (dvoid *)0,
							(sb4 (*)(dvoid *, dvoid *, ub4 *, ub1 *)) 0,
							(ub2) 0, (ub1) SQLCS_IMPLICIT));
						continue;
					}
					else                     /* more than one piece */
					{
						retval = OCILobWrite(_sessImpl->svchp, _sessImpl->errhp, pd._lobl, &amtp, offset, (dvoid *) bufp,
							(ub4) MAXLOBLEN, OCI_FIRST_PIECE, (dvoid *)0,
							(sb4 (*)(dvoid *, dvoid *, ub4 *, ub1 *)) 0,
							(ub2) 0, (ub1) SQLCS_IMPLICIT);
						if(retval!= OCI_NEED_DATA)
						{
							throw OraException(retval, "ERROR: OCILobWrite().");
						}

						piece = OCI_NEXT_PIECE;

						do
						{
							if (remainder > MAXLOBLEN)
								nbytes = MAXLOBLEN;
							else
							{
								nbytes = remainder;
								piece = OCI_LAST_PIECE;
							}

							memset(bufp,0,MAXLOBLEN);
							memcpy(bufp, val + (length - remainder), nbytes);

							retval = OCILobWrite(_sessImpl->svchp,_sessImpl->errhp, pd._lobl, &amtp, offset, (dvoid *) bufp,
								(ub4) nbytes, piece, (dvoid *)0,
								(sb4 (*)(dvoid *, dvoid *, ub4 *, ub1 *)) 0,
								(ub2) 0, (ub1) SQLCS_IMPLICIT);
							remainder -= nbytes;

						} while (retval == OCI_NEED_DATA);
					}

					if (retval != OCI_SUCCESS)
					{
						throw OraException(retval, "Error: stream writing LOB.");
					}

					CHECK( _sessImpl->errhp, OCILobGetLength(_sessImpl->svchp, _sessImpl->errhp, pd._lobl, &loblen));

				}
				break;
			default:
				break;
			}
		}
	}
}

void OraStmtImpl::postData()
{
	std::deque<ParamData>::iterator it = _params.begin();
	short isnull;

	for (; it != _params.end(); ++it)
	{
		ParamData& pd = *it;

		if (pd._dir == ParamData::DirDefine 
			|| plsqlBlock && pd._io == ParamData::IO_OUT) // define
		{
			isnull = pd._dir == ParamData::DirDefine 
				? _definenull[pd._pos - 1] : _bindnull[pd._pos - 1];
			switch (pd._type)
			{
			case ParamData::TypeString: // string
				{
					std::string& str = *(std::string*)pd._ref;
					str = isnull == -1 ? "" : pd._buf;
				}
				break;
			case ParamData::TypeDate:
				{
					Poco::LocalDateTime& ldt = *(Poco::LocalDateTime*)pd._ref;
					if (isnull == -1)
					{
						ldt.assign(1970, 1, 1, 0, 0, 0);
					}
					else
					{
						OCIDate* od = (OCIDate*)pd._buf;
						sb2 year; ub1 month; ub1 day;
						OCIDateGetDate(od, &year, &month, &day);

						ub1 hour; ub1 min; ub1 sec;
						OCIDateGetTime(od, &hour, &min, &sec);
						ldt.assign(year, month, day, hour, min, sec);
					}
				}
				break;
			case ParamData::TypeInt:
				{
					int& intVar = *(int*)pd._ref;
					if (isnull == -1)
					{
						intVar = 0;
					}
				}
				break;
			case ParamData::TypeLong:
				{
					long& intVar = *(long*)pd._ref;
					if (isnull == -1)
					{
						intVar = 0;
					}
				}
				break;
			case ParamData::TypeBlob:
				{
					unsigned char** data = (unsigned char**)pd._ref;
					const int MAXBUFLEN = 5000;

					ub4   offset = 1;
					ub4   loblen = 0;
					ub1   bufp[MAXBUFLEN];
					ub4   amtp = 0;
					sword retval;
					ub4   piece = 0;
					ub4   remainder;            /* the number of bytes for the last piece */
					int   pos = 0;

					// blob is NULL
					if(isnull == -1){
						*(pd._len) = 0;
						*data = NULL;
						continue;
					}

					CHECK( _sessImpl->errhp, OCILobGetLength(_sessImpl->svchp, _sessImpl->errhp, pd._lobl, &loblen));
					*(pd._len)= amtp = loblen;

					if(loblen == 0)continue;

					*data = new unsigned char[loblen];

					unsigned char* val = *data;
					//memset(val, '\0', loblen);

					memset(bufp, '\0', MAXBUFLEN);
					retval = OCILobRead(_sessImpl->svchp, _sessImpl->errhp, pd._lobl, &amtp, offset, (dvoid *) bufp,
						(loblen < MAXBUFLEN ? loblen : MAXBUFLEN), (dvoid *)0,
						(sb4 (*)(dvoid *, const dvoid *, ub4, ub1)) 0,
						(ub2) 0, (ub1) SQLCS_IMPLICIT);

					switch (retval)
					{
					case OCI_SUCCESS:             /* only one piece */
						memcpy(val, bufp, loblen);
						break;
					case OCI_ERROR:
						CHECK(_sessImpl->errhp, retval);
						break;
					case OCI_NEED_DATA:           /* there are 2 or more pieces */

						remainder = loblen;
						memcpy(val, bufp, MAXBUFLEN);
						pos += MAXBUFLEN;
						do
						{
							memset(bufp, '\0', MAXBUFLEN);
							amtp = 0;

							remainder -= MAXBUFLEN;

							retval = OCILobRead(_sessImpl->svchp, _sessImpl->errhp, pd._lobl, &amtp, offset, (dvoid *) bufp,
								(ub4) MAXBUFLEN, (dvoid *)0,
								(sb4 (*)(dvoid *, const dvoid *, ub4, ub1)) 0,
								(ub2) 0, (ub1) SQLCS_IMPLICIT);

							if (remainder < MAXBUFLEN)     /* last piece not a full buffer piece */
							{
								memcpy(val + pos, bufp, remainder);
								pos += remainder;
							}
							else
							{
								memcpy(val + pos, bufp, MAXBUFLEN);
								pos += MAXBUFLEN;
							}

						} while (retval == OCI_NEED_DATA);
						break;
					default:
						break;
					}
				}
				break;
			default:
				break;
			}
		}
	}
}

void OraStmtImpl::delayBind()
{
	if (!_delayBind)
	{
		return;
	}

	std::deque<DelayBindData>::const_iterator it = _delayBinds.begin();
	for (; it != _delayBinds.end(); ++it)
	{
		const DelayBindData& dbd = *it;
		CHECK(_sessImpl->errhp, OCIBindByPos(stmthp, &m_pBind, _sessImpl->errhp, 
			dbd.position, dbd.valuep, dbd.value_sz, dbd.dty, 
			dbd.indp,0,0,0,0, OCI_DEFAULT));
	}

	_delayBinds.clear();
	_delayBind = false;
}
struct OraProImpl
{
	bool _bound;
	OraProImpl(const DbSession& sess);
	~OraProImpl();
	OraStatement stmt;
	std::string sqlstmt;
	std::string proName;
	void SetProName();
};
OraProImpl::OraProImpl(const DbSession& sess)
: stmt(sess)
, _bound (false)
{
}

OraProImpl::~OraProImpl()
{
}

OraProcedure::OraProcedure(const DbSession& sess)
: DbProcedure(sess)
, _impl(new OraProImpl(sess))
{}

OraProcedure::~OraProcedure()
{
	delete _impl;
}

void OraProcedure::SetProName(const std::string& str)
{
	OraStmtImpl* stmtimpl = _impl->stmt._impl;
	stmtimpl->_delayBind = true;
	_impl->proName = str;

	stmtimpl->_definenull.clear();
	stmtimpl->_bindnull.clear();
	stmtimpl->_params.clear();

	_impl->_bound = false;
}

void OraProImpl::SetProName()
{
	sqlstmt =  "begin " + proName;

	int iNumParam = (int)stmt._impl->_delayBinds.size();
	if( iNumParam >= 1 ) {
		sqlstmt +=  "(:1";
		char buf[8];
		for(int i = 2; i <= iNumParam; i++) {
			sprintf(buf, ", :%d", i );
			sqlstmt += buf;
		}
		sqlstmt += ')';
	}
	sqlstmt += "; end;";

	stmt = sqlstmt;
}

void OraProcedure::SetParamIn(const std::string& str)
{
	_impl->stmt.Bind(str, ParamData::IO_IN);
}
void OraProcedure::SetParamIn(const int& str)
{
	_impl->stmt.Bind(str);
}
void OraProcedure::SetParamIn(const Poco::LocalDateTime& str)
{
	_impl->stmt.Bind(str);
}
void OraProcedure::SetParamOut(std::string& str, unsigned int maxLen)
{
	_impl->stmt._impl->Bind(str, maxLen, ParamData::IO_OUT);
}
void OraProcedure::SetParamOut(int& str)
{
	_impl->stmt.Bind(str);
}
void OraProcedure::SetParamOut(Poco::LocalDateTime& str)
{
	_impl->stmt._impl->Bind(str, ParamData::IO_OUT);
}

void OraProcedure::SetParamIn(unsigned char** valuep,  size_t& len)
{
    _impl->stmt.Bind(valuep, len);
}

void OraProcedure::SetParamOut(unsigned char** valuep, size_t& len)
{
    _impl->stmt._impl->Bind((void**)valuep, len, ParamData::IO_OUT);
}

int OraProcedure::Execute()
{
	if (!_impl->_bound)
	{
		_impl->SetProName();
		_impl->_bound = true;
	}

	return _impl->stmt.Execute();
}

DataTable* OraSession::ExecuteQuery(const std::string& sql,CDataRow& bind)
{
	DataTable* dt = new DataTable();
	std::vector<OraColumnInfo> colVec;
	std::vector<OraColData> colData;

	OraStatement stmt(*this);
	stmt = sql;

	std::vector<int*> pIntVec;
	AutoReleasePointer<int> autoRelease(pIntVec);
	std::vector<Poco::LocalDateTime*> pDateVec;
	AutoReleasePointer<Poco::LocalDateTime> autoRelease2(pDateVec);

	try
	{
		for(int i = 0;i<bind.Count();i++)
		{
			switch(bind[i].type)
			{
			case DataColumn::DATATYPE_INT:
				{
					int* pint = new int(Poco::NumberParser::parse(bind[i].value)); 
					pIntVec.push_back(pint);
					stmt._impl->Bind(*pint);
				}
				break;
			case DataColumn::DATATYPE_STRING:
				stmt._impl->Bind(bind[i].value);
				break;
			case DataColumn::DATATYPE_DATETIME:
				{
					Poco::DateTime dt;
					int tzd;
					Poco::DateTimeParser::parse("%Y-%m-%d %H:%M:%S", bind[i].value, dt, tzd);
					Poco::LocalDateTime* pDate = new Poco::LocalDateTime(dt.year(), dt.month(), dt.day(), dt.hour(),dt.minute(), dt.second());
					pDateVec.push_back(pDate);
					stmt._impl->Bind(*pDate);
				}
				break;
			}
		}

		int cols = stmt._impl->prepareColInfo(colVec, colData);
		for (int ret = stmt.Execute(); ret > 0; ret = stmt.Fetch())
		{
			DataRow* dr = new DataRow();
			for (int i = 0; i < colData.size(); ++i)
			{
				DataColumn* dc = new DataColumn();
				dc->value = colData[i].val;
				dc->ind = colData[i].ind;
				switch(colVec[i]._type)
				{
				case 1:
				case 96:
					dc->type = DataColumn::DATATYPE_STRING;
					break;
				case 2:
					dc->type = DataColumn::DATATYPE_INT;
					break;
				case 12:
					dc->type = DataColumn::DATATYPE_DATETIME;
					break;
				default:
					dc->type = DataColumn::DATATYPE_UNKNOWN;
					break;
				}
				dc->null = colVec[i]._null;
				dr->Add(dc);
			}
			dt->Add(CDataRow(dr));
		}
	}
	catch(Poco::Exception &e)
	{
		std::string msg = e.message();
	}

	return dt;
}

int OraSession::Copyout(const std::string& sql,
						const std::string& file)
{
	std::vector<OraColumnInfo> colVec;
	std::vector<OraColData> colData;

	OraStatement stmt(*this);
	stmt = sql;
	int cols = stmt._impl->prepareColInfo(colVec, colData);

	Poco::FileOutputStream fos(file);

	int rows = 0;
	for (int ret = stmt.Execute(); ret > 0; ret = stmt.Fetch())
	{
		rows += ret;

		fos << colData[0].val;
		for (int i = 1; i < colData.size(); ++i)
		{
			fos << ',' << colData[i].val;
		}
		fos << std::endl;
	}

	return rows;
}

int OraSession::Copyin(const std::string& tableName, 
					   const std::string& file)
{
	std::vector<OraColumnInfo> colVec;
	std::vector<OraColData> colData;

	{
		OraStatement stmt(*this);
		stmt = "select * from " + tableName + " where 1 > 2";
		int cols = stmt._impl->prepareColInfo(colVec, colData);
		int ret = stmt.Execute(true);
	}

	OraStatement stmt(*this);
	std::ostringstream sql;
	int i;

	sql << "insert into " << tableName << " values(";
	for (i = 0; i < colVec.size(); ++i)
	{
		if (i > 0) 
		{
			sql << ",";
		}
		sql << ":" << i + 1;
	}
	sql << ")";

	stmt = sql.str();
	for (i = 0; i < colVec.size(); ++i)
	{
		stmt._impl->Bind(i + 1, colData[i].val, colVec[i]._maxlen, &colData[i].ind);
	}


	Poco::FileInputStream fis(file);
	std::string curLine;
	std::vector<std::string> vecString;

	int ret, rows(0);
	for (; !fis.eof(); curLine.erase())
	{
		std::getline(fis, curLine);
		if (curLine.empty())continue;

		split(vecString, curLine, ",\r\n");

		if (vecString.size() > colData.size())
		{
			throw Poco::SyntaxException("fields number does not match");
		}

		for (i = 0; i < vecString.size(); ++i)
		{
			if (vecString[i].length() > colVec[i]._maxlen)
			{
				strncpy(colData[i].val, vecString[i].c_str(), colVec[i]._maxlen);
				colData[i].vallen = colVec[i]._maxlen;
				colData[i].val[colVec[i]._maxlen] = 0;
			}
			else
			{
				strcpy(colData[i].val, vecString[i].c_str());
				colData[i].vallen = (unsigned short)vecString[i].length();
				colData[i].val[vecString[i].length()] = 0;
			}
			colData[i].ind = vecString[i].length() == 0 ? -1 : 1;
		}

		for (; i < colData.size(); ++i)
		{
			colData[i].val[0] = 0;
			colData[i].vallen = 0;
			colData[i].ind = -1;
		}

		ret = stmt.Execute(true);

		rows += ret;
	}

	return rows;
}



void OraSession::SetAutoCommit( bool autoCommit )
{
	_impl->_autoCommit = autoCommit;
}

void OraStatement::Define(unsigned char** data, unsigned int & length)
{
	_impl->Define((void**)data, length);
}

void OraStatement::Bind(unsigned char** data, unsigned int & length)
{
	_impl->Bind((void**)data, length);
}

void OraStmtImpl::Define(void** valuep, size_t& len)
{
	_definenull.push_back(0);
	_params.push_back(ParamData());

	ParamData& pd = _params.back();
	pd._pos = (short)_definenull.size();
	pd._dir = ParamData::DirDefine; // define
	pd._type = ParamData::TypeBlob; // string
	pd._ref = valuep;
	pd._len = &len;

	if(!pd._lobl)
	{
		CHECK(_sessImpl->errhp, OCIDescriptorAlloc((dvoid *) _sessImpl->envhp, (dvoid **) &pd._lobl,
			(ub4)OCI_DTYPE_LOB, (size_t) 0, (dvoid **) 0));
		ub4 empty = 0;
		CHECK(_sessImpl->errhp, OCIAttrSet(pd._lobl, OCI_DTYPE_LOB, (dvoid *) &empty, 
			(ub4) sizeof(empty), (ub4) OCI_ATTR_LOBEMPTY, _sessImpl->errhp));

		CHECK(_sessImpl->errhp, OCILobCreateTemporary(_sessImpl->svchp, _sessImpl->errhp,
			pd._lobl, OCI_DEFAULT, OCI_DEFAULT, OCI_TEMP_BLOB,
			FALSE, OCI_DURATION_SESSION));
	}

	CHECK(_sessImpl->errhp, OCIDefineByPos(stmthp, &m_pDefine, _sessImpl->errhp, 
		(ub4)pd._pos, (dvoid *) &pd._lobl, (sb4) -1, (ub2)SQLT_BLOB, 
		&(_definenull.back()), (ub2 *) 0, (ub2 *) 0, (ub4) OCI_DEFAULT));
}

void OraStmtImpl::Bind(void** valuep,  size_t& len, ParamData::InOut io)
{
	if(io == ParamData::IO_IN && (*valuep == NULL || len == 0)) 
		_bindnull.push_back( -1);
	else
		_bindnull.push_back(0);

	_params.push_back(ParamData());

	ParamData& pd = _params.back();
	pd._pos = (short)_bindnull.size();
	pd._dir = ParamData::DirBind; // define
	pd._type = ParamData::TypeBlob; // string
	pd._ref = valuep;
	pd._len = &len;
	pd._io  = io;

	if(!pd._lobl)
	{
		//初始化OCILobLocator
		CHECK(_sessImpl->errhp, OCIDescriptorAlloc((dvoid *) _sessImpl->envhp, (dvoid **) &pd._lobl,
			(ub4)OCI_DTYPE_LOB, (size_t) 0, (dvoid **) 0));
		ub4 empty = 0;
		CHECK(_sessImpl->errhp, OCIAttrSet(pd._lobl, OCI_DTYPE_LOB, (dvoid *) &empty, 
			(ub4) sizeof(empty), (ub4) OCI_ATTR_LOBEMPTY, _sessImpl->errhp));

		CHECK(_sessImpl->errhp, OCILobCreateTemporary(_sessImpl->svchp, _sessImpl->errhp,
			pd._lobl, OCI_DEFAULT, OCI_DEFAULT, OCI_TEMP_BLOB,
			FALSE, OCI_DURATION_SESSION));
	}

	if (_delayBind)
	{
		_delayBinds.push_back(DelayBindData());
		DelayBindData& dbd = _delayBinds.back();
		dbd.position = (ub4)_bindnull.size();
		dbd.valuep   = (dvoid*)&pd._lobl;
		dbd.value_sz = -1;
		dbd.dty      = (ub2)SQLT_BLOB;
		dbd.indp     = &(_bindnull.back());
	}
	else
	{
		CHECK(_sessImpl->errhp, OCIBindByPos(stmthp, &m_pBind, _sessImpl->errhp, 
			(ub4)pd._pos, (dvoid*)&pd._lobl, (sb4)-1, (ub2)SQLT_BLOB, 
			&_bindnull.back(),0,0,0,0, OCI_DEFAULT));
	}
}

void OraStmtImpl::clean_blob()
{
	for(std::deque<ParamData>::iterator it = _params.begin(); it != _params.end(); it ++)
	{
		if(it->_lobl){
			boolean is_temporary;
			CHECK(_sessImpl->errhp, OCILobIsTemporary(_sessImpl->envhp, _sessImpl->errhp, it->_lobl, &is_temporary));
			if(is_temporary == 1){
				CHECK(_sessImpl->errhp, OCILobFreeTemporary(_sessImpl->svchp, _sessImpl->errhp, it->_lobl));
			}

			CHECK(_sessImpl->errhp,OCIDescriptorFree((dvoid *) it->_lobl, (ub4) OCI_DTYPE_LOB));
			it->_lobl = NULL;
		}
	}
}