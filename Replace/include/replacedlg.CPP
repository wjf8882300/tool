// ReplaceDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Replace.h"
#include "ReplaceDlg.h"
#include "net/IPHelper.h"
#include "Poco/Net/ICMPClient.h"
#include "Poco/Delegate.h"
#include "Poco/Net/FTPClientSession.h"
#include "Poco/Net/NetException.h"
#include "Poco/Environment.h"
#include "dbi/COracle.h"
#include "S001Dlg.h"
#include "HsmIPAdressDlg.h"
#include "Hsm.h"
#include "GeneratorDlg.h"

using Poco::Net::ICMPClient;
using Poco::Delegate;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	
END_MESSAGE_MAP()


// CReplaceDlg 对话框




CReplaceDlg::CReplaceDlg(CWnd* pParent /*=NULL*/)
: CDialog(CReplaceDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CReplaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT_FILE_EXTENTION, m_sFlieExt);
	DDX_Control(pDX, IDC_EDIT_FILE_PATH, m_sFileName);
	DDX_Control(pDX, IDC_EDIT_OLD_TEXT, m_sSource);
	DDX_Control(pDX, IDC_EDIT_NEW_TEXT, m_sDest);
	DDX_Control(pDX, IDC_EDIT_KEY, m_sKey);

	DDX_Control(pDX, IDC_CHECK_SUB, m_cSub);
	DDX_Control(pDX, IDC_CHECK_CASE, m_FileNameRegex);
	DDX_Control(pDX, IDC_CHECK_ASCII, m_ascii);
	DDX_Control(pDX, IDC_CHECK_PADDING, m_padding);
	DDX_Control(pDX, IDC_CHECK_VERSE, m_bntVerse);
	DDX_Control(pDX, IDC_CHECK_ROWNUMBER, m_cRownumber);
	DDX_Control(pDX, IDC_CHECK_QUOTES, m_cbQuotes);
	DDX_Control(pDX, IDC_CHECK_BYTE, m_cbByte);
	DDX_Control(pDX, IDC_CHECK_HIGLOWER, m_cbHightToLower);
	DDX_Control(pDX, IDC_CHECK_AES, mt_cbAES);

	DDX_Control(pDX, IDC_PROGRESS_Total, m_progressTotal);	
}

BEGIN_MESSAGE_MAP(CReplaceDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BNT_EXPLORER, &CReplaceDlg::OnBnClickedBntExplorer)
	ON_MESSAGE(WM_HOTKEY,OnHotKey) //手动加入 
	ON_WM_DROPFILES()
	ON_WM_KEYUP()
	ON_COMMAND(ID_MAC, &CReplaceDlg::OnMac)
	ON_COMMAND(ID_ABOUT, &CReplaceDlg::OnAbout)
	ON_NOTIFY_EX( TTN_NEEDTEXT, 0, OnToolTipText )
	ON_COMMAND(ID_DES, &CReplaceDlg::OnDes)
	ON_COMMAND(ID_AES, &CReplaceDlg::OnAes)
	ON_COMMAND(ID_MD5, &CReplaceDlg::OnMd5)
	ON_COMMAND(ID_CASE, &CReplaceDlg::OnCase)
	ON_COMMAND(ID_LENGTH, &CReplaceDlg::OnLength)
	ON_COMMAND(ID_BYTE, &CReplaceDlg::OnByte)
	ON_COMMAND(ID_SQLDATE, &CReplaceDlg::OnSqldate)
	ON_COMMAND(ID_SQLFORMAT, &CReplaceDlg::OnSqlformat)
	ON_COMMAND(ID_REPLACE, &CReplaceDlg::OnReplace)
	ON_COMMAND(ID_DELETE, &CReplaceDlg::OnDelete)
	ON_COMMAND(ID_VALIDATE, &CReplaceDlg::OnValidate)
	ON_COMMAND(ID_RECORD, &CReplaceDlg::OnRecord)
	ON_COMMAND(ID_ENABLEPROXY, &CReplaceDlg::OnEnableproxy)
	ON_COMMAND(ID_QUIT, &CReplaceDlg::OnQuit)
	ON_COMMAND(ID_STRINGCAT, &CReplaceDlg::OnStringcat)
	ON_COMMAND(ID_ORACLE_LOGIN, &CReplaceDlg::OnOracleLogin)
	ON_COMMAND(ID_PING, &CReplaceDlg::OnPing)
	ON_COMMAND(ID_FTP, &CReplaceDlg::OnFtp)
	ON_COMMAND(ID_IPADDRESS, &CReplaceDlg::OnIpaddress)
	ON_COMMAND(ID_PURCHASE_VALIDATE, &CReplaceDlg::OnPurchaseValidate)
	ON_WM_SIZE()
	ON_COMMAND(ID_LOADDX_VALIDATE, &CReplaceDlg::OnLoaddxValidate)
	ON_COMMAND(ID_LOADONLINE_VALIDATE, &CReplaceDlg::OnLoadonlineValidate)
	ON_COMMAND(ID_CONIFGS001, &CReplaceDlg::OnConifgs001)
	ON_COMMAND(ID_MENU_HSM, &CReplaceDlg::OnHSM)

	ON_COMMAND(ID_PURCHARSE_MAC1, &CReplaceDlg::OnPurcharseMac1)
	ON_COMMAND(ID_PURCHARSE_MAC2, &CReplaceDlg::OnPurcharseMac2)
	ON_COMMAND(ID_PURCHARSE_TAC, &CReplaceDlg::OnPurcharseTac)
	ON_COMMAND(ID_LOAD_MAC1, &CReplaceDlg::OnLoadMac1)
	ON_COMMAND(ID_LOAD_MAC2, &CReplaceDlg::OnLoadMac2)
	ON_COMMAND(ID_LOAD_TAC, &CReplaceDlg::OnLoadTac)
	ON_COMMAND(ID_SAFE_MAC, &CReplaceDlg::OnSafeTac)
	ON_COMMAND(ID_M1_KEYA, &CReplaceDlg::OnM1KeyA)
	ON_COMMAND(ID_M1_KEYB, &CReplaceDlg::OnM1KeyB)
	ON_COMMAND(ID_M1_TAC, &CReplaceDlg::OnM1TAC)
	ON_COMMAND(ID_JAVASQLAPPEND, &CReplaceDlg::OnJavasqlappend)
	ON_BN_CLICKED(IDC_BUTTON_REVERSE, &CReplaceDlg::OnBnClickedButtonReverse)
	ON_COMMAND(ID_UNICODE, &CReplaceDlg::OnUnicode)
	ON_COMMAND(ID_MENU_GENERATOR, &CReplaceDlg::OnMenuGenerator)
END_MESSAGE_MAP()


// CReplaceDlg 消息处理程序

