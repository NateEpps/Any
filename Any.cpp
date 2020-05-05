//
// Any.cpp
// Nathanael Epps
//

#include "Any.hpp"

#include <utility>

Any::Any() : ptr(nullptr)
{
    
}

Any::Any(const Any& that) : ptr(that.ptr->getCopy())
{
    
}

Any::Any(Any&& that) : Any()
{
    using std::swap;
    
    swap(*this, that);
}

/*private*/ Any::Any(std::unique_ptr<Base> p) : ptr(std::move(p))
{
    
}

Any& Any::operator=(Any that)
{
    swap(*this, that);
    
    return *this;
}

Any::operator void*()
{
    return ptr->getObject();
}

Any::operator const void*() const
{
    return ptr->getObject();
}

#include <iostream>
#include <vector>

void Any::test()
{
    using namespace std;
    
    Any obj = MakeAny(5);
    Any str = MakeAny<string>("Sup dude");
    const Any cstr = MakeAny<const char*>("This is a C string");
    
    cout << AnyCast<int>(obj) << "\n";
    cout << AnyCast<string>(str) << "\n";
    cout << AnyCast<const char*>(cstr) << "\n";
    cout << AnyCast<const char*>(cstr) << "\n";
    
    Any a2 = MakeAny<std::vector<int>>();
    
    for (int x = 1; x <= 10; x++)
        AnyCast<std::vector<int>>(a2).push_back(x * x);
    
    for (int i : AnyCast<std::vector<int>>(a2))
        std::cout << i << " ";
    std::cout << "\n";
}

void swap(Any& left, Any& right)
{
    using std::swap;
    
    swap(left.ptr, right.ptr);
}
