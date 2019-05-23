#include "StdAfx.h"
#include "ConfigManager.h"

vector<CConfigInfo> CConfigBirdge::vinfo;
CConfigBirdge::OPER_CONFIG	CConfigBirdge::de;

CConfigManager::CConfigManager(CString path):spPath(path)
{
	::CoInitialize(NULL);
	HRESULT hre = spDoc.CoCreateInstance(CLSID_DOMDocument);

	VARIANT_BOOL vb;
	spDoc->load(CComVariant(path), &vb); //加载XML文件
	spDoc->get_documentElement(&spRootEle); //根节点

}

CConfigManager::~CConfigManager(void)
{
	
}

void CConfigManager::LoadConfig()
{
	ReadXml();
}

void CConfigManager::SaveConfig()
{
	WriteXml(true);
}

void CConfigManager::DeleteConfig()
{
	WriteXml(false);
}

void CConfigManager::ReadXml()
{
	CComPtr<IXMLDOMNodeList> spNodeList;
	spRootEle->get_childNodes(&spNodeList); //子节点列表

	long nLen;
	spNodeList->get_length(&nLen); //子节点数
	for (long i = 0; i != nLen; ++i) //遍历子节点
	{
		CComPtr<IXMLDOMNode> spNode;
		spNodeList->get_item(i, &spNode);
		CConfigInfo ccin;
		ProcessNode(spNode,0,ccin); //节点处理函数
		vinfo.push_back(ccin);
	}
}

void CConfigManager::WriteXml(bool isUpdate)
{
	for(int i =0 ;i< (int)vinfo.size();i++)
	{
		CComPtr<IXMLDOMNodeList> spNodeList;
		spRootEle->get_childNodes(&spNodeList); //子节点列表

		long nLen;
		spNodeList->get_length(&nLen); //子节点数

		bool found = false;
		
		for (long j = 0; j != nLen; ++j) //遍历子节点
		{
			CComPtr<IXMLDOMNode> spNode;
			spNodeList->get_item(j, &spNode);
			//找到节点
			if(FindNodeByAttribute(spNode,_T("sourcePath"),vinfo[i].getSourcePath()))
			{
				if(isUpdate)
					AccessNode(spNode,vinfo[i],false);
				else
					spRootEle->removeChild(spNode,NULL);
				found = true;
			}
		}

		if(!found)//未找到节点
		{
			CComPtr<IXMLDOMNode> spNewNode;
			spDoc->createNode(CComVariant(NODE_ELEMENT),OLESTR("dir"), OLESTR(""), &spNewNode); //创建新节点
			CComPtr<IXMLDOMNode> spNode;
			spRootEle->selectSingleNode(OLESTR("/root"), &spNode);
			spNode->appendChild(spNewNode,NULL);
			CreateNodeAttribute(spNewNode,vinfo[i]);
		}
	}
	spDoc->save(CComVariant(spPath));//节点处理函数
	////写入XML
	//CComPtr<IXMLDOMNode> spNode;
	//spRootEle->selectSingleNode(OLESTR("/root/"),&spNode);
	//spNode->put_text(OLESTR("newText")); //写入text
	//spRootEle->selectSingleNode(OLESTR("/root/node2/childnode1/@attrib1"), &spNode);
	//spNode->put_nodeValue(CComVariant(OLESTR("newValue"))); //写入value
	//CComPtr<IXMLDOMNode> spNewNode;
	//spDoc->createNode(CComVariant(NODE_ELEMENT),OLESTR("childnode3"), OLESTR(""), &spNewNode); //创建新节点
	//spRootEle->selectSingleNode(OLESTR("/root/node2"), &spNode);
	//spNode->appendChild(spNewNode, &spNewNode); //将新节点加为node2的子节点
	//spNewNode->put_text(OLESTR("childtext2")); //写入新节点text

	//CComQIPtr<IXMLDOMElement> spEle = spNewNode; //注意这里使用CComQIPtr
	//spEle->setAttribute(OLESTR("attrib1"), CComVariant(OLESTR("value1")));//给新节点添加属性
	//spDoc->save(CComVariant(CComVariant(path)));//节点处理函数
}

