// S001Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Replace.h"
#include "S001Dlg.h"
#include "Poco/Environment.h"
#include "dbi/COracle.h"
#include "DetailDlg.h"
#include "tinyxml.h"

#define SETISFINDCHANNEL(x, y) if(x.Find(m_oldChannelNo) != -1) { y = true; }

// CS001Dlg 对话框

IMPLEMENT_DYNAMIC(CS001Dlg, CDialog)

CS001Dlg::CS001Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CS001Dlg::IDD, pParent)
{

}

CS001Dlg::~CS001Dlg()
{
}

void CS001Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_DATABASE, m_tree);
}


BEGIN_MESSAGE_MAP(CS001Dlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_IMPORTTEMPLATE, &CS001Dlg::OnBnClickedButtonImportTemplate)
	ON_BN_CLICKED(IDC_BUTTON_EXPLORER, &CS001Dlg::OnBnClickedButtonExplorer)
	ON_BN_CLICKED(IDC_BUTTON_GENERATE, &CS001Dlg::OnBnClickedButtonGenerate)
	ON_BN_CLICKED(IDC_BUTTON_BODYSTD, &CS001Dlg::OnBnClickedButtonBodystd)
	ON_BN_CLICKED(IDC_CHECK_SELECTALL, &CS001Dlg::OnBnClickedCheckSelectall)
	ON_EN_CHANGE(IDC_EDIT_CHANNELNO, &CS001Dlg::OnEnChangeEditChannelno)
	ON_EN_SETFOCUS(IDC_EDIT_CHANNELNO, &CS001Dlg::OnEnSetfocusEditChannelno)
END_MESSAGE_MAP()


// CS001Dlg 消息处理程序

void CS001Dlg::OnBnClickedButtonImportTemplate()
{
	// TODO: 在此添加控件通知处理程序代码
	SetDlgItemText(IDC_EDIT_CHANNELNO, _T(""));
	SetDlgItemText(IDC_EDIT_CHANNELNAME, _T(""));
	((CButton*)GetDlgItem(IDC_RADIO_IMPORT))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_EXPORT))->SetCheck(FALSE);
	SetDlgItemText(IDC_COMBO_TRADETYPE, _T(""));
	SetDlgItemText(IDC_EDIT_INTERVAL, _T(""));
	SetDlgItemText(IDC_EDIT_SLEEPTIME, _T(""));
	SetDlgItemText(IDC_EDIT_RESTARTTIME, _T(""));
	((CButton*)GetDlgItem(IDC_CHECK_REDO))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_prophase))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_SOURCE))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_HANDLE))->SetCheck(FALSE);
	SetDlgItemText(IDC_EDIT_PROPHASE, _T(""));
	SetDlgItemText(IDC_EDIT_SOURCE, _T(""));
	SetDlgItemText(IDC_EDIT_DEST, _T(""));
	((CButton*)GetDlgItem(IDC_CHECK_HEAD))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_BODY))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_FOLDER))->SetCheck(FALSE);
	SetDlgItemText(IDC_EDIT_HEAD, _T(""));
	SetDlgItemText(IDC_EDIT_HEADHEX, _T(""));
	SetDlgItemText(IDC_EDIT_BODY, _T(""));
	SetDlgItemText(IDC_EDIT_BODYHEX, _T(""));
	SetDlgItemText(IDC_EDIT_BODYSTD, _T(""));
	SetDlgItemText(IDC_EDIT_BODYSTDTYPE, _T(""));
	SetDlgItemText(IDC_EDIT_FILENAME, _T(""));
	SetDlgItemText(IDC_EDIT_FILEVALUE, _T(""));
	SetDlgItemText(IDC_EDIT_PATHIN, _T(""));
	SetDlgItemText(IDC_EDIT_PATHBAK, _T(""));
	SetDlgItemText(IDC_EDIT_PATHBAKERR, _T(""));
	((CButton*)GetDlgItem(IDC_CHECK_HEADMAC))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_BODYMAC))->SetCheck(FALSE);
	SetDlgItemText(IDC_EDIT_HEADMACTXT, _T(""));
	SetDlgItemText(IDC_EDIT_BODYMACTXT, _T(""));
	SetDlgItemText(IDC_EDIT_BODYSPLIT, _T(""));
	SetDlgItemText(IDC_EDIT_FILESPLIT, _T(""));
	SetDlgItemText(IDC_EDIT_DATABASE, _T(""));
	//SetDlgItemText(IDC_EDIT_TEMPLATE, _T(""));
	((CButton*)GetDlgItem(IDC_CHECK_SELECTALL))->SetCheck(FALSE);
	m_tree.DeleteAllItems();

	CString path;
	GetDlgItemText(IDC_EDIT_MYTEMPLATE, path);

	if(!path.IsEmpty())
	{
		path = _T("<config><synhorns>") + path + _T("</synhorns></config>");
		LoadXml(path, TRUE);
	}
	else
	{
		CFileDialog hFileDlg(TRUE,NULL,NULL,OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_READONLY,
			TEXT("XML文件(*.xml)|*.xml||"),NULL);
		hFileDlg.m_ofn.nFilterIndex=2;
		hFileDlg.m_ofn.hwndOwner=m_hWnd;
		hFileDlg.m_ofn.lStructSize=sizeof(OPENFILENAME);
		hFileDlg.m_ofn.lpstrTitle=TEXT("打开XML文件...\0");
		hFileDlg.m_ofn.nMaxFile=MAX_PATH;
		
		if(hFileDlg.DoModal() == IDOK)
		{
			path = hFileDlg.GetPathName();
			if(!path.IsEmpty())
			{
				LoadXml(path);
			}
		}
	}

	//设置原始通道编号
	GetDlgItemText(IDC_EDIT_CHANNELNO, m_oldChannelNo);

	CString fileIn, fileBak, fileBakerr, procProphase, procSource, procHandle;
	GetDlgItemText(IDC_EDIT_PATHIN, fileIn);
	GetDlgItemText(IDC_EDIT_PATHBAK, fileBak);
	GetDlgItemText(IDC_EDIT_PATHBAKERR, fileBakerr);
	GetDlgItemText(IDC_EDIT_PROPHASE, procProphase);
	GetDlgItemText(IDC_EDIT_SOURCE, procSource);
	GetDlgItemText(IDC_EDIT_DEST, procHandle);

	m_isContainChannelNoPathIn = false;
	m_isContainChannelNoPathBak = false; 
	m_isContainChannelNoPathBakerr = false; 
	m_isContainChannelNoProcProphase = false;
	m_isContainChannelNoProcSource = false;
	m_isContainChannelNoProcHandle = false;

	SETISFINDCHANNEL(fileIn, m_isContainChannelNoPathIn);
	SETISFINDCHANNEL(fileBak, m_isContainChannelNoPathBak);
	SETISFINDCHANNEL(fileBakerr, m_isContainChannelNoPathBakerr);
	SETISFINDCHANNEL(procProphase, m_isContainChannelNoProcProphase);
	SETISFINDCHANNEL(procSource, m_isContainChannelNoProcSource);
	SETISFINDCHANNEL(procHandle, m_isContainChannelNoProcHandle);
}

