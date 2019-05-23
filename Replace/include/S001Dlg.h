#pragma once
#include "afxcmn.h"
#include "util/Util.h"

#define IsCTRLPressed() ((GetKeyState(VK_CONTROL) & (1<<(sizeof(SHORT)*8-1))) != 0)

class Column
{
public:
	Column(const CString& columnName, const CString& columnType, const int& width)
	{
		this->ColumnName = columnName;
		this->ColumnType = columnType;
		this->Width = width;
	}
public:
	CString ColumnName;
	CString ColumnType;
	int Width;
};

// CS001Dlg 对话框

class CS001Dlg : public CDialog
{
	DECLARE_DYNAMIC(CS001Dlg)

public:
	CS001Dlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CS001Dlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_S001 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonImportTemplate();
	afx_msg void OnBnClickedButtonExplorer();
	afx_msg void OnBnClickedButtonGenerate();

	virtual BOOL OnInitDialog();

	void LoadXml(CString path, const BOOL& isText = FALSE);
	CString GetNodeName(CComPtr<IXMLDOMNode> spNodeAttrib);
	CString GetNodeValue(CComPtr<IXMLDOMNode> spNodeAttrib, const CString& nodeName);
	CString GetAttributeValue(CComPtr<IXMLDOMNode> spNodeAttrib, const CString& attributeName);
	void CreateNode(IXMLDOMDocument* spDoc, IXMLDOMNode* spParentNode, const CString& nodeName, const CString& nodeValue, IXMLDOMNode** spNode);
	afx_msg void OnBnClickedButtonBodystd();
	void Generate(const vector<Column>& select);

	BOOL InitTree();
	CTreeCtrl m_tree;
	afx_msg void OnBnClickedCheckSelectall();
	vector<Column> m_cols;
	vector<Column> m_select;
	afx_msg void OnEnChangeEditChannelno();
	afx_msg void OnEnSetfocusEditChannelno();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnEnChangeEditBodySplit();
	afx_msg void OnEnSetfocusBodySplit();

	CString m_channelNo;
	CString m_bodySplit;
	CString m_oldChannelNo;
	bool    m_isContainChannelNoPathIn;
	bool    m_isContainChannelNoPathBak;
	bool    m_isContainChannelNoPathBakerr;
	bool    m_isContainChannelNoProcProphase;
	bool    m_isContainChannelNoProcSource;
	bool    m_isContainChannelNoProcHandle;

};