void CConfigManager::CreateNodeAttribute(CComPtr<IXMLDOMNode>& spNode,CConfigInfo& info)
{
	CComQIPtr<IXMLDOMElement> spEle = spNode; //注意这里使用CComQIPtr
	spEle->setAttribute(OLESTR("sourcePath"), CComVariant(info.getSourcePath()));//给新节点添加属性
	spEle->setAttribute(OLESTR("destPath"), CComVariant(info.getDestPath()));//给新节点添加属性
	spEle->setAttribute(OLESTR("isTraversal"), CComVariant(info.getIsTraversal()?OLESTR("true"):OLESTR("false")));//给新节点添加属性
	spEle->setAttribute(OLESTR("isDelete"), CComVariant(info.getIsDelete()?OLESTR("true"):OLESTR("false")));//给新节点添加属性
	spEle->setAttribute(OLESTR("exts"), CComVariant(info.getExtStr()));//给新节点添加属性
}

bool CConfigManager::FindNodeByAttribute(CComPtr<IXMLDOMNode>& spNode,CString attributeName,CString attributeValue)
{
	//递归遍历节点属性
	CComPtr<IXMLDOMNamedNodeMap> spNameNodeMap;
	spNode->get_attributes(&spNameNodeMap);
	long nLength;
	spNameNodeMap->get_length(&nLength);
	for (long i = 0; i != nLength; ++i)
	{
		CComPtr<IXMLDOMNode> spNodeAttrib; //注意属性也是一个IXMLDOMNode
		spNameNodeMap->get_item(i, &spNodeAttrib);
		CComBSTR bsNodeName;
		spNodeAttrib->get_nodeName(&bsNodeName); //节点名
	
		CComVariant varVal;
		spNodeAttrib->get_nodeValue(&varVal); //节点值

		CString nodeName = COLE2CT(bsNodeName);
		CString val		 = COLE2CT(varVal.bstrVal);

		if(nodeName == attributeName && val == attributeValue)
		{
			return true;
		}
	}

	return false;
}

void CConfigManager::AccessNode(CComPtr<IXMLDOMNode>& spNode,CConfigInfo& info,bool readOnly)
{
	//递归遍历节点属性
	CComPtr<IXMLDOMNamedNodeMap> spNameNodeMap;
	spNode->get_attributes(&spNameNodeMap);
	long nLength;
	spNameNodeMap->get_length(&nLength);
	for (long i = 0; i != nLength; ++i)
	{
		CComPtr<IXMLDOMNode> spNodeAttrib; //注意属性也是一个IXMLDOMNode
		spNameNodeMap->get_item(i, &spNodeAttrib);
		
		CComBSTR bsNodeName;
		spNodeAttrib->get_nodeName(&bsNodeName); //节点名
	
		CComVariant varVal;
		spNodeAttrib->get_nodeValue(&varVal); //节点值

		CString nodeName = COLE2CT(bsNodeName);
		CString val		 = COLE2CT(varVal.bstrVal);

		if(nodeName == "sourcePath")
		{
			if(readOnly)
				info.setSourcePath(val);
			else 
				spNodeAttrib->put_nodeValue(CComVariant(info.getSourcePath()));
		}
		if(nodeName == "destPath")
		{
			if(readOnly)
				info.setDestPath(val);
			else 
				spNodeAttrib->put_nodeValue(CComVariant(info.getDestPath()));
		}
		
		if(nodeName == "isTraversal")
		{
			if(readOnly)
				info.setIsTraversal(val=="true"?true:false);
			else
				spNodeAttrib->put_nodeValue(CComVariant(info.getIsTraversal()?OLESTR("true"):OLESTR("false")));
		}

		if(nodeName == "isDelete")
		{
			if(readOnly)
				info.setIsDelete(val=="true"?true:false);
			else
				spNodeAttrib->put_nodeValue(CComVariant(info.getIsDelete()?OLESTR("true"):OLESTR("false")));
		}
	
		if(	nodeName=="exts")
		{
			if(readOnly)
				info.setExts(val);
			else
				spNodeAttrib->put_nodeValue(CComVariant(info.getExtStr()));
		}
	}

}


void CConfigManager::ProcessNode(CComPtr<IXMLDOMNode>& spNode,int flag,CConfigInfo& info)
{
	DOMNodeType eNodeType;
	spNode->get_nodeType(&eNodeType);
	if (eNodeType == NODE_ELEMENT) //只有NODE_ELEMENT类型才能包含有属性和子节点
	{
		//递归遍历节点属性
		AccessNode(spNode,info,true);

		//递归遍历子节点
		CComPtr<IXMLDOMNodeList> spNodeList;
		spNode->get_childNodes(&spNodeList);

		long nLength = 0;
		spNodeList->get_length(&nLength);
		for (long i = 0; i != nLength; ++i)
		{
			CConfigInfo ccin;
			CComPtr<IXMLDOMNode> spChildNode;
			spNodeList->get_item(i, &spChildNode);
			ProcessNode(spChildNode,0,ccin);
			vinfo.push_back(ccin);
		}
	}
}

