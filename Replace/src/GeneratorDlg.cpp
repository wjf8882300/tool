// E:\study\C++\Replace\Replace\src\GeneratorDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Replace.h"
#include "GeneratorDlg.h"
#include "afxdialogex.h"
#include "Poco/Environment.h"
#include "dbi/COracle.h"
#include "util/Util.h"

// CGeneratorDlg �Ի���

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


// CGeneratorDlg ��Ϣ�������
void CGeneratorDlg::OnBnClickedButtonScanTable()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		MessageBox(_T("��¼ʧ�ܣ����ݿ��쳣��") + Util::Mbstowcs(ex.ErrMsg()),_T("����"),MB_OK|MB_ICONERROR);
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	*pResult = 0;
}