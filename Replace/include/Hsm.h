#pragma once

#include <vector>
using std::vector;

// CHsm 对话框

class CHsm : public CDialog
{
	DECLARE_DYNAMIC(CHsm)

public:
	CHsm(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHsm();

// 对话框数据
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheck();

	BOOL OnInitDialog();

	OperType m_operType;
	void SetEnable(const vector<int>& ids);
};
