/***************************************************************
 * FileUtil.h
 * 系统名  : 常用工具管理
 * 子系统名: 常用工具类
 * 更改日期      姓名           摘要
 * ----------    -----------    --------------------------------
 * 2011/04/01    王井锋			初次开发
 ***************************************************************
 */
#include "file/FileUtil.h"
FileUtil::FileUtil(std::vector<CString>& sr,std::vector<CString>& dt):sources(sr),dests(dt)
{

}


void FileUtil::AccessFile(const CString &path,const OPERATION oper,const bool fileNameRegex,const CString &ext,int& replaceFileCount)
{
	CFileFind finder;
	BOOL bWorking = finder.FindFile(path + _T("\\*.*")); 
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if(finder.IsDots()) continue;
		if (finder.IsDirectory())
		{
			AccessFile(finder.GetFilePath(),oper,fileNameRegex,ext,replaceFileCount);
		}
		else
		{
			bool isFound = false;
			CString fileName = finder.GetFileName();
			//检查文件名正则表达式
			if(fileNameRegex)
			{
				CStringArray arr;
				Util::SplitString(ext,_T(","),arr);
				//if(ext.Find(_T(","))!=-1)
				//{
				//	Util::SplitString(ext,_T(","),arr);
				//}
				for(int i=0;i<arr.GetSize();i++)
				{
					CRegexpT <TCHAR> regexp(arr[i].GetString());
					MatchResult result = regexp.Match(fileName);
					if(result.IsMatched())
					{
						isFound = true;
						break;
					}
				}

			}
			else
			{
				fileName = fileName.Right(fileName.GetLength()-fileName.Find(_T("."))).MakeLower();
				//找到符合条件的扩展名
				if(fileName==_T(".")+ext || fileName == ext)
				{
					 isFound = true;
				}

			}

			if(isFound)
			{
				switch(oper)
				{
				case OPERATION_UPDATE:
					if(ReplaceFileText(finder.GetFilePath()))
						replaceFileCount++;
					break;
				case OPERATION_DELETE:
					if(RemoveFile(finder.GetFilePath()))
						replaceFileCount++;
					break;

				}
			}
		}
	} 

	finder.Close();

	return;
}

bool FileUtil::ReplaceFileText(const CString &path)
{
	CString newPath = path + _T(".tmp");

#ifdef UNICODE
	std::wifstream fin(path.GetString());
	std::wofstream fos(newPath.GetString());
	std::wstring str;
	CString tmp;
	int count = 0;
	for (; !fin.eof(); str.clear())
	{
		std::getline(fin,str);

		if(!str.empty())
		{
			tmp = str.c_str();
			for(int i = 0;i<(int)sources.size();i++)
			{
				if(tmp.Replace(sources[i],dests[i])>0)
				{
					count++;
				}
			}

			str = tmp.GetString();
		}

		fos<<str<<std::endl;
	}

	fos.close();
	fin.close();
#else
	std::ifstream fin(path.GetString());
	std::ofstream fos(newPath.GetString());
	std::string str;
	CString tmp;
	int count = 0;
	for (; !fin.eof(); str.clear())
	{
		std::getline(fin,str);

		if(!str.empty())
		{
			tmp = str.c_str();
			for(int i = 0;i<(int)sources.size();i++)
			{
				if(tmp.Replace(sources[i],dests[i])>0)
				{
					count++;
				}
			}

			str = tmp.GetString();
		}

		fos<<str<<std::endl;
	}

	fos.close();
	fin.close();
#endif

	CFile::Remove(path);
	CFile::Rename(newPath,path);

	if(count>0) return true;
	return false;
}

void FileUtil::ReadFileContent(const CString &path)
{
	//std::wifstream fin(path.GetString());
	//std::wstring str;
	//CString tmp;
	//for (; !fin.eof(); str.clear())
	//{
	//	std::getline(fin,str);

	//	if(!str.empty())
	//	{
	//		tmp = str.c_str();
	//		sources.push_back(tmp);
	//	}
	//}
	//fin.close();
	
	setlocale(LC_ALL,"Chinese-simplified");//设置中文环境
	std::string filePath = Util::Wcstombs(path);
	std::ifstream fin(filePath.c_str());
	setlocale(LC_ALL, "C");//还原之前环境
	std::string str;
	for (; !fin.eof(); str.clear())
	{
		std::getline(fin,str);

		if(!str.empty())
		{
			sources.push_back(Util::Mbstowcs(str));
		}
	}
	fin.close();

	/*CString str;
	CStdioFile file(path, CFile::modeRead);
	ULONGLONG flen = file.GetLength();
	while(file.GetPosition() < flen)
	{
		file.ReadString(str);
		sources.push_back(str);
	}
	file.Close();*/
}

bool FileUtil::RemoveFile(CString path)
{
	CFile::Remove(path);

	return true;
}

