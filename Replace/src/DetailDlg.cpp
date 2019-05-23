// DetailDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Replace.h"
#include "DetailDlg.h"


// CDetailDlg �Ի���

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


// CDetailDlg ��Ϣ�������
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

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CDetailDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if (nType==SIZE_RESTORED||nType==SIZE_MAXIMIZED)  
    {  
        ReSize();  
    }  
}

void CDetailDlg::ReSize()  
{  
    float fsp[2];  
    POINT Newp; //��ȡ���ڶԻ���Ĵ�С  
    CRect recta;      
    GetClientRect(&recta);     //ȡ�ͻ�����С    
    Newp.x=recta.right-recta.left;  
    Newp.y=recta.bottom-recta.top;  
    fsp[0]=(float)Newp.x/old.x;  
    fsp[1]=(float)Newp.y/old.y;  
    CRect Rect;  
    int woc;  
    CPoint OldTLPoint,TLPoint; //���Ͻ�  
    CPoint OldBRPoint,BRPoint; //���½�  
    HWND  hwndChild=::GetWindow(m_hWnd,GW_CHILD);  //�г����пؼ�    
    while(hwndChild)      
    {      
        woc=::GetDlgCtrlID(hwndChild);//ȡ��ID  
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

	// TODO: �ڴ˴������Ϣ����������
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