#include <iostream>
#include <cmath>

//#define DEBUG

struct current_time{
    int year = 2021;
    int day  = 2;
    int months = 3;
    int hours = 6;
    int minutes = 30;
    int seconds = 29;
    uint32_t unix_time;

    current_time() = default;
    current_time(uint32_t _unix_time) : unix_time{_unix_time} {}
    current_time(int _year, int _day, int _months, int _hours, int _minutes, int _seconds) : year{_year}, day{_day}, months{_months},hours{_hours}, minutes{_minutes}, seconds{_seconds} {}
    void unix_current_time();
    void leap_years();
    void print();
    void unix_time_to_current_time();

private:
    int year_in_sec = 31536000;
    int day_in_sec = 86400;
    int minute_in_sec = 60;
    int hour_in_sec = 3600;
    int unix_year = 1970;
    int first_leap_year = 1972;
    int _leap_years;
    bool current_year_leap;
    uint32_t year_since_epoch;
    uint32_t months_table[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
};

void current_time::leap_years(){
    _leap_years = static_cast<int>(((year - first_leap_year) / 4)) + 1;
    if(((year - first_leap_year) % 4) == 0){
        current_year_leap = true;
    }else{
        current_year_leap = false;
    }
}

void current_time::unix_current_time(){
    leap_years();
    uint32_t day_in_monts = 0;
    // minus current month and minus one element in array to correctness measure days
    for(int i = 0; i <= (months - 2); i++){
        day_in_monts += months_table[i];
    }
   (current_year_leap && months <= 2) ? day_in_monts = day_in_monts - 1 : day_in_monts;
    unix_time = (_leap_years * day_in_sec) + ((year - unix_year) * year_in_sec) + (day_in_monts * day_in_sec) + ((day - 1) * day_in_sec) + (hours * hour_in_sec) + (minutes * minute_in_sec) + seconds;
}

void current_time::unix_time_to_current_time(){
    year_since_epoch = (unix_time / year_in_sec);
    year = unix_year + year_since_epoch;
    leap_years();
    uint32_t days_since_epoch = static_cast<int>((unix_time / day_in_sec) - _leap_years);
    day = static_cast<int>(days_since_epoch - (year_since_epoch * 365));
    uint32_t hours_sec = (unix_time - ((year_since_epoch * year_in_sec) + ((day + _leap_years) * day_in_sec)));
    hours = static_cast<int>(hours_sec / 3600);
    uint32_t minutes_sec = (hours_sec - hours * 3600);
    minutes = static_cast<int>(minutes_sec / 60);
    seconds = minutes_sec - minutes * 60;
    months = (day / 30);
    for(int i = 0; i < months; i++){
        day -= months_table[i];
    }
    (day > 0) ? months = months + 1 : months;
    (hours >= 0) ? day = day + 1 : day;
}

void current_time::print(){
    std::cout << "unix_time: " << unix_time << '\n';
    std::cout << "year: " << year << " month: " << months << " day: " << day << " hours: " << hours << " minutes: " << minutes << " seconds: " << seconds <<  '\n';
}

int main()
{
    //current_time ct;
   // current_time ct(2004,2,3,16,25,28);
    current_time ct(1614702328);
    ct.unix_time_to_current_time();
    //ct.unix_current_time();
    ct.print();
    return 0;
}
