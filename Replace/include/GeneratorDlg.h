#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include <vector>

using std::vector;


// CGeneratorDlg 对话框

class CGeneratorDlg : public CDialog
{
	DECLARE_DYNAMIC(CGeneratorDlg)

public:
	CGeneratorDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CGeneratorDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_GENERATOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_treeTables;
	CButton m_bntScanTable;
	afx_msg void OnBnClickedButtonScanTable();
	vector<std::string> m_tableNames;
	afx_msg void OnTvnSelchangedTreeTables(NMHDR *pNMHDR, LRESULT *pResult);
};
