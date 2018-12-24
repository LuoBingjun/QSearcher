#pragma once
#include <fstream>
#include <string>
#include <set>
#include "CharString.h"
#include "Stack.hpp"
#include "LinkedList.hpp"
#include "HashMap.hpp"
#include "AVLTree.hpp"
#include "Vector.hpp"
#include "LinkedList.hpp"

struct htmlInfo
{
	struct word {
		word(){}
		word(int _tf, double _tf_idf = 0): tf(_tf), tf_idf(_tf_idf){}

		int tf = 0;
		double tf_idf = 0;
	};

	int ID = -1;
	CharString title;
	CharString source;
	CharString time;
	CharString content;
	CharString description;
	double vlength = 0;
	AVLTree<CharString, word> keywords;
};

class htmlHandler
{
	struct htmlLabel
	{
		htmlLabel(const CharString& src, int begin);

		int state; //0为字符串非法,1为成对标签开始符(eg. <script>),2为成对标签结束符(eg. </script>),3为非成对标签(eg. <br />),4为标签外内容,5为注释
		int begin = 0;
		int end = 0;
		CharString type;
		CharString content;
	};

public:
	htmlHandler(const std::string& _filename);
	~htmlHandler();

	static void init(const char* filepath);
	static void initDictionary();
	static LinkedList<CharString> divideWords(const CharString& str);

	void extractInfo();
	void divideWords();

	CharString filename;
	htmlInfo info;
	LinkedList<CharString> dividedWords;
	int htmlType = -1;

private:
	const static int MAX_TYPES = 10;
	static CharString strTag[MAX_TYPES];
	static CharString strTimesource[MAX_TYPES];
	static LinkedList<CharString> strContent[MAX_TYPES];
	static HashMap<CharString, int> dicWords;
	static HashMap<CharString, int> dicStop;
	static int maxDiclen;
	static int htmltypes;

	CharString htmlSrc;
};