BOOL CReplaceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	m_TBarImages.Create(16, 16, ILC_COLOR32 | ILC_MASK, 0, 1);

	m_TBarImages.Add(AfxGetApp()->LoadIcon(IDI_ICON_MAC)); 
	m_TBarImages.Add(AfxGetApp()->LoadIcon(IDI_ICON_DES)); 
	m_TBarImages.Add(AfxGetApp()->LoadIcon(IDI_ICON_AES)); 
	m_TBarImages.Add(AfxGetApp()->LoadIcon(IDI_ICON_MD5));

	m_TBarImages.Add(AfxGetApp()->LoadIcon(IDI_ICON_UPPER));
	m_TBarImages.Add(AfxGetApp()->LoadIcon(IDI_ICON_LENGTH));
	m_TBarImages.Add(AfxGetApp()->LoadIcon(IDI_ICON_HEX));
	m_TBarImages.Add(AfxGetApp()->LoadIcon(IDI_ICON_STRINGCAT));
	m_TBarImages.Add(AfxGetApp()->LoadIcon(IDI_ICON_JAVASQLAPPEND));
	m_TBarImages.Add(AfxGetApp()->LoadIcon(IDI_ICON_UNICODE));
	
	m_TBarImages.Add(AfxGetApp()->LoadIcon(IDI_ICON_SQL_FORMAT));
	//m_TBarImages.Add(AfxGetApp()->LoadIcon(IDI_ICON_SQL));
	
	m_TBarImages.Add(AfxGetApp()->LoadIcon(IDI_ICON_REPLACE));
	m_TBarImages.Add(AfxGetApp()->LoadIcon(IDI_ICON_DELETE));

	m_TBarImages.Add(AfxGetApp()->LoadIcon(IDI_ICON_MAC_VALIDATE));
	m_TBarImages.Add(AfxGetApp()->LoadIcon(IDI_ICON_PURCHASE));
	m_TBarImages.Add(AfxGetApp()->LoadIcon(IDI_ICON_LOADDX));
	m_TBarImages.Add(AfxGetApp()->LoadIcon(IDI_ICON_LOADONLINE));
	m_TBarImages.Add(AfxGetApp()->LoadIcon(IDI_ICON_CONFIGS001));

	//m_TBarImages.Add(AfxGetApp()->LoadIcon(IDI_ICON_PROXY));
	
	//添加一个平面工具条
	if (!m_wndToolBar.CreateEx( this,TBSTYLE_FLAT ,  WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP | CBRS_TOOLTIPS,
		CRect(4,4,0,0)) ||	!m_wndToolBar.LoadToolBar(IDR_MENU_MAINFRAME) )
	{
		TRACE0("failed to create toolbar\n");
		return FALSE;
	}

	m_wndToolBar.GetToolBarCtrl().SetImageList(&m_TBarImages); 
	m_wndToolBar.GetToolBarCtrl().SetButtonSize(CSize(24,24));  
	m_wndToolBar.GetToolBarCtrl().SetBitmapSize(CSize(16,16));  

	m_wndToolBar.ShowWindow(SW_SHOW);

	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);

	//设置菜单
	SetMenu(NULL);	
	m_menu.LoadMenu(IDR_MENU_MAINFRAME);
	SetMenu(&m_menu);
	DrawMenuBar();

	// TODO: 在此添加额外的初始化代码
	m_sFlieExt.SetWindowText(_T("properties"));
	//默认密钥32个1
	m_sKey.SetWindowText(_T("11111111111111111111111111111111"));
	replaceFileCount = 0;
	m_bcase = false;
	m_nHotKeyID[0]=IDC_EDIT_OLD_TEXT;
	m_nHotKeyID[1]=IDC_EDIT_OLD_TEXT;
	Init();
	m_ascii.SetCheck(TRUE);
	m_padding.SetCheck(TRUE);
	m_bntVerse.SetCheck(TRUE);
	mt_cbAES.SetCheck(TRUE);
	m_cSub.SetCheck(TRUE);

	((CComboBox *)GetDlgItem(IDC_COMBO_OPER))->InsertString(0, _TEXT("空"));
	((CComboBox *)GetDlgItem(IDC_COMBO_OPER))->InsertString(1, _TEXT("双引号"));
	((CComboBox *)GetDlgItem(IDC_COMBO_OPER))->InsertString(2, _TEXT("单引号"));
	((CComboBox *)GetDlgItem(IDC_COMBO_OPER))->SetCurSel(0);
	((CButton*)GetDlgItem(IDC_CHECK_APPEND))->SetCheck(TRUE);

	//GetParent()->GetDlgItem(ID_ENABLEPROXY)->EnableWindow(FALSE);
	
	m_sSource.SetLimitText(UINT_MAX);
	m_sDest.SetLimitText(UINT_MAX);

	CRect rect;      
	GetClientRect(&rect);     //取客户区大小    
	old.x=rect.right-rect.left;  
	old.y=rect.bottom-rect.top; 

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CReplaceDlg::Init()   
{   
	//此函数可以在PreSubclassWindow和OnCreate中使用；   
	//如果不在前两者中使用，可以到放到CDialog::OnInitDialog或者   
	//CView::OnInitialUpdate函数中使用   

	ASSERT(NULL != GetSafeHwnd());   

	//Register热键   
#ifdef _DEBUG //debug版本   
	//int nRet = RegisterHotKey(GetSafeHwnd(),m_nHotKeyID[0],MOD_CONTROL,'A'); //热键 ctrl + d   
	//if(!nRet)   
	//	AfxMessageBox(_T("RegisterHotKey 0 false"));   
	//nRet = RegisterHotKey(GetSafeHwnd(),m_nHotKeyID[1],MOD_CONTROL,'D'); //热键 alt + m   
	//if(!nRet)   
	//	AfxMessageBox(_T("RegisterHotKey 1 false"));  
#else //release版本   
	//RegisterHotKey(GetSafeHwnd(),m_nHotKeyID[0],MOD_CONTROL,'A'); //热键 ctrl + d   
	//RegisterHotKey(GetSafeHwnd(),m_nHotKeyID[1],MOD_CONTROL,'D'); //热键 alt + m 
#endif   
} 


void CReplaceDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CReplaceDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		//dc.DrawIcon(x, y, m_hIcon);

	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CReplaceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//工具栏提示
