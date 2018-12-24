#include "CharString.h"
#include <Windows.h>

CharString::CharString(int _size)
{
	if (_size < DEFAULT_SIZE)
	{
		size = DEFAULT_SIZE;
	}
	else
	{
		size = _size * 1.1;
	}
	data = static_cast<wchar_t*> (malloc(sizeof(wchar_t) * size));
}

CharString::CharString(const wchar_t* src)
{
	_length = 0;
	while (*src != 0)
	{
		_length++;
		src++;
	}




	if (_length < DEFAULT_SIZE)
	{
		size = DEFAULT_SIZE;
	}
	else
	{
		size = _length * 1.1;
	}

	data = static_cast<wchar_t*> (malloc(sizeof(wchar_t) * size));
	memcpy(data, src-_length, sizeof(wchar_t)*_length);
}

CharString::CharString(const std::string & src)
{
	const char* _source = src.c_str();
	_length = src.size() + 1;
	if (_length < DEFAULT_SIZE)
	{
		size = DEFAULT_SIZE;
	}
	else
	{
		size = _length * 1.1;
	}
	data = static_cast<wchar_t*> (malloc(sizeof(wchar_t)*size));
	size_t i;
	wmemset(data, 0x0, _length);
	mbstowcs_s(&i, data, _length, _source, _length);
	_length--;
}

CharString::CharString(const std::wstring & src)
{
	_length = src.length();
	if (_length < DEFAULT_SIZE)
	{
		size = DEFAULT_SIZE;
	}
	else
	{
		size = _length * 1.1;
	}
	data = static_cast<wchar_t*> (malloc(sizeof(wchar_t)*size));
	src.copy(data, _length);
}

CharString::CharString(std::wifstream& in)
{
	in.seekg(0, std::ios::end);
	_length = in.tellg();
	in.seekg(0, std::ios::beg);
	if (_length < DEFAULT_SIZE)
	{
		size = DEFAULT_SIZE;
	}
	else
	{
		size = _length * 1.1;
	}
	data = static_cast<wchar_t*> (malloc(sizeof(wchar_t) * size));
	in.read(data, _length);
}

CharString::CharString(const CharString & src)
{
	size = src.size;
	_length = src._length;
	data = static_cast<wchar_t*> (malloc(sizeof(wchar_t)*size));
	memcpy(data, src.data, sizeof(wchar_t)*_length);
}

CharString::CharString(CharString && src)
{
	size = src.size;
	_length = src._length;
	data = src.data;
	src.size = src._length = 0;
	src.data = nullptr;
}

const int CharString::length() const
{
	return _length;
}

void CharString::append(wchar_t p)
{
	if (_length >= size)
	{
		size = (size + 1)*1.1;
		data = static_cast<wchar_t*>(realloc(data, sizeof(wchar_t)*size));
	}
	data[_length] = p;
	_length++;
}

void CharString::append(const CharString & src)
{
	int len = _length + src._length;
	if (len >= size)
	{
		size = len * 1.1;
		data = static_cast<wchar_t*>(realloc(data, sizeof(wchar_t)*size));
	}
	memcpy(data + _length, src.data, sizeof(wchar_t)*src._length);
	_length = len;
}

void CharString::pop()
{
	_length--;
}

void CharString::clear()
{
	_length = 0;
}

LinkedList<CharString> CharString::split(wchar_t t)
{
	LinkedList<CharString> res;
	int p = 0, q = 0;
	for (; p < _length; p++)
	{
		if (data[p] == t)
		{
			res.add(substring(q, p - 1));
			q = p + 1;
		}
	}

	if(p - 1 > q)
		res.add(substring(q, p - 1));

	return res;
}

void CharString::tolower()
{
	for (int i = 0; i < _length; i++)
	{
		if (data[i] >= L'A' && data[i] <= L'Z')
		{
			data[i] += (L'a' - L'A');
		}
	}
}

void CharString::trimSpace(int c)
{
	//去除首尾的空格、换行符
	int m, n;
	m = 0;
	n = _length - 1;
	while (m < _length)
	{
		if (data[m] != L' ' && data[m] != L'　' && data[m] != L'\n')
			break;
		m++;
	}
	while (n >= 0)
	{
		if (data[n] != L' ' && data[n] != L'　' && data[n] != L'\n')
			break;
		n--;
	}
	_length = n - m + 1;

	//去除中间多余的空格 换行符
	int i = 0;
	int j = 0;
	while (i < _length)
	{
		if (!(data[m + i] == ' ' || data[m + i] == '\n') || !(data[m + i + 1] == ' ' || data[m + i + 1] == '\n'))
		{
			data[j] = data[m + i];
			j++;
		}
		i++;
	}
	_length = j;

	if (c == 1)
	{
		// 去除中间的所有换行符和空格
		int i = 0;
		int j = 0;
		while (i < _length)
		{
			if (!(data[m + i] == L'\n'))
			{
				data[j] = data[m + i];
				j++;
			}
			i++;
		}
		_length = j;
	}
}

