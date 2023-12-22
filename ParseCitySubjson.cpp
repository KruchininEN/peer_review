// структура CountryData для группировки данных о стране
struct CountryData {
    string phone_code
    string name
    string iso_code
    string time_zone
    vector <Language> languages
};

// передаются несколько параметров вместо длинного списка аргументов
void ParseCitySubjson (vector <City> & cities, const Json & json, const CountryData & country_info) {
    
    for (const auto& cityJson : json.AsList()) {
        const auto& cityObj = cityJson.AsObject();
        cities.push_back (
            {
                cityObj["name"s].AsString(),
                cityObj["iso_code"s].AsString(),
                country_info.phone_code + cityObj["phone_code"s].AsString(),
                country_info.name,
                country_info.iso_code,
                country_info.time_zone,
                country_info.languages
            }
        );
    }
}

// ParseCountryJson возвращает CountryData, чтобы разделить логику парсинга страны и создания объекта Country
CountryData ParseCountryJson (const Json& country_json) {

    CountryData country_info {
        country_json["name"s].AsString(),
        country_json["iso_code"s].AsString(),
        country_json["phone_code"s].AsString(),
        country_json["time_zone"s].AsString()
    };

    for (const auto& langObj : country_json["languages"s].AsList()) {
        country_info.languages.push_back ( FromString <Language> (langObj.AsString()) );
    }

    return country_info;
}

void ParseCountriesJson (vector <Country> & countries, vector <City> & cities, const Json & json) {
    for (const auto& country_json : json.AsList()) {
        CountryData country_info = ParseCountryJson(country_json);
        countries.push_back (
            { 
                country_info.name,
                country_info.iso_code,
                country_info.phone_code,
                country_info.time_zone
            }
        );
        ParseCitySubjson(cities, countryJson["cities"s], country_info);
    }
}