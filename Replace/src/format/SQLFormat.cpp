/***************************************************************
* SQLFormat.cpp
* ϵͳ��  : ���ù��߹���
* ��ϵͳ��: ���ù�����
* ��������      ����           ժҪ
* ----------    -----------    --------------------------------
* 2011/04/01    ������			���ο���
***************************************************************
*/
#include "format/SQLFormat.h"
#include <map>

SQLFormat::SQLFormat(CString &sql,bool isQt):strSql(sql),isQuotes(isQt)
{
	sepChangeLine = _T("\r\n");
	sepSpace	  = _T(" ");
	sepQuotes	  = _T("\"");
	sepTab		  = _T("\t");
}

void SQLFormat::parseSql(CString& strOut)
{
	strSql.Replace(sepChangeLine,sepSpace);
	strSql.Replace(sepQuotes,sepSpace);
	strSql.Replace(sepTab,sepSpace);

	Initlize();
	//��ʽ���ؼ���
	formatKey(vbefore,true);
	formatKey(vbehind,false);
	//��ʽ���ո�
	formatSpace();
	//��ʽ������
	formatComma();
	//��ʽ������
	judgeQuotes();
	//��ʽ������
	formatIndentation();
    //��ʽ��TAB
	formatTab();
	//��ʽ������
	formatLength(strOut);

	strOut.Replace(_T("\r\n\r\n"),_T("\r\n"));
}

bool SQLFormat::formatSql(CString& strOut)
{
	Initlize();

	return formatColumn(strOut);
}

void SQLFormat::formatLength(CString& strOut)
{
	CStringArray arr;
	Util::SplitString(strSql,sepChangeLine,arr);

	int length = 0;
	for(int i = 0 ;i<arr.GetSize();i++)
	{
		CString tmp = arr.GetAt(i);
		if(length<tmp.GetLength())
			length = tmp.GetLength();
	}

	for(int i = 0 ;i<arr.GetSize();i++)
	{
		CString tmp = arr.GetAt(i);
		if(CString(tmp).Trim().IsEmpty())continue;
		int len = length - tmp.GetLength();
		for(int j = 0;j <= len;j++)
		{
			tmp.Append(sepSpace);
		}
		if(!tmp.IsEmpty()&&isQuotes)
		{
			tmp = sepQuotes+tmp+sepQuotes;
		}
		strOut.Append(tmp);
		strOut.Append(sepChangeLine);
	}
}

void SQLFormat::Initlize()
{
	CString beforeStr[] = {_T("UPDATE"),_T("SELECT"),_T("BEGIN"),_T("("),_T(";"),_T("SET"),_T("THEN")};
	CString behindStr[] = {_T("VALUES"),_T("WHERE"),_T("END"),_T("GROUP"),_T("ORDER"),_T("AND"),
							_T("FROM"),_T("INSERT"),_T("DELETE"),_T("WHEN"),_T(")"),_T("SET"),
							_T("START"),_T("CONNECT"),_T("("),_T("USING")

						};
	CString functionStr[] = {_T("TO_DATE"),_T("VARCHAR"),_T("CHAR"),_T("NUMBER"),_T("TO_CAHR"),
								_T("COUNT"),_T("SUM"),_T("NVL"),_T("DATE")
						};

	vector<CString> vbeforeStr(beforeStr+0,beforeStr+(sizeof(beforeStr)/sizeof(CString)));
	vector<CString> vbehindStr(behindStr+0,behindStr+(sizeof(behindStr)/sizeof(CString)));
	vector<CString> vfunctionStr(functionStr+0,functionStr+(sizeof(functionStr)/sizeof(CString)));

	vbefore.assign(vbeforeStr.begin(),vbeforeStr.end());
	vbehind.assign(vbehindStr.begin(),vbehindStr.end());
	vfunction.assign(vfunctionStr.begin(),vfunctionStr.end());
}