void CS001Dlg::OnBnClickedButtonExplorer()
{
	// TODO: 在此添加控件通知处理程序代码
	CString txt;
	CString channelNo, channelName, type;
	CString typecode, interval, sleeptime, restarttime, redo;
	CString procs, procProphase, procSource, procHandle;
	CString headText, headHex, bodyText, bodyHex, stdText, stdHex;
	CString headMac, bodyMac;
	CString fileName, fileValue, fileIn, fileBak, fileBakerr;
	CString split, filesplit;
	GetDlgItemText(IDC_EDIT_CHANNELNO, channelNo);
	GetDlgItemText(IDC_EDIT_CHANNELNAME, channelName);
	GetDlgItemText(IDC_COMBO_TRADETYPE, typecode);
	GetDlgItemText(IDC_EDIT_INTERVAL, interval);
	GetDlgItemText(IDC_EDIT_SLEEPTIME, sleeptime);
	GetDlgItemText(IDC_EDIT_RESTARTTIME, restarttime);

	GetDlgItemText(IDC_EDIT_HEAD, headText);
	GetDlgItemText(IDC_EDIT_HEADHEX, headHex);
	GetDlgItemText(IDC_EDIT_BODY, bodyText);
	GetDlgItemText(IDC_EDIT_BODYHEX, bodyHex);
	GetDlgItemText(IDC_EDIT_BODYSTD, stdText);
	GetDlgItemText(IDC_EDIT_BODYSTDTYPE, stdHex);
	GetDlgItemText(IDC_EDIT_FILENAME, fileName);
	GetDlgItemText(IDC_EDIT_FILEVALUE, fileValue);
	GetDlgItemText(IDC_EDIT_PATHIN, fileIn);
	GetDlgItemText(IDC_EDIT_PATHBAK, fileBak);
	GetDlgItemText(IDC_EDIT_PATHBAKERR, fileBakerr);
	GetDlgItemText(IDC_EDIT_HEADMACTXT, headMac);
	GetDlgItemText(IDC_EDIT_BODYMACTXT, bodyMac);
	GetDlgItemText(IDC_EDIT_BODYSPLIT, split);
	GetDlgItemText(IDC_EDIT_FILESPLIT, filesplit);

	if(split.IsEmpty())
		split = _T(",");

	if(filesplit.IsEmpty())
		filesplit = _T("$");

	if(((CButton*)GetDlgItem(IDC_RADIO_EXPORT))->GetCheck())
	{	
		type = _T("export");
	}
	else
	{	
		type = _T("import");
	}
	if(((CButton*)GetDlgItem(IDC_CHECK_REDO))->GetCheck())
	{
		redo = _T("1");
	}
	else
	{
		redo = _T("0");
	}

	if(((CButton*)GetDlgItem(IDC_CHECK_prophase))->GetCheck())
	{
		GetDlgItemText(IDC_EDIT_PROPHASE, procProphase);
	}

	if(((CButton*)GetDlgItem(IDC_CHECK_SOURCE))->GetCheck())
	{
		GetDlgItemText(IDC_EDIT_SOURCE, procSource);
	}

	if(((CButton*)GetDlgItem(IDC_CHECK_HANDLE))->GetCheck())
	{
		GetDlgItemText(IDC_EDIT_DEST, procHandle);
	}

	procs = _T("<procs>") + procProphase + procSource + procHandle + _T("</procs>");
	
	::CoInitialize(NULL);
	CComPtr<IXMLDOMDocument> spDoc;
	HRESULT hre = spDoc.CoCreateInstance(CLSID_DOMDocument);	
	
	CComPtr<IXMLDOMNode> spSynNode;
	spDoc->createNode(CComVariant(NODE_ELEMENT),OLESTR("synhorn"), OLESTR(""), &spSynNode);
	CComQIPtr<IXMLDOMElement> spSynEle = spSynNode; //注意这里使用CComQIPtr
	spSynEle->setAttribute(OLESTR("channelno"), CComVariant(channelNo));//给新节点添加属性
	spSynEle->setAttribute(OLESTR("name"), CComVariant(channelName));//给新节点添加属性
	spSynEle->setAttribute(OLESTR("type"), CComVariant(type));//给新节点添加属性

	CComPtr<IXMLDOMNode> spCycleNode;
	CreateNode(spDoc, spSynNode, _T("cycle"), _T(""), &spCycleNode);
	CComPtr<IXMLDOMElement> typecodeNode, intervalNode, sleeptimeNode, restarttimeNode, redoNode;

	CreateNode(spDoc, spCycleNode, _T("typecode"), typecode, (IXMLDOMNode**)&typecodeNode);
	CreateNode(spDoc, spCycleNode, _T("interval"), interval, (IXMLDOMNode**)&intervalNode);
	CreateNode(spDoc, spCycleNode, _T("sleeptime"), sleeptime, (IXMLDOMNode**)&sleeptimeNode);
	CreateNode(spDoc, spCycleNode, _T("restarttime"), restarttime, (IXMLDOMNode**)&restarttimeNode);
	CreateNode(spDoc, spCycleNode, _T("redo"), redo, (IXMLDOMNode**)&redoNode);

	CComPtr<IXMLDOMDocument> spSubDoc;
	HRESULT hre1 = spSubDoc.CoCreateInstance(CLSID_DOMDocument);
	VARIANT_BOOL vb;
	spSubDoc->loadXML(CComBSTR(procs), &vb);
	CComPtr<IXMLDOMNodeList> procsNodeList;
	spSubDoc->get_childNodes(&procsNodeList);
	long nLength = 0;
	procsNodeList->get_length(&nLength);
	for(long i = 0; i < nLength; i ++)
	{
		CComPtr<IXMLDOMNode> spNodeAttrib; //注意属性也是一个IXMLDOMNode
		procsNodeList->get_item(i, &spNodeAttrib);
		spSynNode->appendChild(spNodeAttrib, NULL);
	}
	

	CComPtr<IXMLDOMNode> spContentNode;
	CreateNode(spDoc, spSynNode, _T("content"), _T(""), &spContentNode);
	CComQIPtr<IXMLDOMElement> spContentEle = spContentNode;
	spContentEle->setAttribute(OLESTR("type"), CComVariant(_T("file")));

	CComPtr<IXMLDOMNode> spHeadNode, spBodyNode, spStdNode;
	CComPtr<IXMLDOMNode> spMacsNode, spHeadMacNode, spBodyMacNode;
	CreateNode(spDoc, spContentNode, _T("head"), headText, &spHeadNode);
	CreateNode(spDoc, spContentNode, _T("body"), bodyText, &spBodyNode);
	CreateNode(spDoc, spContentNode, _T("std"), stdText, &spStdNode);

	if(((CButton*)GetDlgItem(IDC_CHECK_HEADMAC))->GetCheck() 
		&& ((CButton*)GetDlgItem(IDC_CHECK_BODYMAC))->GetCheck())
	{
		CreateNode(spDoc, spContentNode, _T("macs"), _T(""), &spMacsNode);
		CreateNode(spDoc, spMacsNode, _T("mac"), headMac, &spHeadMacNode);
		CreateNode(spDoc, spMacsNode, _T("mac"), bodyMac, &spBodyMacNode);
		CComQIPtr<IXMLDOMElement> spHeadMacEle = spHeadMacNode;
		CComQIPtr<IXMLDOMElement> spBodyMacEle = spBodyMacNode;
		spHeadMacEle->setAttribute(OLESTR("type"), CComVariant(_T("head")));
		spBodyMacEle->setAttribute(OLESTR("type"), CComVariant(_T("body")));
	}
	CComQIPtr<IXMLDOMElement> spBodyEle = spBodyNode;
	spBodyEle->setAttribute(OLESTR("split"), CComVariant(split));
	spBodyEle->setAttribute(OLESTR("filesplit"), CComVariant(filesplit));

	CComPtr<IXMLDOMNode> spFilesNode;
	CreateNode(spDoc, spContentNode, _T("files"),  _T(""), &spFilesNode);
	CComPtr<IXMLDOMNode> spFileNode;
	CreateNode(spDoc, spFilesNode, _T("file"),  _T(""), &spFileNode);
	CComPtr<IXMLDOMNode> spFileNameNode, spFileValueNode;
	CreateNode(spDoc, spFileNode, _T("name"),  fileName, &spFileNameNode);
	CreateNode(spDoc, spFileNode, _T("value"),  fileValue, &spFileValueNode);

	CComPtr<IXMLDOMNode> spPathsNode;
	CreateNode(spDoc, spFileNode, _T("paths"),  _T(""), &spPathsNode);
	CComPtr<IXMLDOMNode> spPathNode;
	CreateNode(spDoc, spPathsNode, _T("path"),  _T(""), &spPathNode);
	CComPtr<IXMLDOMNode> spPathInNode, spPathBakNode, spPathBakErrNode;
	CreateNode(spDoc, spPathNode, _T("in"),  fileIn, &spPathInNode);
	CreateNode(spDoc, spPathNode, _T("bak"),  fileBak, &spPathBakNode);
	CreateNode(spDoc, spPathNode, _T("bakerr"),  fileBakerr, &spPathBakErrNode);

	CComPtr<IXMLDOMNode> spConvertNode;
	CreateNode(spDoc, spSynNode, _T("convert"), _T(""), &spConvertNode);
	CComPtr<IXMLDOMNode> spConvertHeadNode, spConvertBodyNode, spConvertStdNode;
	CreateNode(spDoc, spConvertNode, _T("head"), headHex, &spConvertHeadNode);
	CreateNode(spDoc, spConvertNode, _T("body"), bodyHex, &spConvertBodyNode);
	CreateNode(spDoc, spConvertNode, _T("std"), stdHex, &spConvertStdNode);

	CComBSTR varVal;
	spSynNode->get_xml(&varVal); //节点值

	txt = COLE2CT(varVal);

	TiXmlDocument doc( "demotest.xml" );
	doc.Parse( Util::Wcstombs(txt).c_str() );

	TiXmlPrinter printer;
	printer.SetLineBreak("\r\n");
	doc.Accept( &printer );

	string str = printer.CStr();
	txt = Util::Mbstowcs(str);

	txt.Replace(_T("&gt;"), _T(">"));
	txt.Replace(_T("&lt;"), _T("<"));
	txt.Replace(_T("&amp;"), _T("&"));
	txt.Replace(_T("&apos;"), _T("'"));
	txt.Replace(_T("&quot;"), _T("\""));

	CoUninitialize();

	CDetailDlg dlg;
	dlg.SetItemText(txt);
	dlg.DoModal();
}

