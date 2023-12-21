vector <Person> LoadPersons 
(
    std::string_view db_name,       // имя
    int db_connection_timeout,      // таймаут подключения к базе данных
    bool db_allow_exceptions,       // разрешение генерации исключений при работе с базой данных
    DBLogLevel db_log_level,        // уровень логирования базы данных
    int min_age,                    // минимальный возраст людей
    int max_age,                    // максимальный возраст людей
    std::string_view name_filter    // фильтр имени
) 
{
    if (min_age > max_age) {
        throw std::invalid_argument("Invalid age range");
    }

    DBConnector connector (db_allow_exceptions, db_log_level);
    DBHandler db;

    if (db_name.starts_with("tmp."s)) {
        db = connector.ConnectTmp(db_name, db_connection_timeout);
    }
    else {
        db = connector.Connect(db_name, db_connection_timeout);
    }
    if (!db_allow_exceptions && !db.IsOK()) {
        return {};
    }

    ostringstream query_str;
    query_str   << "FROM Persons "s
                << "SELECT Name, Age "s
                << "WHERE Age between "s
                << min_age << " AND "s << max_age 
                << " and Name like '%"s << db.Quote(name_filter) << "%'"s;
    
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