BOOL CReplaceDlg::OnToolTipText(UINT, NMHDR* pNMHDR, LRESULT* pResult)
{
	ASSERT(pNMHDR->code == TTN_NEEDTEXTA || pNMHDR->code == TTN_NEEDTEXTW);

	// UNICODE消息
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	//TCHAR szFullText[512];
	CString strTipText;
	UINT nID = (UINT)pNMHDR->idFrom;

	if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
		pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
	{
		// idFrom为工具条的HWND 
		nID = ::GetDlgCtrlID((HWND)(UINT_PTR)nID);
	}

	if (nID != 0) //不为分隔符
	{
		strTipText.LoadString(nID);
		strTipText = strTipText.Mid(strTipText.Find(_T("\n"),0)+1);

#ifndef _UNICODE
		if (pNMHDR->code == TTN_NEEDTEXTA)
		{
			lstrcpyn(pTTTA->szText, strTipText, sizeof(pTTTA->szText));
		}
		else
		{
			_mbstowcsz(pTTTW->szText, strTipText, sizeof(pTTTW->szText));
		}
#else
		if (pNMHDR->code == TTN_NEEDTEXTA)
		{
			_wcstombsz(pTTTA->szText, strTipText,sizeof(pTTTA->szText));
		}
		else
		{
			lstrcpyn(pTTTW->szText, strTipText, sizeof(pTTTW->szText));
		}
#endif

		*pResult = 0;

		// 使工具条提示窗口在最上面
		::SetWindowPos(pNMHDR->hwndFrom, HWND_TOP, 0, 0, 0, 0,SWP_NOACTIVATE|
			SWP_NOSIZE|SWP_NOMOVE|SWP_NOOWNERZORDER); 
		return TRUE;
	}
	return TRUE;
}


void CReplaceDlg::OnBnClickedBntExplorer()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	MFCExt::Control::CFolderDialog dlg(CSIDL_DRIVES, _T( "" ), _T( "请选择文件夹" ) );
	if ( dlg.DoModal() == IDOK ) {
		//AfxMessageBox( dlg.GetPath() );
		m_sFileName.SetWindowText(dlg.GetPath());
	}

	UpdateData(FALSE);	
}

void CReplaceDlg::Replace()
{
	int len = m_sFileName.LineLength(m_sFileName.LineIndex(0));
	if(len ==0)
	{
		MessageBox(_T("请先选择文件夹！"),_T("操作"),MB_OK|MB_ICONINFORMATION);
		return;
	}
	getDialogData();

	FileUtil fu(sources,dests);
	fu.AccessFile(m_path,OPERATION_UPDATE,m_FileNameRegex.GetCheck(),m_SFileExt,replaceFileCount);

	CString mes;
	mes.Format(_T("成功替换 %d 个文件！"),replaceFileCount);
	//AfxMessageBox(mes,MB_YES);
	MessageBox(mes,_T("操作成功"),MB_OK|MB_ICONINFORMATION);

	replaceFileCount = 0;
}


void CReplaceDlg::Delete()
{
	int len = m_sFileName.LineLength(m_sFileName.LineIndex(0));
	if(len ==0)
	{
		MessageBox(_T("请先选择文件夹！"),_T("操作"),MB_OK|MB_ICONINFORMATION);
		return;
	}
	getDialogData();

	if(MessageBox(_T("确定删除指定文件？"),_T("删除"),MB_OKCANCEL|MB_ICONQUESTION)==IDOK)
	{
		FileUtil fu(sources,dests);
		fu.AccessFile(m_path,OPERATION_DELETE,m_FileNameRegex.GetCheck(),m_SFileExt,replaceFileCount);
	}
	else
		return ;

	CString mes;
	mes.Format(_T("成功删除 %d 个文件！"),replaceFileCount);
	//AfxMessageBox(mes,MB_YES);
	MessageBox(mes,_T("操作成功"),MB_OK|MB_ICONINFORMATION);

	replaceFileCount = 0;
}

void CReplaceDlg::Length()
{
	if(!IsExistsData())return;
	CString str;
	for(vector<CString>::iterator iter =sources.begin();iter!=sources.end();iter++)
	{
		str.Append(*iter);
	}

	std::string output = Util::Wcstombs(str);
	if(((CButton*)GetDlgItem(IDC_CHECK_SPECIAL))->GetCheck())
	{
		CString split;
		GetDlgItemText(IDC_EDIT_FILE_PATH, split);
		if(split.IsEmpty()) split = _T(",");
		CStringArray elems;
		Util::SplitStringWithEmpty(str, split, elems);
		str.Format(_T("%d"),elems.GetCount());
	}
	else
		str.Format(_T("%d"),output.length());
	OutPut(str);
}

void CReplaceDlg::Case()
{
	if(!IsExistsData())return;

	CString str;
	Case(str,sources,m_bcase,m_cbQuotes.GetCheck());

	str.Replace(_T("%S"),_T("%s"));
	str.Replace(_T("%D"),_T("%d"));
	OutPut(str);
}

void CReplaceDlg::Case(CString& result,vector<CString>&toBeFormat, bool isCase,bool isQuotes)
{
	for(vector<CString>::iterator iter =toBeFormat.begin();iter!=toBeFormat.end();iter++)
	{
		CString tmp = (isCase?(*iter).MakeUpper():(*iter).MakeLower());
		tmp.Replace(_T("\t"),_T(""));
		tmp.Replace(_T("\""),_T(""));
		tmp.TrimLeft(_T("\t"));
		tmp.TrimLeft(_T(" "));
		if(isQuotes&&!tmp.Trim().IsEmpty())
			result.AppendFormat(_T("\"%s\""),tmp);
		else
			result.Append(tmp);
		result.Append(iter!=toBeFormat.end()-1?_T("\r\n"):_T(""));
	}
}

void CReplaceDlg::Append(CString& result,vector<CString>&toBeFormat, bool isAppend)
{
	for(vector<CString>::iterator iter =toBeFormat.begin();iter!=toBeFormat.end();iter++)
	{
		CString tmp = *iter;
		if(isAppend)
			result.AppendFormat(_T(".append(\" %s \")"),tmp);
		else
		{
			tmp.Replace(_T(".append(\""), _T(""));
			tmp.Replace(_T("\")"), _T(""));
			tmp.Replace(_T("\\\""), _T("\""));
			//tmp.TrimLeft(_T(" "));
			//tmp.TrimRight(_T(" "));
			result.Append(tmp);
		}
		result.Append(iter!=toBeFormat.end()-1?_T("\r\n"):_T(""));
	}
}

void CReplaceDlg::Mac()
{
	if(!IsExistsData())return;

	CString cinput;
	Util::formatList(cinput,sources);
	cinput.Replace(_T(","),_T(""));
	if(cinput.GetLength()>999)
	{
		MessageBox(_T("输入的明文过长！"),_T("操作"),MB_OK|MB_ICONERROR);
		return;
	}
	CString output;
	MacUtil mu(sources,dests);
	mu.MacCalculate(cinput,output,m_padding.GetCheck(),m_ascii.GetCheck(),m_bntVerse.GetCheck(),m_cSub.GetCheck());

	OutPut(output);
}

void CReplaceDlg::ValidateMac()
{
	LoadFile(false);
	if(sources.size()==0)
	{
		MessageBox(_T("请先输入文本！"),_T("操作"),MB_OK|MB_ICONINFORMATION);
		return;
	}

	CString coutput;
	MacUtil mu(sources,dests);
	mu.MacValidate(coutput,m_padding.GetCheck(),m_ascii.GetCheck(),m_bntVerse.GetCheck(),m_cSub.GetCheck(),m_progressTotal);
	OutPut(coutput);
}