void CS001Dlg::OnBnClickedButtonGenerate()
{
	// TODO: 在此添加控件通知处理程序代码
	m_select.clear();
	HTREEITEM hItem = m_tree.GetRootItem();
	while (hItem != NULL)
	{
		if(m_tree.GetCheck(hItem))
		{
			CString strItem = m_tree.GetItemText(hItem);
			for(vector<Column>::iterator it = m_cols.begin(); it != m_cols.end(); it ++)
			{
				if(it->ColumnName == strItem)
				{
					m_select.push_back(*it);
					break;
				}
			}
		}
		hItem = m_tree.GetNextItem(hItem, TVGN_NEXT) ;
	}

	if(m_select.size() == 0)//获取老版本
	{
		CString headText, headHex;
		CString bodyText, bodyHex, stdText, stdHex;
		CString split, filesplit;
		GetDlgItemText(IDC_EDIT_BODY, bodyText);
		GetDlgItemText(IDC_EDIT_BODYHEX, bodyHex);
		GetDlgItemText(IDC_EDIT_BODYSTD, stdText);
		GetDlgItemText(IDC_EDIT_BODYSTDTYPE, stdHex);
		GetDlgItemText(IDC_EDIT_BODYSPLIT, split);
		GetDlgItemText(IDC_EDIT_FILESPLIT, filesplit);
		GetDlgItemText(IDC_EDIT_HEAD, headText);
		GetDlgItemText(IDC_EDIT_HEADHEX, headHex);

		CStringArray arrayBody, arrayBodyHex, arrayStdHex;
		Util::SplitString(bodyText, split, arrayBody);
		Util::SplitString(bodyHex, _T(","), arrayBodyHex);
		Util::SplitString(stdHex, _T(","), arrayStdHex);

		if(arrayBody.GetSize() == arrayBodyHex.GetSize() 
			&& arrayBody.GetSize() == arrayStdHex.GetSize())
		{
			for(int i = 0; i < arrayBody.GetSize(); i ++)
			{
				CString columnName, columnType;
				int width;
				
				columnName = arrayBody[i];
				if(arrayStdHex[i] == "string")
				{
					CRegexpT <TCHAR> regexp(_T("(?<=\\()\\w+(?=\\))"));
					MatchResult result = regexp.Match(arrayBodyHex[i]);
					CString number;
					if(result.IsMatched())
					{
						int start = result.GetStart();
						int end = result.GetEnd();
						number = arrayBodyHex[i].Mid(start, end - start);
					}
					if(number.IsEmpty())
					{
						columnType = _T("VARCHAR2");
						width = 0;
					}
					else
					{
						columnType = _T("CHAR");
						width = _ttoi(number);
					}
				}else if(arrayStdHex[i] == _T("int")){
					columnType = _T("INT");
					width = 0;
				}else if(arrayStdHex[i] == _T("date")){
					columnType = _T("DATE");
					width = 14;
				}
					
				m_select.push_back(Column(columnName, columnType, width)); 
			}
		}
	}

	Generate(m_select);
}

