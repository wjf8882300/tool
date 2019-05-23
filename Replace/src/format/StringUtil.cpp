#include "format/StringUtil.h"
#include "Util/util.h"

void StringUtil::stringToUnicode(const std::string& characterStr, std::string& unicodeStr)
{
	vector<std::string> result;
	Util::parseListIgnoreEmpty(result, characterStr, "\\u");
	char* str = new char[result.size()*2 + 1];
	wchar_t* wstr = new wchar_t[result.size() + 1];
	memset(wstr, 0, result.size() + 1);
	for(int i = 0; i < result.size(); i ++)
	{
		if(result[i].length() != 4)continue;
		int c;
		sscanf(result[i].c_str(),"%X",&c);
		wstr[i] = c;
	}
    setlocale(LC_ALL, "");
    wcstombs(str, wstr, result.size()*2 + 1);
	str[result.size()*2] = '\0';
	unicodeStr = std::string(str);
	delete [] str;
	delete [] wstr;
}