void CReplaceDlg::Aes()
{
	if(!IsExistsData())return;

	AesUtil aes(sources,dests);

	CString key = m_key;
	CString input = m_source;
	CString output = _T("");

	if(mt_cbAES.GetCheck())//加密
	{
		aes.Encrypt(input,key,output);

	}
	else//解密
	{
		aes.Decrypt(input,key,output);
	}

	OutPut(output);
}

void CReplaceDlg::Md5()
{
	if(!IsExistsData())return;
	MD5Util md5;

	CString key = m_key;
	CString input = m_source;

	string soutput;
	string tmp = Util::Wcstombs(input);
	tmp = Util::GBKToUtf8(tmp);
	md5.Encrypt(tmp, soutput);
	OutPut(Util::Mbstowcs(soutput));
}

void CReplaceDlg::Des()
{
	if(!IsExistsData())return;
	DESUtil des(sources,dests);

	CString key = m_key;
	CString input = m_source;
	CString output = _T("");

	string soutput;
	string tmp = Util::Wcstombs(input);
	//tmp = Util::GBKToUtf8(tmp);

	if(mt_cbAES.GetCheck())//加密
	{
		output = Util::Mbstowcs(des.Encrypt(tmp,Util::Wcstombs(key)));
	}
	else//解密
	{
		tmp = des.Decrypt(tmp,Util::Wcstombs(key));
		output = Util::Mbstowcs(Util::Utf8ToGBK(tmp));
	}

	OutPut(output);
}

void CReplaceDlg::SqlFormat()
{
	if(!IsExistsData())return;

	bool isQuotes = m_cbQuotes.GetCheck();

	CString str;
	//转换为大写，无逗号
	Case(str,sources,true,isQuotes);
	CString strOut;
	SQLFormat sf(str,isQuotes);
	sf.parseSql(strOut);

	OutPut(strOut);
}

void CReplaceDlg::JavaAppend()
{
	if(!IsExistsData())return;

	if(((CButton*)GetDlgItem(IDC_CHECK_SPECIAL))->GetCheck()) // 选中“特殊”时表示需要SQL处理
	{
		CString strOut;
		CString str;
		Util::formatList(str, sources, _T("\r\n"));
		SQLFormat sf(str, ((CButton*)GetDlgItem(IDC_CHECK_QUOTES))->GetCheck()); // 表示是否需要给别名添加双引号
		bool result = sf.formatSql(strOut);
		if(!result)
		{
			MessageBox(_T("不是标准的SQL语句，无法处理！"),_T("操作"),MB_OK|MB_ICONINFORMATION);
			return;
		}
		sources.clear();
		Util::parseList(sources, strOut, _T("\r\n"));
	}

	if(((CButton*)GetDlgItem(IDC_CHECK_APPEND))->GetCheck())
	{
		CString strOut;
		Append(strOut, sources, true);
		OutPut(strOut);
	}
	else
	{
		CString strOut;
		Append(strOut, sources, false);
		OutPut(strOut);
	}
}

void CReplaceDlg::HexParse()
{
	if(!IsExistsData())return;

	CString strInput,strOutput;
	Util::formatList(strInput,sources);

	bool isByte = m_cbByte.GetCheck();
	bool isHighToLower = m_cbHightToLower.GetCheck();

	HexParseUtil hex;
	if(!isByte)
	{
		hex.formatHexToBcd(strInput,strOutput,isHighToLower);
	}
	else
	{
		hex.formatBcdToHex(strInput,strOutput,isHighToLower);
	}

	OutPut(strOutput);
}

void CReplaceDlg::StringCat()
{
	if(!IsExistsData())return;

	CString strOut;
	//
	if(((CButton*)GetDlgItem(IDC_CHECK_SPECIAL))->GetCheck())
	{
		CString split(_TEXT(""));
		if(!m_path.IsEmpty()) 
			split = m_path;
		//content body
		CString contentBodyExport;
		CString contentBody;
		CString convertBody;
		CString convertStd;

		int i = 0;
		for(vector<CString>::iterator it = sources.begin(); it != sources.end(); it ++, i++)
		{
			CString temp = *it;
			temp = temp.MakeUpper();
			temp.Replace(_T(" NOT NULL"), _T(""));
			temp.Replace(_T(" NULL"), _T(""));

			CStringArray ca;
			Util::SplitString(temp, _T(" "), ca);
			//如果存在二项为以下方式：ID CHAR(30)NOT NULL; 去除NOT NULL 和空格后为 ID和CHAR(30);
			//如果存在三项为以下方式：ID CHAR(30) NOT NULL;去除NOT NULL 和空格后为 ID和CHAR(30)和;
			if(ca.GetSize() >= 2)
			{
				bool isDate = false;
				CString temp2 = ca[1];
				if(((CButton*)GetDlgItem(IDC_CHECK_EMPTY))->GetCheck())
					temp2 = temp2.Trim();
				if(ca.GetSize() == 3)
					temp2.Append(ca[2].Trim());
				if(temp2.Find(_T("VARCHAR")) != -1)	{
					convertBody.Append(_T("BCD"));
					convertStd.Append(_T("string"));
					if(it != sources.end() - 1)
					{
						convertBody.Append(_T(","));
						convertStd.Append(_T(","));
					}
				}else if(temp2.Find(_T("CHAR")) != -1){
					temp2.Replace(_T("CHAR"), _T("BCD"));
					convertBody.Append(temp2);
					convertStd.Append(_T("string"));
					if(it != sources.end() - 1)
						convertStd.Append(_T(","));
				}else if(temp2.Find(_T("INTEGER")) != -1
					||temp2.Find(_T("INT")) != -1
					||temp2.Find(_T("NUMBER")) != -1)
				{
					convertBody.Append(_T("BCD"));
					convertStd.Append(_T("int"));
					if(it != sources.end() - 1)
					{
						convertBody.Append(_T(","));
						convertStd.Append(_T(","));
					}

				}else if(temp2.Find(_T("DATE")) != -1){
					convertBody.Append(_T("BCD(14)"));
					convertStd.Append(_T("date"));
					if(it != sources.end() - 1)
					{
						convertBody.Append(_T(","));
						convertStd.Append(_T(","));
					}
					isDate = true;
				}else {
					convertBody.Append(_T("BCD"));
					convertStd.Append(_T("string"));
					if(it != sources.end() - 1)
					{
						convertBody.Append(_T(","));
						convertStd.Append(_T(","));
					}
				}

				CString temp1 = ca[0];
				if(((CButton*)GetDlgItem(IDC_CHECK_EMPTY))->GetCheck())
					temp1 = temp1.Trim();
				if(isDate){
					contentBodyExport.Append(_T("TO_CHAR("));
					contentBodyExport.Append(temp1);
					contentBodyExport.Append(_T(",'YYYYMMDDHH24MISS')"));
				}
				else{
					contentBodyExport.Append(temp1);
				}

				contentBody.Append(temp1);
				if(it != sources.end() - 1)
				{
					contentBodyExport.Append(split);
					contentBody.Append(split);
				}
			}			
		}	

		strOut.Append(contentBodyExport);
		strOut.Append(_T("\r\n"));
		strOut.Append(_T("\r\n"));
		strOut.Append(contentBody);
		strOut.Append(_T("\r\n"));
		strOut.Append(convertBody);
		strOut.Append(_T("\r\n"));
		strOut.Append(convertStd);
		strOut.Append(_T("\r\n"));

	}
	else {
		CString split(_TEXT(""));
		CString oper(_TEXT(""));
		CString returnStr(_T(""));
		if(!m_path.IsEmpty()) 
			split = m_path;

		if(((CButton*)GetDlgItem(IDC_CHECK_ROWNUMBER))->GetCheck()){
			returnStr = _T("\r\n");
		}
		int index = ((CComboBox *)GetDlgItem(IDC_COMBO_OPER))->GetCurSel();
		switch(index)
		{
		case 0:
			oper = _TEXT("");
			break;
		case 1:
			oper = _TEXT("\"");
			break;
		case 2:
			oper = _TEXT("'");
			break;
		}

		//判断字符串是否要做切割
		if(sources.size() == 1)
		{
			CString temp = sources[0];
			CStringArray array;
			Util::SplitStringWithEmpty(temp, split, array); 
			sources.clear();
			for(int i = 0; i < array.GetSize(); i ++)
			{
				sources.push_back(array.GetAt(i));
			}
		}

		for(vector<CString>::iterator it = sources.begin(); it != sources.end(); it ++)
		{
			CString temp = *it;
			CString tmp;
			if(((CButton*)GetDlgItem(IDC_CHECK_EMPTY))->GetCheck())
				temp = temp.Trim();
			if(!temp.IsEmpty() && temp.Right(1) == split)
				temp.Remove(split[0]);
			tmp.Format(_TEXT("%s%s%s"), oper, temp, oper);
			if(it != sources.end() - 1)
				strOut += tmp + split + returnStr;
			else
				strOut += tmp;
		}
	}
	OutPut(strOut);
}

