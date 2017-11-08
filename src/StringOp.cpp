#include "StringOp.h"

#include "PublicDefine.h"

//CStringOp::CStringOp(void)
//{
//}
//
//
//CStringOp::~CStringOp(void)
//{
//}
//
//功能说明：在字符串中查找子串
//
size_t CStringOp::Find(const string& str, const string& sub, size_t offset)
{
	size_t nSize1 = str.size();
	size_t nSize2 = sub.size();

	if (nSize1 == 0 || nSize2 == 0 || nSize1<nSize2)
		return string::npos;

	size_t k = offset;
	size_t d = nSize1 - nSize2;

	while (k <= d)
	{
		if (sub == str.substr(k, nSize2))
			return k;

		if (!isascii(str[k]))
			//if (str[k] < 0)
			k += 2;
		else
			++k;
	}

	return string::npos;
} 
//
//功能说明：查找子串在字符串中最后出现的位置
//
size_t CStringOp::Find_Last_Of(const string& str, const string& sub)
{
	 size_t nSize1 = str.size();
	 size_t nSize2 = sub.size();
 
	 if( nSize1==0 || nSize2==0 || nSize1<nSize2 )
	  return string::npos;
 
	 size_t k = 0;
	 size_t d = nSize1 - nSize2;
	 size_t index=-1;
 
	 while ( k <= d )
	 {
		if ( sub == str.substr(k, nSize2) )
		{
			index=k;
		}
 
		if ( str[k] < 0 )
		//--if (!isascii(str[k]))
		k += 2;
		else
		++k;
	 }
	 if(index>=0&&index<nSize1)
		 return index;
	 else
		return string::npos;
} 
//
//功能说明：在字符串中查找所有子串最开始出现的子串及其位置
//
size_t CStringOp::FindAny(const string& str, const vector<string>& subs, string& foundStr)
{
	vector<string>::const_iterator itor = subs.begin();
	size_t t = 0;
	size_t min = string::npos;
	for(; itor!=subs.end(); ++itor)
	{
		if(itor->size()==0) continue;

		t = Find(str, *itor);
		if(t != string::npos)
		{
			if(min==string::npos || t<min)
			{
				min = t;
				foundStr = *itor;
			}
		}
	}
	return min;
}
bool CStringOp::IsFindAll(const string & str, const vector<string>& subs)
{
	for (int i = 0; i < subs.size(); i++) {
		if (Find(str, subs[i]) == string::npos)
			return false;
	}
	return true;
}
//
//将strSource按strSep拆分成字符串数组
//
bool CStringOp::Split(vector<string> &arrResult, const string& strSource, const string& strSep)
{
	arrResult.clear();
	string strSplit = strSource;

	if (strSep == "")//分隔符为空，一个字一个字读入
	{
	    int nSize = strSource.size();
        for( int ix=0; ix<nSize; ++ix )
        {
            if( strSource.at(ix) > 0 )//Ascii字符
            {
                strSplit = strSource.substr(ix, 1);
                arrResult.push_back(strSplit);
            }
            else if( (ix+1 < nSize))//汉字
            {
                strSplit = strSource.substr(ix++, 2);
                arrResult.push_back(strSplit);
            }
        }
	}
	else//分隔符不为空，按分隔符切分
	{
        string::size_type nEnd = 0;

        if ( !strSep.empty() )
        {
            while ( (nEnd = Find(strSplit, strSep)) != string::npos )
            {
                string item = strSplit.substr(0, nEnd);
                if ( item != "" )
                {
                    arrResult.push_back(item);
                }
                strSplit = strSplit.substr(nEnd + strSep.size());
            }

            if ( strSplit != "" )
            {
                arrResult.push_back(strSplit);
            }
        }
	}

	return true;
}
//Split
//remove_null : 是否删除空的项
//remove_sep  : 在split后的各个项中，是否删除分隔符
vector<string> CStringOp::Split(const string& strSource, const vector<string>& strsSep, const bool remove_null, const bool remove_sep)
{
	vector<string> vRes;
	string strSplit = strSource;
	string::size_type nEnd = 0;
	string foundStr;

	if ( strsSep.size()!=0 )
	{
		while ( (nEnd = FindAny(strSplit, strsSep, foundStr)) != string::npos )
		{
			string item = strSplit.substr(0, nEnd);			
			if(remove_null)
			{
				if(!item.empty())
				{
					vRes.push_back(item);
				}
			}
			else
			{
				vRes.push_back(item);
			}
			if(!remove_sep)
				vRes.push_back(foundStr);
			strSplit = strSplit.substr(nEnd + foundStr.length());
		}
		if(remove_null)
		{
			if(!strSplit.empty())
			{
				vRes.push_back(strSplit);
			}
		}
		else
		{
			vRes.push_back(strSplit);
		}
	}
	return vRes;
}
//
//功能说明：字符串拆分成字符序列
//
vector<string> CStringOp::getSequence(const string& str)
{
	std::vector<std::string> ret;
	int len = 1;
	for (int i = 0; i < str.size(); i += len) {
		len = isascii(str.at(i)) ? 1 : 2;
		myAssert(i + len <= str.size());
		ret.push_back(str.substr(i, len));
	}
	return ret;
}
size_t CStringOp::getSequenceCnt(const std::string & str)
{
	size_t ret = 0;
	int len = 1;
	for (int i = 0; i < str.size(); i += len) {
		len = isascii(str.at(i)) ? 1 : 2;
		myAssert(i + len <= str.size());
		++ret;
	}
	return ret;
}
//
//功能说明：将大写字母转为小写字母
//
string CStringOp::tolowercase(const string& str)
{
	string strResult="";
	int ix=0, nSize=str.size();
	while ( ix < nSize )
	{
		if ( str[ix] < 0 && (ix+1 < nSize) )//双字节字符，如汉字等
		//--if ( !isascii(str[ix]) && (ix+1 < nSize) )//双字节字符，如汉字等
		{
			strResult+=str.substr(ix,2);
			ix+=2;
		}
		else
		{
			strResult+=(char)tolower(str[ix]);
			++ix;
		}
	}
	return strResult;
}
/*
	去掉首尾连续的指定的字符
*/
string CStringOp::trim(const string& str,const string& delimiters)
{
	string result="";
	vector<string> seqVec=getSequence(str);
	int len=seqVec.size();
	bool flag=false;
	int begin_index=0;
	int end_index=len-1;
	for(int i=0;i<len;++i)
	{
		if(Find(delimiters,seqVec[i])!=string::npos&&!flag)
		{
			flag=false;	
			//控制句子只是标点符号或空格
			if(i==len-1)
			{
				begin_index=i+1;
			}
		}
		else
		{
			begin_index=i;
			break;
		}
	}
	flag=false;
	for(int i=len-1;i>=0;--i)
	{
		if(Find(delimiters,seqVec[i])!=string::npos&&!flag)
		{
			flag=false;			
		}
		else
		{
			end_index=i;
			break;
		}
	}
	for(int ix=begin_index;ix<=end_index;++ix)
	{
		result+=seqVec[ix];
	}
	return result;
}

