#pragma once
#include "util/stdafx.h"
#include <vector>


using namespace std;

//������Ϣ��
class CConfigInfo
{
public:
	CConfigInfo(void){};
	~CConfigInfo(void){};
public:
	CString getSourcePath()	const {return sourcePath;		}
	CString getDestPath()	const {return destPath;			}
	bool	getIsTraversal()const {return isTraversal;		}
	bool	getIsDelete()	const {return isDelete;			}
	vector<CString>	getExts() const{return exts;}
	CString getExtStr()		const {return extStr;};

	void	setSourcePath	(const CString& path){sourcePath = path;		}
	void	setDestPath		(const CString& path	){destPath = path;			}
	void	setIsTraversal	(const bool istraversal	){isTraversal = istraversal;}
	void	setIsDelete		(const bool isdelete		){isDelete = isdelete;		}
	void	setExts(const CString& ext)
	{
		extStr = ext;
		int curPos= 0;
		CString token=ext.Tokenize(_T(","),curPos);
		if(token != _T(""))
		{
			exts.push_back(token);
			token= ext.Tokenize(_T(","),curPos);
		}
	}


private:
	//Դ·��
	CString sourcePath;
	//Ŀ��·��
	CString	destPath;
	//��չ��
	CString extStr;
	//�Ƿ������Ŀ¼
	bool	isTraversal;
	//�Ƿ�ɾ��Դ�ļ�
	bool	isDelete;
	//��չ������
	vector<CString>	exts;
	
};

//���ù�����
class CConfigManager
{
public:
	CConfigManager(CString path);
	~CConfigManager(void);
public:
	//��ȡxml
	void LoadConfig();
	//�������߸���xml
	void SaveConfig();
	//ɾ��xml
	void DeleteConfig();
private:
	//��ȡ
	void ReadXml();
	//��д
	void WriteXml(bool isUpdate);
	//flag:0 �ڵ㣬1 ����
	void ProcessNode(CComPtr<IXMLDOMNode>& spNode,int flag,CConfigInfo& info);
	//readOnly: true ֻ����false ��д
	void AccessNode(CComPtr<IXMLDOMNode>& spNode,CConfigInfo& info,bool readOnly);
	//�����ڵ�����
	void CreateNodeAttribute(CComPtr<IXMLDOMNode>& spNode,CConfigInfo& info);
	//���ݽڵ�����Ѱ�ҽڵ�
	bool FindNodeByAttribute(CComPtr<IXMLDOMNode>& spNode,CString attributeName,CString attributeValue);
private:
	//��ȡXML
	CComPtr<IXMLDOMDocument> spDoc;    //DOM
	//���ڵ�
	CComPtr<IXMLDOMElement> spRootEle;
	//xml·��
	CString spPath;
public:
	//��������
	vector<CConfigInfo> vinfo;
};


//�����Ž���
class CConfigBirdge
{
public:
	 enum OPER_CONFIG{OPER_INSERT=1,OPER_UPDATE=2,OPER_DELETE=3};
public:
	CConfigBirdge()
	{

	}

	static void setConfig(vector<CConfigInfo> &info)
	{
		vinfo = info;
	}

	static vector<CConfigInfo>& getConfig()
	{
		return vinfo;
	}

	static void setOper(OPER_CONFIG oper)
	{
		de = oper;
	}	

	static OPER_CONFIG getOper()
	{
		return de;
	}

	static void clearConfig()
	{
		vinfo.clear();
	}

private:
	static vector<CConfigInfo> vinfo;
	static OPER_CONFIG		   de;
};