void CReplaceDlg::ValidatePurchase()
{
	if(!IsExistsData())return;

	CString coutput;
	FileValidator fv(sources,dests);
	fv.PurcharseValidate(coutput,m_progressTotal);
	OutPut(coutput);
}

void CReplaceDlg::ValidateLoadDx()
{
	if(!IsExistsData())return;

	CString coutput;
	FileValidator fv(sources,dests);
	fv.LoadValidateDX(coutput,m_progressTotal);
	OutPut(coutput);
}

void CReplaceDlg::ValidateLoadOnline()
{
	if(!IsExistsData())return;

	CString coutput;
	FileValidator fv(sources,dests);
	fv.LoadValidateOnline(coutput,m_progressTotal);
	OutPut(coutput);
}


void CReplaceDlg::SqlDate()
{
	getDialogData();
	CString   strDate; 
	CString   strTime;
	if(sources.size()==0||sources[0]=="")
	{
		GetDateTimeString(strDate,strTime);

		if(m_cbQuotes.GetCheck())
		{
			strDate.Append(_T(" "));
			strDate.Append(strTime);
		}

		m_sSource.SetWindowText(strDate);
		UpdateData(FALSE);
	}
	else
	{
		strDate = sources[0];
	}

	CString strOutput;
	if(!m_cbQuotes.GetCheck())
		strOutput.Format(_T("TO_DATE('%s','YYYY-MM-DD')"),strDate);
	else
		strOutput.Format(_T("TO_DATE('%s','YYYY-MM-DD HH24:MI:SS')"),strDate);

	OutPut(strOutput);
}

void CReplaceDlg::GetDateTimeString(CString& date,CString& time)
{
	COleDateTime   TD; 
	TD = COleDateTime::GetCurrentTime();
	date.Format(_T("%d-%d-%d"),TD.GetYear(),TD.GetMonth(),TD.GetDay()); 
	time.Format(_T("%02d:%02d:%02d"),TD.GetHour(),TD.GetMinute(),TD.GetSecond());
}

void CReplaceDlg::getDialogData()
{
	m_sFileName.GetWindowText(m_path);
	m_sFlieExt.GetWindowText(m_SFileExt);
	m_sSource.GetWindowText(m_source);
	m_sDest.GetWindowText(m_dest);
	m_sKey.GetWindowText(m_key);

	sources.clear();
	GetEditLines(m_sSource,sources);
	dests.clear();
	GetEditLines(m_sDest,dests);

	m_SFileExt = m_SFileExt.MakeLower();
	m_bcase = m_cSub.GetCheck();

	//CButton *pButton = (CButton *)GetDlgItem(IDC_BUTTON_CASE);
	//BOOL b =  pButton->GetCheck();

	//DWORD dwState = ((CButton*)GetDlgItem(IDC_BUTTON_CASE))->GetCheck();
	//if(dwState==BST_CHECKED)
	//	m_bcase=false;
	//else 
	//	m_bcase=true;
}

void CReplaceDlg::GetEditLines(CEdit& ced,std::vector<CString>& sources)
{
	/*int i,len = 0;
	TCHAR buf[8000];

	for(i= 0;i<ced.GetLineCount();i++)
	{
		len = ced.LineLength(ced.LineIndex(i));
		memset(buf,0,sizeof(buf));
		ced.GetLine(i,buf,len);
		sources.push_back(CString(buf));
	}*/
	int i,len = 0;
	TCHAR* buf;

	for(i= 0;i<ced.GetLineCount();i++)
	{
		len = ced.LineLength(ced.LineIndex(i));
		buf = new TCHAR[len + 1];
		memset(buf, 0, len + 1);
		len = ced.GetLine(i, buf, len);
		sources.push_back(CString(buf, len));
		delete[] buf;
	}
}

LRESULT CReplaceDlg::OnHotKey(WPARAM wParam,LPARAM lParam)            
{   
	UINT fuModifiers = (UINT) LOWORD(lParam); // key-modifier flags    
	UINT uVirtKey = (UINT) HIWORD(lParam);     // virtual-key code    

	//判断响应了什么热键   
	if( MOD_CONTROL == fuModifiers && 'A' == uVirtKey )   
	{   

	}   
	else if( MOD_CONTROL == fuModifiers && 'D' == uVirtKey )   
	{   


	}   
	else 
		AfxMessageBox(_T("你按下了未知热键"));     

	return 0;           
}   




void CReplaceDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	UINT count;
	TCHAR filePath[200];
	files.clear();
	count = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	if(count)
	{        
		for(UINT i=0; i<count; i++)
		{
			int pathLen = DragQueryFile(hDropInfo, i, filePath, sizeof(filePath));
			files.push_back(CString(filePath));
		}
	}
	DragFinish(hDropInfo);
	LoadFile(true);

	CDialog::OnDropFiles(hDropInfo);
}

