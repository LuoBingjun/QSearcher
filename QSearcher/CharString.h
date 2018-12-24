#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <memory>
#include "LinkedList.hpp"

class CharString
{
public:
	// 构造和析构函数
	CharString(int _size = DEFAULT_SIZE);
	CharString(const wchar_t* src);
	CharString(const std::string& src);
	CharString(const std::wstring& src);
	CharString(std::wifstream& in);
	CharString(const CharString& src);
	CharString(CharString&& src);
	~CharString();

	// 添加、删除、清空、截取、查找等函数接口
	const int length() const;
	void append(wchar_t p);
	void append(const CharString& src);
	void pop();
	void clear();
	LinkedList<CharString> split(wchar_t t);
	void tolower();
	void trimSpace(int c = 0); // 1表示去除所有换行符
	void initNext();
	CharString substring(int begin, int end) const;
	int indexOf(wchar_t c, int begin = 0) const;
	int indexOf(CharString& substring, int begin = 0) const;
	int indexOf(const CharString& substring, int begin = 0) const;

	// 重载转换、比较、赋值等运算符
	operator std::wstring() const;
	wchar_t& operator[](int index) const;
	void operator=(const CharString& src);
	void operator=(CharString&& src);
	bool operator<(const CharString& rhs) const;
	bool operator>(const CharString& rhs) const;

	friend CharString operator+(const CharString& a, const CharString& b);
	friend std::wostream& operator<<(std::wostream& out, const CharString& src);

private:
	wchar_t* data = nullptr;	// 指向底层wchar_t数组的指针
	int size;
	int _length = 0;			// 字符串长度
	int *next = nullptr;

	const static int DEFAULT_SIZE = 100;
};

CharString operator+(const CharString& a, const CharString& b);
bool operator==(const CharString& a, const CharString& b);
std::wistream& operator>>(std::wistream& in, CharString& dest);
std::wostream& operator<<(std::wostream& out, const CharString& src);