//
// Any.hpp
// Nathanael Epps
//

#ifndef ANY
#define ANY

#include <memory>
#include <typeinfo>
#include <stdexcept>
#include <string>
#include <utility>
#include <functional>

class Any
{
public:
    Any();
    Any(const Any&);
    Any(Any&&);
    
    Any& operator=(Any);
    
    // pointers only valid as long as object is around
    explicit operator void*();
    explicit operator const void*() const;
    
    static void test();
    
private:
    class Base
    {
    public:
        Base() = default;
        virtual ~Base() = default;
        
        virtual void* getObject() const = 0;
        virtual const std::type_info& getTypeInfo() const = 0;
        virtual std::unique_ptr<Base> getCopy() const = 0;
    };
    
    template <class T>
    class Impl : public Base
    {
    public:
        Impl() = default;
        Impl(T&& t) : obj(t) { }
        virtual ~Impl() = default;
        
        virtual inline void* getObject() const override
        { return static_cast<void*>(&obj); }
        virtual inline const std::type_info& getTypeInfo() const override
        { return typeid(T); }
        virtual std::unique_ptr<Base> getCopy() const override
        { return std::make_unique<Impl<T>>(std::forward<T>(obj)); }
        //{ return std::unique_ptr<Base>(new Impl<T>(std::forward<T>(obj))); }
        
    private:
        mutable T obj;
    };
    
    template <class T>
    friend Any MakeAny(T);
    template <class T>
    friend Any MakeAny();
    
    template <class T>
    friend T& AnyCast(Any&);
    template <class T>
    friend const T& AnyCast(const Any&);
    
    friend void swap(Any&, Any&);
    
    Any(std::unique_ptr<Base>);
    
    std::unique_ptr<Base> ptr;
};

typedef std::function<std::string(Any)> AnyConvFunc;

void swap(Any&, Any&);

#define BadAnyCast(type1, type2) std::runtime_error(std::string("Bad AnyCast: real type \"") + type1 + "\" != asked for type \"" + type2 + "\"")

template <class T>
Any MakeAny(T t)
{
    return Any(std::make_unique<Any::Impl<T>>(std::forward<T>(t)));
}

template <class T>
Any MakeAny()
{
    return Any(std::make_unique<Any::Impl<T>>());
}

template <class T>
T& AnyCast(Any& a)
{
    if (a.ptr->getTypeInfo() != typeid(T))
        throw BadAnyCast(a.ptr->getTypeInfo().name(), typeid(T).name());
    
    return *static_cast<T*>(static_cast<void*>(a));
}

template <class T>
const T& AnyCast(const Any& a)
{
    if (a.ptr->getTypeInfo() != typeid(T))
        throw BadAnyCast(a.ptr->getTypeInfo().name(), typeid(T).name());
    
    return *static_cast<const T*>(static_cast<const void*>(a));
}

#endif
