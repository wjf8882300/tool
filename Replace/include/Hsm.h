#pragma once

#include <vector>
using std::vector;

// CHsm �Ի���

class CHsm : public CDialog
{
	DECLARE_DYNAMIC(CHsm)

public:
	CHsm(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHsm();

// �Ի�������
	enum { IDD = IDD_DIALOG_HSM };

	enum OperType{ 
		PURCHARSE_MAC1,
		PURCHARSE_MAC2,	
		PURCHARSE_TAC,
		LOAD_MAC1,
		LOAD_MAC2,
		LOAD_TAC,
		SAFE_MAC,
		M1_KEYA,
		M1_KEYB,
		M1_TAC
			};

	void SetOperType(const OperType& type){ m_operType = type; }
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheck();

	BOOL OnInitDialog();

	OperType m_operType;
	void SetEnable(const vector<int>& ids);
};
