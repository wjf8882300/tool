// DetailDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Replace.h"
#include "DetailDlg.h"


// CDetailDlg 对话框

IMPLEMENT_DYNAMIC(CDetailDlg, CDialog)

CDetailDlg::CDetailDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDetailDlg::IDD, pParent)
{

}

CDetailDlg::~CDetailDlg()
{
}

void CDetailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDetailDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDetailDlg 消息处理程序
BOOL CDetailDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetDlgItemText(IDC_EDIT_DETAIL, m_txt);
	UpdateData(FALSE);

	CWnd    *pEdit   =   GetDlgItem(IDC_EDIT_DETAIL);
	CFont   *pfont   =   pEdit->GetFont();
	LOGFONT   logfont;
	pfont->GetLogFont(&logfont);
	logfont.lfHeight   =   -16;
	m_font.CreateFontIndirect(&logfont);
	pEdit->SetFont(&m_font);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDetailDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (nType==SIZE_RESTORED||nType==SIZE_MAXIMIZED)  
    {  
        ReSize();  
    }  
}

void CDetailDlg::ReSize()  
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

void CDetailDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	m_font.DeleteObject();
}

BOOL CDetailDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == 'A' && IsCTRLPressed())
	{
		if(((CEdit*)GetDlgItem(IDC_EDIT_DETAIL))->GetFocus())
		{
			((CEdit*)GetDlgItem(IDC_EDIT_DETAIL))->SetSel(0,-1);
		}

		return FALSE;
	}

	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == 'D' && IsCTRLPressed())
	{
		if(((CEdit*)GetDlgItem(IDC_EDIT_DETAIL))->GetFocus())
		{
			((CEdit*)GetDlgItem(IDC_EDIT_DETAIL))->SetFocus();
			((CEdit*)GetDlgItem(IDC_EDIT_DETAIL))->Clear();  
		}	

		return FALSE;
	}

	return CDialog::PreTranslateMessage(pMsg);

}