#pragma once
template <typename T>
class Stack
{
	// ջ�ڵ���
	struct Node
	{
		Node(const T& _data, Node* _lower = nullptr) :data(_data), lower(_lower) {}
		friend class Stack;

		T data;
		Node* lower = nullptr;
	};

public:
	// ��������������
	Stack(){}
	~Stack() {
		while (iter_top)
		{
			pop();
		}
	}

	// ѹջ����ջ��ջ�����Ƿ�Ϊ�յĺ����ӿ�
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
	int length = 0;				// ջ�ĳ���
	Node *iter_top = nullptr;	// ջ��ָ��
};