void CharString::initNext()
{
	next = new int[_length];
	int i = 0;
	int j = -1;
	next[i] = j;
	while (i < _length - 1)
	{
		if (j == -1 || data[i] == data[j])
		{
			i++;
			j++;
			next[i] = j;
		}
		else
		{
			j = next[j];
		}
	}
}

CharString CharString::substring(int begin, int end) const
{
	int len = end - begin + 1;
	CharString result(len);
	memcpy(result.data, data + begin, sizeof(wchar_t)*len);
	result._length = len;
	return result;
}

int CharString::indexOf(wchar_t c, int begin) const
{
	int i = begin;
	while (i < _length)
	{
		if (data[i] == c)
		{
			return i;
		}
		i++;
	}
	return i;
}

int CharString::indexOf(CharString & substring, int begin) const
{
	if (!substring.next)
		substring.initNext();

	int i = begin;
	int j = 0;
	while (i < _length && j < substring._length)
	{
		if (j == -1 || data[i] == substring.data[j])
		{
			i++;
			j++;
		}
		else
		{
			j = substring.next[j];
		}
	}

	if (j == substring._length)
	{
		return i - j;
	}
	else
	{
		return -1;
	}
}

int CharString::indexOf(const CharString& substring, int begin) const
{
	int *_next = new int[substring._length];
	int i, j;
	i = 0;
	j = -1;
	_next[i] = j;
	while (i < substring._length - 1)
	{
		if (j == -1 || data[i] == substring.data[j])
		{
			i++;
			j++;
			_next[i] = j;
		}
		else
		{
			j = _next[j];
		}
	}

	i = begin;
	j = 0;
	while (i < _length && j < substring._length)
	{
		if (j == -1 || data[i] == substring.data[j])
		{
			i++;
			j++;
		}
		else
		{
			j = _next[j];
		}
	}

	delete[] _next;

	if (j == substring._length)
	{
		return i - j;
	}
	else
	{
		return -1;
	}
}

CharString::operator std::wstring() const
{
	return std::wstring(data, _length);
}

wchar_t & CharString::operator[](int index) const
{
	return data[index];
}

void CharString::operator=(const CharString & src)
{
	if (src._length > size)
	{
		size = src._length*1.1;
		data = static_cast<wchar_t*> (realloc(data, sizeof(wchar_t) * size));
	}
	_length = src._length;
	memcpy(data, src.data, sizeof(wchar_t) * _length);
}

void CharString::operator=(CharString && src)
{
	size = src.size;
	_length = src._length;
	if (data)
	{
		free(data);
	}
	data = src.data;
	src.size = src._length = 0;
	src.data = nullptr;
}

bool CharString::operator<(const CharString & rhs) const
{
	int i = 0;
	while (true)
	{
		if (i >= rhs._length)
			return false;
		if (i >= _length)
			return true;

		if (data[i] < rhs.data[i])
			return true;
		else if (data[i] > rhs.data[i])
			return false;

		i++;
	}
}

bool CharString::operator>(const CharString & rhs) const
{
	int i = 0;
	while (true)
	{
		if (i >= _length)
			return false;
		if (i >= rhs._length)
			return true;

		if (data[i] < rhs.data[i])
			return false;
		else if (data[i] > rhs.data[i])
			return true;

		i++;
	}
}


CharString::~CharString()
{
	if (data)
	{
		free(data);
	}
	if (next)
	{
		delete[] next;
	}
}

CharString operator+(const CharString & a, const CharString & b)
{
	int len = a._length + b._length;
	CharString result(len);
	result._length = len;
	memcpy(result.data, a.data, sizeof(wchar_t)*a._length);
	memcpy(result.data + a._length, b.data, sizeof(wchar_t)*b._length);
	return result;
}

bool operator==(const CharString & a, const CharString & b)
{
	if (a.length() == b.length())
	{
		for (int i = 0; i < a.length(); i++)
		{
			if (a[i] != b[i])
				return false;
		}
		return true;
	}
	else
	{
		return false;
	}
}

std::wistream & operator>>(std::wistream & in, CharString & dest)
{
	std::wstring str;
	in >> str;
	dest = str;
	return in;
}

std::wostream & operator<<(std::wostream & out, const CharString & src)
{
	out << std::wstring(src.data, src._length);
	return out;
}
