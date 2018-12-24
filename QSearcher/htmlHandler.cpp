#include "htmlHandler.h"
#include <sstream>

#define MAX(a,b) (a)>(b)?(a):(b) 

CharString htmlHandler::strTag[MAX_TYPES];
CharString htmlHandler::strTimesource[MAX_TYPES];
LinkedList<CharString> htmlHandler::strContent[MAX_TYPES];
HashMap<CharString, int> htmlHandler::dicWords(800000);
HashMap<CharString, int> htmlHandler::dicStop(8000);
int htmlHandler::maxDiclen = 0;
int htmlHandler::htmltypes = 0;

CharString str1(L"来源");
CharString str2(L"时间");
CharString str3_0(L"</script>");
CharString str3_1(L"</style>");
CharString str3_2(L"</i>");

htmlHandler::htmlHandler(const std::string& _filename): filename(_filename)
{
	std::wifstream in(".\\input\\" + _filename);
	htmlSrc = std::wstring((std::istreambuf_iterator<wchar_t>(in)), std::istreambuf_iterator<wchar_t>());
	CharString ID = filename.substring(0, filename.indexOf(L".htm") - 1);
	info.ID = std::stoi(ID);
	filename = L".\\output\\" + ID;
}

void htmlHandler::init(const char* filepath)
{
	// 加载html格式的标签设置
	std::wifstream in(filepath);
	CharString Config = std::wstring((std::istreambuf_iterator<wchar_t>(in)), std::istreambuf_iterator<wchar_t>());

	Stack<htmlLabel> s;
	int p = 0;
	while (p < Config.length())
	{
		if (Config[p] == L' ' || Config[p] == L'\n' || Config[p] == L'\t')
		{
			p++;
			continue;
		}

		htmlLabel label(Config, p);

		switch (label.state)
		{
		case 1:
			s.push(label);
			if (label.type == L"label")
			{
				strTag[htmltypes] = label.content;
			}
			break;

		case 2:
			if (label.type == L"label")
			{
				htmltypes++;
			}

			while (!s.empty())
			{
				if (s.top().state == 1 && s.top().type == label.type)
					break;
				s.pop();
			}
			break;

		case 3:
			if (label.type == L"timesource")
			{
				strTimesource[htmltypes] = label.content;
			}
			else if (label.type == L"content")
			{
				strContent[htmltypes].add(label.content);
			}
			break;
		}

		if (htmltypes >= MAX_TYPES)
			break;

		p = label.end + 1;
	}

	/*
	strTag[0] = L"class=\"post_content post_area clearfix\"";
	strTimesource[0] = L"class=\"post_time_source\"";
	strContent[0].add(L"id=\"endText\"");
	strContent[0].add(L"class=\"hidden\"");

	strTag[1] = L"class=\"ep-content\"";
	strTimesource[1] = L"class=\"ep-time-soure cDGray\"";
	strContent[1].add(L"class=\"end-text\"");

	strTag[2] = L"class=\"g-wrap\"";
	strTimesource[2] = L"class=\"art-about-new\"";
	strContent[2].add(L"class=\"end-text clearfix\"");
	*/
}

void htmlHandler::initDictionary()
{
	CharString word;

	std::wifstream in;
	in.open("in.dic");
	while (in.peek() != WEOF)
	{
		in >> word;
		maxDiclen = MAX(maxDiclen, word.length());
		dicWords.Insert(word, 0);
	}
	in.close();

	in.open("stopwords.dic");
	while (in.peek() != WEOF)
	{
		in >> word;
		dicStop.Insert(word, 0);
	}
	in.close();
}

LinkedList<CharString> htmlHandler::divideWords(const CharString& strquery)
{
	auto&& query = CharString(strquery).split(L' ');
	LinkedList<CharString> res;
	for(auto q = query.head(); q != nullptr; q = ++*q)
	{
		int p = 0;
		int i;

		auto& str = q->data;
		// 根据已有词库分词
		while (p < str.length())
		{
			for (i = maxDiclen; i >= 0; i--)
			{
				CharString&& word = str.substring(p, p + i);
				word.tolower();
				if (dicWords.Count(word) && !res.search(word))
				{
					res.add(word);
					p = p + i + 1;
					break;
				}

				if (dicStop.Count(word))
				{
					p = p + i + 1;
					i = -2;
					break;
				}
			}
			if (i == -1)
			{
				if (str[p] != L' ' && str[p] != L'\n')
				{
					res.add(str.substring(p, p));
				}
				p = p + 1;
			}
		}
	}

	return res;
}


