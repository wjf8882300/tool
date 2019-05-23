// HsmIPAdressDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Replace.h"
#include "HsmIPAdressDlg.h"
#include <fstream>
#include "util/Util.h"
// CHsmIPAdressDlg �Ի���

#define WM_MESSAGE_ERROR_DISPLAY (WM_USER + 0x001) 

IMPLEMENT_DYNAMIC(CHsmIPAdressDlg, CDialog)

CHsmIPAdressDlg::CHsmIPAdressDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHsmIPAdressDlg::IDD, pParent)
{

}

CHsmIPAdressDlg::~CHsmIPAdressDlg()
{
}

void CHsmIPAdressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHsmIPAdressDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CHsmIPAdressDlg::OnBnClickedOk)
	//ON_BN_CLICKED(IDC_BUTTON_TEST, &CHsmIPAdressDlg::OnBnClickedButtonTest)
	ON_MESSAGE(WM_MESSAGE_ERROR_DISPLAY, &CHsmIPAdressDlg::OnErrorDisplay)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CHsmIPAdressDlg ��Ϣ�������

void CHsmIPAdressDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItemText(IDC_IPADDRESS_HSM, m_sIpAddress);
	GetDlgItemText(IDC_EDIT_PORT, m_sIpPort);

	CString strPath;
	strPath.Append(_T("C:\\hsm.conf"));

	std::ofstream fos(Util::Wcstombs(strPath).c_str());
	if(!fos) return;

	fos << "MAIN=" <<Util::Wcstombs(m_sIpAddress) << std::endl;
	fos << "PORT=" <<Util::Wcstombs(m_sIpPort) << std::endl;
	fos << "TIMEOUT=5" << std::endl;
	fos.close();
	
	OnOK();
}

BOOL CHsmIPAdressDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	CString strPath;
	strPath.Append(_T("C:\\hsm.conf"));

	std::ifstream in(Util::Wcstombs(strPath).c_str());
	if(!in)
		return TRUE;
	std::string content;
	int index = 0;
	for(;!in.eof();content.clear(), index ++ )
	{
		std::getline(in, content);
		if(content.empty())continue;
		if(index == 0)
		{
			CStringArray arr;
			Util::SplitString(Util::Mbstowcs(content), _T("="), arr);
			if(arr.GetSize() == 2)
				m_sIpAddress = arr[1];
		}
		else if(index == 1)
		{
			CStringArray arr;
			Util::SplitString(Util::Mbstowcs(content), _T("="), arr);
			if(arr.GetSize() == 2)
				m_sIpPort = arr[1];
		}
	}

	in.close();

	SetDlgItemText(IDC_IPADDRESS_HSM, m_sIpAddress);
	SetDlgItemText(IDC_EDIT_PORT, m_sIpPort);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