void CReplaceDlg::LoadFile(bool isDisplay)
{
	sources.clear();
	dests.clear();
	FileUtil fu(sources,dests);
	for(vector<CString>::iterator iter =files.begin();iter!=files.end();iter++)
	{
		m_sFileName.SetWindowText(*iter);
		fu.ReadFileContent(*iter);
		if(isDisplay)
			DisplayText();
	}
}

void CReplaceDlg::DisplayText()
{
	m_progressTotal.SetRange(0,(short)sources.size());
	m_progressTotal.SetStep(1);
	CString str;
	int count = 1;
	for(vector<CString>::iterator iter =sources.begin();iter!=sources.end();iter++)
	{
		CString mes;
		if(m_cRownumber.GetCheck())
			mes.Format(_T("第%08d行 "),count++);
		str.Append(mes + (*iter)+(iter!=sources.end()-1?_T("\r\n"):_T("")));
		m_progressTotal.StepIt();
	}
	m_sSource.SetWindowText(str);
	UpdateData(FALSE);
}

void CReplaceDlg::ProgressStepIt()
{
	m_progressTotal.StepIt();
}

void CReplaceDlg::InitProgressStep(short size)
{
	m_progressTotal.SetRange(0,size);
	m_progressTotal.SetStep(1);
}

bool CReplaceDlg::IsExistsData()
{
	getDialogData();

	if(sources.size()==0||sources[0]=="")
	{
		MessageBox(_T("请先输入文本！"),_T("操作"),MB_OK|MB_ICONINFORMATION);
		return false;
	}

	return true;
}
void CReplaceDlg::OutPut(const CString& coutput)
{
	m_sDest.SetWindowText(coutput);
	UpdateData(FALSE);
}

void CReplaceDlg::Append(const CString& coutput)
{
	int  nLength  =  (int)m_sDest.SendMessage(WM_GETTEXTLENGTH);
	m_sDest.SetSel(nLength, nLength);
	m_sDest.ReplaceSel(coutput); 
	UpdateData(FALSE);
}

BOOL CReplaceDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == 'A' && IsCTRLPressed())
	{
		if(m_sSource.GetFocus())
		{
			m_sSource.SetSel(0,-1);
		}

		if(m_sDest.GetFocus())
		{
			m_sDest.SetSel(0,-1);
		}

		return FALSE;
	}

	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == 'D' && IsCTRLPressed())
	{
		if(m_sSource.GetFocus())
		{
			m_sSource.SetFocus();
			m_sSource.Clear();  
		}

		if(m_sDest.GetFocus())
		{
			m_sDest.SetFocus();
			m_sDest.Clear();  
		}	

		return FALSE;
	}

	return CDialog::PreTranslateMessage(pMsg);

}

void CReplaceDlg::WriteTxtFile()
{
	if(!IsExistsData())return;

	CString path;
	CFile   file;   
	CString input;

	if(m_path == "")
	{
		CString   strDate; 
		CString   strTime;

		GetDateTimeString(strDate,strTime);

		input = strDate+_T(",")+sources[0]+_T("\r\n");
		std::string output = Util::Wcstombs(input);

		path = _T("log.csv");

		if(!file.Open(path,CFile::modeReadWrite))   
		{   
			file.Open(path,CFile::modeCreate|CFile::modeReadWrite);   
		}   
		file.SeekToEnd();
		file.Write(output.c_str(),(UINT)output.size());

	}
	else
	{
		path = m_path;
		FileUtil fu(sources,dests);

		file.Open(path,CFile::modeWrite);  
		file.SetLength(0);

		for(vector<CString>::iterator iter=sources.begin();iter!=sources.end();iter++)
		{
			CString tmp = *iter + _T("\r\n");
			file.SeekToEnd();
			std::string output = Util::Wcstombs(tmp);
			file.Write(output.c_str(),(UINT)output.size());
			file.Flush();
			input.Append(tmp);
		}
	}

	file.Close();
	OutPut(input);

	//CArchive ar(&mFile,CArchive::store);
	//ar.WriteString(strDate+_T(",")+sources[0]);
	//	//<<  strDate
	//	//<<  _T(",")
	//	//<<  sources[0];
	//ar.Flush();
	//ar.Close();

}

void CReplaceDlg::EnableProxy()
{
	//下面的代码将注册表项HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\\ProxyServer 
	//的内容取出来 
	RegeditEditor reg;

	CString proxyServer = _T("178.213.33.129:25");//178.213.33.129:25
	int proxyEnable = 0x00000001;

	//GetDlgItem(ID_ENABLEPROXY)
	CMenu* pMenu = &m_menu;
	CMenu*   subMenu = pMenu->GetSubMenu(2);
	if(subMenu->GetMenuState(ID_ENABLEPROXY,MF_BYCOMMAND)==MF_CHECKED) 
	{ 
        //如果菜单是check状态
		subMenu->CheckMenuItem(ID_ENABLEPROXY,MF_UNCHECKED|MF_BYCOMMAND); 
		proxyEnable = 0x00000000;
	} 
   else 
   { 
		subMenu->CheckMenuItem(ID_ENABLEPROXY,MF_CHECKED|MF_BYCOMMAND); 
   }  

	CString subKey = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings");

	//CString input = sources[0];
	//std::string strProxyServer("");
	//std::string strProxyEnable("");
	//Util::Wcstombs(proxyServer,strProxyServer);
	//Util::Wcstombs(proxyEnable,strProxyEnable);

	if(!reg.UpdateRegedit(subKey,_T("ProxyServer"),proxyServer))
	{
		MessageBox(_TEXT("无法修改键值ProxyServer"));
		return ;
	}
	if(!reg.UpdateRegedit(subKey,_T("ProxyEnable"),proxyEnable))
	{
		MessageBox(_TEXT("无法修改键值ProxyEnable"));
		return ;
	}



}

void CReplaceDlg::OnAbout()
{
	// TODO: 在此添加命令处理程序代码
	CAboutDlg dlg;
	dlg.DoModal();
}

void CReplaceDlg::OnMac()
{
	// TODO: 在此添加命令处理程序代码
	UpdateData(TRUE);
	Mac();
	UpdateData(FALSE);
}
void CReplaceDlg::OnDes()
{
	// TODO: 在此添加命令处理程序代码
	UpdateData(TRUE);
	Des();
	UpdateData(FALSE);
}

void CReplaceDlg::OnAes()
{
	// TODO: 在此添加命令处理程序代码
	UpdateData(TRUE);
	Aes();
	UpdateData(FALSE);
}

void CReplaceDlg::OnMd5()
{
	// TODO: 在此添加命令处理程序代码
	UpdateData(TRUE);
	Md5();
	UpdateData(FALSE);
}


void CReplaceDlg::OnCase()
{
	// TODO: 在此添加命令处理程序代码
	UpdateData(TRUE);
	Case();
	UpdateData(FALSE);
}

