#pragma once


// CHsmIPAdressDlg 对话框
typedef struct HsmIPAddressTarget
{
	HWND hWnd;
	CString ipAddress;
	CString ipPort;
	CString* errMessage;
	bool*    flag;
}HsmIPAddress;

class CHsmIPAdressDlg : public CDialog
{
	DECLARE_DYNAMIC(CHsmIPAdressDlg)

public:
	CHsmIPAdressDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHsmIPAdressDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_IPADDRESS };

	CString GetIPAddress() const { return m_sIpAddress; }
	CString GetIPPort()    const { return m_sIpPort; }
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

	BOOL OnInitDialog();
private:
	CString m_sIpAddress;
	CString m_sIpPort;
	CString m_errMessage;
	bool m_flag;
	HsmIPAddress m_tagIpAddress;
public:
	//afx_msg void OnBnClickedButtonTest();

	//static DWORD WINAPI TuxedoTest(LPVOID lpParameter);
	afx_msg LRESULT OnErrorDisplay(WPARAM wp, LPARAM lp);	
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