void CS001Dlg::LoadXml(CString path, const BOOL& isText)
{
	::CoInitialize(NULL);
	CComPtr<IXMLDOMDocument> spDoc;
	CComPtr<IXMLDOMElement> spRootEle;
	HRESULT hre = spDoc.CoCreateInstance(CLSID_DOMDocument);
	VARIANT_BOOL vb;
	if(isText)
		spDoc->loadXML(CComBSTR(path), &vb);
	else
		spDoc->load(CComVariant(path), &vb); //加载XML文件
	spDoc->get_documentElement(&spRootEle); //根节点 config
	CComPtr<IXMLDOMNodeList> spNodeList;
	spRootEle->get_childNodes(&spNodeList); //子节点列表 synhorns
	long nLen;
	spNodeList->get_length(&nLen); //子节点数
	if(nLen == 0)return;
	CComPtr<IXMLDOMNode> spNode;
	spNodeList->get_item(0, &spNode);// 第一个synhorns
	DOMNodeType eNodeType;
	spNode->get_nodeType(&eNodeType);
	if (eNodeType == NODE_ELEMENT)
	{
		//递归遍历子节点
		CComPtr<IXMLDOMNodeList> spSynsNodeList;
		spNode->get_childNodes(&spSynsNodeList);//子节点列表 synhorn 
		spSynsNodeList->get_length(&nLen);
		if(nLen == 0)return;
		CComPtr<IXMLDOMNode> spSynNode;
		spSynsNodeList->get_item(0, &spSynNode);// 第一个synhorn
		CComPtr<IXMLDOMNamedNodeMap> spNameNodeMap;
		spSynNode->get_attributes(&spNameNodeMap);
		long nLength;
		spNameNodeMap->get_length(&nLength);
		for (long i = 0; i != nLength; ++i)
		{
			CComPtr<IXMLDOMNode> spNodeAttrib; //注意属性也是一个IXMLDOMNode
			spNameNodeMap->get_item(i, &spNodeAttrib);

			CString val;

			val = GetAttributeValue(spNodeAttrib,_T("channelno"));
			if(!val.IsEmpty())
			{
				SetDlgItemText(IDC_EDIT_CHANNELNO, val);
				continue;
			}
			val = GetAttributeValue(spNodeAttrib,_T("name"));
			if(!val.IsEmpty())
			{
				SetDlgItemText(IDC_EDIT_CHANNELNAME, val);
				continue;
			}
			val = GetAttributeValue(spNodeAttrib,_T("type"));
			if(!val.IsEmpty())
			{
				if(val.MakeLower() == _T("import"))
				{
					((CButton*)GetDlgItem(IDC_RADIO_IMPORT))->SetCheck(TRUE);
				}
				else if(val.MakeLower() == _T("export"))
				{
					((CButton*)GetDlgItem(IDC_RADIO_EXPORT))->SetCheck(TRUE);
				}
				else
				{
					MessageBox(_T("XML格式不正确！"),_T("操作"),MB_OK|MB_ICONWARNING);
				}
				continue;
			}

		}// for syn attribute

		CComPtr<IXMLDOMNodeList> spCycleNodeList;
		spSynNode->get_childNodes(&spCycleNodeList); //子节点列表 Cycle
		spCycleNodeList->get_length(&nLength);
		if(nLength == 0)return;
		CComPtr<IXMLDOMNode> spCycleChildNode;
		spCycleNodeList->get_item(0, &spCycleChildNode);// 第一个Cycle
		CComPtr<IXMLDOMNodeList> spCycleChildNodeList;
		spCycleChildNode->get_childNodes(&spCycleChildNodeList); //子节点列表 typecode interval sleeptime ...
		spCycleChildNodeList->get_length(&nLength);
		for (long i = 0; i != nLength; ++i)
		{
			CComPtr<IXMLDOMNode> spNodeAttrib; //注意属性也是一个IXMLDOMNode
			spCycleChildNodeList->get_item(i, &spNodeAttrib);
			
			CComBSTR bsNodeName;
			spNodeAttrib->get_nodeName(&bsNodeName); //节点名
		
			CComBSTR varVal;
			spNodeAttrib->get_text(&varVal); //节点值

			CString nodeName = COLE2CT(bsNodeName);
			CString val		 = COLE2CT(varVal);

			if(nodeName.MakeLower() == _T("typecode"))
			{
				SetDlgItemText(IDC_COMBO_TRADETYPE, val);
				continue;
			}
			if(nodeName.MakeLower() == _T("interval"))
			{
				SetDlgItemText(IDC_EDIT_INTERVAL, val);
				continue;
			}
			if(nodeName.MakeLower() == _T("sleeptime"))
			{
				SetDlgItemText(IDC_EDIT_SLEEPTIME, val);
				continue;
			}
			if(nodeName.MakeLower() == _T("restarttime"))
			{
				SetDlgItemText(IDC_EDIT_RESTARTTIME, val);
				continue;
			}
			if(nodeName.MakeLower() == _T("redo"))
			{
				if(val.MakeLower() == _T("1"))
					((CButton*)GetDlgItem(IDC_CHECK_REDO))->SetCheck(TRUE);
				continue;
			}
		}// for Cycle 

		CComPtr<IXMLDOMNode> spProcsNode;
		spCycleChildNode->get_nextSibling(&spProcsNode);// 节点procs
		CComPtr<IXMLDOMNodeList> spProcNodeList;
		spProcsNode->get_childNodes(&spProcNodeList);	// 节点列表procs
		spProcNodeList->get_length(&nLength);
		for (long i = 0; i != nLength; ++i)
		{
			CComPtr<IXMLDOMNode> spNodeAttrib; //注意属性也是一个IXMLDOMNode
			spProcNodeList->get_item(i, &spNodeAttrib);
			
			CComBSTR bsNodeName;
			spNodeAttrib->get_nodeName(&bsNodeName); //节点名
		
			CComBSTR varVal;
			spNodeAttrib->get_xml(&varVal); //节点值

			CString nodeName = COLE2CT(bsNodeName);
			CString val		 = COLE2CT(varVal);

			CComQIPtr<IXMLDOMElement> spElemAttrib = spNodeAttrib;			
			CComVariant varAttribVal;
			spElemAttrib->getAttribute(_T("type"), &varAttribVal);
			CString valType		 = COLE2CT(varAttribVal.bstrVal);

			val.Replace(_T("&gt;"), _T(">"));
			val.Replace(_T("&lt;"), _T("<"));
			val.Replace(_T("&amp;"), _T("&"));
			val.Replace(_T("&apos;"), _T("'"));
			val.Replace(_T("&quot;"), _T("\""));

			if(valType.MakeLower() == _T("prophase"))
			{
				((CButton*)GetDlgItem(IDC_CHECK_prophase))->SetCheck(TRUE);
				SetDlgItemText(IDC_EDIT_PROPHASE, val);
				continue;
			}else if(valType.MakeLower() == _T("source")) 
			{
				((CButton*)GetDlgItem(IDC_CHECK_SOURCE))->SetCheck(TRUE);
				SetDlgItemText(IDC_EDIT_SOURCE, val);
				continue;
			}else if(valType.MakeLower() == _T("handle")) 
			{
				((CButton*)GetDlgItem(IDC_CHECK_HANDLE))->SetCheck(TRUE);
				SetDlgItemText(IDC_EDIT_DEST, val);
				continue;
			}
		}// for proc

		CComPtr<IXMLDOMNode> spContentNode;
		spProcsNode->get_nextSibling(&spContentNode);// 节点content	

		CComPtr<IXMLDOMNodeList> spContentNodeList;
		spContentNode->get_childNodes(&spContentNodeList);	// 节点列表content
		spContentNodeList->get_length(&nLength);

		for (long i = 0; i != nLength; ++i)
		{
			CComPtr<IXMLDOMNode> spNodeAttrib; //注意属性也是一个IXMLDOMNode
			spContentNodeList->get_item(i, &spNodeAttrib);
			
			CComBSTR bsNodeName;
			spNodeAttrib->get_nodeName(&bsNodeName); //节点名
			CString nodeName = COLE2CT(bsNodeName);

			if(nodeName.MakeLower() == _T("head"))
			{
				CComBSTR varVal;
				spNodeAttrib->get_text(&varVal); //节点值
		
				CString val		 = COLE2CT(varVal);
				SetDlgItemText(IDC_EDIT_HEAD, val);
				((CButton*)GetDlgItem(IDC_CHECK_HEAD))->SetCheck(TRUE);
				continue;
			}

			if(nodeName.MakeLower() == _T("body"))
			{
				CComBSTR varVal;
				spNodeAttrib->get_text(&varVal); //节点值

				CString val		 = COLE2CT(varVal);
				SetDlgItemText(IDC_EDIT_BODY, val);
			
				CComQIPtr<IXMLDOMElement> spElemAttrib = spNodeAttrib;			
				CComVariant varAttribVal;
				spElemAttrib->getAttribute(_T("split"), &varAttribVal);
				CString valType	 = COLE2CT(varAttribVal.bstrVal);
				SetDlgItemText(IDC_EDIT_BODYSPLIT, valType);
				spElemAttrib->getAttribute(_T("filesplit"), &varAttribVal);
				valType	 = COLE2CT(varAttribVal.bstrVal);
				SetDlgItemText(IDC_EDIT_FILESPLIT, valType);
				((CButton*)GetDlgItem(IDC_CHECK_BODY))->SetCheck(TRUE);
				continue;
			}

			if(nodeName.MakeLower() == _T("macs"))
			{
				CComPtr<IXMLDOMNodeList> spMacsdeList;
				spNodeAttrib->get_childNodes(&spMacsdeList);	// 节点列表macs
				long macsLength;
				spMacsdeList->get_length(&macsLength);
				if(macsLength != 0)
				{
					CComPtr<IXMLDOMNode> spMacNodeAttrib;
					CComBSTR varVal;
					CString val;
					spMacsdeList->get_item(0, &spMacNodeAttrib);

					if(spMacNodeAttrib != NULL)
					{
						spMacNodeAttrib->get_text(&varVal); //节点值
						val		 = COLE2CT(varVal);

						((CButton*)GetDlgItem(IDC_CHECK_HEADMAC))->SetCheck(TRUE);
						SetDlgItemText(IDC_EDIT_HEADMACTXT, val);
					}

					CComPtr<IXMLDOMNode> spMacNodeAttrib1;
					spMacsdeList->get_item(1, &spMacNodeAttrib1);
					if(spMacNodeAttrib1 != NULL)
					{
						spMacNodeAttrib1->get_text(&varVal); //节点值
						val		 = COLE2CT(varVal);
						((CButton*)GetDlgItem(IDC_CHECK_BODYMAC))->SetCheck(TRUE);
						SetDlgItemText(IDC_EDIT_BODYMACTXT, val);
					}

					
				}
				continue;
			}

			if(nodeName.MakeLower() == _T("std"))
			{
				CComBSTR varVal;
				spNodeAttrib->get_text(&varVal); //节点值

				CString val		 = COLE2CT(varVal);
				SetDlgItemText(IDC_EDIT_BODYSTD, val);		
				continue;
			}

			if(nodeName.MakeLower() == _T("files"))
			{
				CComPtr<IXMLDOMNode> spNodeFile;
				spNodeAttrib->get_firstChild(&spNodeFile);//节点file

				CComPtr<IXMLDOMNode> spNodeName;
				spNodeFile->get_firstChild(&spNodeName);  //节点Name
				SetDlgItemText(IDC_EDIT_FILENAME, GetNodeValue(spNodeName, _T("name")));	

				CComPtr<IXMLDOMNode> spNodeValue;
				spNodeName->get_nextSibling(&spNodeValue);//节点Value
				SetDlgItemText(IDC_EDIT_FILEVALUE, GetNodeValue(spNodeValue, _T("value")));

				CComPtr<IXMLDOMNode> spNodePaths;
				spNodeValue->get_nextSibling(&spNodePaths);//节点Paths
				CComPtr<IXMLDOMNode> spNodePath;
				spNodePaths->get_firstChild(&spNodePath);  //节点Path

				CComPtr<IXMLDOMNode> spNodeIN;
				spNodePath->get_firstChild(&spNodeIN);  //节点IN
				SetDlgItemText(IDC_EDIT_PATHIN, GetNodeValue(spNodeIN, _T("in")));

				CComPtr<IXMLDOMNode> spNodeBAK;
				spNodeIN->get_nextSibling(&spNodeBAK);  //节点BAK
				SetDlgItemText(IDC_EDIT_PATHBAK, GetNodeValue(spNodeBAK, _T("bak")));

				CComPtr<IXMLDOMNode> spNodeBAKERR;
				spNodeBAK->get_nextSibling(&spNodeBAKERR);  //节点BAKERR
				SetDlgItemText(IDC_EDIT_PATHBAKERR, GetNodeValue(spNodeBAKERR, _T("bakerr")));
				
				((CButton*)GetDlgItem(IDC_CHECK_FOLDER))->SetCheck(TRUE);
				continue;
			}
		}// for

		CComPtr<IXMLDOMNode> spConvertNode;
		spContentNode->get_nextSibling(&spConvertNode);// 节点content	

		CComPtr<IXMLDOMNodeList> spConvertNodeList;
		spConvertNode->get_childNodes(&spConvertNodeList);	// 节点列表content
		spConvertNodeList->get_length(&nLength);

		for (long i = 0; i != nLength; ++i)
		{
			CComPtr<IXMLDOMNode> spNodeAttrib; //注意属性也是一个IXMLDOMNode
			spConvertNodeList->get_item(i, &spNodeAttrib);

			CString val;

			val = GetNodeValue(spNodeAttrib,_T("head"));
			if(!val.IsEmpty())
			{
				SetDlgItemText(IDC_EDIT_HEADHEX, val);
				continue;
			}

			val = GetNodeValue(spNodeAttrib,_T("body"));
			if(!val.IsEmpty())
			{
				SetDlgItemText(IDC_EDIT_BODYHEX, val);
				continue;
			}

			val = GetNodeValue(spNodeAttrib,_T("std"));
			if(!val.IsEmpty())
			{
				SetDlgItemText(IDC_EDIT_BODYSTDTYPE, val);
				continue;
			}
		}
	}

	CString procProphase, procSource, procHandle;
	if(((CButton*)GetDlgItem(IDC_CHECK_prophase))->GetCheck())
	{
		GetDlgItemText(IDC_EDIT_PROPHASE, procProphase);
	}

	if(((CButton*)GetDlgItem(IDC_CHECK_SOURCE))->GetCheck())
	{
		GetDlgItemText(IDC_EDIT_SOURCE, procSource);
	}

	if(((CButton*)GetDlgItem(IDC_CHECK_HANDLE))->GetCheck())
	{
		GetDlgItemText(IDC_EDIT_DEST, procHandle);
	}

	CString oldTableName;
	CString procs = procProphase + procSource + procHandle;//
	CRegexpT <TCHAR> regexp(_T("(?<=\\<output.*\\>)[^\\<\\>]*(?=\\</output\\>)"));
	MatchResult result = regexp.Match(procs);
	if(result.IsMatched())
	{
		int start = result.GetStart();
		int end = result.GetEnd();
		oldTableName = procs.Mid(start, end - start);
		SetDlgItemText(IDC_EDIT_DATABASE, oldTableName);
	}

	CoUninitialize();

}