void CReplaceDlg::OnLength()
{
	// TODO: 在此添加命令处理程序代码
	UpdateData(TRUE);
	Length();
	UpdateData(FALSE);
}

void CReplaceDlg::OnByte()
{
	// TODO: 在此添加命令处理程序代码
	UpdateData(TRUE);
	HexParse();
	UpdateData(FALSE);
}

void CReplaceDlg::OnSqldate()
{
	// TODO: 在此添加命令处理程序代码
	UpdateData(TRUE);
	SqlDate();
	UpdateData(FALSE);
}

void CReplaceDlg::OnSqlformat()
{
	// TODO: 在此添加命令处理程序代码
	UpdateData(TRUE);
	SqlFormat();
	UpdateData(FALSE);
}

void CReplaceDlg::OnReplace()
{
	// TODO: 在此添加命令处理程序代码
	UpdateData(TRUE);
	Replace();
	UpdateData(FALSE);
}

void CReplaceDlg::OnDelete()
{
	// TODO: 在此添加命令处理程序代码
	UpdateData(TRUE);
	Delete();
	UpdateData(FALSE);
}

void CReplaceDlg::OnValidate()
{
	// TODO: 在此添加命令处理程序代码
	UpdateData(TRUE);
	ValidateMac();
	UpdateData(FALSE);
}

void CReplaceDlg::OnRecord()
{
	// TODO: 在此添加命令处理程序代码
	UpdateData(TRUE);
	WriteTxtFile();
	UpdateData(FALSE);
}

void CReplaceDlg::OnEnableproxy()
{
	// TODO: 在此添加命令处理程序代码
	UpdateData(TRUE);
	//EnableProxy();
	UpdateData(FALSE);
}

void CReplaceDlg::OnQuit()
{
	// TODO: 在此添加命令处理程序代码
	PostMessage(WM_CLOSE);
}

void CReplaceDlg::OnStringcat()
{
	// TODO: 在此添加命令处理程序代码
	UpdateData(TRUE);
	StringCat();
	UpdateData(FALSE);
}

void CReplaceDlg::OnOracleLogin()
{
	// TODO: 在此添加命令处理程序代码
	if(!IsExistsData())return;
	if(sources.size() != 3)
	{
		m_sSource.SetWindowText(_T("请在此处输入数据库实例\r\n请在此处输入用户名\r\n请在此处输入密码"));
		return ;
	}

	string dbName = Util::Wcstombs(sources[0]);
	string userName = Util::Wcstombs(sources[1]);
	string password = Util::Wcstombs(sources[2]);

	try
	{
		Poco::Environment::set("NLS_DATE_FORMAT", "YYYY-MM-DD HH24:MI:SS");
		CSession session(new OraSession());
		session.Connect( userName, password, dbName);
		if( session.IsConnected() ) {
			session.Close();
		}
		Util::getDBConnectInfo().set(dbName, userName, password);
		OutPut(_T("登录成功"));

	} catch ( DbException& ex ) {
		OutPut(_T("登录失败！数据库异常：") + Util::Mbstowcs(ex.ErrMsg()));
	}


}

void CReplaceDlg::onBegin(const void* pSender, ICMPEventArgs& args)
{
	std::ostringstream os;
	os << "Pinging " << args.hostName() << " [" << args.hostAddress() << "] with " << args.dataSize() << " bytes of data:" 
	   << "\r\n" << "---------------------------------------------" << "\r\n";
	Append(Util::Mbstowcs(os.str()));
}

void CReplaceDlg::onReply(const void* pSender, ICMPEventArgs& args)
{
	std::ostringstream os;
	os << "Reply from " << args.hostAddress()
	   << " bytes=" << args.dataSize() 
	   << " time=" << args.replyTime() << "ms"
	   << " TTL=" << args.ttl() << "\r\n";
	Append(Util::Mbstowcs(os.str()));
}

void CReplaceDlg::onError(const void* pSender, ICMPEventArgs& args)
{
	std::ostringstream os;
	os << args.error() << "\r\n";
	Append(Util::Mbstowcs(os.str()));
}

void CReplaceDlg::onEnd(const void* pSender, ICMPEventArgs& args)
{
	std::ostringstream os;
	os << "\r\n" << "--- Ping statistics for " << args.hostName() << " ---"
	   << "\r\n" << "Packets: Sent=" << args.sent() << ", Received=" << args.received()
	   << " Lost=" << args.repetitions() - args.received() << " (" << 100.0 - args.percent() << "% loss),"
	   << "\r\n" << "Approximate round trip times in milliseconds: " << "\r\n"
	   << "Minimum=" << args.minRTT() << "ms, Maximum=" << args.maxRTT()  
	   << "ms, Average=" << args.avgRTT() << "ms" 
	   << "\r\n" << "------------------------------------------";
	Append(Util::Mbstowcs(os.str()));
}

void CReplaceDlg::OnPing()
{
	// TODO: 在此添加命令处理程序代码
	if(!IsExistsData())return;
	ICMPClient  _icmpClient(IPAddress::IPv4);
	_icmpClient.pingBegin += Delegate<CReplaceDlg, ICMPEventArgs>(this, &CReplaceDlg::onBegin);
	_icmpClient.pingReply += Delegate<CReplaceDlg, ICMPEventArgs>(this, &CReplaceDlg::onReply);
	_icmpClient.pingError += Delegate<CReplaceDlg, ICMPEventArgs>(this, &CReplaceDlg::onError);
	_icmpClient.pingEnd   += Delegate<CReplaceDlg, ICMPEventArgs>(this, &CReplaceDlg::onEnd);
	_icmpClient.ping(Util::Wcstombs(sources[0]), 4);
	_icmpClient.pingBegin -= Delegate<CReplaceDlg, ICMPEventArgs>(this, &CReplaceDlg::onBegin);
	_icmpClient.pingReply -= Delegate<CReplaceDlg, ICMPEventArgs>(this, &CReplaceDlg::onReply);
	_icmpClient.pingError -= Delegate<CReplaceDlg, ICMPEventArgs>(this, &CReplaceDlg::onError);
	_icmpClient.pingEnd   -= Delegate<CReplaceDlg, ICMPEventArgs>(this, &CReplaceDlg::onEnd);
}

void CReplaceDlg::OnFtp()
{
	// TODO: 在此添加命令处理程序代码
	if(!IsExistsData())return;
	if(sources.size() != 3)
	{
		m_sSource.SetWindowText(_T("请在此处输入FTP地址\r\n请在此处输入用户名\r\n请在此处输入密码"));
		return ;
	}
	try
	{
		Poco::Net::FTPClientSession session(Util::Wcstombs(sources[0]), 21);
		session.setTimeout(Poco::Timespan(5, 0));
		session.login(Util::Wcstombs(sources[1]), Util::Wcstombs(sources[2]));
		session.close();
		OutPut(_T("登录成功"));
	}
	catch(Poco::Net::ConnectionRefusedException& cex)
	{
		OutPut(_T("登录失败！连接异常：") + Util::Mbstowcs(cex.message()));
	}
	catch(Poco::Net::FTPException& fex)
	{
		OutPut(_T("登录失败！FTP异常：") + Util::Mbstowcs(fex.message()));
	}
	catch(Poco::Net::NetException& nex)
	{
		OutPut(_T("登录失败！网络异常：") + Util::Mbstowcs(nex.message()));
	}
}

