#include <iostream>
#include <string>
#include <utility>
#include <type_traits>

using namespace std;

template<typename T>
class unique 
{
	T* data;
public:
	constexpr unique() noexcept: data{nullptr} {

	}

	unique(T* data) noexcept: data{data} {

	}

	template<typename U, enable_if_t<is_same<U, T>::value, int> = 0>
	unique(U* data) noexcept: data{data} {

	}

	unique(const unique& u) = delete;
		
	unique(unique&& u) noexcept: data{u.data} {
		u.data = nullptr;
	}
	
	unique& operator=(const unique& u) = delete;
	
	unique& operator=(unique&& u) noexcept {
		if(this != &u) {
			delete data;
			data = u.data;
			u.data = nullptr;
		}
		return *this;
	}

	T* get() noexcept {
		return data;
	}

	T* operator->() noexcept {
		return data;
	}

	T& operator*() {
		return *data;
	}

	~unique() noexcept {
		if(data) {
			delete data;
		}
	}
};

template<typename T, typename... Args>
unique<T> make_unique_ptr(Args&&... args) {
	return unique<T>(new T(forward<Args>(args)...));
}

class A
{
public:
	A() noexcept { cout << "A()" << endl; }
	A(int x) noexcept: x{x} { cout << "A(int)" << endl; }  
	~A() noexcept { cout << "~A()" << endl; }
	int x;
	
};

int main() {
	auto u = make_unique_ptr<A>(1);
	auto t = make_unique_ptr<A>(2);
	u = move(t);
	cout << u->x << endl;
	return 0;
}
