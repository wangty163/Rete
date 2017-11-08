/*************************************************
File name:   // OOVRecognition.h
Author:      // 刘亮亮
Version:     // V1.0
Date:        // 2014-11-6
Description: // 未登录词识别相关类与操作
Class List: // 主要类列表
MatchType——后件操结构
MatchResultNode——匹配结果节点
PNodeType——谓词类型
COOVRecognition——未登录词识别类
*************************************************/
#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>

#include "Rete/ReteNet/Net.h"

using namespace std;

//20150228添加后件操作，执行后件的类型
typedef enum MatchType
{
	oov,
	ok,
	mark,
	rewrite
}MatchType;

//20150228添加后件操作
//记录了词语的区间：nBegin、nEnd，和对应的正确的词语
struct MatchResultNode
{
	int nBegin;
	int nEnd;
	MatchType type;
	string sCorrectWord;
	std::string matchedPattern;
};

/*************************************************
Class name:   // 新词识别类COOVRecognition
Author:      // 刘亮亮
Version:     // V1.0
Date:        //
Description: // 该类主要利用词的模式来识别文本中的未登录词;
Function List: // 主要函数列表，包括函数名及功能简要说明
*************************************************/

const string OOV_WORDCLASS_FILE = "rul\\oov_wordclass.txt";
const string OOV_PATTERN_FILE = "rul\\oov_pattern.txt";

class COOVRecognitionByPattern
{
public:
	COOVRecognitionByPattern(void);
	~COOVRecognitionByPattern(void);
	//初始化函数
	static bool Init();
	//模式识别
	bool RecognitionByOOVPattern(vector<MatchResultNode>&vecResult, const std::vector<std::string>& words, const std::vector<std::string>& poses);
protected:
	static bool ReadWordFile(const string& sFileName = OOV_WORDCLASS_FILE);	//读取词类词典
	static bool ReadPatternFile(const string& sFileName = OOV_PATTERN_FILE);	//读取模式词典
private:
	// 添加属性对应的操作
	static void addReteJudgeFunctions();
	// 由一条文本规则，生成对应的条件
	static std::pair<std::vector<ConditionVector>, ConditionVector> parse(const std::string& rawLine);
	// 由含有“|”运算符的规则，生成若干个不包含“|”的完整规则（这些规则的或运算的结果和原有规则相同）。
	static void COOVRecognitionByPattern::generate(std::vector<std::vector<Condition>>& result, size_t index, std::vector<Condition>& partResult, const std::vector<std::vector<std::vector<Condition>>>& cs);
	// 将句子转化后，添加进rete网络中等待识别
	static void addSentence(const std::vector<std::string>& words, const std::vector<std::string>& poses);

	static unordered_map<string, vector<string>> mapWord2Class;	//词到词类的索引 
	map<string, string> mapValue;
	static Net net;
};