CString CS001Dlg::GetNodeValue(CComPtr<IXMLDOMNode> spNodeAttrib, const CString& nodeName)
{
	CString val(_T(""));
	
	if(GetNodeName(spNodeAttrib).MakeLower() == nodeName)
	{
		CComBSTR varVal;
		spNodeAttrib->get_text(&varVal); //节点值
		val	= COLE2CT(varVal);
	}

	return val;
}

CString CS001Dlg::GetNodeName(CComPtr<IXMLDOMNode> spNodeAttrib)
{
	CString nodeName(_T(""));
	CComBSTR bsNodeName;
	spNodeAttrib->get_nodeName(&bsNodeName); //节点名
	nodeName = COLE2CT(bsNodeName);
	return nodeName;
}

CString CS001Dlg::GetAttributeValue(CComPtr<IXMLDOMNode> spNodeAttrib, const CString& attributeName)
{
	CString val(_T(""));
	
	if(GetNodeName(spNodeAttrib).MakeLower() == attributeName)
	{
		CComVariant varVal;
		spNodeAttrib->get_nodeValue(&varVal); //节点值
		val = COLE2CT(varVal.bstrVal);
	}

	return val;
}

void CS001Dlg::OnBnClickedButtonBodystd()
{
	// TODO: 在此添加控件通知处理程序代码
	CString tableName;
	GetDlgItemText(IDC_EDIT_DATABASE, tableName);

	m_cols.clear();
	if(!tableName.IsEmpty())
	{
		try
		{
			Poco::Environment::set("NLS_DATE_FORMAT", "YYYY-MM-DD HH24:MI:SS");
			CSession session(new OraSession());
			session.Connect( 
				Util::getDBConnectInfo().getUserName(),
				Util::getDBConnectInfo().getPassword(),
				Util::getDBConnectInfo().getDBName());
			if( session.IsConnected() ) {
				string strTableName = Util::Wcstombs(tableName);
				string cname, coltype;
				int width;
				CStatement stmt(session);
				stmt = "select cname,coltype,width from col where tname=upper(:1)";
				stmt.Define(cname);
				stmt.Define(coltype);
				stmt.Define(width);
				stmt.Bind(strTableName);

				for(int i = stmt.Execute(); i > 0; i = stmt.Fetch())
				{
					m_cols.push_back(Column(Util::Mbstowcs(cname), Util::Mbstowcs(coltype), width));
				}
			}
			
			session.Close();
		} catch ( DbException& ex ) {
			MessageBox(_T("登录失败！数据库异常：") + Util::Mbstowcs(ex.ErrMsg()),_T("操作"),MB_OK|MB_ICONERROR);
		}
	}

	m_tree.DeleteAllItems();

	for(vector<Column>::iterator it = m_cols.begin();
		it != m_cols.end(); it ++)
	{
		m_tree.InsertItem(it->ColumnName, 0, 0, TVI_ROOT, TVI_LAST);
	}

	((CButton*)GetDlgItem(IDC_CHECK_SELECTALL))->SetCheck(TRUE);
	OnBnClickedCheckSelectall();
}

