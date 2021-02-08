#include <iostream>

template<typename T, typename Container = std::deque<T>>
class stack
{
public:
	explicit stack(const Container&);
	explicit stack(Container&& = Container());
	template <class Alloc> explicit stack(const Alloc&);
	template <class Alloc> stack(const Container&, const Alloc&);
	template <class Alloc> stack(Container&&, const Alloc&);
	template <class Alloc> stack(stack&&, const Alloc&);

	bool empty() const;
	size_t size() const;
	T& top();
	T const& top() const;
	void push(T const&);
	void pop();
	void swap(stack&&);
	template<class... Args> void emplace(Args&&... args); //C+++14ÐÂÌØÐÔ
};


int main()
{
	system("pause");
}