void SQLFormat::formatKey(const vector<CString>& vcstr,bool isBefore)
{
	int tmp = 0;
	for(int i=0;i<(int)vcstr.size();i++)
	{
		int istart = 0;
		while(istart<strSql.GetLength()&&istart>=0)
		{
			tmp = strSql.Find(vcstr[i],istart);

			if(tmp !=-1)
			{
				istart = tmp + vcstr[i].GetLength();
				//�жϹؼ����Ƿ��ǵ��ʵ�һ����
				if(tmp+vcstr[i].GetLength()<strSql.GetLength()
					&& vcstr[i]!=_T("(")
					&& vcstr[i]!=_T(")")
					&& vcstr[i]!=_T(";")
					)
				{
					TCHAR ch = strSql.GetAt(tmp+vcstr[i].GetLength());
					
					if(isalpha(ch))
					{
						continue;
					}
				}

				if(isBefore)
				{
					strSql.Insert(istart ,_T("\r\n\t"));
					if(vcstr[i]==_T("AND")||vcstr[i]==_T("("))
					{
						strSql.Insert(istart+3 ,sepTab);
						istart ++;
					}
				}
				else
				{
					strSql.Insert(tmp ,_T("\r\n\t"));
					if(vcstr[i]==_T("AND")||vcstr[i]==_T("("))
					{
						strSql.Insert(tmp+3 ,sepTab);
						istart ++;
					}
				}

				istart += 3;
				tmp = 0;
			}
			else
			{
				break;
			}
		}
	}

}

void SQLFormat::judgeQuotes()
{
	std::stack<CString> keys;
	std::stack<CString> oper;
	
	for(int i=0;i<(int)vfunction.size();i++)
	{
		int istart = 0;
		while(istart<strSql.GetLength()&&istart>=0)
		{
			int tmp = strSql.Find(vfunction[i],istart);
			if(tmp>-1&&tmp>istart)
			{
				istart = tmp + vfunction[i].GetLength();
				int left = strSql.Find(_T("("),tmp);
				
				if(left!=-1 && left>tmp)
				{
					int right = strSql.Find(_T(")"),left);
					if(right!=-1 &&right >left)
					{
						CString strLeft= strSql.Mid(0,left+1);
						CString strMid = strSql.Mid(left+5,right-left-8);
						CString strRight = strSql.Mid(right);

						strLeft = strLeft.Mid(0,left-4) + _T("(");
						strSql = strLeft + strMid + strRight;
					}
				}
			}
			else
			{
				break;
			}

		}
	}
}

void SQLFormat::formatTab()
{
	CString tabSpace;
	for(int i=0;i<8;i++)
	{
		tabSpace.Append(sepSpace);
	}

	strSql.Replace(sepTab,tabSpace);
}

