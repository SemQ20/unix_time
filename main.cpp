#include <iostream>
#include <cmath>

//#define DEBUG

struct current_time{
    uint16_t year = 2021;
    uint16_t day  = 2;
    uint16_t months = 3;
    uint16_t hours = 6;
    uint16_t minutes = 30;
    uint16_t seconds = 29;
    uint32_t unix_time;

    current_time() = default;
    current_time(uint32_t _unix_time) : unix_time{_unix_time} {}
    current_time(uint16_t _year, uint16_t _day, uint16_t _months, uint16_t _hours, uint16_t _minutes, uint16_t _seconds) : year{_year}, day{_day}, months{_months},hours{_hours}, minutes{_minutes}, seconds{_seconds} {}
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

void bcd_represent(uint8_t *buf, current_time _struct){
    uint16_t value[7] = {_struct.year, _struct.day, _struct.months, _struct.hours, _struct.minutes, _struct.seconds};
    uint16_t thousands;
    uint16_t hundreds;
    uint16_t tens;
    uint16_t units;
    uint16_t tmp;
    uint16_t j = 0;
    for(uint16_t i = 0; i <= 6; i++){
    if(value[i] > 1000){
        tmp = value[i] % 1000;
        units = (tmp % 100) % 10;
        hundreds = tmp / 100;
        tens = (tmp % 100) / 10;
        thousands = value[i] / 1000;
        buf[j++] = (thousands << 4) | hundreds;
        buf[j++] = (tens << 4) | units;
    }else{
        buf[j++] = ((value[i] / 10) << 4) | (value[i] % 10);
    }
   }
}

void bcd_to_decimal(uint8_t *buf, current_time* _time){
    _time->year =  (((buf[0] & 0xf0)>>4) * 1000) + (((buf[0] & 0x0f)) * 100 + (((buf[1] & 0xf0)>>4) * 10 + ((buf[1] & 0x0f))));
    _time->day =  (((buf[2] & 0xf0)>>4) * 10) + (((buf[2] & 0x0f)));
    _time->months = (((buf[3] & 0xf0)>>4) * 10) + (((buf[3] & 0x0f)));
    _time->hours = (((buf[4] & 0xf0)>>4) * 10) + (((buf[4] & 0x0f)));
    _time->minutes = (((buf[5] & 0xf0)>>4) * 10) + (((buf[5] & 0x0f)));
    _time->seconds = (((buf[6] & 0xf0)>>4) * 10) + (((buf[6] & 0x0f)));
}

int main()
{
    current_time ct(2021,15,3,17,5,5);
    //current_time ct(1615050305);
    //ct.unix_time_to_current_time();
    //ct.print();
    uint8_t bcd_repr[7];
    bcd_represent(bcd_repr, ct);
    bcd_to_decimal(bcd_repr,&ct);
    ct.unix_current_time();
    ct.print();
    return 0;
}
