// Replace.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	/*#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"*/
#include "util/stdafx.h"
#endif

#include "../resource.h"		// ������

// CReplaceApp:
// �йش����ʵ�֣������ Replace.cpp
//

class CReplaceApp : public CWinApp
{
public:
	CReplaceApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CReplaceApp theApp;