void SQLFormat::formatComma()
{
	int istart = 0;
	int tmp = 0;
	
	//���ŵ�λ��
	std::vector<int> vctComma;
	//����
	std::vector<CString> vctWords;

	std::stack<int> stkComma;
	std::stack<int> stkCommaTmp;

	CString strOper(_T("\",=-+.;!?)(\\/"));
	
	while (istart<strSql.GetLength()&&istart>=0)
	{
		tmp = strSql.Find(_T(","),istart);
		if(tmp!=-1)
		{
			istart = tmp +1;
			stkComma.push(tmp);
			if(stkComma.size()>1)
			{
				int wordEnd = stkComma.top();
				stkComma.pop();
				int wordBegin = stkComma.top();
				
				//��鶺���붺��֮���Ƿ��ǵ���
				CString strTmp = strSql.Mid(wordBegin+1,wordEnd-wordBegin-1);
				strTmp.Trim();
				
				
				//�����Ƿ���
				bool isFind = false;
				for(int i=0;i<strOper.GetLength();i++)
				{
					if(strTmp.Find(strOper[i],0)!=-1)
					{
						isFind = true;
						break;
					}
				}
				
				//���Ƿ��ż�Ϊ���ʣ����ʺ󶺺���Ҫ�ָ�
				if(!isFind)
				{
					stkComma.push(wordEnd);
				}
				else
				{
					while(!stkComma.empty())
					{
						//�洢����
						stkCommaTmp.push(stkComma.top());
						stkComma.pop();
					}
					int count = 0;
					while(!stkCommaTmp.empty())
					{
						vctComma.push_back(stkCommaTmp.top());
						stkCommaTmp.pop();

						count++;
						//ÿ��3�����ʻ���
						if(count%3==0)
						{
							vctComma.push_back(-1);
						}
					}
					stkComma.push(wordEnd);
				}
			}
		}
		else
		{
			while(!stkComma.empty())
			{
				//�洢����
				stkCommaTmp.push(stkComma.top());
				stkComma.pop();
			}
			int count = 0;
			while(!stkCommaTmp.empty())
			{
				vctComma.push_back(stkCommaTmp.top());
				stkCommaTmp.pop();

				count++;
				//ÿ��3�����ʻ���
				if(count%3==0)
				{
					vctComma.push_back(-1);
				}
			}
			break;
		}

	}

	CString fbefore;
	for(std::vector<int>::iterator  iter = vctComma.begin();iter!=vctComma.end();iter++)
	{
		int commaStart = *iter;
		if(commaStart!=-1)
		{
			//�����Ƿ���
			bool isFind = false;
			CString strTmp;
			while(!isFind)
			{
				commaStart--;
				if(commaStart == -1)break;
				TCHAR ch = strSql.GetAt(commaStart);
				for(int i=0;i<strOper.GetLength();i++)
				{
					if(strOper[i] == ch)
					{
						isFind = true;
						break;
					}
				}
				
				if(!isFind)
					strTmp.AppendChar(ch);
			}
			strTmp.MakeReverse();
			if(iter == vctComma.begin())
				fbefore = strTmp;
			if(strTmp.Trim()!=sepSpace)
				vctWords.push_back(strTmp);
		}
		else
		{
			vctWords.push_back(sepSpace);
		}
	}

	int maxLen = 0;
	if(vctWords.size()>0)
	{
		for(vector<CString>::iterator it = vbefore.begin(); it != vbefore.end(); it ++)
		{
			if(fbefore.Find(*it) != -1)
			{
				fbefore = *it;
				break;
			}
		}
		maxLen = (*vctWords.begin()).GetLength();
	}
	
	for(int j=0;j<fbefore.GetLength();j++)
	{
		fbefore.SetAt(j, _T(' '));
		//if(isalpha(fbefore.GetAt(j))==0)
		//{
		//	fbefore.Replace(fbefore.GetAt(j),_T(""));
		//}
	}

	for(std::vector<CString>::iterator iter = vctWords.begin();iter!=vctWords.end();iter++)
	{
		if(maxLen<(*iter).GetLength())
		{
			maxLen = (*iter).GetLength();
		}
	}

	istart = 0;
	for(int i = 0;i< (int)vctComma.size();i++)
	{
		int commaStart = vctComma[i];
		if(commaStart!=-1)
		{
			int len = maxLen-vctWords[i].GetLength();
			CString strTmp(_T(""));
			while(len-->0)
			{
				strTmp+=sepSpace;
			}
			strSql.Insert(commaStart+istart,strTmp);
			istart += maxLen-vctWords[i].GetLength();
		}
		else
		{
			strSql.Insert(vctComma[i-1]+istart+1,_T("\r\n")+fbefore);
			istart += 2 + fbefore.GetLength();
		}
	}

	if(vctWords.size() > 0 && vctWords[0].Find(_T("SELECT")) != -1)
	{
		vctWords[0].Replace(_T("SELECT"), _T(""));
		vctWords[0].Trim();
		int len = maxLen-vctWords[0].GetLength();
		CString strTmp(_T(""));
		while(len-->0)
		{
			strTmp+=sepSpace;
		}
		strSql.Insert(vctComma[0], strTmp);	
		strSql.Insert(vctComma[0] - vctWords[0].GetLength() - 1, fbefore);
	}

}

void SQLFormat::formatIndentation()
{

}

void SQLFormat::formatSpace()
{
	CStringArray arr;
	Util::SplitString(strSql, _T(" "), arr);
	vector<CString> vstrs;
	for(int i = 0 ;i<arr.GetSize();i++)
	{
		CString tmp = arr.GetAt(i);
		tmp.Trim();
		vstrs.push_back(tmp);
	}

	CString strTmp;
	bool first = true;
	bool second = false;
	bool before = false;
	for(vector<CString>::iterator it = vstrs.begin(); it != vstrs.end(); it++)
	{
		first = second;
		if(std::find(vbefore.begin(), vbefore.end(), *it)!= vbefore.end())
		{
			second = true;
			before = true;
		}
		else
		{
			second = false;
		}

		if(!second)
		{
			if(std::find(vbehind.begin(), vbehind.end(), *it)!= vbehind.end())
			{
				second = true;
				before = false;
			}
			else
			{
				second = false;
			}
		}

		CString tmp;
		if(second)
		{
			if(before)
			{
				tmp.Append(*it);		
				tmp.Append(sepChangeLine);	
			}
			else
			{
				tmp.Append(sepChangeLine);	
				tmp.Append(*it);
			}
		}
		else
		{
			tmp.Append(*it);
		}

		if(first)
		{
			strTmp.Append(_T(" "));
			strTmp.Append(tmp);	
		}
		else
			strTmp.Append(tmp);	

		
	}

	strSql = strTmp;
}