void htmlHandler::extractInfo()
{
	CharString timesource;
	bool monitored = false;
	htmlLabel *head = nullptr;
	CharString* dest = nullptr;

	Stack<htmlLabel> s;
	int p = 0; //p为CharString中的操作指针
	while (p < htmlSrc.length())
	{
		if (htmlSrc[p] == L' ' || htmlSrc[p] == L'\n' || htmlSrc[p] == L'\t')
		{
			p++;
			continue;
		}

		htmlLabel label(htmlSrc, p);

		// 过滤特定标签中的代码
		if (label.state == 1)
		{
			if (label.type == L"script")
			{
				p = label.end + 1;
				int m = htmlSrc.indexOf(str3_0, p);
				p = m + str3_0.length();
				continue;
			}
			else if (label.type == L"style")
			{
				p = label.end + 1;
				int m = htmlSrc.indexOf(str3_1, p);
				p = m + str3_1.length();
				continue;
			}
			else if (label.type == L"i")
			{
				p = label.end + 1;
				int m = htmlSrc.indexOf(str3_2, p);
				p = m + str3_2.length();
				continue;
			}
			else if (label.type == L"!DOCTYPE")
			{
				p = label.end + 1;
				continue;
			}
			else if (label.type == L"meta")
			{
				if (label.content.indexOf(L"property=\"article:published_time\"") != -1)
				{
					label.content.trimSpace();
					info.time = label.content.substring(43, label.content.length() - 2);
				}
				p = label.end + 1;
				continue;
			}
		}

		switch (label.state)
		{
		case 0:
			// 报错
			break;

		case 1:
			s.push(label);
			if (label.type == L"h1")
			{
				monitored = true;
				dest = &info.title;
				head = &s.top();
			}
			else if (label.type == L"span")
			{
				if (label.content == L"class=\"author\"")
				{
					monitored = true;
					dest = &info.source;
					head = &s.top();
				}
				else if (label.content == L"class=\"ptime\"" || label.content == L"class=\"time\"")
				{
					monitored = true;
					dest = &info.time;
					head = &s.top();
				}
			}
			else
			{
				if (htmlType == -1)
				{
					for (int i = 0; i < htmltypes; i++)
					{
						if (label.content.indexOf(strTag[i]) != -1)
						{
							htmlType = i;
							break;
						}
					}
				}
				else
				{
					if (label.content.indexOf(strTimesource[htmlType]) != -1)
					{
						monitored = true;
						dest = &timesource;
						head = &s.top();
					}
					else
					{
						auto p = strContent[htmlType].head();
						while (p)
						{
							if (label.content.indexOf(p->data) != -1)
							{
								monitored = true;
								dest = &info.content;
								head = &s.top();
								break;
							}
							p = p->next;
						}
					}
				}
			}
			//std::wcout << label.type << L'@' << label.content << std::endl;
			break;

		case 2:
			if (monitored)
			{
				while (!s.empty())
				{
					if (s.top().state == 1 && s.top().type == label.type)
					{
						if (label.type == L"p" && (*dest)[dest->length() - 1] != '\n')
						{
							dest->append('\n');
						}
						if (&s.top() == head)
						{
							monitored = false;
						}
						s.pop();
						break;
					}
					else
					{
						s.pop();
					}
				}
			}
			else
			{
				while (!s.empty())
				{
					if (s.top().state == 1 && s.top().type == label.type)
					{
						s.pop();
						break;
					}
					s.pop();
				}
			}
			break;

		case 3:
			if (label.type == L"meta")
			{
				if (label.content.indexOf(L"name=\"description\"") != -1)
				{
					label.content.trimSpace();
					info.description = label.content.substring(28, label.content.length() - 2);
				}
				else if (label.content.indexOf(L"name=\"author\"") != -1)
				{
					label.content.trimSpace();
					info.source = label.content.substring(23, label.content.length() - 2);
				}
				//else if (label.content.indexOf(L"name=\"keywords\"") != -1)
				//{
				//	label.content.trimSpace();
				//	CharString keywords = label.content.substring(25, label.content.length() - 2);
				//	int i = 0, j = -1;
				//	std::wofstream s("in.dic", std::wfstream::app);
				//	while ((j = keywords.indexOf(L',', i)) != keywords.length())
				//	{
				//		CharString&& word = keywords.substring(i, j - 1);
				//		word.tolower();
				//		if (!dicWords.Count(word))
				//		{
				//			s << std::endl << word;
				//		}
				//		dicWords[word] = 0;
				//		info.keywords.Insert(word, 20);
				//		i = j + 1;
				//	}
				//	CharString&& word = keywords.substring(i, keywords.length() - 1);
				//	word.tolower();
				//	if (!dicWords.Count(word))
				//	{
				//		s << std::endl << word;
				//	}
				//	s.close();
				//	dicWords[word] = 0;
				//	info.keywords.Insert(word, 20);
				//}
			}
			break;

		case 4:
			if (monitored)
			{
				dest->append(label.content);
			}
			break;
		}

		p = label.end + 1;
	}

	//判空
	if (s.empty())
	{
		timesource.trimSpace();
		if (info.time.length() == 0 || info.source.length() == 0)
		{
			int m = timesource.indexOf(str1);
			int n = timesource.indexOf(str2);
			if (info.time.length() == 0)
			{
				if (n > 0)
				{
					info.time = timesource.substring(n + 1 + str2.length(), timesource.length() - 1);
				}
				else if (m > 0)
				{
					info.time = timesource.substring(0, m - 1);
				}
			}
			
			if (info.source.length() == 0)
			{
				if (m > 0)
				{
					info.source = timesource.substring(m + 1 + str1.length(), timesource.length() - 1);
				}
				else if (n > 0)
				{
					info.source = timesource.substring(0, n - 1);
				}
			}
		}

		info.title.trimSpace();
		info.source.trimSpace(1);
		info.time.trimSpace();
		info.content.trimSpace();
	}
}

