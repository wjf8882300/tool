#pragma once

#define IsCTRLPressed() ((GetKeyState(VK_CONTROL) & (1<<(sizeof(SHORT)*8-1))) != 0)
// CDetailDlg 对话框

class CDetailDlg : public CDialog
{
	DECLARE_DYNAMIC(CDetailDlg)

public:
	CDetailDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDetailDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_DETAIL };

	void SetItemText(const CString& txt) { m_txt = txt; }
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();
	void OnSize(UINT nType, int cx, int cy);
	void ReSize();

	CString m_txt;
	POINT old;  
	CFont   m_font;
	
public:
	afx_msg void OnDestroy();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
