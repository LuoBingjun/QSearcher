#pragma once
#include <memory>
#include "CharString.h"
#include "LinkedList.hpp"

template<typename Key, typename Value>
class HashMap
{
	struct Data
	{
		Data(const Key& _key, const Value& _value): key(_key), value(_value) {}
		Data(const Key& _key) : key(_key) {}

		bool operator==(const Data& data) {
			if (key == data.key)
				return true;
			else
				return false;
		}

		Key key;
		Value value;
	};

public:
	HashMap(int _size = DEFAULT_SIZE):size(_size){
		data = new LinkedList<Data>*[size];
		memset(data, 0, sizeof(LinkedList<Data>*)*size);
	}

	~HashMap() {
		delete[] data;
	}

	void Insert(const Key& key, const Value& value) {
		int hash = getHash(key);

		if (data[hash] == nullptr)
		{
			data[hash] = new LinkedList<Data>;
		}
		else
		{
			repeat++;
		}

		data[hash]->add(Data(key, value));
	}

	bool Count(const Key& key) {
		unsigned int hash = getHash(key);
		if (data[hash] == nullptr)
			return false;

		if (data[hash]->search(Data(key)))
			return true;
		else
			return false;
	}

	Value& Search(const Key& key) {
		unsigned int hash = getHash(key);
		if (data[hash] == nullptr)
			return *null;
		
		auto p = data[hash]->search(Data(key));
		if (p)
			return p->data.value;
		else
			return *null;
	}

	Value& operator[](const Key& key) {
		unsigned int hash = getHash(key);
		LinkedList<Data>::Node* p = nullptr;
		if (data[hash] == nullptr)
		{
			data[hash] = new LinkedList<Data>;
			p = data[hash]->add(Data(key));
		}
		else
		{
			p = data[hash]->search(Data(key));
			if (p == nullptr)
			{
				p = data[hash]->add(Data(key));
			}
		}
		return p->data.value;
	}

	static Value* null;

private:
	const static int DEFAULT_SIZE = 200000;
	int size = 0;
	int repeat = 0;
	LinkedList<Data>** data = nullptr;

	unsigned int getHash(const Key& str) {
		int len = str.length();
		unsigned int hash = 0;
		for (int i = 0; i < len; i++)
			hash = (hash << 5) + hash + (unsigned int)str[i];

		return hash % size;
	}
};

template<typename Key, typename Value>
Value* HashMap<Key, Value>::null = new Value;