BOOL CS001Dlg::InitTree()
{
	//创建图像列表控件
	LONG lStyle;
	lStyle = GetWindowLong(m_tree.m_hWnd, GWL_STYLE);//获取当前窗口style
	lStyle |= TVS_HASBUTTONS; //设置style
	lStyle |= TVS_HASLINES ; //设置style
	lStyle |= TVS_LINESATROOT  ; //设置style 
	lStyle |= TVS_SHOWSELALWAYS ;
	lStyle |= TVS_FULLROWSELECT ;
	lStyle |= TVS_SINGLEEXPAND ;
	lStyle |= TVS_CHECKBOXES;
	
	SetWindowLong(m_tree.m_hWnd, GWL_STYLE, lStyle);//设置style
	return TRUE;
}

BOOL CS001Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitTree();

	((CEdit*)GetDlgItem(IDC_EDIT_MYTEMPLATE))->SetLimitText(UINT_MAX);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


void CS001Dlg::OnBnClickedCheckSelectall()
{
	// TODO: 在此添加控件通知处理程序代码
	
	HTREEITEM hItem = m_tree.GetRootItem() ;

	if(((CButton*)GetDlgItem(IDC_CHECK_SELECTALL))->GetCheck())
	{
         while (hItem != NULL)
         {
            m_tree.SetCheck(hItem, TRUE) ;
            hItem = m_tree.GetNextItem(hItem, TVGN_NEXT) ;
         }
	}
	else
	{
         while (hItem != NULL)
         {
            m_tree.SetCheck(hItem, FALSE) ;
            hItem = m_tree.GetNextItem(hItem, TVGN_NEXT) ;
         }
	}

}

