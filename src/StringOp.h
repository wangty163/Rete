/*************************************************
File name:   // StringOp.h
Author:      // 刘亮亮
Version:     // V1.0
Date:        // 2012-5-1
Description: // 该文件提供CStringOp类--字符串操作类，与字符串操作相关的类的封装
Class List: // 主要类与结构
	CStringOp-字符串操作类
*************************************************/
#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <windows.h>

using namespace std;

/*************************************************
Class name:   // CStringOp
Author:      // 刘亮亮
Version:     // V1.0
Date:        // 2012-5-1
Description: // CCurrentDoc——字符串操作类
Function List: // 主要函数与方法
	1、Find(const string& str, const string& sub)：字符串查找
	2、Find_Last_Of(const string& str, const string& sub)：查找最后出现的位置
	3、FindAny(const string& str, const vector<string>& subs, string& foundStr)：查找指定的数组中任意子串
	4、Split(vector<string> &arrResult, const string& strSource, const string& strSep)：分割
	5、vector<string> Split(const string& strSource, const vector<string>& strsSep, const bool remove_null, const bool remove_sep)：分割函数
	6、vector<string> getSequence(const string& str)：取得字符序列
	7、tolowercase(const string& str)：转为小写字母
	8、string trim(const string& str,const string& delimiter=" ")：//去掉头尾指定的字符串，默认空格
	9、int CharType(const string& str)：判断指定字符
	10、CT FromString(const string& str)：字符串转换成指定类型
	11、string ToString(CT t)：转换成字符串
	12、int GBK_ID(const string &sChar)：根据GBK编码返回一个字符的ID
	13、bool GBK_CHAR(string &sChar, int id)：根据GBK_ID返回对应字符
	14、bool isSingleWord(const string& word)：判断是否单个汉字、英文单词、数字串
	15、int StringType(const string& str)：判断一个字符串的类型
	16、string ReverseString(const string& str)：逆转字符串
	17、bool IsPunc(const string& str)：判断是否断句标点
	18、IsAllDiffChar(const string& sWord1,const string& sWord2)：是否对应位置字符都不相同
	19、DiffCount(const string& sWord1,const string& sWord2)：不同字符的个数
	20、bool DiffChar(vector<string>& vecDiff,const string& sWord1,const string& sWord2)：是否不同字符
	21、int GetPosOfDiffChr(const string& sWord1,const string& sWord2)：取得两个字符串不同字符的位置
	22、bool IsNum(const string& s)：是否数字
	23、bool IsTime(const string& s)：是否时间
	24、bool IsYear(const string& s)：是否年
	25、bool IsArabNum(const string& s)：是否Arab数字
	26、bool IsChnNum(const string& s)：是否中文数字
*************************************************/
namespace CStringOp
{
	size_t Find(const string& str, const string& sub, size_t offset = 0);
	size_t Find_Last_Of(const string& str, const string& sub);
	size_t FindAny(const string& str, const vector<string>& subs, string& foundStr);
	//str是否含有subs中的所有子串
	bool IsFindAll(const string& str, const vector<string>& subs);
	bool Split(vector<string> &arrResult, const string& strSource, const string& strSep);
	vector<string> Split(const string& strSource, const vector<string>& strsSep, const bool remove_null, const bool remove_sep);
	vector<string> getSequence(const std::string& str);
	size_t getSequenceCnt(const std::string& str);
	string tolowercase(const string& str);
	//去掉行首、行尾连续的指定的字符。delimiter中可以指定多个字符
	string trim(const string& str,const string& delimiter=" ");
	template <class CT> CT FromString(const string& str);
	template <class CT> string ToString(CT t);	
	//获取文本中，指定关键字后第一个数字串
	long long GetNumberAfterFirstKey(const string& content, const string& key);
	//获取文本中，指定关键字前第一个数字串
	long long GetNumberBeforeFirstKey(const string& content, const string& key);
	size_t GetLength(const string& str);
	//替换文本中所有指定的子串
	std::string replaceAll(const std::string& str, const std::string& target, const std::string& replacement);
	// 判断str是否是以prefix为开始
	bool startsWith(const std::string& str, const std::string& prefix);

	//将str进行分割，以sep为界（如果出现连续的sep，那么会被分割多次，此功能和Split函数不同）
	std::vector<std::string> split(const std::string& str, const std::string& sep);
	//将str进行分割，以seps中的任意一个为界（如果出现连续的sep，那么会被分割多次）
	std::vector<std::string> split(const std::string & str, const std::vector<std::string>& seps, bool remove_sep);

	// 计算两个字符串的最长公共子序列长度
	size_t lcsCnt(const std::string& origin, const std::string& suggestion);

	// 将str重复若干次返回
	std::string repeat(const std::string& str, int repeatCnt);
	bool isChinese(const std::string& str);
	bool isDigit(const std::string& str);
	bool isAlpha(const std::string& str);

	/*	编码相关	*/
	//gbk转utf8
	string GBKToUTF8(const std::string& strGBK);
	//utf8转gbk
	string UTF8ToGBK(const std::string& strUTF8);

	// line形如：aa/w1 bb/w2 cc/w3
	bool GetWordAndPos(const string& sLine, vector<string>& vecWords, vector<string>& vecPos);
	// str形如：aa/w1
	void GetWordAndPos(const string& str, string& word, string& pos);

	string ReverseString(const string& str);
	bool IsPunc(const string& str); //判断是否断句标点
	bool IsAllDiffChar(const string& sWord1,const string& sWord2);
	int DiffCount(const string& sWord1,const string& sWord2);
	//bool DiffChar(vector<string>& vecDiff,const string& sWord1,const string& sWord2);
	int GetPosOfDiffChr(const string& sWord1,const string& sWord2); //取得两个字符串不同字符的位置

	bool IsNum(const string& s);
	bool IsTime(const string& s);
	bool IsYear(const string& s);

	bool IsArabNum(const string& s);
	bool IsChnNum(const string& s);
};
//数据类型转换 string->CT

template <class CT> CT CStringOp::FromString(const string& str)
{
	CT t;
	istringstream istr(str);
	istr >> t;
	return t;
}

//数据类型转换 string<-CT

template <class CT> 
string CStringOp::ToString(CT t)
{
	ostringstream ostr;
	ostr <<setprecision(8)<<fixed<< t;  //固定不以科学计数法输出，用cout.unsetf(ostream::floatfield)取消
	return (string)ostr.str();
}
//原始版本
//template <class CT> 
//string CStringOp::ToString(CT t)
//{
//	ostringstream ostr;
//	ostr << t;
//	return (string)ostr.str();
//}

enum StringType {
	STR_EMPTY = 0,
	STR_NUM = 1,
	STR_LETTER = 2,
	STR_CHINESE = 3,
	STR_OTHER = 4,
};