//
//逆转字符串
//
string CStringOp::ReverseString(const string& str)
{
	string sResult = "";
	
	for (int ix=str.size()-1; ix>=0; --ix)
	{
		if (str[ix] > 0)
		{
			sResult += str.substr(ix, 1);
		}
		else if (ix-1 >= 0)
		{
			sResult += str.substr(ix-1, 2);
			--ix;
		}
	}

	return sResult;
}
bool CStringOp::IsPunc(const string& str)//判断是否断句标点
{
	if (str == "," || str == "." || str == ";" || str == "?" || str == "!" || str == "？" || str == "。" || str == "，" || str == "！" || str == "；")
	{
		return true;
	}
	return false;
}
/*
*功能说明：判断两个字符串是不是都是不同的字符
*/
bool CStringOp::IsAllDiffChar(const string& sWord1,const string& sWord2)
{
	vector<string> vecWord1,vecWord2;
	 Split(vecWord1,sWord1,"");
	 Split(vecWord2,sWord2,"");

	 int nSize1=vecWord1.size();
	 if(nSize1!=vecWord2.size())
	 {
		 return false;
	 }
	 int nDiff=0;
	 for(int ix=0;ix<nSize1;++ix)
	 {
		 if(vecWord1[ix]!=vecWord2[ix])
		 {
			 ++nDiff;
		 }
	 }

	 if(nDiff==nSize1)
	 {
		 return true;
	 }

	 return false;
}
/*
*功能说明：取得两个字符串不同的字数个数
*
*/
int CStringOp::DiffCount(const string& sWord1,const string& sWord2)
{
	vector<string> vecWord1,vecWord2;
	 Split(vecWord1,sWord1,"");
	 Split(vecWord2,sWord2,"");
	 
	 int nSize1=vecWord1.size();
	 if(nSize1!=vecWord2.size())
	 {
		 return false;
	 }
	 int nDiff=0;
	 for(int ix=0;ix<nSize1;++ix)
	 {
		 if(vecWord1[ix]!=vecWord2[ix])
		 {
			 ++nDiff;
		 }
	 }
	 return nDiff;
}
/*
*功能说明：取得两个字符串不同字符的位置
*返回值：返回0表示头位置字符不同，返回sWord1.size()-1表示尾字符字符不同，0<diff<sWord1.size()-1表示中间位置不同，否则返回-1
*注意：两个字符串长度必须相等。
*/
int CStringOp::GetPosOfDiffChr(const string& sWord1,const string& sWord2)
{
	vector<string> vecWord1;
	vector<string> vecWord2;

	Split(vecWord1,sWord1,"");
	Split(vecWord2,sWord2,"");
	int nDiffPos=-1;
	size_t nSize=vecWord1.size();
	if(nSize!=vecWord2.size())
	{
		return -1;
	}
	if(vecWord1[0]!=vecWord2[0])
	{
		for(size_t ix=1;ix<nSize;++ix)
		{
			if(vecWord1[ix]!=vecWord2[ix])
			{
				return -1;
			}
		}
		nDiffPos=0;
	}
	else if(vecWord1[nSize-1]!=vecWord2[nSize-1])
	{
		for(size_t ix=0;ix<nSize-1;++ix)
		{
			if(vecWord1[ix]!=vecWord2[ix])
			{
				return -1;
			}
		}
		nDiffPos=nSize-1;
	}
	else
	{
		//中间可以控制几个不等
		for(size_t ix=1;ix<nSize-1;++ix)
		{
			if(vecWord1[ix]!=vecWord2[ix])
			{
				return ix;
			}
		}
		//表示两个字符串相等
		nDiffPos=-1;
	}

	return nDiffPos;
}
bool CStringOp::IsArabNum(const string& s) 
{
	int nDotCount=0;
	int nMinusCount=0;
	int nSize=s.length();
     for (int i=0;i<nSize;i++)
	 {
         if (s[i]<48 || s[i]>57)
		 {
			 if((s[i]=='.'&&(i==0||i==s.length()-1)))
			 {
				 return false;
			 }
			 if(s[i]=='-'&&i!=0)
			 {
				 return false;
			 }
			 if(s[i]=='.')
			 {
				 ++nDotCount;
			 }
			 if(s[i]=='-')
			 {
				 ++nMinusCount;
			 }
			 if(nDotCount>1||nMinusCount>1)
			 {
				 return false;
			 }
            if (s[i]!='.' && s[i]!='-') 
			{
				return false;
			}
		 }
	 }
     return true;
}
bool CStringOp::IsChnNum(const string& s)
{
	int nSize=s.length();
     for (int i=0;i<nSize;i+=2)
	 {
         string t = s.substr(i,2);
		 if(t=="百"||t=="千"||t=="亿"||t=="兆"||t=="点")
		 {
			 if(i==0)
			 {
				 return false;
			 }
		 }
         if (t!="一" && t!="二" && t!="三" && t!="四" && t!="五" && 
            t!="六" && t!="七" && t!="八" && t!="九" && t!="零" &&
            t!="十" && t!="百" && t!="千" && t!="万" && t!="亿" &&
            t!="兆" && t!="点")
		 {
			return false;
		 }
     }
     return true;
}
/*
*功能说明：判断一个串是否是数字串
*
*/
bool CStringOp::IsNum(const string& s) 
{
    string t = s.substr(0,2);
    if (t=="第" && (IsArabNum(s.substr(2)) || IsChnNum(s.substr(2))))
	{
		return true;//序数
	}
    if (IsArabNum(s))
	{
		return true;//阿拉伯数字 
	}
    if (IsChnNum(s))
	{
		return true; //中文数字
	}
    return false; 
}
bool CStringOp::IsTime(const string& s)
{
	string str;
	if(CStringOp::Find(s,"年")==s.size()-2||CStringOp::Find(s,"月")==s.size()-2||CStringOp::Find(s,"日")==s.size()-2)
	{
		str=s.substr(0,s.size()-1);
		if(IsNum(str))
		{
			return true;
		}
	}
	return false;
}
/*
*功能说明：判断一个字符串是否是数字年份
*格式：2003-2004
*/
bool CStringOp::IsYear(const string& s)
{
	size_t nSize=0;
	size_t ix=0;

	if(s.at(0)<0)
	//--if(!isascii(s.at(0)))
	{
		return false;
	}

	nSize=s.size();

	while(ix<nSize)
	{
		if(s.at(ix)>0)
		{
			if((s.at(ix)>='0'&&s.at(ix)<='9')||s.at(ix)=='-')
			{
				++ix;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	if(ix==nSize)
	{
		return true;
	}
	
	return false;
}

string CStringOp::GBKToUTF8(const std::string& strGBK)
{
	string strOutUTF8 = "";
	WCHAR * str1;
	int n = MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, NULL, 0);
	str1 = new WCHAR[n];
	MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, str1, n);
	n = WideCharToMultiByte(CP_UTF8, 0, str1, -1, NULL, 0, NULL, NULL);
	char * str2 = new char[n];
	WideCharToMultiByte(CP_UTF8, 0, str1, -1, str2, n, NULL, NULL);
	strOutUTF8 = str2;
	delete[]str1;
	str1 = NULL;
	delete[]str2;
	str2 = NULL;
	return strOutUTF8;
}

string CStringOp::UTF8ToGBK(const std::string& strUTF8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, NULL, 0);
	wchar_t* wszGBK = new wchar_t[len + 1];
	memset(wszGBK, 0, len + 1);
	MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, wszGBK, len);

	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	char *szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
	//strUTF8 = szGBK;  
	std::string strTemp(szGBK);
	delete[]szGBK;
	delete[]wszGBK;
	return strTemp;
}

