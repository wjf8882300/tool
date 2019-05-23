#include "dbi/dbi5.h"
//#include "dbi/CSybase.h"
//#include "dbi/COracle.h"

DbException::DbException()
{

}

DbException::~DbException()
{

}

CSession::CSession(Poco::AutoPtr<DbSession> ptrImpl):
_ptrImpl(ptrImpl)
{
}

/*
CSession::CSession(const std::string& connector)
{
    DbSession* dbs = 0;
    if (connector == "oracle")
    {
        Poco::Environment::set("NLS_DATE_FORMAT", "YYYY-MM-DD HH24:MI:SS");
        dbs = new OraSession();
    }
    else if (connector == "sybase")
    {
        dbs = new SybSession();
    }

    CSession newSession(dbs);

    swap(newSession);
}
*/

CSession::CSession(const CSession& other):
_ptrImpl(other._ptrImpl)
{
}

CSession::~CSession()
{
}


CSession& CSession::operator = (const CSession& other)
{
    CSession tmp(other);
    swap(tmp);
    return *this;
}


void CSession::swap(CSession& other)
{
    using std::swap;
    swap(_ptrImpl, other._ptrImpl);
}


DbSession::DbSession()
{

}


DbSession::~DbSession()
{

}


void DbSession::SetAutoCommit( bool autoCommit )
{

}

DbProcedure::DbProcedure( const DbSession& sess )
{

}

DbProcedure::~DbProcedure()
{

}
DbStatement::DbStatement( const DbSession& sess )
{

}

DbStatement::~DbStatement()
{

}

CStatement::CStatement(Poco::AutoPtr<DbStatement> ptrImpl):
_ptrImpl(ptrImpl)
{
}

CStatement::CStatement(CSession& sess)
{
    CStatement newStmt = sess.createStatement();

    swap(newStmt);
}

CStatement::CStatement(const CStatement& other):
_ptrImpl(other._ptrImpl)
{
}

CStatement::~CStatement()
{
}


CStatement& CStatement::operator = (const CStatement& other)
{
    CStatement tmp(other);
    swap(tmp);
    return *this;
}


void CStatement::swap(CStatement& other)
{
    using std::swap;
    swap(_ptrImpl, other._ptrImpl);
}


CProcedure::CProcedure( Poco::AutoPtr<DbProcedure> ptrImpl )
: _ptrImpl(ptrImpl)
{

}

CProcedure::CProcedure( CSession& sess )
{
    CProcedure newPro = sess.createProcedure();

    swap(newPro);

}

CProcedure::CProcedure(const CProcedure& other):
_ptrImpl(other._ptrImpl)
{
}

CProcedure::~CProcedure()
{
}


CProcedure& CProcedure::operator = (const CProcedure& other)
{
    CProcedure tmp(other);
    swap(tmp);
    return *this;
}


void CProcedure::swap(CProcedure& other)
{
    using std::swap;
    swap(_ptrImpl, other._ptrImpl);
}