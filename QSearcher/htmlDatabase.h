#pragma once
#include "Vector.hpp"
#include "htmlHandler.h"
#include "DocLList.h"


class htmlDatabase
{
	struct result1 {
		int urlID;
		int occurTimes = 0;
		int occurPages = 0;

		bool operator<(const result1& a) {
			return (occurPages < a.occurPages) || (occurPages == a.occurPages && occurTimes < a.occurTimes);
		}

		bool operator>=(const result1& a) {
			return !operator<(a);
		}
	};

	struct result2 {
		int urlID;
		CharString newsName;
		double similarity = 0;

		bool operator<(const result2& a) {
			return similarity < a.similarity;
		}

		bool operator>=(const result2& a) {
			return !operator<(a);
		}
	};
public:
	htmlDatabase();
	~htmlDatabase();
	void saveToFile(const CharString& hmifpath, const CharString& w2lpath);
	void loadFromFile(const CharString& hmifpath, const CharString& w2lpath);

	void addHtml(htmlInfo&& html);
	void preTreat();
	LinkedList<result1> query1(const LinkedList<CharString>& words);
	LinkedList<result2> query2(CharString title, bool* exist = nullptr, double inf = 0.02);

	AVLTree<CharString, DocLList> word2list;
	Vector<htmlInfo> htmls;

private:
	void DFS_word(AVLTree<CharString, htmlInfo::word>::Node* p, htmlInfo* info);
	double calCosine(htmlInfo* a, htmlInfo* b);
};

extern htmlDatabase globalBase;

