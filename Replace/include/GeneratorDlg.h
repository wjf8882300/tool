#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include <vector>

using std::vector;


// CGeneratorDlg �Ի���

class CGeneratorDlg : public CDialog
{
	DECLARE_DYNAMIC(CGeneratorDlg)

public:
	CGeneratorDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CGeneratorDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_GENERATOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_treeTables;
	CButton m_bntScanTable;
	afx_msg void OnBnClickedButtonScanTable();
	vector<std::string> m_tableNames;
	afx_msg void OnTvnSelchangedTreeTables(NMHDR *pNMHDR, LRESULT *pResult);
};