//#include "fml32.h"
//#include <stdio.h>
//#include <stdlib.h>
//#include "atmi.h"               /* TUXEDO  Header File */
//#include "fml.h"
//#include "BankInterface.flds.h"
//#pragma comment(lib, "wtuxws32.lib")
//void CHsmIPAdressDlg::OnBnClickedButtonTest()
//{
//	GetDlgItemText(IDC_IPADDRESS_HSM, m_sIpAddress);
//	GetDlgItemText(IDC_EDIT_PORT, m_sIpPort);
//	if(m_sIpAddress.IsEmpty() || m_sIpPort.IsEmpty())
//	{
//		MessageBox(_T("IP��ַ���߶˿ڲ���Ϊ��"), _T("Tuxedo��ͨ����"));
//		return;
//	}
//
//	GetDlgItem(IDC_BUTTON_TEST)->EnableWindow(FALSE);
//	GetDlgItem(IDOK)->EnableWindow(FALSE);
//	m_flag = false;
//
//	m_tagIpAddress.ipAddress = m_sIpAddress;
//	m_tagIpAddress.ipPort    = m_sIpPort;
//	m_tagIpAddress.hWnd      = this->GetSafeHwnd();
//	m_tagIpAddress.errMessage= &m_errMessage;
//	m_tagIpAddress.flag      = &m_flag;
//
//	DWORD dwThreadID = 0;
//	HANDLE hThread = CreateThread(NULL,0,&CHsmIPAdressDlg::TuxedoTest, (LPVOID)&m_tagIpAddress, 0, &dwThreadID);
//}
//
//DWORD WINAPI CHsmIPAdressDlg::TuxedoTest(LPVOID lpParameter)
//{
//	HsmIPAddress* ip = (HsmIPAddress*)lpParameter;
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	FBFR32 *sendbuf, *rcvbuf;
//	FLDLEN32 sendlen,revlen ;
//	long errorcode;
//	char retmsg[256];
//	char hostdate[10], hosttime[10];
//	long rcvlen;
//
//	// ���ͳ���
//	sendlen = 500;
//	//���ܳ���
//	revlen = 100;
//
//	long needsendlen = Fneeded32(1,sendlen);
//	long needrevlen = Fneeded32(1,revlen);
//	
//	CString ipAddress;
//	ipAddress.AppendFormat(_T("%s:%s"), ip->ipAddress, ip->ipPort);
//
//	std::string szIpAddress = Util::Wcstombs(ipAddress);
//
//	char url[32];
//	sprintf(url, "WSNADDR=//%s", szIpAddress.c_str());
//	//10.0.4.107:3300//10.20.9.1:3300
//	tuxputenv(url);//WSNADDR=//192.168.109.1:3300
//
//	*ip->errMessage = _T("�������ӷ�����...");
//	::PostMessage(ip->hWnd, WM_MESSAGE_ERROR_DISPLAY, (WPARAM)ip, 0L);
//	
//	// ���߳�֧��
//	TPINIT * tpinitbuf;
//	tpinitbuf=(TPINIT *)tpalloc("TPINIT",NULL,TPINITNEED(0)); 	
//	tpinitbuf->flags = TPMULTICONTEXTS;
//	/* Attach to System/T as a Client Process */
//	if (tpinit((TPINIT *) tpinitbuf) == -1) {
//		(void) fprintf(stderr, "Tperrno = %s\n", tpstrerror(tperrno));
//		*ip->errMessage = _T("����ʧ�ܣ��޷�����");
//		*ip->flag = true;
//		::PostMessage(ip->hWnd, WM_MESSAGE_ERROR_DISPLAY, (WPARAM)ip, 0L);
//		return -1;
//	}
//
//	/* Allocate FML buffers for the request and the reply */
//	if((sendbuf = (FBFR32 *) tpalloc("FML32", NULL, needsendlen)) == NULL) {		
//		tpterm();
//		*ip->errMessage = _T("����洢�ռ�ʧ��");
//		*ip->flag = true;
//		::PostMessage(ip->hWnd, WM_MESSAGE_ERROR_DISPLAY, (WPARAM)ip, 0L);
//		return -2;
//	}
//
//	if((rcvbuf = (FBFR32 *) tpalloc("FML32", NULL, needrevlen)) == NULL) {
//		tpfree((char*)sendbuf);
//		tpfree((char *)tpinitbuf);
//		tpterm();
//		*ip->errMessage = _T("����洢�ռ�ʧ��");
//		*ip->flag = true;
//		::PostMessage(ip->hWnd, WM_MESSAGE_ERROR_DISPLAY, (WPARAM)ip, 0L);
//		return -2;
//	}
//
//	/* Request the service MAC1AUTH, waiting for a reply */
//	int ret = tpcall("NOW", (char *)sendbuf, 0, (char **)&rcvbuf, &rcvlen, (long)0);
//
//	if(ret == -1) {
//		tpfree((char *)sendbuf);
//		tpfree((char *)rcvbuf);
//		tpfree((char *)tpinitbuf);
//		tpterm();
//
//		CString errMsg;
//		errMsg.Format(_T("����NOW����ʧ�ܣ����ش������:%d"), tperrno);
//		*ip->errMessage = errMsg;
//		*ip->flag = true;
//		::PostMessage(ip->hWnd, WM_MESSAGE_ERROR_DISPLAY, (WPARAM)ip, 0L);
//		return -3;
//
//	}
//
//	/*get data from reply buffer and show in console */
//	FLDLEN32 len ;
//	len = sizeof(long);	
//	Fget32(rcvbuf,ERRNO,0,(char*)&errorcode,&len);
//	len = 100;	
//	Fget32(rcvbuf,ERRINFO,0,retmsg,&len);
//	Fget32(rcvbuf,HOSTDATE,0,hostdate,&len);
//	Fget32(rcvbuf,HOSTTIME,0,hosttime,&len);
//
//	CString msg;
//	if(errorcode == 0)
//	{
//		CString strHostDate(hostdate);
//		CString strHostTime(hosttime);
//		msg.Format(_T("������óɹ��������������:%s, �������ʱ��:%s"), strHostDate, strHostTime);
//	}
//	else
//	{
//		CString strMsg(retmsg);
//		msg.Format(_T("���÷��������󡣴������:%d������ԭ��:%s"), errorcode, strMsg);
//	}
//
//	*ip->errMessage = msg;
//	*ip->flag = true;
//	::PostMessage(ip->hWnd, WM_MESSAGE_ERROR_DISPLAY, (WPARAM)ip, 0L);
//
//	/* Free Buffers & Detach from System/T */
//	tpfree((char *)tpinitbuf);
//	tpfree((char *)sendbuf);
//	tpfree((char *)rcvbuf);
//	tpterm();
//
//	return 0;
//
//}

LRESULT CHsmIPAdressDlg::OnErrorDisplay(WPARAM wp, LPARAM lp)
{
	HsmIPAddress* ip = (HsmIPAddress*)wp;
	if(*ip->flag)
	{
		GetDlgItem(IDC_BUTTON_TEST)->EnableWindow(TRUE);
		GetDlgItem(IDOK)->EnableWindow(TRUE);
	}
	
	SetDlgItemText(IDC_STATIC_RESULT, *ip->errMessage); 
	return 0L;
}
HBRUSH CHsmIPAdressDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if( pWnd->GetDlgCtrlID() == IDC_STATIC_RESULT )    
	{        
		pDC->SetTextColor(RGB(255,0,0)); //������ɫ        
		//pDC->SetBkColor(TRANSPARENT); //���屳��ɫ    
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}