#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/IPAddress.h"

void CReplaceDlg::OnIpaddress()
{
	// TODO: 在此添加命令处理程序代码
	Append(Util::Mbstowcs(IPHelper::GetHostName()));
	Append(Util::Mbstowcs(IPHelper::GetHostIPAddress()));
	Append(Util::Mbstowcs(IPHelper::GetMacAddress()));
}

void CReplaceDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (nType==SIZE_RESTORED||nType==SIZE_MAXIMIZED)  
    {  
        ReSize();  
    }  
}

void CReplaceDlg::ReSize()  
{  
    float fsp[2];  
    POINT Newp; //获取现在对话框的大小  
    CRect recta;      
    GetClientRect(&recta);     //取客户区大小    
    Newp.x=recta.right-recta.left;  
    Newp.y=recta.bottom-recta.top;  
    fsp[0]=(float)Newp.x/old.x;  
    fsp[1]=(float)Newp.y/old.y;  
    CRect Rect;  
    int woc;  
    CPoint OldTLPoint,TLPoint; //左上角  
    CPoint OldBRPoint,BRPoint; //右下角  
    HWND  hwndChild=::GetWindow(m_hWnd,GW_CHILD);  //列出所有控件    
    while(hwndChild)      
    {      
        woc=::GetDlgCtrlID(hwndChild);//取得ID  
        GetDlgItem(woc)->GetWindowRect(Rect);    
        ScreenToClient(Rect);    
        OldTLPoint = Rect.TopLeft();    
        TLPoint.x = long(OldTLPoint.x*fsp[0]);    
        TLPoint.y = long(OldTLPoint.y*fsp[1]);    
        OldBRPoint = Rect.BottomRight();    
        BRPoint.x = long(OldBRPoint.x *fsp[0]);    
        BRPoint.y = long(OldBRPoint.y *fsp[1]);    
        Rect.SetRect(TLPoint,BRPoint);    
        GetDlgItem(woc)->MoveWindow(Rect,TRUE);  
        hwndChild=::GetWindow(hwndChild, GW_HWNDNEXT);      
    }  
    old=Newp;  
}  

void CReplaceDlg::OnPurchaseValidate()
{
	// TODO: 在此添加命令处理程序代码
	UpdateData(TRUE);
	ValidatePurchase();
	UpdateData(FALSE);
}

void CReplaceDlg::OnLoaddxValidate()
{
	// TODO: 在此添加命令处理程序代码
	UpdateData(TRUE);
	ValidateLoadDx();
	UpdateData(FALSE);
}

void CReplaceDlg::OnLoadonlineValidate()
{
	// TODO: 在此添加命令处理程序代码
	UpdateData(TRUE);
	ValidateLoadOnline();
	UpdateData(FALSE);
}

void CReplaceDlg::OnConifgs001()
{
	// TODO: 在此添加命令处理程序代码
	CS001Dlg dlg;
	dlg.DoModal();
}

void CReplaceDlg::OnHSM()
{
	CHsmIPAdressDlg dlg;
	if(dlg.DoModal() == IDOK)
	{
		m_sIpAddress = dlg.GetIPAddress();
		m_sIpPort = dlg.GetIPPort();
	}
}

void CReplaceDlg::OnPurcharseMac1()
{
	CHsm hsm;
	hsm.SetOperType(CHsm::PURCHARSE_MAC1);
	hsm.DoModal();
}

void CReplaceDlg::OnPurcharseMac2()
{
	CHsm hsm;
	hsm.SetOperType(CHsm::PURCHARSE_MAC2);
	hsm.DoModal();
}

void CReplaceDlg::OnPurcharseTac()
{
	CHsm hsm;
	hsm.SetOperType(CHsm::PURCHARSE_TAC);
	hsm.DoModal();
}

void CReplaceDlg::OnLoadMac1()
{
	CHsm hsm;
	hsm.SetOperType(CHsm::LOAD_MAC1);
	hsm.DoModal();
}

void CReplaceDlg::OnLoadMac2()
{
	CHsm hsm;
	hsm.SetOperType(CHsm::LOAD_MAC2);
	hsm.DoModal();
}

void CReplaceDlg::OnLoadTac()
{
	CHsm hsm;
	hsm.SetOperType(CHsm::LOAD_TAC);
	hsm.DoModal();
}

void CReplaceDlg::OnSafeTac()
{
	CHsm hsm;
	hsm.SetOperType(CHsm::SAFE_MAC);
	hsm.DoModal();
}

void CReplaceDlg::OnM1KeyA()
{
	CHsm hsm;
	hsm.SetOperType(CHsm::M1_KEYA);
	hsm.DoModal();
}

void CReplaceDlg::OnM1KeyB()
{
	CHsm hsm;
	hsm.SetOperType(CHsm::M1_KEYB);
	hsm.DoModal();
}

void CReplaceDlg::OnM1TAC()
{
	CHsm hsm;
	hsm.SetOperType(CHsm::M1_TAC);
	hsm.DoModal();
}

void CReplaceDlg::OnJavasqlappend()
{
	// TODO: 在此添加命令处理程序代码
	UpdateData(TRUE);
	JavaAppend();
	UpdateData(FALSE);
}


void CReplaceDlg::OnBnClickedButtonReverse()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_sSource.GetWindowText(m_source);
	m_sDest.GetWindowText(m_dest);
	m_sSource.SetWindowTextW(m_dest);
	m_sDest.SetWindowTextW(m_source);
	UpdateData(FALSE);
}



void CReplaceDlg::OnUnicode()
{
	UpdateData(TRUE);

    if(!IsExistsData())return;
	CString strOut;
	if(((CButton*)GetDlgItem(IDC_CHECK_APPEND))->GetCheck()) { //Unicode to 中文
			
		for(std::vector<CString>::iterator it = sources.begin(); it != sources.end(); it++)
		{
			std::string unicode;
			StringUtil::stringToUnicode(Util::Wcstombs(*it), unicode);
			strOut.Append(Util::Mbstowcs(unicode));
			strOut.Append(_T("\r\n"));
		}
	}
	else { // 中文 to Unicode

	}
	OutPut(strOut);



	UpdateData(FALSE);
}


void CReplaceDlg::OnMenuGenerator()
{
	// TODO: 在此添加命令处理程序代码
	CGeneratorDlg dlg;
	dlg.DoModal();
}
