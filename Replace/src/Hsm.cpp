// Hsm.cpp : 实现文件
//

#include "stdafx.h"
#include "Replace.h"
#include "Hsm.h"


// CHsm 对话框

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


// CHsm 消息处理程序

void CHsm::OnBnClickedCheck()
{
	// TODO: 在此添加控件通知处理程序代码
	
}

BOOL CHsm::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	vector<int> ids;

	if(m_operType == PURCHARSE_MAC1)
	{
		SetDlgItemText(IDC_EDIT_COMMENT,
			_T("CPU卡MAC1计算：\r\n\r\n\t")
			_T("过程密钥：随机数(4字节) + 卡片脱机交易序号(2字节) + 终端交易序号(2字节)\r\n\r\n\t")
			_T("MAC数据 ：交易金额(4字节) + 交易类型(1字节) + 终端编号(6字节)\r\n\t\t")
			_T(" + 交易日期(4字节) + 交易时间(3字节) + 卡片认证码(9字节)")
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
			_T("CPU卡TAC计算：\r\n\r\n\t")
			_T("TAC数据 ：交易金额(4字节) + 交易类型(1字节) + 终端编号(6字节)\r\n\t\t")
			_T(" + 终端交易序号(4字节) +交易日期(4字节) + 交易时间(3字节)")
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

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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