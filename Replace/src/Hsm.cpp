// Hsm.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Replace.h"
#include "Hsm.h"


// CHsm �Ի���

IMPLEMENT_DYNAMIC(CHsm, CDialog)

CHsm::CHsm(CWnd* pParent /*=NULL*/)
	: CDialog(CHsm::IDD, pParent)
{

}

CHsm::~CHsm()
{
}

void CHsm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHsm, CDialog)
	ON_BN_CLICKED(IDCHECK, &CHsm::OnBnClickedCheck)
END_MESSAGE_MAP()


// CHsm ��Ϣ�������

void CHsm::OnBnClickedCheck()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
}

BOOL CHsm::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	vector<int> ids;

	if(m_operType == PURCHARSE_MAC1)
	{
		SetDlgItemText(IDC_EDIT_COMMENT,
			_T("CPU��MAC1���㣺\r\n\r\n\t")
			_T("������Կ�������(4�ֽ�) + ��Ƭ�ѻ��������(2�ֽ�) + �ն˽������(2�ֽ�)\r\n\r\n\t")
			_T("MAC���� �����׽��(4�ֽ�) + ��������(1�ֽ�) + �ն˱��(6�ֽ�)\r\n\t\t")
			_T(" + ��������(4�ֽ�) + ����ʱ��(3�ֽ�) + ��Ƭ��֤��(9�ֽ�)")
		);
	
		int szId [] = {IDC_EDIT_HSM_ROM, IDC_EDIT_HSM_CARDTRADENO, IDC_EDIT_HSM_SAMTRADENO,
					   IDC_EDIT_HSM_MONEY, IDC_EDIT_HSM_TRADETYPE, IDC_EDIT_HSM_SAMNO,
					   IDC_EDIT_HSM_TRADEDATE, IDC_EDIT_HSM_TRADETIME, IDC_EDIT_HSM_CARDUID,
					   IDC_EDIT_HSM_MAC1};
		ids.assign(szId, szId + 10);
		SetEnable(ids);
	} 
	else if(m_operType == PURCHARSE_TAC)
	{
		SetDlgItemText(IDC_EDIT_COMMENT,
			_T("CPU��TAC���㣺\r\n\r\n\t")
			_T("TAC���� �����׽��(4�ֽ�) + ��������(1�ֽ�) + �ն˱��(6�ֽ�)\r\n\t\t")
			_T(" + �ն˽������(4�ֽ�) +��������(4�ֽ�) + ����ʱ��(3�ֽ�)")
		);
	
		int szId [] = {IDC_EDIT_HSM_MONEY, IDC_EDIT_HSM_TRADETYPE, IDC_EDIT_HSM_SAMNO,
					   IDC_EDIT_HSM_TRADEDATE, IDC_EDIT_HSM_TRADETIME, IDC_EDIT_HSM_SAMTRADENO,
					   IDC_EDIT_HSM_TAC};
		ids.assign(szId, szId + 7);
		SetEnable(ids);
	}
	else if(m_operType == LOAD_MAC1)
	{
		
	}
	else if(m_operType == M1_KEYA)
	{
		
	}
	else if(m_operType == M1_KEYB)
	{
		
	}
	else if(m_operType == M1_TAC)
	{
		
	}

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CHsm::SetEnable(const vector<int>& ids)
{
	((CEdit*)GetDlgItem(IDC_EDIT_HSM_CARDNO))->EnableWindow(FALSE);
	((CEdit*)GetDlgItem(IDC_EDIT_HSM_CARDTRADENO))->EnableWindow(FALSE);
	((CEdit*)GetDlgItem(IDC_EDIT_HSM_PREMONEY))->EnableWindow(FALSE);
	((CEdit*)GetDlgItem(IDC_EDIT_HSM_MONEY))->EnableWindow(FALSE);
	((CEdit*)GetDlgItem(IDC_EDIT_HSM_TRADEDATE))->EnableWindow(FALSE);
	((CEdit*)GetDlgItem(IDC_EDIT_HSM_TRADETIME))->EnableWindow(FALSE);
	((CEdit*)GetDlgItem(IDC_EDIT_HSM_SAMNO))->EnableWindow(FALSE);
	((CEdit*)GetDlgItem(IDC_EDIT_HSM_SAMTRADENO))->EnableWindow(FALSE);
	((CEdit*)GetDlgItem(IDC_EDIT_HSM_ROM))->EnableWindow(FALSE);
	((CEdit*)GetDlgItem(IDC_EDIT_HSM_CARDUID))->EnableWindow(FALSE);
	((CEdit*)GetDlgItem(IDC_EDIT_HSM_MAC1))->EnableWindow(FALSE);
	((CEdit*)GetDlgItem(IDC_EDIT_HSM_MAC2))->EnableWindow(FALSE);
	((CEdit*)GetDlgItem(IDC_EDIT_HSM_TAC))->EnableWindow(FALSE);

	for(vector<int>::const_iterator it = ids.begin(); it != ids.end(); it ++)
	{
		((CEdit*)GetDlgItem(*it))->EnableWindow(TRUE);
	}
}