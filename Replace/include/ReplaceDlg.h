// ReplaceDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include <vector>
#include <stdlib.h>
#include "file/FolderDialog.h"
#include "file/FileUtil.h"
#include "util/Util.h"
#include "format/SQLFormat.h"
#include "format/HexParseUtil.h"
#include "encrypt/MacUtil.h"
#include "encrypt/AesUtil.h"
#include "encrypt/MD5Util.h"
#include "encrypt/DESUtil.h"
#include "reg/RegeditEditor.h"
#include "../Resource.h"
#include "Poco/Net/ICMPSocket.h"
#include "Poco/Net/ICMPClient.h"
#include "Poco/Net/IPAddress.h"
#include "Poco/Net/ICMPEventArgs.h"
#include "file/FileValidator.h"
#include "format/StringUtil.h"

using Poco::Net::ICMPSocket;
using Poco::Net::ICMPClient;
using Poco::Net::IPAddress;
using Poco::Net::ICMPEventArgs;

using namespace std;
#define IsCTRLPressed() ((GetKeyState(VK_CONTROL) & (1<<(sizeof(SHORT)*8-1))) != 0)

// CReplaceDlg 对话框
class CReplaceDlg : public CDialog
{
// 构造
public:
	CReplaceDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_REPLACE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnHotKey(WPARAM wParam,LPARAM lParam);//手动加入
	afx_msg	void Init();
	afx_msg BOOL OnToolTipText(UINT, NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBntExplorer();
	void getDialogData();
	void DisplayText();
	void ValidateMac();
	void Replace();
	void Delete();
	void Mac();
	void Length();
	void Case();
	void SqlFormat();
	void HexParse();
	void SqlDate();
	void WriteTxtFile();
	void Aes();
	void Md5();
	void Des();
	void EnableProxy();
	void StringCat();
	void ValidatePurchase();
	void ValidateLoadDx();
	void ValidateLoadOnline();
	void JavaAppend();

	CEdit m_sFlieExt;
	CString m_SFileExt;
	CString m_source;
	CString m_dest;
	CString m_path;
	CString m_key;
	CEdit m_sFileName;
	CEdit m_sSource;
	CEdit m_sDest;
	std::vector<CString> sources;
	std::vector<CString> dests;
	int replaceFileCount;
	afx_msg void OnBnClickedButtonMac();
	CButton m_cSub;
	afx_msg void OnBnClickedButtonCase();
	bool	m_bcase;
	int m_nHotKeyID[10];
	afx_msg void OnBnClickedButtonLength();
	afx_msg void OnBnClickedButtonDelete();
	CButton m_FileNameRegex;
	CButton m_ascii;
	CButton m_padding;
	CButton m_bntMac;
	CButton m_bntCase;
	CButton m_bntLength;
	CButton m_bntReplace;
	CButton m_bntDelete;
	CButton m_bntValidate;
	CButton m_bntVerse;
	CButton m_cRownumber;
	CProgressCtrl m_progressTotal;
	vector<CString> files;
	afx_msg void OnDropFiles(HDROP hDropInfo);
	CButton m_sql;
	CButton m_cbQuotes;
	CToolBar    m_wndToolBar;
	//
	CImageList m_TBarDis;
	CImageList m_TBarImages;

private:
	void GetEditLines(CEdit& ced,std::vector<CString>& sources);
	void OutPut(const CString& coutput);
	void Append(const CString& coutput);
	bool IsExistsData();
	void Case(CString& result,vector<CString>&toBeFormat, bool isCase,bool isQuotes);
	void ProgressStepIt();
	void InitProgressStep(short size);
	void LoadFile(bool isDisplay);
	void GetDateTimeString(CString& date,CString& time);
	void Append(CString& result,vector<CString>&toBeFormat, bool isAppend);
public:
	CButton m_bntByte;
	CButton m_cbByte;
	CButton m_cbHightToLower;
	CButton m_bntSqlDate;
	CButton m_bntWrite;
	CButton mt_bntAES;
	CButton mt_cbAES;
	CButton m_bntMD5;
	CButton m_bntDes;
	CEdit m_sKey;
	CMenu m_menu;
	afx_msg void OnMac();
	afx_msg void OnAbout();
	afx_msg void OnDes();
	afx_msg void OnAes();
	afx_msg void OnMd5();
	afx_msg void OnCase();
	afx_msg void OnLength();
	afx_msg void OnByte();
	afx_msg void OnSqldate();
	afx_msg void OnSqlformat();
	afx_msg void OnReplace();
	afx_msg void OnDelete();
	afx_msg void OnValidate();
	afx_msg void OnRecord();
	afx_msg void OnEnableproxy();
	afx_msg void OnQuit();
	afx_msg void OnStringcat();
	afx_msg void OnOracleLogin();

public:
	void onBegin(const void* pSender, ICMPEventArgs& args);
	void onReply(const void* pSender, ICMPEventArgs& args);
	void onError(const void* pSender, ICMPEventArgs& args);
	void onEnd(const void* pSender, ICMPEventArgs& args);
	afx_msg void OnPing();
	afx_msg void OnFtp();
	afx_msg void OnIpaddress();
	afx_msg void OnPurchaseValidate();

	void ReSize();  
	POINT old;  
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLoaddxValidate();
	afx_msg void OnLoadonlineValidate();
	afx_msg void OnConifgs001();
	afx_msg void OnHSM();

	afx_msg void OnPurcharseMac1();
	afx_msg void OnPurcharseMac2();
	afx_msg void OnPurcharseTac();
	afx_msg void OnLoadMac1();
	afx_msg void OnLoadMac2();
	afx_msg void OnLoadTac();
	afx_msg void OnSafeTac();
	afx_msg void OnM1KeyA();
	afx_msg void OnM1KeyB();
	afx_msg void OnM1TAC();


	CString m_sIpAddress;
	CString m_sIpPort;
	afx_msg void OnJavasqlappend();
	afx_msg void OnBnClickedButtonReverse();
	afx_msg void OnUnicode();
	afx_msg void OnMenuGenerator();
};