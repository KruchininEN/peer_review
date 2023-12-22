struct DBParameters
{
    std::string_view db_name,       // имя
    int db_connection_timeout,      // таймаут подключения к базе данных
    bool db_allow_exceptions,       // разрешение генерации исключений при работе с базой данных
    DBLogLevel db_log_level,        // уровень логирования базы данных
    int min_age,                    // минимальный возраст людей
    int max_age,                    // максимальный возраст людей
    std::string_view name_filter    // фильтр имени

};

vector <Person> LoadPersons ( const DBParameters & params) {

    DBConnector connector (params.db_allow_exceptions, params.db_log_level);
    DBHandler db;

    if (params.db_name.starts_with("tmp."s)) {
        db = connector.ConnectTmp(params.db_name, params.db_connection_timeout);
    }
    else {
        db = connector.Connect(params.db_name, params.db_connection_timeout);
    }
    if (!params.db_allow_exceptions && !db.IsOK()) {
        return {};
    }

    ostringstream query_str;
    query_str   << "FROM Persons "s
                << "SELECT Name, Age "s
                << "WHERE Age between "s
                << params.min_age << " AND "s << params.max_age
                << " and Name like '%"s << db.Quote(params.name_filter) << "%'"s;
    
    DBQuery query(query_str.str());

    auto loaded_rows = db.LoadRows<std::string, int>(query); // добавлен optional, чтобы обрабатывать случаи ошибок более явно
    if (!loaded_rows) {
        return {};
    }

    vector<Person> persons;
    for (auto [name, age] : *loaded_rows) {
        persons.push_back({ std::move(name), age });
    }
    return persons;
}