bool CStringOp::GetWordAndPos(const string & sLine, vector<string>& vecWords, vector<string>& vecPos)
{
	vector<string> vecSen;
	CStringOp::Split(vecSen, sLine, " ");
	vecWords.clear();
	vecWords.reserve(vecSen.size());
	vecPos.clear();
	vecPos.reserve(vecSen.size());

	for (auto&& sen : vecSen) {
		std::string sWord, sPos;
		GetWordAndPos(sen, sWord, sPos);
		vecWords.push_back(sWord);
		vecPos.push_back(sPos);
	}

	return vecWords.size() > 0;
}

void CStringOp::GetWordAndPos(const string & str, string & word, string & pos)
{
	size_t index = CStringOp::Find_Last_Of(str, "/");
	word = str.substr(0, index);
	//20140625添加，将空格作为标点符号，在split的时候由于按空格split，将空格去掉了：
	//例如：3.1/m  /w 气候/n 特征/n
	if (word == "")
		word = " ";
	pos = index == std::string::npos ? "" : str.substr(index + 1);
}

//在content中，获取第一个key之后，紧邻的数字（掠过空格和逗号）
long long CStringOp::GetNumberAfterFirstKey(const string& content, const string& key) {
	int index = content.find(key);
	if (index != -1) {
		index += key.length();
		long long value = 0;
		while (index < content.length() && content[index] >= -1 && content[index] <= 255) {
			if (isdigit(content[index]))
				value = value * 10 + content[index++] - '0';
			else if (content[index] == ',' || content[index] == ' ') {
				index++;
				continue;
			}
			else
				break;
		}
		return value;
	}
	return -1;
}

