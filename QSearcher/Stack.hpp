#pragma once
template <typename T>
class Stack
{
	// 栈节点类
	struct Node
	{
		Node(const T& _data, Node* _lower = nullptr) :data(_data), lower(_lower) {}
		friend class Stack;

		T data;
		Node* lower = nullptr;
	};

public:
	// 构造与析构函数
	Stack(){}
	~Stack() {
		while (iter_top)
		{
			pop();
		}
	}

	// 压栈、退栈、栈顶、是否为空的函数接口
	void push(const T& data){
		Node *p = new Node(data, iter_top);
		iter_top = p;
		length++;
	}

	void pop() {
		if (iter_top)
		{
			Node *p = iter_top->lower;
			delete iter_top;
			iter_top = p;
			length--;
		}
	}

	T& top() {
		return iter_top->data;
	}

	Node* top_iter() {
		return iter_top;
	}

	bool empty() {
		if (iter_top)
			return false;
		else
			return true;
	}

private:
	int length = 0;				// 栈的长度
	Node *iter_top = nullptr;	// 栈顶指针
};
