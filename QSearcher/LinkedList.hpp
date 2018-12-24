#pragma once
template<typename T>
class LinkedList
{
public:
	// 链表节点类
	struct Node
	{
		Node(const T& _data, Node* _prior = nullptr, Node* _next = nullptr) :data(_data), prior(_prior), next(_next) {}

		T data;
		Node* next = nullptr;
		Node* prior = nullptr;

		Node* operator++() {
			if (this)
				return next;
			else
				return nullptr;
		}
	};

	// 构造与析构函数
	LinkedList() {}
	LinkedList(LinkedList&& src) {
		iter_head = src.iter_head;
		iter_tail = src.iter_tail;
		_length = src._length;
		src.iter_head = src.iter_tail = nullptr;
	}
	~LinkedList() {
		Node* p = iter_head;
		Node* q = nullptr;
		while (p != nullptr)
		{
			q = p;
			p = p->next;
			delete q;
		}
	}
    void operator=(LinkedList&& src){
        Node* p = iter_head;
        Node* q = nullptr;
        while (p != nullptr)
        {
            q = p;
            p = p->next;
            delete q;
        }

        iter_head = src.iter_head;
        iter_tail = src.iter_tail;
        _length = src._length;
        src.iter_head = src.iter_tail = nullptr;
    }

	// 添加、查找、删除等操作
	Node* head() const {
		return iter_head;
	}

	Node* tail() const {
		return iter_tail;
	}

	int length() const {
		return _length;
	}

	Node* add(const T& data) {
		if (iter_head == nullptr)
		{
			iter_tail = iter_head = new Node(data);
		}
		else
		{
			iter_tail->next = new Node(data, iter_tail);
			iter_tail = iter_tail->next;
		}
		_length++;
		return iter_tail;
	}

	void addInDescOrder(const T& data) {
		if (iter_head == nullptr)
		{
			iter_tail = iter_head = new Node(data);
		}
		else
		{
			if (iter_tail->data >= data)
			{
				iter_tail = new Node(data, iter_tail, nullptr);
				iter_tail->prior->next = iter_tail;
			}
			else
			{
				Node* p = iter_tail;
				while (p && p->data < data)
				{
					p = p->prior;
				}
				if (p)
				{
					p->next = new Node(data, p, p->next);
					p->next->next->prior = p->next;
				}
				else
				{
					iter_head = new Node(data, nullptr, iter_head);
					iter_head->next->prior = iter_head;
				}
			}
		}
		_length++;
	}

	Node* search(const T& data) const {
		Node* p = iter_head;
		while (p)
		{
			if (p->data == data)
			{
				return p;
			}
			p = p->next;
		}
		return nullptr;
	}

	Node* search(Node* q) const {
		Node* p = iter_head;
		while (p)
		{
			if (p == q)
			{
				return p;
			}
			p = p->next;
		}
		return nullptr;
	}

	bool remove(const T& data){
		Node* p = search(data);
		if (p == nullptr)
		{
			return false;
		}

		if (p == iter_head)
		{
			iter_head = p->next;
			iter_head->prior = nullptr;
			delete p;
		}
		else if(p == iter_tail)
		{
			iter_tail = p->prior;
			iter_tail->next = nullptr;
			delete p;
		}
		else
		{
			p->prior->next = p->next;
			p->next->prior = p->prior;
			delete p;
		}
		--_length;
	}

	bool remove(Node* p) {
		if (p == nullptr)
		{
			return false;
		}

		if (p == iter_head)
		{
			iter_head = p->next;
			if(iter_head)
				iter_head->prior = nullptr;
			delete p;
		}
		else if (p == iter_tail)
		{
			iter_tail = p->prior;
			if(iter_tail)
				iter_tail->next = nullptr;
			delete p;
		}
		else
		{
			p->prior->next = p->next;
			p->next->prior = p->prior;
			delete p;
		}
		--_length;
	}

private:
	Node* iter_head = nullptr;	// 表头指针
	Node* iter_tail = nullptr;	// 表尾指针
	int _length = 0;			// 链表的长度
};
