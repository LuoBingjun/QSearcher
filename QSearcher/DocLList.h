
#include "CharString.h"
#include "htmlHandler.h"

class DocLList
{
public:
	//文档链表节点类
	struct Node
	{
		Node(htmlInfo* _html, htmlInfo::word& _word, Node* _prior = nullptr, Node* _next = nullptr) :html(_html), word(_word), prior(_prior), next(_next) {}

		htmlInfo* html;	//该网页的全部信息
		int Times;	//出现该关键词的次数
		htmlInfo::word& word;
		Node* next = nullptr;
		Node* prior = nullptr;

		Node* operator++() {
			if (this)
				return next;
			else
				return nullptr;
		}
	};

	//构造和析构函数
	DocLList();
	~DocLList();

	//添加、搜索、编辑、删除文档等操作
	void Add(htmlInfo * _html, htmlInfo::word& word);
	bool Search(htmlInfo* _html);
	bool Search(int ID);
	htmlInfo* Edit(int ID);
	bool Remove(htmlInfo* _html);
	bool Remove(int ID);

	int length() const {
		return _length;
	}

	Node* head() {
		return iter_head;
	}

private:
	Node * iter_head = nullptr;	// 表头指针
	Node* iter_tail = nullptr;	// 表尾指针
	int _length = 0;			// 链表的长度
	CharString Term;			// 关键词
	int occur = 0;				// 关键词的总词频
};