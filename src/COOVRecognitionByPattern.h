/*************************************************
File name:   // OOVRecognition.h
Author:      // ������
Version:     // V1.0
Date:        // 2014-11-6
Description: // δ��¼��ʶ������������
Class List: // ��Ҫ���б�
MatchType��������ٽṹ
MatchResultNode����ƥ�����ڵ�
PNodeType����ν������
COOVRecognition����δ��¼��ʶ����
*************************************************/
#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>

#include "Rete/ReteNet/Net.h"

using namespace std;

//20150228��Ӻ��������ִ�к��������
typedef enum MatchType
{
	oov,
	ok,
	mark,
	rewrite
}MatchType;

//20150228��Ӻ������
//��¼�˴�������䣺nBegin��nEnd���Ͷ�Ӧ����ȷ�Ĵ���
struct MatchResultNode
{
	int nBegin;
	int nEnd;
	MatchType type;
	string sCorrectWord;
	std::string matchedPattern;
};

/*************************************************
Class name:   // �´�ʶ����COOVRecognition
Author:      // ������
Version:     // V1.0
Date:        //
Description: // ������Ҫ���ôʵ�ģʽ��ʶ���ı��е�δ��¼��;
Function List: // ��Ҫ�����б����������������ܼ�Ҫ˵��
*************************************************/

const string OOV_WORDCLASS_FILE = "rul\\oov_wordclass.txt";
const string OOV_PATTERN_FILE = "rul\\oov_pattern.txt";

class COOVRecognitionByPattern
{
public:
	COOVRecognitionByPattern(void);
	~COOVRecognitionByPattern(void);
	//��ʼ������
	static bool Init();
	//ģʽʶ��
	bool RecognitionByOOVPattern(vector<MatchResultNode>&vecResult, const std::vector<std::string>& words, const std::vector<std::string>& poses);
protected:
	static bool ReadWordFile(const string& sFileName = OOV_WORDCLASS_FILE);	//��ȡ����ʵ�
	static bool ReadPatternFile(const string& sFileName = OOV_PATTERN_FILE);	//��ȡģʽ�ʵ�
private:
	// ������Զ�Ӧ�Ĳ���
	static void addReteJudgeFunctions();
	// ��һ���ı��������ɶ�Ӧ������
	static std::pair<std::vector<ConditionVector>, ConditionVector> parse(const std::string& rawLine);
	// �ɺ��С�|��������Ĺ����������ɸ���������|��������������Щ����Ļ�����Ľ����ԭ�й�����ͬ����
	static void COOVRecognitionByPattern::generate(std::vector<std::vector<Condition>>& result, size_t index, std::vector<Condition>& partResult, const std::vector<std::vector<std::vector<Condition>>>& cs);
	// ������ת������ӽ�rete�����еȴ�ʶ��
	static void addSentence(const std::vector<std::string>& words, const std::vector<std::string>& poses);

	static unordered_map<string, vector<string>> mapWord2Class;	//�ʵ���������� 
	map<string, string> mapValue;
	static Net net;
};