void CS001Dlg::Generate(const vector<Column>& select)
{
	CString bodyText, bodyHex, stdText, stdHex;
	CString mac;
	CString split;

	GetDlgItemText(IDC_EDIT_BODYSPLIT, split);
	if(split.IsEmpty())split = _T(",");

	int i = 0;
	for(i = 0; i < select.size(); i ++)
	{
		CString name = select[i].ColumnName;
		CString str = select[i].ColumnType;
		name.MakeUpper();
		str.MakeUpper();
		/*if((((CButton*)GetDlgItem(IDC_RADIO_EXPORT))->GetCheck()) && str.Find(_T("DATE")) != -1)
		{
			bodyText.Append(_T("TO_CHAR("));
			bodyText.Append(name);
			bodyText.Append(_T(",'YYYYMMDDHH24MISS')"));
		}
		else*/
			bodyText.Append(name);
		stdText.Append(name);
		if(i != select.size() - 1)
		{
			bodyText.Append(split);
			stdText.Append(_T(","));
		}

		if(name.Find(_T("MAC")) == -1)
		{
			mac.AppendFormat(_T("%d"), i);
			if(i != select.size() - 1)
			{
				mac.Append(_T(","));
			}
		}
		else
		{
			if(mac.Right(1) == _T(","))
				mac.Delete(mac.GetLength() - 1);
		}
			

		if(str.Find(_T("VARCHAR")) != -1)	{
			bodyHex.Append(_T("BCD"));
			stdHex.Append(_T("string"));
			if(i != select.size() - 1)
			{
				bodyHex.Append(_T(","));
				stdHex.Append(_T(","));
			}
		}
		else if(str.Find(_T("CHAR")) != -1)	{
			bodyHex.Append(_T("BCD"));
			if(((CButton*)GetDlgItem(IDC_RADIO_EXPORT))->GetCheck())
				bodyHex.AppendFormat(_T("(%d)"),  select[i].Width);
			stdHex.Append(_T("string"));
			if(i != select.size() - 1)
			{
				bodyHex.Append(_T(","));
				stdHex.Append(_T(","));
			}
		}else if(str.Find(_T("INTEGER")) != -1
			||str.Find(_T("INT")) != -1
			||str.Find(_T("NUMBER")) != -1)	{
			bodyHex.Append(_T("BCD"));
			stdHex.Append(_T("int"));
			if(i != select.size() - 1)
			{
				bodyHex.Append(_T(","));
				stdHex.Append(_T(","));
			}
		}else if(str.Find(_T("DATE")) != -1)	{
			if(((CButton*)GetDlgItem(IDC_RADIO_EXPORT))->GetCheck())
				bodyHex.Append(_T("BCD(14)"));
			else
				bodyHex.Append(_T("BCD"));	
			stdHex.Append(_T("date"));
			if(i != select.size() - 1)
			{
				bodyHex.Append(_T(","));
				stdHex.Append(_T(","));
			}
		}else {
			bodyHex.Append(_T("BCD"));
			stdHex.Append(_T("string"));
			if(i != select.size() - 1)
			{
				bodyHex.Append(_T(","));
				stdHex.Append(_T(","));
			}
		}
	}

	SetDlgItemText(IDC_EDIT_BODY, bodyText);
	SetDlgItemText(IDC_EDIT_BODYHEX, bodyHex);
	SetDlgItemText(IDC_EDIT_BODYSTD, stdText);
	SetDlgItemText(IDC_EDIT_BODYSTDTYPE, stdHex);
	SetDlgItemText(IDC_EDIT_BODYMACTXT, mac);
	SetDlgItemText(IDC_EDIT_BODYSPLIT, split);

	//调整表名
	CString tableName;
	CString channelName;
	CString procProphase, procSource, procHandle; 
	GetDlgItemText(IDC_EDIT_CHANNELNAME, channelName);
	GetDlgItemText(IDC_EDIT_DATABASE, tableName);

	if(!tableName.IsEmpty())
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_prophase))->GetCheck())
		{
			GetDlgItemText(IDC_EDIT_PROPHASE, procProphase);
		}

		if(((CButton*)GetDlgItem(IDC_CHECK_SOURCE))->GetCheck())
		{
			GetDlgItemText(IDC_EDIT_SOURCE, procSource);
		}

		if(((CButton*)GetDlgItem(IDC_CHECK_HANDLE))->GetCheck())
		{
			GetDlgItemText(IDC_EDIT_DEST, procHandle);
		}

		//取得原表名
		CString oldTableName;
		CString procs = procProphase + procSource + procHandle;//
		CRegexpT <TCHAR> regexp(_T("(?<=\\<output.*\\>)[^\\<\\>]*(?=\\</output\\>)"));
		MatchResult result = regexp.Match(procs);
		if(result.IsMatched())
		{
			int start = result.GetStart();
			int end = result.GetEnd();
			oldTableName = procs.Mid(start, end - start);
		}

		channelName.Replace(oldTableName, tableName);
		procProphase.Replace(oldTableName, tableName);
		procSource.Replace(oldTableName, tableName);
		procHandle.Replace(oldTableName, tableName);

		SetDlgItemText(IDC_EDIT_CHANNELNAME, channelName);
		SetDlgItemText(IDC_EDIT_PROPHASE, procProphase);
		SetDlgItemText(IDC_EDIT_SOURCE, procSource);
		SetDlgItemText(IDC_EDIT_DEST, procHandle);
	}
	

	CString fileValue;
	GetDlgItemText(IDC_EDIT_FILEVALUE, fileValue);

	CStringArray arrayFileValue;
	Util::SplitString(fileValue, _T(","), arrayFileValue);
	if(arrayFileValue.GetSize() > 0)
	{
		GetDlgItemText(IDC_EDIT_DATABASE, arrayFileValue[0]);
		SetDlgItemText(IDC_EDIT_FILEVALUE, Util::formatList(arrayFileValue));			
	}	
}