void htmlHandler::divideWords()
{
	int p = 0;
	int i;

	// 提取标题关键字
	while (p < info.title.length())
	{
		for (i = maxDiclen; i >= 0; i--)
		{
			CharString&& word = info.title.substring(p, p + i);
			word.tolower();
			if (dicWords.Count(word))
			{
				info.keywords[word].tf = 1;
				p = p + i + 1;
				break;
			}

			if (dicStop.Count(word))
			{
				p = p + i + 1;
				i = -2;
				break;
			}
		}
		if (i == -1)
		{
			p = p + 1;
		}
	}

	// 根据已有词库分词
	p = 0;
	while (p < info.content.length())
	{
		for (i = maxDiclen; i >= 0; i--)
		{
			CharString&& word = info.content.substring(p, p + i);
			word.tolower();
			if (dicWords.Count(word))
			{
				dividedWords.add(word);
				if (info.keywords.Count(word))
				{
					++info.keywords[word].tf;
				}
				else
				{
					info.keywords.Insert(word, 1);
				}
				p = p + i + 1;
				break;
			}

			if (dicStop.Count(word))
			{
				p = p + i + 1;
				i = -2;
				break;
			}
		}
		if (i == -1)
		{
			if (info.content[p] != L' ' && info.content[p] != L'\n')
			{
				dividedWords.add(info.content.substring(p, p));
			}
			p = p + 1;
		}
	}
}

htmlHandler::~htmlHandler()
{

}

htmlHandler::htmlLabel::htmlLabel(const CharString & src, int _begin):begin(_begin)
{
	if (src.substring(begin, begin + 3) == L"<!--")
	{
		end = src.indexOf(L"-->", begin + 3) + 2;
		state = 5;
	}
	else if (src[begin] == '<')
	{
		end = src.indexOf('>', begin);

		CharString&& label = src.substring(begin, end);
		int p = label.indexOf(' ');
		if (label[1] == '/')
		{
			state = 2;
			type = label.substring(2, label.length() - 2);
		}
		else if (label[label.length() - 2] == '/')
		{
			state = 3;
			if (p == label.length())
			{
				type = label.substring(0, label.length() - 3);
			}
			else
			{
				type = label.substring(1, p - 1);
				content = label.substring(p + 1, label.length() - 3);
			}
		}
		else
		{
			state = 1;
			if (p == label.length())
			{
				type = label.substring(1, label.length() - 2);
			}
			else
			{
				type = label.substring(1, p - 1);
				content = label.substring(p + 1, label.length() - 2);
			}
		}
	}
	else
	{
		state = 4;
		end = src.indexOf('<', begin) - 1;
		CharString str = src.substring(begin, end);

		// 过滤&;符
		int p = 0;
		int q, r;
		while (true)
		{
			q = str.indexOf('&', p);
			if (q == str.length())
				break;
			r = str.indexOf(';', q);
			if (r == str.length())
				break;
			content.append(str.substring(p, q - 1));
			p = r + 1;
		}
		content.append(str.substring(p, str.length() - 1));
	}
}
