#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <map>


enum Gender
{
    Female,
    Male
};

template<typename first, typename second,typename third,typename fourth>
struct Employee_class{
    first name;
    second gender;
    third age;
    fourth salary;
};

using Employee = Employee_class<std::string,Gender,int,int>;


template<typename Container>
void print(const Container& c){
    for(const auto& item : c)
    {
       std::cout << "[" << item << "]; ";
    }
    std::cout << std::endl;
}


void print(const std::vector<Employee>& c){
    for(const auto& item : c)
    {
       std::cout << "[" << item.name << ", " << (item.gender ? "Male" : "Female") <<", "<<item.age<<", "<< item.salary<<"TL" << "]; ";
    }
    std::cout << std::endl;
}

template<typename Container, typename FuncObj>
auto filter(Container& container, FuncObj fobj)
{
    Container v;
    for(auto& item : container)
        if(fobj(item)) v.push_back(item);
    return v ;
}

template<typename Container, typename FuncObj>
auto operator&(Container& container, FuncObj fobj)
{
    Container v;
    for(auto& item : container)
        if(fobj(item)) v.push_back(item);
    return v ;
}

template<typename FuncObj>
auto operator|(const std::vector<Employee>& c,FuncObj fobj){
    fobj(c);
}


auto PRINT = [](auto c){print(c);};

template<typename Container, typename Lambda>
auto group_by(const Container& container, Lambda lambda){
    std::map <std::string,Container> mp;
    for(auto item : container){
        for (int bin = 10; bin < 110; bin +=10) {
            if(lambda(item,bin))
                mp["<"+std::to_string(bin-10)+".."+std::to_string(bin-1)+">"].push_back(item);
        }
    }
    return mp;
}

template<typename Container>
void print(const std::map <std::string,Container>& m){
    for(auto iter=m.begin(); iter!=m.end(); ++iter){
        const auto& item = *iter;
        const auto& key = item.first;
        const auto& value = item.second;
        std::cout << key << ": ";
        print(value);
     }
}


template<typename Container, typename Lambda>
auto transform(const Container& container, Lambda lambda){
    std::map <std::string,int> mp;
    for(auto iter=container.begin(); iter!=container.end(); ++iter){
        const auto& item = *iter;
        const auto& key = item.first;
        const auto& value = item.second;
        mp[key] = lambda(value);
    }
    return mp;
}

void print(const std::map <std::string,int>& m){
    for(auto iter=m.begin(); iter!=m.end(); ++iter){
        const auto& item = *iter;
        const auto& key = item.first;
        const auto& value = item.second;
        std::cout << key << ": ";
        std::cout << value << std::endl;
     }
}


template<typename Lambda,typename first,typename second>
auto operator|(const std::map<std::string,int>& container, Lambda lambda){
    std::map <std::string,int> mp;
    for(auto iter=container.begin(); iter!=container.end(); ++iter){
        const auto& item = *iter;
        const auto& key = item.first;
        const auto& value = item.second;
        mp[key] = lambda(value);
    }
    return mp;
}


template<typename Lambda,typename first>
auto operator|(first& container, Lambda lambda){
    for(auto& item : container)
        return lambda(item);
}


int main(int, char* [])
{
    auto employees = std::vector<Employee>{
            {"Fatih", Male, 35, 8500},
            {"Elif", Female, 28, 6350},
            {"Erkin", Male, 43, 9500},
            {"Mehmet", Male, 46, 10500},
            {"Melis", Female, 48, 12500},
            };

    // Q1) (5pts) Make employee directly sendable to the console
    //std::cout << "First Employee:" << std::endl << employees[0] << std::endl;
    // Fatih, Male, 35, 8500TL

    // Q2) (5pts) Create a print overload that can print "employees" variable
    std::cout << "All Employees:" << std::endl;
    print(employees);
    // [Fatih, Male, 35, 8500TL]; [Elif, Female, 28, 6350TL]; [Erkin, Male, 43, 9500TL]; [Mehmet, Male, 46, 10500TL]; [Melis, Female, 48, 12500TL]

    // Q3) (5pts) Create a filter overload that workswith allcontainers.
    //           Make it work with "employees"
    auto age_thresh = 40;
    std::cout << "Employees older than 40 years old:" << std::endl;
    auto lambda_filter = [&age_thresh](const Employee& emp){return emp.age > age_thresh;};
    auto age_gt_40 = filter(employees, lambda_filter);
    print(age_gt_40);
    // [Erkin, Male, 43, 9500TL]; [Mehmet, Male, 46, 10500TL]; [Melis, Female, 48, 12500TL]

    // Q4a) (5pts) Apply filter to a container by & operator
    // Q4b) (5pts) Send container to console by piping it to PRINT with | operator.
    age_thresh = 45;
    std::cout << "Employees older than 45 years old (again in functional composition):" << std::endl;
    employees & lambda_filter | PRINT;
    // [Mehmet, Male, 46, 10500TL]; [Melis, Female, 48, 12500TL]

    // Q5) (5pts) Make below code work.
    std::cout << "Female employees who take less salary than 9000TL:" << std::endl;
    print(filter(employees,[](const Employee& emp){return (emp.salary < 9000) && (emp.gender==0);}));
    // [Elif, Female, 28, 6350TL]


    // Q6a) (15pts) Create group_by algorithm that groupselements of containersby
    //             a specific criterion
    // Q6b) (5pts) Group employees by age ranges of 10 year increments by filling in
    //            lambda_group below
    // Q6c) (5pts) Make the grouped result printable via print(grouped) as below
    std::cout << "Employees grouped by age ranges of 10 year increments:" << std::endl;
    auto lambda_group = [](const Employee& emp,int item){return (emp.age < item) && (emp.age >= item-10);};
    auto grouped = group_by(employees, lambda_group);
    print(grouped);
    // <20..29>: [Elif, Female, 28, 6350TL]
    // <30..39>: [Fatih, Male, 35, 8500TL]
    // <40..49>: [Erkin, Male, 43, 9500TL]; [Mehmet, Male, 46, 10500TL]; [Melis, Female, 48, 12500TL]

    // Q7a) (10pts) Create a transform algorithm that can work on "grouped" variable.
    // Q7b) (5pts) Write necessary transformation lambda for aggregating each
    //            group's element count.
    // Q7c) (5pts) Make the grouped and transformed result printable via
    //            print(grouped_transformed) as below
    std::cout << "Number of employees available in age ranges of 10 year increments:" << std::endl;
    auto lambda_transform = [](auto vectors){ return vectors.size();};
    auto grouped_transformed = transform(grouped, lambda_transform);
    print(grouped_transformed);
    // <20..29>: 1
    // <30..39>: 1
    // <40..49>: 3

    // Q8) (10pts) Make the below code work as the same as Q7's code block
    //std::cout << "Number of employees available in age ranges of 10 year increments (again):" << std::endl;
    //grouped | lambda_transform | PRINT;

    //Q9) (5pts) Make below line work
    //std::array<int, 3>{10, 20, 30} | [](int a) { return a*a; } | PRINT;
    // [100]; [400]; [900]

    // Q10) Overload "operator-" to get a container withreverse content
    // Q10a) (5pts) Make it work with array containers with resultspipeable to PRINT
    // Q10b) (5pts) Make it work with other containers with resultspipeable to PRINT
    //-std::array<float, 4>{1.1, 2.2, 3.3, 4.4} | PRINT;
    // [4.4]; [3.3]; [2.2]; [1.1]
    //-std::vector<float>{1.1, 2.2, 3.3, 4.4} | PRINT;
    // [4.4]; [3.3]; [2.2]; [1.1]
    return 0;
}