//在content中，获取第一个key之前，紧邻的数字（掠过空格和逗号）
long long CStringOp::GetNumberBeforeFirstKey(const string& content, const string& key) {
	//int index = content.find(key);
	int index = Find(content, key);
	//if (index != -1) {
	if (index != string::npos) {
		long long value = 0, mask = 1;
		while (--index>= 0) {
			if (isdigit(content[index])) {
				value = value + (content[index] - '0') * mask;
				mask *= 10;
			}
			else if (content[index] == ',' || content[index] == ' ')
				continue;
			else
				break;
		}
		return value;
	}
	return string::npos;
}

size_t CStringOp::GetLength(const string & str)
{
	size_t length = 0;
	for (size_t i = 0, width = 1; i < str.size(); ) {
		if (str[i] < 0)
		//--if (!isascii(str[i]))
			width = 2;
		else
			width = 1;
		i += width;
		length++;
	}
	return length;
}

std::string CStringOp::replaceAll(const std::string & str, const std::string & target, const std::string & replacement)
{
	std::string ret = str;
	size_t index = Find(ret, target);
	size_t targetLen = target.length(), replacementLen = replacement.length();
	while (index != std::string::npos) {
		ret = ret.replace(index, targetLen, replacement);
		index = Find(ret, target, index + replacementLen);
	}
	return ret;
}

