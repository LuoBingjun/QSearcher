#include "DocLList.h"

DocLList::DocLList()
{
}

DocLList::~DocLList()
{
	Node* p = iter_head;
	Node* q = nullptr;
	while (p != nullptr)
	{
		q = p;
		p = p->next;
		delete q;
	}
}

void DocLList::Add(htmlInfo *_html, htmlInfo::word& word)
{
	if (iter_head == nullptr)
	{
		iter_tail = iter_head = new Node(_html, word);
	}
	else
	{
		if (iter_tail->word.tf >= word.tf)
		{
			iter_tail = new Node(_html, word, iter_tail, nullptr);
			iter_tail->prior->next = iter_tail;
		}
		else
		{
			Node* p = iter_tail;
			while (p && p->word.tf < word.tf)
			{
				p = p->prior;
			}
			if (p)
			{
				p->next = new Node(_html, word, p, p->next);
				p->next->next->prior = p->next;
			}
			else
			{
				iter_head = new Node(_html, word, nullptr, iter_head);
				iter_head->next->prior = iter_head;
			}
		}
	}
	_length++;
}

bool DocLList::Search(htmlInfo * _html)
{
	Node* p = iter_head;
	while (p)
	{
		if (p->html == _html)
		{
			return true;
		}
		p = p->next;
	}
	return false;
}

bool DocLList::Search(int ID)
{
	Node* p = iter_head;
	while (p)
	{
		if (p->html->ID == ID)
		{
			return true;
		}
		p = p->next;
	}
	return false;
}

bool DocLList::Remove(htmlInfo * _html)
{
	Node* p = iter_head;
	while (p)
	{
		if (p->html == _html)
		{
			p->prior->next = p->next;
			if (p == iter_tail)
			{
				iter_tail = p->prior;
			}
			delete p;
			return true;
		}
		p = p->next;
	}
	return false;
}

bool DocLList::Remove(int ID)
{
	Node* p = iter_head;
	while (p)
	{
		if (p->html->ID == ID)
		{
			p->prior->next = p->next;
			if (p == iter_tail)
			{
				iter_tail = p->prior;
			}
			delete p;
			return true;
		}
		p = p->next;
	}
	return false;
}

htmlInfo * DocLList::Edit(int ID)
{
	Node* p = iter_head;
	while (p)
	{
		if (p->html->ID == ID)
		{
			return p->html;
		}
		p = p->next;
	}
	return nullptr;
}
