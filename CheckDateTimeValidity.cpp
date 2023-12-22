void CheckDateRange(int value, int min, int max, const std::string& field) {    // универсальная функция проверки периода по заданным рамкам для различных временных отрезков
    if (value < min || value > max) {
        throw std::domain_error(field + " is out of range"s);
    }
}

int GetMonthLength(const DateTime& dt) {
    const bool is_leap_year = ((dt.year % 4 == 0) && !(dt.year % 100 == 0 && dt.year % 400 != 0));
    const std::array <int, 12> month_lengths = { 31, 28 + is_leap_year, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    return month_lengths[dt.month - 1];
}

void CheckDateTimeValidity (const DateTime& dt) {
     CheckDateRange(dt.year,     1, 9999,                "year"  ); 
     CheckDateRange(dt.month,    1, 12,                  "month" );
     CheckDateRange(dt.day,      1, GetMonthLength(dt),  "day"   );
     CheckDateRange(dt.hour,     0, 23,                  "hour"  );
     CheckDateRange(dt.minute,   0, 59,                  "minute");
     CheckDateRange(dt.second,   0, 59,                  "second");
}