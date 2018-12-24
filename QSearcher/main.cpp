#include <iostream>
#include <locale>
#include <io.h>
#include "htmlDatabase.h"
#include "htmlHandler.h"
#include "CharString.h"
#include "Stack.hpp"
#include "LinkedList.hpp"
#include "Vector.hpp"

using namespace std;

int main()
{
	std::locale::global(std::locale("chs"));
	htmlHandler::init(".\\config.xml");
	htmlHandler::initDictionary();

	////提前解析网页文件的接口
	//long handle;
	//struct _finddata_t fileinfo;
	//handle = _findfirst(".\\input\\*.html", &fileinfo);
	//if (handle == -1)
	//	return -1;
	//do
	//{
	//	std::cout << "正在解析文件：" << fileinfo.name << std::endl;

	//	htmlHandler handle(fileinfo.name);
	//	std::wofstream out;

	//	handle.extractInfo();
	//	handle.divideWords();

	//	const htmlInfo& info = handle.info;
	//	out.open(handle.filename + L".info", std::ios::out);
	//	out << info.title << std::endl << info.source << std::endl << info.time << std::endl << info.content;
	//	out.close();

	//	const LinkedList<CharString>& dividedWords = handle.dividedWords;
	//	out.open(handle.filename + L".txt", std::ios::out);
	//	auto p = dividedWords.head();
	//	while (p)
	//	{
	//		out << p->data << std::endl;
	//		p = p->next;
	//	}
	//	out.close();

	//	globalBase.addHtml(std::move(handle.info));
	//	
	//	std::cout << "解析完成：" << fileinfo.name << std::endl;

	//} while (!_findnext(handle, &fileinfo));

	//_findclose(handle);

	//globalBase.preTreat();
	//globalBase.saveToFile(L"data.hmif", L"data.w2l");

	//加载数据文件
	globalBase.loadFromFile(L"data.hmif", L"data.w2l");

	wifstream in;
	wofstream out;
	wstring str;
	int i = 0;
	cout << "query1:" << endl;
	in.open(L"query1.txt");
	out.open(L"result1.txt");
	while (in.peek() != WEOF)
	{
		printf("正在检索：%d\n", i);
		i++;
		getline(in, str);
		auto&& query1 = htmlHandler::divideWords(str);
		auto&& result1 = globalBase.query1(query1);
		for (auto p = result1.head(); p != nullptr; p = ++*p)
		{
			out << '(' << p->data.urlID << ',' << p->data.occurTimes << ") ";
		}
		out << std::endl;
	}
	in.close();
	out.close();

	cout << "query2:" << endl;
	in.open(L"query2.txt");
	out.open(L"result2.txt");
	i = 0;
	while (in.peek() != WEOF)
	{
		printf("正在推荐：%d\n", i);
		i++;
		getline(in, str);
		bool flag = false;
		auto&& result2 = globalBase.query2(str, &flag);
		if (flag)
		{
			out << L"该新闻不在数据库中，无法推荐";
		}
		else
		{
			int i = 0;
			for (auto p = result2.head(); p != nullptr; p = ++*p)
			{
				if (i == 5)
					break;
				out << '(' << p->data.urlID << ',' << p->data.newsName << ") ";
				i++;
			}
		}
		out << std::endl;
	}
	in.close();
	out.close();

	return 0;
}