bool SQLFormat::formatColumn(CString& strOut)
{
	//1���ҵ�FROM�ؼ��֣���SELECT��FROMȡ��
	//������ͨ��ɨ���ҵ�SELECT��ջ������FROM�򵯳�SELECT��
	//      ��������SELECTΪ���һ�����ʾѰ�����  
	CString strTemp;
	int offset = findColumn(strTemp, strSql);

	if(offset == -1)
	{
		return false;
	}

	//2������ɨ��SELECTѡ�1����ɨ�赽������������������ջ������ɨ�裬
	//   ����ɨ�赽���������ߡ�����������������ɨ�赽������������ջ��
	//   ��ɨ�赽�������򡰣�����ջ���жϡ������ǲ������һ����
	//   �������һ��������ȡ�������롰����֮��ĵ��ʡ�
	//   ���õ��������´�������ɨ�衰������������һ��������ʱ�������(���롰����֮��ĵ��ʣ�
	//   �жϸõ���֮���Ƿ��ж��ţ��ж���ȡ���ŷָ�֮ǰ���Ǹ����ʣ�����ȡ���е��ʡ�
	//   2��ɨ�赽����������֮ǰɨ�赽�Ĳ���Ϊһ�����ʣ���������ջ��������ջ��
	//   ����ջ���ڲ������λ�ã�����ջ���ڴ������
	int curPosition = 0;
	CString strWord;
	std::vector<CString> stackWords;
	std::vector<int> statckComma;
	std::stack<int> statckQuotes;
	for(int i = 0; i < strTemp.GetLength(); i ++ )
	{
		TCHAR chr = strTemp.GetAt(i);
		if(chr == _T('(')) 
		{
			statckQuotes.push(i);
		}
		else if(chr == _T(')')) 
		{
			int iStart = statckQuotes.top();
			statckQuotes.pop();
			if(statckQuotes.empty())
			{
				int iEnd = i;
				strWord = strTemp.Mid(iStart, iEnd - iStart + 1);
			}
		}
		else if(statckQuotes.empty() && chr == _T(',')) 
		{
			statckComma.push_back(i);
			stackWords.push_back(strWord);
			strWord = _T("");
		}
		else if(statckQuotes.empty())
		{
			strWord.AppendChar(chr);
		}
	}

	statckComma.push_back(strTemp.ReverseFind(_T('\r')));
	stackWords.push_back(strWord);

	CString alia;
	std::map<CString, CString> maps;
	std::vector<CString> stackAlias;
	std::vector<int>::iterator jt = statckComma.begin();
	for(std::vector<CString>::iterator it = stackWords.begin(); it != stackWords.end(); it ++)
	{
		CString str = *it;
		strWord = _T("");
		alia = _T("");

		if(str.Find(_T("\"")) != -1) // �Ѿ����б���
		{			
			/*int iStart = str.Find(_T("\""));
			int iEnd = str.Find(_T("\""), iStart);
			alia = str.Mid(iStart + 1, iEnd - iStart - 1);
			stackAlias.push_back(alia);*/
			jt = statckComma.erase(jt);
		}
		else if(judgeColumn(str, strWord))
		{
			jt = statckComma.erase(jt);
			maps.insert(std::map<CString, CString>::value_type(str, strWord));
		}
		else // ��������
		{
			jt ++;
			for(int i = 0; i < str.GetLength(); i ++)
			{
				TCHAR chr = str.GetAt(i);
				if(chr == _T('(')) 
				{
					statckQuotes.push(i);
				}
				else if(chr == _T(')')) 
				{
					int iStart = statckQuotes.top();
					statckQuotes.pop();
					int iEnd = i;
					strWord = str.Mid(iStart + 1, iEnd - iStart - 1);
					while(!statckQuotes.empty())
					{
						statckQuotes.pop();
					}
					// �ж�strWord �Ƿ��С���������Select
					strTemp = _T("");
					findColumn(strTemp, strWord);
					strWord = _T("");
					for(int k = 0; k < strTemp.GetLength(); k ++ )
					{
						TCHAR chr = strTemp.GetAt(k);
						if(chr == _T(',')) 
						{
							break;
						}
						else
						{
							strWord.AppendChar(chr);
						}
					}
					break;
				}
				else if(statckQuotes.empty())
				{
					strWord.AppendChar(chr);
				}
			}

			strWord.Trim();
			strWord.Trim(_T("\r\n"));

			// �������������
			for(int i = 0; i < strWord.GetLength(); i ++)
			{
				TCHAR chr = strWord.GetAt(i);
				if(i == 0)
				{
					alia.AppendChar(tolower(chr));
				}
				else if(chr == _T('_')) 
				{
					if(++i < strWord.GetLength())
					{
						chr = strWord.GetAt(i);
						alia.AppendChar(toupper(chr));
					}
				}
				else if(chr == _T('.'))
				{
					alia = _T("");
				}
				else
				{
					alia.AppendChar(tolower(chr));
				}
			}

			stackAlias.push_back(_T(" \"") + alia + _T("\""));
			
		}
		
	}

	// �����ݲ���ָ���ĵط�
	strOut = strSql;
	for(int i = 0; i < statckComma.size(); i ++)
	{
		strOut.Insert(statckComma[i] + offset, stackAlias[i]);
		offset += stackAlias[i].GetLength();
	}

	for(std::map<CString, CString>::iterator it = maps.begin(); it != maps.end(); it ++)
	{
		int i = strOut.Replace(it->first, it->second);
		printf("%d", i);
	}

	if(isQuotes)
	{
		strOut.Replace(_T("\""), _T("\\\""));
	}

	return true;
}


