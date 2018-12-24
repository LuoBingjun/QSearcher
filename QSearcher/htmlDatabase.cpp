#include "htmlDatabase.h"
#include "htmlHandler.h"
#include "cmath"
#include <queue>

htmlDatabase globalBase;

htmlDatabase::htmlDatabase() {}

htmlDatabase::~htmlDatabase() {}

void htmlDatabase::saveToFile(const CharString& hmifpath, const CharString& w2lpath)
{
	std::wofstream out;
	out.open(hmifpath, std::ios::out);
	for (int i = 0; i < htmls.length(); i++)
	{
		auto &p = htmls[i];
		out << p.ID << " " <<  p.vlength << std::endl;
		out << p.title << std::endl;
		out << p.source << std::endl;
		out << p.time << std::endl;
		out << p.description << std::endl;
		out << p.content << std::endl;
		out << L"CON_END" << std::endl;

		std::queue<decltype(p.keywords.root)> q;
		if(p.keywords.root)
			q.push(p.keywords.root);
		while (!q.empty())
		{
			auto front = q.front();
			q.pop();
			out << front->key << L' ' << front->value.tf << L' ' << front->value.tf_idf << std::endl;

			if (front->lchild)
				q.push(front->lchild);
			if (front->rchild)
				q.push(front->rchild);
		}
		out << L"_END" << std::endl;
	}
	out.close();

	out.open(w2lpath, std::ios::out);
	std::queue<decltype(word2list.root)> q;
	q.push(word2list.root);
	while (!q.empty())
	{
		auto front = q.front();
		q.pop();
		out << front->key << std::endl;
		auto p = front->value.head();
		while (p)
		{
			out << p->html->ID << std::endl;
			p = p->next;
		}
		out << L"-1" <<std::endl;

		if(front->lchild)
			q.push(front->lchild);
		if(front->rchild)
			q.push(front->rchild);
	}
	out.close();
}

void htmlDatabase::loadFromFile(const CharString& hmifpath, const CharString& w2lpath)
{
	std::wifstream in;
	in.open(hmifpath, std::ios::in);
	std::wstring str;

	int i;
	while (in >> i)
	{
		auto& p = htmls[i];

		p.ID = i;
		in >> p.vlength;
		in.get();
		std::getline(in, str);
		p.title = str;
		std::getline(in, str);
		p.source = str;
		std::getline(in, str);
		p.time = str;
		std::getline(in, str);
		p.description = str;

		std::getline(in, str);
		while (str != L"CON_END")
		{
			p.content.append(str);
               p.content.append(L'\n');
			std::getline(in, str);
		}

		int tf;
		double tf_idf;
		in >> str;
		while (str != L"_END")
		{
			in >> tf >> tf_idf;
			p.keywords.Insert(str, htmlInfo::word(tf, tf_idf));
			in >> str;
		}
	}
	in.close();

	in.open(w2lpath, std::ios::in);
	while (in >> str)
	{
		auto &doc = word2list[str];
		int ID;
		while (in >> ID)
		{
			if (ID == -1)
				break;
			doc.Add(&htmls[ID], htmls[ID].keywords[str]);
		}
	}
	in.close();
}

void htmlDatabase::addHtml(htmlInfo&& info)
{
	int ID = info.ID;
	htmls[ID] = std::move(info);
	DFS_word(htmls[ID].keywords.root, &htmls[ID]);
}


void htmlDatabase::preTreat()
{
	for (int i = 0; i < htmls.length(); i++)
	{
		auto& p = htmls[i];
		p.vlength = 0;
		std::queue<decltype(p.keywords.root)> q;
		if (p.keywords.root)
			q.push(p.keywords.root);
		while (!q.empty())
		{
			auto front = q.front();
			q.pop();
			front->value.tf_idf = log((double)htmls.length() / (double)word2list[front->key].length()) * front->value.tf;
			p.vlength += front->value.tf_idf*front->value.tf_idf;

			if (front->lchild)
				q.push(front->lchild);
			if (front->rchild)
				q.push(front->rchild);
		}
		p.vlength = sqrt(p.vlength);
	}
}

LinkedList<htmlDatabase::result1> htmlDatabase::query1(const LinkedList<CharString>& words)
{
	AVLTree<int, result1> map;
	for (auto j = words.head(); j != nullptr; j = ++*j)
	{
		for (auto k = word2list[j->data].head(); k != nullptr; k = ++*k)
		{
			map[k->html->ID].urlID = k->html->ID;
			++map[k->html->ID].occurPages;
			map[k->html->ID].occurTimes += k->word.tf;
		}
	}

	LinkedList<htmlDatabase::result1> res;
	std::queue<decltype(map.root)> q;
	if (map.root)
		q.push(map.root);
	while (!q.empty())
	{
		auto front = q.front();
		q.pop();
		res.addInDescOrder(front->value);

		if (front->lchild)
			q.push(front->lchild);
		if (front->rchild)
			q.push(front->rchild);
	}

	return res;
}

LinkedList<htmlDatabase::result2> htmlDatabase::query2(CharString title, bool* exist, double inf)
{
	htmlInfo* html = nullptr;
	LinkedList<htmlDatabase::result2> res;
	title.trimSpace();

	bool flag = true;
	for (int i = 0; i < htmls.length(); i++)
	{
		auto& p = htmls[i];
		if (p.title == title)
		{
			html = &p;
			flag = false;
			break;
		}
	}
	if (flag)
	{
		if (exist)
			*exist = true;
		return res;
	}

	AVLTree<int, char> searchlist;
	std::queue<decltype(html->keywords.root)> q;
	if (html->keywords.root)
		q.push(html->keywords.root);
	while (!q.empty())
	{
		auto front = q.front();
		q.pop();
		if (front->lchild)
			q.push(front->lchild);
		if (front->rchild)
			q.push(front->rchild);

		for (auto i = word2list[front->key].head(); i != nullptr; i = ++*i)
		{
			searchlist[i->html->ID] = 0;
		}
	}

	std::queue<decltype(searchlist.root)> q1;
	if (searchlist.root)
		q1.push(searchlist.root);
	while (!q1.empty())
	{
		auto front = q1.front();
		q1.pop();
		if (front->lchild)
			q1.push(front->lchild);
		if (front->rchild)
			q1.push(front->rchild);

		result2 result;
		result.urlID = front->key;
		result.newsName = htmls[front->key].title;
		result.similarity = calCosine(html, &htmls[front->key]);
		if(result.similarity > inf)
			res.addInDescOrder(result);
	}

	res.remove(res.head());

	return res;
}

void htmlDatabase::DFS_word(AVLTree<CharString, htmlInfo::word>::Node * p, htmlInfo* info)
{
	if (p == nullptr)
		return;

	word2list[p->key].Add(info, p->value);
	DFS_word(p->lchild, info);
	DFS_word(p->rchild, info);
}

double htmlDatabase::calCosine(htmlInfo * a, htmlInfo * b)
{
	std::queue<decltype(a->keywords.root)> q;
	if (a->keywords.root)
		q.push(a->keywords.root);
	double product = 0;
	while (!q.empty())
	{
		auto front = q.front();
		q.pop();
		
		if (b->keywords.Count(front->key))
		{
			auto &word = b->keywords[front->key];
            product += front->value.tf_idf * word.tf_idf;
		}

		if (front->lchild)
			q.push(front->lchild);
		if (front->rchild)
			q.push(front->rchild);
	}

	return product / (a->vlength*b->vlength);
}
