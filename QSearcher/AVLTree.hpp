#pragma once
#include <iostream>

#define MAX(a, b) (a)>(b)?(a):(b)

template<typename Key, typename Value>
class AVLTree
{
public:
	//ƽ��������ڵ���
	struct Node {
		Node(const Key& _key, const Value& _value, Node* _parent = nullptr) :key(_key), value(_value), parent(_parent) {}
		Node(const Key& _key, Node* _parent = nullptr) :key(_key), parent(_parent) {}

		int height() {
			if (this)
				return _height;
			else
				return 0;
		}

		Key key;
		Value value;
		int _height = 1;
		Node* lchild = nullptr;
		Node* rchild = nullptr;
		Node* parent = nullptr;
	};

	//�������������
	AVLTree() {}
	AVLTree(AVLTree&& src) {
		root = src.root;
		_size = src._size;
		src.root = nullptr;
	}
	~AVLTree(){
		Destroy(root);
	}
	void operator=(AVLTree&& src) {
		Destroy(root);
		root = src.root;
		_size = src._size;
		src.root = nullptr;
	}

	//��ӡ����ҡ��Ƴ����༭�Ȳ���
	void Insert(const Key& key, const Value& value = Value()) {
		root = insertNode(root, key, value);
		root->parent = nullptr;
	}

	Value& Search(const Key& key) const {
		Node* p = searchNode(root, key);
		if (p)
			return p->value;
		else
			return *null;
	}

	bool Count(const Key& key) const {
		return searchNode(root, key);
	}

	void Remove(const Key& key) {
		root = deleteNode(root, key);
	}

	Value& operator[](const Key& key) {
		Node* p = searchNode(root, key);
		if (p)
		{
			return p->value;
		}
		else
		{
			Insert(key);
			p = searchNode(root, key);
			return p->value;
		}
	}

	int size() const {
		return _size;
	}

	static Value* null;		//����Ϊ��
	Node* root = nullptr;	//���ڵ�ָ��

private:
	int _size = 0;
	enum Type { LL, RR, LR, RL };

	//����ƽ�����
	Node* Adjust(Node* p, Type t) 
	{
		switch (t)
		{
		case LL:
			return LLRotate(p);
		case RR:
			return RRRotate(p);
		case LR:
			return LRRotate(p);
		case RL:
			return RLRotate(p);
		}
	}

	//�ݹ�ʵ�ֶ�ƽ��������Ĳ���
	Node* insertNode(Node* p, const Key& key, const Value& value)
	{
		if (p == nullptr)
		{
			p = new Node(key, value);
			++_size;
		}
		else if (key < p->key)
		{
			p->lchild = insertNode(p->lchild, key, value);
			p->lchild->parent = p;
			if (p->lchild->height() - p->rchild->height() == 2)
			{
				if (key < p->lchild->key)
					p = Adjust(p, LL);
				else
					p = Adjust(p, LR);
			}
		}
		else if (key > p->key)
		{
			p->rchild = insertNode(p->rchild, key, value);
			p->rchild->parent = p;
			if (p->rchild->height() - p->lchild->height() == 2)
			{
				if (key > p->rchild->key)
					p = Adjust(p, RR);
				else 
					p = Adjust(p, RL);
			}
		}
		p->_height = MAX(p->lchild->height(), p->rchild->height()) + 1;
		return p;
	}

	Node* searchNode(Node* p, const Key& key) const
	{
		if (p == nullptr)
			return nullptr;
		while (p)
		{
			if (p->key > key)
			{
				p = p->lchild;
			}
			else if (p->key < key)
			{
				p = p->rchild;
			}
			else
			{
				break;
			}
		}
		return p;
	}

