// E:\study\C++\Replace\Replace\src\GeneratorDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Replace.h"
#include "GeneratorDlg.h"
#include "afxdialogex.h"
#include "Poco/Environment.h"
#include "dbi/COracle.h"
#include "util/Util.h"

// CGeneratorDlg 对话框

IMPLEMENT_DYNAMIC(CGeneratorDlg, CDialog)

CGeneratorDlg::CGeneratorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGeneratorDlg::IDD, pParent)
{

}

CGeneratorDlg::~CGeneratorDlg()
{
}

void CGeneratorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_TABLES, m_treeTables);
	DDX_Control(pDX, ID_BUTTON_SCAN_TABLE, m_bntScanTable);
}


BEGIN_MESSAGE_MAP(CGeneratorDlg, CDialog)
	ON_BN_CLICKED(ID_BUTTON_SCAN_TABLE, &CGeneratorDlg::OnBnClickedButtonScanTable)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_TABLES, &CGeneratorDlg::OnTvnSelchangedTreeTables)
END_MESSAGE_MAP()


// CGeneratorDlg 消息处理程序
void CGeneratorDlg::OnBnClickedButtonScanTable()
{
	// TODO: 在此添加控件通知处理程序代码
	try
	{
		Poco::Environment::set("NLS_DATE_FORMAT", "YYYY-MM-DD HH24:MI:SS");
		CSession session(new OraSession());
		session.Connect( 
			Util::getDBConnectInfo().getUserName(),
			Util::getDBConnectInfo().getPassword(),
			Util::getDBConnectInfo().getDBName());
		if( session.IsConnected() ) {
			string tableName;
			int width;
			CStatement stmt(session);
			stmt = "select table_Name from user_tables ";
			stmt.Define(tableName);

			for(int i = stmt.Execute(); i > 0; i = stmt.Fetch())
			{
				m_tableNames.push_back(tableName);
			}
		}
			
		session.Close();
	} catch ( DbException& ex ) {
		MessageBox(_T("登录失败！数据库异常：") + Util::Mbstowcs(ex.ErrMsg()),_T("操作"),MB_OK|MB_ICONERROR);
	}

	m_treeTables.DeleteAllItems();

	for(vector<std::string>::iterator it = m_tableNames.begin();
		it != m_tableNames.end(); it ++)
	{
		m_treeTables.InsertItem(Util::Mbstowcs(*it), 0, 0, TVI_ROOT, TVI_LAST);
	}
}


void CGeneratorDlg::OnTvnSelchangedTreeTables(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	*pResult = 0;
	// TODO: 在此添加控件通知处理程序代码

	*pResult = 0;
}