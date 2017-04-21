#ifndef CH_13_EX_25_H
#define CH_13_EX_25_H

#include <vector>
#include <string>
#include <memory>

class StrBlob
{
public:
	typedef std::vector<std::string>::size_type size_type;	
	StrBlob(); //constructor
	StrBlob(std::initializer_list<std::string> il);
	size_type size() const
	{
		return data->size(); 
	}

	bool empty() const
	{
		return data->empty();
	}

	//添加和删除元素
	void push_back(const std::string& t)
	{
		data->push_back(t);
	}

	void pop_back();

	//元素访问
	std::string& front();
	std::string& back();

private:
	std::shared_ptr<std::vector<std::string>> data;
	//如果data[i]不合法，抛出一个异常
	void check(size_type i, const std::string& msg) const;
};

#endif //CH_13_EX_25_H

