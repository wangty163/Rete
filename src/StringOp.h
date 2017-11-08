/*************************************************
File name:   // StringOp.h
Author:      // ������
Version:     // V1.0
Date:        // 2012-5-1
Description: // ���ļ��ṩCStringOp��--�ַ��������࣬���ַ���������ص���ķ�װ
Class List: // ��Ҫ����ṹ
	CStringOp-�ַ���������
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
Author:      // ������
Version:     // V1.0
Date:        // 2012-5-1
Description: // CCurrentDoc�����ַ���������
Function List: // ��Ҫ�����뷽��
	1��Find(const string& str, const string& sub)���ַ�������
	2��Find_Last_Of(const string& str, const string& sub)�����������ֵ�λ��
	3��FindAny(const string& str, const vector<string>& subs, string& foundStr)������ָ���������������Ӵ�
	4��Split(vector<string> &arrResult, const string& strSource, const string& strSep)���ָ�
	5��vector<string> Split(const string& strSource, const vector<string>& strsSep, const bool remove_null, const bool remove_sep)���ָ��
	6��vector<string> getSequence(const string& str)��ȡ���ַ�����
	7��tolowercase(const string& str)��תΪСд��ĸ
	8��string trim(const string& str,const string& delimiter=" ")��//ȥ��ͷβָ�����ַ�����Ĭ�Ͽո�
	9��int CharType(const string& str)���ж�ָ���ַ�
	10��CT FromString(const string& str)���ַ���ת����ָ������
	11��string ToString(CT t)��ת�����ַ���
	12��int GBK_ID(const string &sChar)������GBK���뷵��һ���ַ���ID
	13��bool GBK_CHAR(string &sChar, int id)������GBK_ID���ض�Ӧ�ַ�
	14��bool isSingleWord(const string& word)���ж��Ƿ񵥸����֡�Ӣ�ĵ��ʡ����ִ�
	15��int StringType(const string& str)���ж�һ���ַ���������
	16��string ReverseString(const string& str)����ת�ַ���
	17��bool IsPunc(const string& str)���ж��Ƿ�Ͼ���
	18��IsAllDiffChar(const string& sWord1,const string& sWord2)���Ƿ��Ӧλ���ַ�������ͬ
	19��DiffCount(const string& sWord1,const string& sWord2)����ͬ�ַ��ĸ���
	20��bool DiffChar(vector<string>& vecDiff,const string& sWord1,const string& sWord2)���Ƿ�ͬ�ַ�
	21��int GetPosOfDiffChr(const string& sWord1,const string& sWord2)��ȡ�������ַ�����ͬ�ַ���λ��
	22��bool IsNum(const string& s)���Ƿ�����
	23��bool IsTime(const string& s)���Ƿ�ʱ��
	24��bool IsYear(const string& s)���Ƿ���
	25��bool IsArabNum(const string& s)���Ƿ�Arab����
	26��bool IsChnNum(const string& s)���Ƿ���������
*************************************************/
namespace CStringOp
{
	size_t Find(const string& str, const string& sub, size_t offset = 0);
	size_t Find_Last_Of(const string& str, const string& sub);
	size_t FindAny(const string& str, const vector<string>& subs, string& foundStr);
	//str�Ƿ���subs�е������Ӵ�
	bool IsFindAll(const string& str, const vector<string>& subs);
	bool Split(vector<string> &arrResult, const string& strSource, const string& strSep);
	vector<string> Split(const string& strSource, const vector<string>& strsSep, const bool remove_null, const bool remove_sep);
	vector<string> getSequence(const std::string& str);
	size_t getSequenceCnt(const std::string& str);
	string tolowercase(const string& str);
	//ȥ�����ס���β������ָ�����ַ���delimiter�п���ָ������ַ�
	string trim(const string& str,const string& delimiter=" ");
	template <class CT> CT FromString(const string& str);
	template <class CT> string ToString(CT t);	
	//��ȡ�ı��У�ָ���ؼ��ֺ��һ�����ִ�
	long long GetNumberAfterFirstKey(const string& content, const string& key);
	//��ȡ�ı��У�ָ���ؼ���ǰ��һ�����ִ�
	long long GetNumberBeforeFirstKey(const string& content, const string& key);
	size_t GetLength(const string& str);
	//�滻�ı�������ָ�����Ӵ�
	std::string replaceAll(const std::string& str, const std::string& target, const std::string& replacement);
	// �ж�str�Ƿ�����prefixΪ��ʼ
	bool startsWith(const std::string& str, const std::string& prefix);

	//��str���зָ��sepΪ�磨�������������sep����ô�ᱻ�ָ��Σ��˹��ܺ�Split������ͬ��
	std::vector<std::string> split(const std::string& str, const std::string& sep);
	//��str���зָ��seps�е�����һ��Ϊ�磨�������������sep����ô�ᱻ�ָ��Σ�
	std::vector<std::string> split(const std::string & str, const std::vector<std::string>& seps, bool remove_sep);

	// ���������ַ���������������г���
	size_t lcsCnt(const std::string& origin, const std::string& suggestion);

	// ��str�ظ����ɴη���
	std::string repeat(const std::string& str, int repeatCnt);
	bool isChinese(const std::string& str);
	bool isDigit(const std::string& str);
	bool isAlpha(const std::string& str);

	/*	�������	*/
	//gbkתutf8
	string GBKToUTF8(const std::string& strGBK);
	//utf8תgbk
	string UTF8ToGBK(const std::string& strUTF8);

	// line���磺aa/w1 bb/w2 cc/w3
	bool GetWordAndPos(const string& sLine, vector<string>& vecWords, vector<string>& vecPos);
	// str���磺aa/w1
	void GetWordAndPos(const string& str, string& word, string& pos);

	string ReverseString(const string& str);
	bool IsPunc(const string& str); //�ж��Ƿ�Ͼ���
	bool IsAllDiffChar(const string& sWord1,const string& sWord2);
	int DiffCount(const string& sWord1,const string& sWord2);
	//bool DiffChar(vector<string>& vecDiff,const string& sWord1,const string& sWord2);
	int GetPosOfDiffChr(const string& sWord1,const string& sWord2); //ȡ�������ַ�����ͬ�ַ���λ��

	bool IsNum(const string& s);
	bool IsTime(const string& s);
	bool IsYear(const string& s);

	bool IsArabNum(const string& s);
	bool IsChnNum(const string& s);
};
//��������ת�� string->CT

template <class CT> CT CStringOp::FromString(const string& str)
{
	CT t;
	istringstream istr(str);
	istr >> t;
	return t;
}

//��������ת�� string<-CT

template <class CT> 
string CStringOp::ToString(CT t)
{
	ostringstream ostr;
	ostr <<setprecision(8)<<fixed<< t;  //�̶����Կ�ѧ�������������cout.unsetf(ostream::floatfield)ȡ��
	return (string)ostr.str();
}
//ԭʼ�汾
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