int SQLFormat::findColumn(CString& strTemp, const CString& strInput)
{
	std::stack<int> stackSelect;
	bool found = false;
	int iStart = -1;
	CString firstWord;
	for(int i = 0; i < strInput.GetLength(); i ++ )
	{
		TCHAR chr = strInput.GetAt(i);
		if(chr != _T(' ') && chr != _T('(') && chr != _T(')') && chr != _T(','))
		{
			strTemp.AppendChar(chr);
		}
		else
		{
			if(!strTemp.IsEmpty())
			{
				strTemp.Trim();
				strTemp.Trim(_T("\r\n"));
				if(Util::compareIgnorCase(strTemp, _T("SELECT")))
				{
					stackSelect.push(i);
				}
				else if(Util::compareIgnorCase(strTemp, _T("FROM")))
				{
					if(!stackSelect.empty())
					{
						iStart = stackSelect.top();
						stackSelect.pop();
						if(stackSelect.empty())
						{
							int iEnd = i - 5; // �ո�+FROM
							strTemp = strInput.Mid(iStart, iEnd - iStart);
							found = true;
							break;
						}
					}					
				}
				if(firstWord.IsEmpty())
				{
					firstWord = strTemp;
				}
				strTemp = _T("");
			}
		}
	}

	if(!found)
	{
		strTemp = strInput;
	}

	// ��һ�����ʲ���select֤��Ϊ�Ǳ�׼SQL����-1
	if(!Util::compareIgnorCase(firstWord, _T("SELECT")))
	{
		return -1;
	}
	
	return iStart;
}

bool SQLFormat::judgeColumn(const CString& strInput, CString& strOutput)
{
	CString strTemp = strInput;
	strTemp.MakeReverse();
	strTemp.Trim();
	strTemp.Trim(_T("\r\n"));
	CString strWord;
	for(int i = 0; i < strTemp.GetLength(); i ++ )
	{
		TCHAR chr = strTemp.GetAt(i);
		if(chr == _T(' '))
		{
			for(int i = 0; i < strWord.GetLength(); i ++ )
			{
				if(chr == _T('(') || chr == _T(')') || chr == _T(','))
				{
					return false;
				}
			}
			strOutput = strInput;
			strOutput.Insert(strInput.ReverseFind(_T(' ')) + 1, _T("\""));
			strOutput.Insert(strInput.ReverseFind(_T(' ')) + 2 + strWord.GetLength(), _T("\""));
			return true;
		}
		else
		{
			strWord.AppendChar(chr);
		}
	}

	return false;
}

