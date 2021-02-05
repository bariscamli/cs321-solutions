#include <iostream>
#include <string>
#include <vector>
#include <tuple>


// + Type aliases and traits for easy usage of std classes are declared for you as shown below.
using String = std::string;
template<typename ... Ts> using Vector = std::vector<Ts...>;
template<typename ... Ts> using Tuple = std::tuple<Ts...>;

template<typename T>struct      is_tuple { static const bool value = false; };
template<typename ... Ts>struct is_tuple<std::tuple<Ts...>> { static const bool value = true; };


// custom user literal _s is declared for easy std::string constant usage such as "hello"_s
auto operator"" _s(const char* str, size_t) { return std::string(str); }

// Distinct new classes are derived from already available base classes for you
struct Integer
{
    int value;
    Integer(int value) : value(value) { }
};

// A simple struct and its container is declared in a generic way that has 4 dimensions
struct Name : String { using String::String; Name(const String& s) : String(s) {};};
struct Surname : String { using String::String; Surname(const String& s) : String(s) {};};
struct BirthYear : Integer { using Integer::Integer; };
struct DeathYear : Integer { using Integer::Integer;};
using Scientist = Tuple<Name, Surname, BirthYear, DeathYear>;
using Scientists = Vector<Scientist>;

// A simple struct and its container is declared in a generic way that has 2 dimensions
struct FullName : String { using String::String; FullName(const String& s) : String(s) {}; };
struct Url : String { using String::String; Url(const String& s) : String(s) {}; };
using Link = Tuple<FullName, Url>;
using Links = Vector<Link>;


// Declare necessary concepts, operator overloads, algorithms, their higher order versions, etc.
// for to make below code execute and produce the wanted output exactly as described
// WRITE YOUR CODE HERE, DO NOT CHANGE ANY OTHER PLACE


// Below are written for you and shall not be changed.

template<typename T> struct remove_const          { using type = T; };
template<typename T> struct remove_const<const T> { using type = T; };

template<typename T>
concept IsContainer = requires(T t)
{
    t.begin();
    t.end();
    t.size();
};

template<typename T>
concept IsTuple = is_tuple<T>::value;


std::ostream& operator<<(std::ostream& s, const BirthYear& b){
    std::cout << b.value;
    return s;
}

std::ostream& operator<<(std::ostream& s, const DeathYear& d){
    std::cout << d.value;
    return s;
}

template <size_t index = 0, typename... Ts>
constexpr void printTuple(Tuple<Ts...> tup)
{
    if constexpr(index == sizeof...(Ts))
        {
            std::cout<< std::endl;
            return;
        }
    else {
        std::cout<< "(" << std::get<index>(tup) << ") ";
        printTuple<index + 1>(tup);
    }
}


template<typename T>
void print(const T& container)
{
    if constexpr(IsContainer<T>)
    {
        for(const auto& item : container){
            print(item);
        }
        std::cout << std::endl;
    }
    else if constexpr(IsTuple<T>)
    {
        printTuple(container);
    }
    else
    {
        std::cout << container;
    }
}

auto printer(auto head)
{
    return [=](const auto& value) {
        std::cout << std::endl;
        print(head);
        std::cout << std::endl;
        print(value);
        return value; };
}
auto printer()
{
    return [=](const auto& value) {
        print(value);
        return value; };
}


template<typename T>
auto operator|(const T& anything,const auto& lambda) {
    if constexpr(IsContainer<T>){
        return lambda(anything);
    }
    else if constexpr(IsTuple<T>){
        return lambda(anything);
    }
    else{
        return [=](const auto& value) {
          return lambda(anything(value));
        };
    }
}


auto eval_namesurname(){
    return [](auto t){
        return String(std::get<Name>(t) +" "+std::get<Surname>(t));
    };
}


auto transform(auto func)
{
    return [=]<IsContainer Container>(Container& container) {
        using newType = decltype(func(*container.begin()));
        auto c = std::vector<newType>();
        for(const auto& item : container){
            c.push_back(func(item));
        }
        return c;
    };
}

template<typename T>
auto get(){
    return []<IsTuple Container>(const Container& container){
        return std::get<T>(container);
    };
}

template<typename T1,typename T2>
constexpr auto lengthTuple(const T1&,const T2&) {
    return std::tuple_size_v<T1>+std::tuple_size_v<T2>;
}

template<typename T1,typename T2>
auto cartesian(T1 left,T2 right){
    auto b = std::tuple_cat(*left.begin(),*right.begin());
    using newKey = decltype(b);
    auto result = std::vector<newKey>();

    for(auto i = left.begin(); i != left.end(); ++i){
        for(auto j = right.begin(); j != right.end(); ++j){
            result.push_back(std::tuple_cat(*i,*j));
        }
    }
    return result;
}


auto where(const auto& lambda)
{
    return [=]<IsContainer Container>(const Container& container) {
        auto c = Container();
        for(const auto& item : container)
            if(lambda(item))
                c.push_back(item);
        return c;
    };
}

template<typename... Rest>
auto select(){
    return [=]<IsContainer Container>(const Container& container){
        auto c = std::vector<Tuple<Rest...>>();
        for(const auto& item : container){
            c.push_back(Tuple<Rest...>(get<Rest>(item)...));
        }
        return c;
    };
}

auto equal_to(const auto& value1){
     return [=](const auto& value2){
         return value1 == value2;
     };
}



// Below are written for you and shall not be changed.
int main(int, char* [])
{
    auto scientists = Scientists{
        {"Charles","Babbage",1791,1871},
        {"Ada", "Lovelace", 1815, 1852},
        {"Alan", "Turing", 1912, 1954},
        };

    auto links = Links{
        {"Ada Lovelace", "https://en.wikipedia.org/wiki/Ada_Lovelace"},
        {"Alan Turing", "https://en.wikipedia.org/wiki/Alan_Turing"},
        };

    "CS321 Midterm2 begins:"_s | printer();

    print("Q1 -Print 'scientists' and 'links' containers");
    scientists | printer("Scientists");
    links | printer("Links");

    *scientists.begin()
        | eval_namesurname()
        | printer("Q2 - Write a lambda that returns 'Name' and 'Surname' concatenated with a space");

    scientists
        | transform(eval_namesurname())
        | printer("Q3 - Transform tuples with a new one with 'Name Surname' concatenated");

    cartesian(scientists, links)
        | printer("Q4 - Cartesian Product of two containers")
        | where([=](const auto& tpl) { return eval_namesurname()(tpl) == get<FullName>(tpl); })
        | printer("Q5 - Filter the elements where evaluated name surname is equal to the full name")
        | select<Url, FullName, BirthYear, DeathYear>()
        | printer("Q6 - Select Url, FullName, BirthYear, DeathYear fields in that order");

    scientists
        | where(get<Name>() | equal_to("Ada"_s))
        | printer("Q7 - Filter the elements where Name field is equal to 'Ada'");

    return 0;
}






