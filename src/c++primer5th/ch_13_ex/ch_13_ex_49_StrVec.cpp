#include "ch_13_ex_49_StrVec.h"

#include <algorithm> // for_each

void StrVec::push_back(const std::string& s)
{
	chk_n_alloc();
	alloc.construct(first_free++, s);
}

std::pair<std::string*, std::string*> StrVec::alloc_n_copy(const std::string* b,
		const std::string* e)
{
	auto data = alloc.allocate(e - b);
	return {data, std::uninitialized_copy(b, e, data)};
}

void StrVec::free()
{
	if(elements) {
		for_each(elements, first_free, [this](std::string& rhs)
				{
					alloc.destroy(&rhs);
				});
		alloc.deallocate(elements, cap - elements);
	}
}

void StrVec::range_initialize(const std::string* first, const std::string* last)
{
	auto new_data = alloc_n_copy(first, last);
	elements = new_data.first;
	first_free = cap = new_data.second;
}

StrVec::StrVec(const StrVec& rhs)
{
	range_initialize(rhs.begin(), rhs.end());
}

//Constructors taking only one argument of this type are a special kind of
//constructor, called initializer-list constructor.
StrVec::StrVec(std::initializer_list<std::string> il)
{
	range_initialize(il.begin(), il.end());
}

StrVec::~StrVec()
{
	free();
}

StrVec& StrVec::operator=(const StrVec& rhs)
{
	auto data = alloc_n_copy(rhs.begin(), rhs.end());
	free();
	elements = data.first;
	first_free = cap = data.second;
	return *this;
}

//specific 13.6
void StrVec::alloc_n_move(size_t new_cap)
{
	auto new_data = alloc.allocate(new_cap);
	auto dest = new_data;
	auto elem = elements;
	for(size_t i = 0; i != size(); ++i)
		alloc.construct(dest++, std::move(*elem++));
	free();
	elements = new_data;
	first_free = dest;
	cap = elements + new_cap;
}

void StrVec::reallocate()
{
	auto new_capacity = size() ? 2*size() : 1;
	alloc_n_move(new_capacity);
}

void StrVec::reserve(size_t new_cap)
{
	if(new_cap <= capacity())
		return;
	alloc_n_move(new_cap);
}

void StrVec::resize(size_t count, const std::string& s)
{
	if(count > size()) {
		if(count > capacity())
			reserve(count * 2);

		for(size_t i = size(); i != count; ++i)
			alloc.construct(first_free++, s);
	}else if(count < size()) {
		while(first_free != elements + count)
			alloc.destroy(--first_free);
	}
}

StrVec::StrVec(StrVec&& s) NOEXCEPT : elements(s.elements),
	first_free(s.first_free), cap(s.cap)
{
	//leave s in a state in which it is safe to run the destructor
	s.elements = s.first_free = s.cap = nullptr;
}

StrVec& StrVec::operator=(StrVec&& rhs) NOEXCEPT
{
	if(this != &rhs) {
		free();
		elements = rhs.elements;
		first_free = rhs.first_free;
		cap = rhs.cap;
		rhs.elements = rhs.first_free = rhs.cap = nullptr;
	}
	return *this;
}
