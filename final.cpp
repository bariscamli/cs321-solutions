#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <tuple>


std::ostream& operator<<(std::ostream& str, const std::tuple<int, int, int, int>& tpl)
{
    auto [v1, v2, v3, v4] = tpl;
    str << " d" << v1 << "w" << v2 << "=" << v3 << "/" << v4;
    return str;
}


#include <range/v3/all.hpp>

using std::vector;
using std::array;
using std::string;
using std::tuple;
using std::cout;
using std::endl;

namespace views = ranges::views; 
namespace actions = ranges::actions; 
using namespace ranges; 


enum DayOfWeek { Mon, Tue, Wed, Thu, Fri, Sat, Sun };
auto day_names = array<string, 7>{"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
auto month_days = array<int, 12>{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};



void print_calendar(int year, DayOfWeek first_day){
    // calculate how many days are there in February
    month_days[1] = 28 + (int)(year % 4 == 0); 


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






