	Node* deleteNode(Node* p, const Key& key)
	{
		if (p == nullptr)
			return nullptr;

		if (key < p->key)
		{
			p->lchild = deleteNode(p->lchild, key);
			if (p->rchild->height() - p->lchild->height() == 2)
			{
				Node* rchild = p->rchild;
				if (rchild->lchild->height() > rchild->rchild->height())
					p = Adjust(p, RL);
				else
					p = Adjust(p, RR);
			}
		}
		else if (key > p->key)
		{
			p->rchild = deleteNode(p->rchild, key);
			if (p->lchild->height() - p->rchild->height() == 2)
			{
				Node* lchild = p->lchild;
				if (lchild->lchild->height() > lchild->rchild->height())
					p = Adjust(p, LL);
				else
					p = Adjust(p, LR);
			}
		}
		else
		{
			--_size;
			if (p->lchild && p->rchild)
			{
				if (p->lchild->height() > p->rchild->height())
				{
					Node* maxNode = p->lchild;
					while (maxNode->rchild)
					{
						maxNode = maxNode->rchild;
					}

					if (p->parent)
					{
						if (p == p->parent->lchild)
						{
							p->parent->lchild = maxNode;
						}
						else
						{
							p->parent->rchild = maxNode;
						}
					}

					if (maxNode == maxNode->parent->lchild)
					{
						maxNode->parent->lchild = nullptr;
					}
					else
					{
						maxNode->parent->rchild = nullptr;
					}
					maxNode->parent = p->parent;
					maxNode->lchild = p->lchild;
					maxNode->rchild = p->rchild;
					delete p;
					p = maxNode;
					p->lchild = deleteNode(p->lchild, maxNode->key);	//�������������ڵ�ĸ߶�
				}
				else
				{
					Node* minNode = p->rchild;
					while (minNode->lchild)
					{
						minNode = minNode->lchild;
					}

					// ��miniNode��ԭ���ڵ�Ͽ�
					if (minNode == minNode->parent->lchild)
					{
						minNode->parent->lchild = nullptr;
					}
					else
					{
						minNode->parent->rchild = nullptr;
					}

					minNode->parent = p->parent;
					minNode->lchild = p->lchild;
					minNode->rchild = p->rchild;
					delete p;
					p = minNode;
					p->rchild = deleteNode(p->rchild, minNode->key);	//�������������ڵ�ĸ߶�
				}
			}
			else if(p->lchild || p->rchild)
			{
				Node* temp = p;
				if (p->lchild)
				{
					p = p->lchild;
				}
				else
				{
					p = p->rchild;
				}
				p->parent = temp->parent;
				delete temp;
			}
			else
			{
				delete p;
				return nullptr;
			}
		}
		p->_height = MAX(p->lchild->height(), p->rchild->height()) + 1;
		return p;
	}

	Node* LLRotate(Node* p)
	{
		Node* lchild = p->lchild;
		p->lchild = lchild->rchild;
		if(lchild->rchild)
			lchild->rchild->parent = p;

		lchild->rchild = p;
		p->parent = lchild;

		lchild->_height = MAX(lchild->lchild->height(), p->height()) + 1;
		p->_height = MAX(p->lchild->height(), p->rchild->height()) + 1;

		return lchild;
	}

	Node* RRRotate(Node* p)
	{
		Node* rchild = p->rchild;
		p->rchild = rchild->lchild;
		if(rchild->lchild)
			rchild->lchild->parent = p;

		rchild->lchild = p;
		p->parent = rchild;

		rchild->_height = MAX(p->height(), rchild->rchild->height()) + 1;
		p->_height = MAX(p->lchild->height(), p->rchild->height()) + 1;
		
		return rchild;
	}

	Node* LRRotate(Node* p)
	{
		p->lchild = RRRotate(p->lchild);	//�ȶ�����������
		return LLRotate(p);					//�ٶԸ��������
	}

	Node* RLRotate(Node* p)
	{
		p->rchild = LLRotate(p->rchild);	//�ȶ�����������
		return RRRotate(p);					//�ٶԸ��ڵ�����
	}

	void Destroy(Node* p)
	{
		if (p == nullptr)
			return;

		Destroy(p->lchild);
		Destroy(p->rchild);
		delete p;
	}
};

template<typename Key, typename Value>
Value* AVLTree<Key, Value>::null = new Value;