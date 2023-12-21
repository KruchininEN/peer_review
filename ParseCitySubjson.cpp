void ParseCitySubjson
(
    // передаются несколько параметров вместо длинного списка аргументов
    vector <City>& cities,
    const Json& json,
    const CountryInfo& countryInfo
)
{
    for (const auto& cityJson : json.AsList())
    {
        const auto& cityObj = cityJson.AsObject();
        cities.push_back
        (
            {
                cityObj["name"s].AsString(),
                cityObj["iso_code"s].AsString(),
                countryInfo.phone_code + cityObj["phone_code"s].AsString(),
                countryInfo.name,
                countryInfo.iso_code,
                countryInfo.time_zone,
                countryInfo.languages
            }
        );
    }
}

// ParseCountryJson возвращает CountryInfo, чтобы разделить логику парсинга страны и создания объекта Country
CountryInfo ParseCountryJson (const Json& countryJson) {

    CountryInfo countryInfo // структура CountryInfo для группировки данных о стране
    {
        countryJson["name"s].AsString(),
        countryJson["iso_code"s].AsString(),
        countryJson["phone_code"s].AsString(),
        countryJson["time_zone"s].AsString()
    };

    for (const auto& langObj : countryJson["languages"s].AsList()) {
        countryInfo.languages.push_back ( FromString <Language>(langObj.AsString()) );
    }

    return countryInfo;
}

void ParseCountriesJson 
(
    vector <Country> & countries, 
    vector <City >& cities, 
    const Json& json
) 
{
    for (const auto& countryJson : json.AsList()) 
    {
        CountryInfo countryInfo = ParseCountryJson(countryJson);
        countries.push_back
        (
            { 
                countryInfo.name, 
                countryInfo.iso_code,
                countryInfo.phone_code, 
                countryInfo.time_zone 
            }
        );
        ParseCitySubjson 
        (
            cities, 
            countryJson["cities"s], 
            countryInfo
        );
    }
}