bool CStringOp::startsWith(const std::string & str, const std::string & prefix)
{
	if (str.size() < prefix.size())
		return false;
	for (size_t i = 0; i < prefix.size(); i++) {
		if (str.at(i) != prefix.at(i))
			return false;
	}
	return true;
}



std::vector<std::string> CStringOp::split(const std::string & str, const std::string& sep)
{
	myAssertPlus(!sep.empty(), "split函数的分隔符不能为空！");
	std::vector<std::string> ret;
	//分隔符不为空，按分隔符切分
	size_t offset = 0, index;
	while (offset < str.length()) {
		index = Find(str, sep, offset);
		if (index == std::string::npos)
			index = str.length();
		ret.push_back(str.substr(offset, index - offset));
		offset = index += sep.length();
	}
	return ret;
}

std::vector<std::string> CStringOp::split(const std::string & str, const std::vector<std::string>& seps, bool remove_sep) {
	std::vector<std::string> ret[2];
	int cur = 0;
	ret[cur].push_back(str);
	for (auto&& sep : seps) {
		ret[cur ^ 1].clear();
		for (auto&& s : ret[cur]) {
			bool first = true;
			for (auto&& part : split(s, sep)) {
				if (!remove_sep && !first)
					ret[cur ^ 1].push_back(sep);
				ret[cur ^ 1].push_back(part);
				first = false;
			}
		}
		cur ^= 1;
	}
	return ret[cur];
}

size_t CStringOp::lcsCnt(const std::string & origin, const std::string & suggestion) {
	auto vt0 = getSequence(origin), vt1 = getSequence(suggestion);
	std::vector<std::vector<size_t>> dp(vt0.size() + 1, std::vector<size_t>(vt1.size() + 1, 0));
	for (size_t i = 1; i <= vt0.size(); ++i) {
		for (size_t j = 1; j <= vt1.size(); ++j) {
			if (vt0[i - 1] == vt1[j - 1])
				dp[i][j] = dp[i - 1][j - 1] + 1;
			else
				dp[i][j] = max(dp[i][j - 1], dp[i - 1][j]);
		}
	}
	return dp[vt0.size()][vt1.size()];
}

std::string CStringOp::repeat(const std::string & str, int repeatCnt)
{
	std::string ret;
	for (int i = 0; i < repeatCnt; ++i)
		ret += str;
	return ret;
}

bool CStringOp::isChinese(const std::string & str) {
	return !str.empty() && !isascii((unsigned char)str.front());
}
bool CStringOp::isDigit(const std::string& str) {
	return !str.empty() && isdigit((unsigned char)str.front());
}
bool CStringOp::isAlpha(const std::string& str) {
	return !str.empty() && isalpha((unsigned char)str.front());
}