void CS001Dlg::CreateNode(IXMLDOMDocument* spDoc, IXMLDOMNode* spParentNode, const CString& nodeName, const CString& nodeValue, IXMLDOMNode** spNode)
{
	if(nodeValue.IsEmpty())
	{
		spDoc->createNode(CComVariant(NODE_ELEMENT),CComBSTR(nodeName), OLESTR(""), spNode);
	}
	else
	{
		IXMLDOMElement** spEle= (IXMLDOMElement**)(spNode);
		spDoc->createElement(CComBSTR(nodeName), spEle);
		(*spEle)->put_text(CComBSTR(nodeValue));
	}
	spParentNode->appendChild(*spNode, NULL);
}

void CS001Dlg::OnEnChangeEditChannelno()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CString channelNo;
	CString fileIn, fileBak, fileBakerr;
	GetDlgItemText(IDC_EDIT_CHANNELNO, channelNo);
	GetDlgItemText(IDC_EDIT_PATHIN, fileIn);
	GetDlgItemText(IDC_EDIT_PATHBAK, fileBak);
	GetDlgItemText(IDC_EDIT_PATHBAKERR, fileBakerr);

	CString procProphase, procSource, procHandle;
	if(((CButton*)GetDlgItem(IDC_CHECK_prophase))->GetCheck())
	{
		GetDlgItemText(IDC_EDIT_PROPHASE, procProphase);
	}

	if(((CButton*)GetDlgItem(IDC_CHECK_SOURCE))->GetCheck())
	{
		GetDlgItemText(IDC_EDIT_SOURCE, procSource);
	}

	if(((CButton*)GetDlgItem(IDC_CHECK_HANDLE))->GetCheck())
	{
		GetDlgItemText(IDC_EDIT_DEST, procHandle);
	}

	if(channelNo.IsEmpty() || channelNo.GetLength() < 4)return;

	// 修改路径中通道编号
	if(m_isContainChannelNoPathIn)
		fileIn.Replace(m_channelNo, channelNo);
	if(m_isContainChannelNoPathBak)
		fileBak.Replace(m_channelNo, channelNo);
	if(m_isContainChannelNoPathBakerr)
		fileBakerr.Replace(m_channelNo, channelNo);

	// 修改数据来源中通道编号
	if(m_isContainChannelNoProcProphase)
		procProphase.Replace(m_channelNo, channelNo);
	if(m_isContainChannelNoProcSource)
		procSource.Replace(m_channelNo, channelNo);
	if(m_isContainChannelNoProcHandle)
		procHandle.Replace(m_channelNo, channelNo);

	// 
	SetDlgItemText(IDC_EDIT_PATHIN, fileIn);
	SetDlgItemText(IDC_EDIT_PATHBAK, fileBak);
	SetDlgItemText(IDC_EDIT_PATHBAKERR, fileBakerr);
	SetDlgItemText(IDC_EDIT_PROPHASE, procProphase);
	SetDlgItemText(IDC_EDIT_SOURCE, procSource);
	SetDlgItemText(IDC_EDIT_DEST, procHandle);

	m_channelNo = channelNo;

	UpdateData(FALSE);
}

void CS001Dlg::OnEnSetfocusEditChannelno()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	GetDlgItemText(IDC_EDIT_CHANNELNO, m_channelNo);
}

BOOL CS001Dlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == 'A' && IsCTRLPressed())
	{
		if(((CEdit*)GetDlgItem(IDC_EDIT_MYTEMPLATE))->GetFocus())
		{
			((CEdit*)GetDlgItem(IDC_EDIT_MYTEMPLATE))->SetSel(0,-1);
		}

		return FALSE;
	}

	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == 'D' && IsCTRLPressed())
	{
		if(((CEdit*)GetDlgItem(IDC_EDIT_MYTEMPLATE))->GetFocus())
		{
			((CEdit*)GetDlgItem(IDC_EDIT_MYTEMPLATE))->SetFocus();
			((CEdit*)GetDlgItem(IDC_EDIT_MYTEMPLATE))->Clear();  
		}	

		return FALSE;
	}

	return CDialog::PreTranslateMessage(pMsg);

}

void CS001Dlg::OnEnChangeEditBodySplit()
{
	UpdateData(TRUE);
	CString bodySplit;
	CString bodyText;
	GetDlgItemText(IDC_EDIT_BODYSPLIT, bodySplit);
	GetDlgItemText(IDC_EDIT_BODY, bodyText);

	if(bodySplit.IsEmpty())return;

	bodyText.Replace(m_bodySplit, bodySplit);

	SetDlgItemText(IDC_EDIT_BODY, bodyText);
	
	m_bodySplit = bodySplit;
	UpdateData(FALSE);
}

void CS001Dlg::OnEnSetfocusBodySplit()
{
	UpdateData(TRUE);
	GetDlgItemText(IDC_EDIT_BODYSPLIT, m_bodySplit);
}