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

	unique(const unique& u) = delete;
		
	unique(unique&& u) noexcept: data{u.data} {
		u.data = nullptr;
	}

	template<typename U, enable_if_t<is_base_of<T, U>::value, int> = 0>
	unique(unique<U>&& u) noexcept {
                data = u.release();
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

	template<typename U, enable_if_t<is_base_of<T, U>::value, int> = 0>
	unique& operator=(unique<U>&& u) noexcept {
                if(this != &u) {
                        delete data;
                        data = u.release();
                }
                return *this;
        }

	T* get() noexcept {
		return data;
	}

	T* release() {
                T* old_data = data;
                data = nullptr;
                return old_data;
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
	virtual ~A() noexcept { cout << "~A()" << endl; }
	virtual int f() {
		return 1;
	}
	int x;
	
};

class B: public A
{
public:
	B() { cout << "B()" << endl; }
	B(int x): A(x) { cout << "B(int)" << endl; }
	~B() { cout << "~B()" << endl; }
	int f() override {
		return 2;
	}
};

int main() {
	unique<A> u = make_unique_ptr<B>(0);
	cout << u-> x << endl << u->f() << endl;
	return 0;
}
