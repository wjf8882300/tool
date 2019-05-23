#pragma once
#include "util/stdafx.h"
#include <vector>


using namespace std;

//配置信息类
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
	//源路径
	CString sourcePath;
	//目标路径
	CString	destPath;
	//扩展名
	CString extStr;
	//是否遍历子目录
	bool	isTraversal;
	//是否删除源文件
	bool	isDelete;
	//扩展名集合
	vector<CString>	exts;
	
};

//配置管理类
class CConfigManager
{
public:
	CConfigManager(CString path);
	~CConfigManager(void);
public:
	//读取xml
	void LoadConfig();
	//新增或者更新xml
	void SaveConfig();
	//删除xml
	void DeleteConfig();
private:
	//读取
	void ReadXml();
	//改写
	void WriteXml(bool isUpdate);
	//flag:0 节点，1 属性
	void ProcessNode(CComPtr<IXMLDOMNode>& spNode,int flag,CConfigInfo& info);
	//readOnly: true 只读，false 改写
	void AccessNode(CComPtr<IXMLDOMNode>& spNode,CConfigInfo& info,bool readOnly);
	//创建节点属性
	void CreateNodeAttribute(CComPtr<IXMLDOMNode>& spNode,CConfigInfo& info);
	//根据节点属性寻找节点
	bool FindNodeByAttribute(CComPtr<IXMLDOMNode>& spNode,CString attributeName,CString attributeValue);
private:
	//读取XML
	CComPtr<IXMLDOMDocument> spDoc;    //DOM
	//根节点
	CComPtr<IXMLDOMElement> spRootEle;
	//xml路径
	CString spPath;
public:
	//配置容器
	vector<CConfigInfo> vinfo;
};


//配置桥接类
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

