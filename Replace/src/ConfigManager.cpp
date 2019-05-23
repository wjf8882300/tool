#include "StdAfx.h"
#include "ConfigManager.h"

vector<CConfigInfo> CConfigBirdge::vinfo;
CConfigBirdge::OPER_CONFIG	CConfigBirdge::de;

CConfigManager::CConfigManager(CString path):spPath(path)
{
	::CoInitialize(NULL);
	HRESULT hre = spDoc.CoCreateInstance(CLSID_DOMDocument);

	VARIANT_BOOL vb;
	spDoc->load(CComVariant(path), &vb); //����XML�ļ�
	spDoc->get_documentElement(&spRootEle); //���ڵ�

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
	spRootEle->get_childNodes(&spNodeList); //�ӽڵ��б�

	long nLen;
	spNodeList->get_length(&nLen); //�ӽڵ���
	for (long i = 0; i != nLen; ++i) //�����ӽڵ�
	{
		CComPtr<IXMLDOMNode> spNode;
		spNodeList->get_item(i, &spNode);
		CConfigInfo ccin;
		ProcessNode(spNode,0,ccin); //�ڵ㴦����
		vinfo.push_back(ccin);
	}
}

void CConfigManager::WriteXml(bool isUpdate)
{
	for(int i =0 ;i< (int)vinfo.size();i++)
	{
		CComPtr<IXMLDOMNodeList> spNodeList;
		spRootEle->get_childNodes(&spNodeList); //�ӽڵ��б�

		long nLen;
		spNodeList->get_length(&nLen); //�ӽڵ���

		bool found = false;
		
		for (long j = 0; j != nLen; ++j) //�����ӽڵ�
		{
			CComPtr<IXMLDOMNode> spNode;
			spNodeList->get_item(j, &spNode);
			//�ҵ��ڵ�
			if(FindNodeByAttribute(spNode,_T("sourcePath"),vinfo[i].getSourcePath()))
			{
				if(isUpdate)
					AccessNode(spNode,vinfo[i],false);
				else
					spRootEle->removeChild(spNode,NULL);
				found = true;
			}
		}

		if(!found)//δ�ҵ��ڵ�
		{
			CComPtr<IXMLDOMNode> spNewNode;
			spDoc->createNode(CComVariant(NODE_ELEMENT),OLESTR("dir"), OLESTR(""), &spNewNode); //�����½ڵ�
			CComPtr<IXMLDOMNode> spNode;
			spRootEle->selectSingleNode(OLESTR("/root"), &spNode);
			spNode->appendChild(spNewNode,NULL);
			CreateNodeAttribute(spNewNode,vinfo[i]);
		}
	}
	spDoc->save(CComVariant(spPath));//�ڵ㴦����
	////д��XML
	//CComPtr<IXMLDOMNode> spNode;
	//spRootEle->selectSingleNode(OLESTR("/root/"),&spNode);
	//spNode->put_text(OLESTR("newText")); //д��text
	//spRootEle->selectSingleNode(OLESTR("/root/node2/childnode1/@attrib1"), &spNode);
	//spNode->put_nodeValue(CComVariant(OLESTR("newValue"))); //д��value
	//CComPtr<IXMLDOMNode> spNewNode;
	//spDoc->createNode(CComVariant(NODE_ELEMENT),OLESTR("childnode3"), OLESTR(""), &spNewNode); //�����½ڵ�
	//spRootEle->selectSingleNode(OLESTR("/root/node2"), &spNode);
	//spNode->appendChild(spNewNode, &spNewNode); //���½ڵ��Ϊnode2���ӽڵ�
	//spNewNode->put_text(OLESTR("childtext2")); //д���½ڵ�text

	//CComQIPtr<IXMLDOMElement> spEle = spNewNode; //ע������ʹ��CComQIPtr
	//spEle->setAttribute(OLESTR("attrib1"), CComVariant(OLESTR("value1")));//���½ڵ��������
	//spDoc->save(CComVariant(CComVariant(path)));//�ڵ㴦����
}

void CConfigManager::CreateNodeAttribute(CComPtr<IXMLDOMNode>& spNode,CConfigInfo& info)
{
	CComQIPtr<IXMLDOMElement> spEle = spNode; //ע������ʹ��CComQIPtr
	spEle->setAttribute(OLESTR("sourcePath"), CComVariant(info.getSourcePath()));//���½ڵ��������
	spEle->setAttribute(OLESTR("destPath"), CComVariant(info.getDestPath()));//���½ڵ��������
	spEle->setAttribute(OLESTR("isTraversal"), CComVariant(info.getIsTraversal()?OLESTR("true"):OLESTR("false")));//���½ڵ��������
	spEle->setAttribute(OLESTR("isDelete"), CComVariant(info.getIsDelete()?OLESTR("true"):OLESTR("false")));//���½ڵ��������
	spEle->setAttribute(OLESTR("exts"), CComVariant(info.getExtStr()));//���½ڵ��������
}

bool CConfigManager::FindNodeByAttribute(CComPtr<IXMLDOMNode>& spNode,CString attributeName,CString attributeValue)
{
	//�ݹ�����ڵ�����
	CComPtr<IXMLDOMNamedNodeMap> spNameNodeMap;
	spNode->get_attributes(&spNameNodeMap);
	long nLength;
	spNameNodeMap->get_length(&nLength);
	for (long i = 0; i != nLength; ++i)
	{
		CComPtr<IXMLDOMNode> spNodeAttrib; //ע������Ҳ��һ��IXMLDOMNode
		spNameNodeMap->get_item(i, &spNodeAttrib);
		CComBSTR bsNodeName;
		spNodeAttrib->get_nodeName(&bsNodeName); //�ڵ���
	
		CComVariant varVal;
		spNodeAttrib->get_nodeValue(&varVal); //�ڵ�ֵ

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
	//�ݹ�����ڵ�����
	CComPtr<IXMLDOMNamedNodeMap> spNameNodeMap;
	spNode->get_attributes(&spNameNodeMap);
	long nLength;
	spNameNodeMap->get_length(&nLength);
	for (long i = 0; i != nLength; ++i)
	{
		CComPtr<IXMLDOMNode> spNodeAttrib; //ע������Ҳ��һ��IXMLDOMNode
		spNameNodeMap->get_item(i, &spNodeAttrib);
		
		CComBSTR bsNodeName;
		spNodeAttrib->get_nodeName(&bsNodeName); //�ڵ���
	
		CComVariant varVal;
		spNodeAttrib->get_nodeValue(&varVal); //�ڵ�ֵ

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
	if (eNodeType == NODE_ELEMENT) //ֻ��NODE_ELEMENT���Ͳ��ܰ��������Ժ��ӽڵ�
	{
		//�ݹ�����ڵ�����
		AccessNode(spNode,info,true);

		//�ݹ�����ӽڵ�
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

