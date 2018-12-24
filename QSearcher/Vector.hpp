#pragma once
#include <memory>

const int DEFAULT_SIZE = 1000;

template<typename T>
class Vector
{
public:
	Vector(int _size = DEFAULT_SIZE) :size(_size) {
		data = new T[size];
	}

	Vector(const Vector& src) {
		size = src.size;
		_length = src._length;
		data = new T[size];
		for (int i = 0; i < _length; ++i)
		{
			data[i] = src[i];
		}
	}

	Vector(Vector&& src) {
		size = src.size;
		_length = src._length;
		data = src.data;
		src.data = nullptr;
	}


	~Vector() {
		if (data)
			delete[] data;
	}

	void push_back(T&& p) {
		if (_length >= size)
		{
			size = (size + 1)*1.1;
			T* old = data;
			data = new T[size];
			for (int i = 0; i < _length; ++i)
			{
				data[i] = std::move(old[i]);
			}
			delete[] old;
		}
		data[_length] = p;
		++_length;
	}

	void push_back(const T& p) {
		if (_length >= size)
		{
			size = (size + 1)*1.1;
			T* old = data;
			data = new T[size];
			for (int i = 0; i < _length; ++i)
			{
				data[i] = std::move(old[i]);
			}
			delete[] old;
		}
		data[_length] = p;
		++_length;
	}

	bool erase(const T& p) {
		for (int i = 0; i < _length; ++i)
		{
			if (data[i] == p)
			{
				--_length;
				for (; i < _length; i++)
				{
					data[i] = data[i + 1];
				}
				return true;
			}
		}
		return false;
	}

	T& operator[](int i) {
		if (i >= size)
		{
			size = i + 1;
			size = (size + 1)*1.1;
			T* old = data;
			data = new T[size];
			for (int i = 0; i < _length; ++i)
			{
				data[i] = std::move(old[i]);
			}
			delete[] old;
		}
		if (i >= _length)
		{
			_length = i + 1;
		}
		return data[i];
	}

	const T& operator[](int i) const {
		return data[i];
	}

	int length() const{
		return _length;
	}

private:
	T * data = nullptr;
	int size = 0;
	int _length = 0;
};
