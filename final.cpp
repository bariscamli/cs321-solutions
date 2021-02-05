#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <tuple>

// printer for a tuple of 4 int dimensions.you will need this in Q4 for easily sending tuples to the console.
// this must be defined before including range lib here to make it compatible with clang

std::ostream& operator<<(std::ostream& str, const std::tuple<int, int, int, int>& tpl)
{
    auto [v1, v2, v3, v4] = tpl;
    str << " d" << v1 << "w" << v2 << "=" << v3 << "/" << v4;
    return str;
}


#include <range/v3/all.hpp>

// Exam is about generating a year's days with extra knowledge of what week, month a day belongs.

// Sample calendars provided for your convenience. Use a calendar software to see whole


// YOU CANNOT CHANGETHE GIVEN CODE TEMPLATE. USE ONLY THE LOCATIONS UNDER THE QUESTION REGIONS.

// YOU CANNOT USE "if" statement. ranges::find_ifandranges::find_if_notareallowed.
// YOU CANNOT USE "for" statement.
// YOU CANNOT USE "for_each" statement of range library.
// YOU CANNOT USE "while" or "do-while" statement.
// YOU CANNOT USE A MUTABLE LAMBDA (you can use non-mutable lambdas as many times as you want).
// YOU CANNOT INSTANTIATE vector<T> or array<T, sz> yourself. You can use "to<vector<T>>" of range library.
//  i.e. auto v = vector<int>{...} is forbidden.//  i.e. auto a = array<int, 10>{...} is forbidden.
// YOU CANNOT ADD new "#include" directives.// YOU CANNOT ADD new "class" or "struct" declarations/definitions.
// USE ONLY range-v3 library and allowed C++ facilities.// !! ANY FAILURE TO DO SO WILL MAKE YOU GET 0 GRADE FROM RELATED QUESTIONS !!

// All questions will be tested with a different year other than 2020 or2021.
// Q4 has 2 different grading paths:Q4_full path (70 pts), Q4_partial (45 pts) path.
// Q4 is graded such that:// -If Q4_full works you get directly Q4's total points (70 pts). (Must exactly match correct result, solve however as you wish obeying the rules above)
// -Else you can go for partial points by solving Q4a, Q4b, Q4c, Q4d.
// You cansolve Q4_full and still solve Q4a, Q4b, Q4c, Q4d to guarantee partial points only in case Q4_full does not work in our test case.
// Hint:You can print a container by changing it to a range and then sending to cout// cout << (container | views::all);
// Hint:You can do arithmetic operations over iterators. iter2 -iter1 gives you the number of elements after iter1 to reach iter2.
// Hint:below range-v3 facilities were enough for solving all the questions.
// This does not mean that you are limited by the below tools. There are many ways to solve the same problem.
// ranges::begin
// ranges::find_if
// to<vector<int>>
// views::all
// views::concat
// views::exclusive_scan
// views::iota
// views::group_by
// views::reverse
// views::take
// views::tail
// views::transform
// views::zip


using std::vector;
using std::array;
using std::string;
using std::tuple;
using std::cout;
using std::endl;

namespace views = ranges::views; // instead of ranges::views::*, directly use views::*
namespace actions = ranges::actions; // instead of ranges::views::*, directly use views::*
using namespace ranges; // instead of ranges::*, directly use *


enum DayOfWeek { Mon, Tue, Wed, Thu, Fri, Sat, Sun };
auto day_names = array<string, 7>{"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
auto month_days = array<int, 12>{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};



void print_calendar(int year, DayOfWeek first_day){
    // calculate how many days are there in February
    month_days[1] = 28 + (int)(year % 4 == 0); // you can use this trick to add 1 or 0 conditionally without an if


    cout << "Calendar Year is " << year << "." << endl;
    cout <<"First day of the year is " << day_names[first_day] << "." << endl;

    cout << "Q1) (10 pts) Day boundaries of a new month are:" << endl;
    // for 2020:  [0,31,60,91,121,152,182,213,244,274,305,335,366]
    // for 2021:  [0,31,59,90,120,151,181,212,243,273,304,334,365]
    auto first_boundry = views::iota(0,1);
    auto rng_boundries = month_days | views::partial_sum(std::plus<>());
    cout << views::concat(first_boundry,rng_boundries) << endl;


    cout << "Q2) (5 pts) Total days in " << year << " is " << endl;
    cout << accumulate(month_days,0,std::plus<>()) << endl;


    cout << "Q3) (15 pts) Month of Day Sequence" << endl;
    auto const all_months =views::iota(1,13) | to<vector<int>>();
    auto month_of_day_sequence = views::zip_with([](auto a, auto b){return views::concat(views::repeat_n(a,b));}, all_months,month_days) | views::join;
    cout << month_of_day_sequence << endl;


    cout << "Q4a) (10 pts) Week of Day Sequence" << endl;
    auto const all_weeks =views::iota(1,54) | to<vector<int>>();
    auto first_week = 7-(int)first_day;
    auto number_of_days = views::concat(views::single(first_week),views::repeat_n(7,51),views::single((365+ (int)(year % 4 == 0))-(first_week+7*51)));
    auto week_of_day_sequence = views::zip_with([](auto a, auto b){return views::concat(views::repeat_n(a,b));}, all_weeks,number_of_days) | views::join;
    cout << week_of_day_sequence << endl;


    cout << "Q4b) (10 pts) Month Offsets as Days" << endl;
    auto month_offsets_day_sequence = views::zip_with([](auto a, auto b){return views::concat(views::repeat_n(a,b));},month_days | views::exclusive_scan(0),month_days) | views::join;
    cout << month_offsets_day_sequence << endl;


    cout << "Q4c) (10 pts) Day of Month Sequence" << endl;
    auto day_of_month_sequence = month_days | views::transform([](auto days){return views::concat(views::iota(1,days+1));}) | views::join;
    cout << day_of_month_sequence << endl;


    cout << "Q4d) (15 pts) Days in Same Week and Month" << endl;
    auto first_part = number_of_days | views::take(1) | views::transform([](auto days){return views::iota(1,days+1);});
    auto mid_part_cumulativ = number_of_days | views::reverse | views::drop(1) | views::drop(1) | views::reverse | views::partial_sum(std::plus<>());
    auto mid_part =  mid_part_cumulativ |  views::transform([](auto days){return views::iota(days+1,days+8);});
    auto last_part = number_of_days | views::reverse | views::take(1) | views::transform([&](auto days){return views::iota((365+ (int)(year % 4 == 0)) - days + 1,(365+ (int)(year % 4 == 0)) + 1);});
    cout << views::concat(first_part,mid_part,last_part) << endl;


}

int main(){
    print_calendar(2020, Wed); // 2020 starts with Wednesday
    print_calendar(2021, Fri); // 2021 starts with Friday
    return 0;
}






















