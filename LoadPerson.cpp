vector <Person> LoadPersons 
(
    std::string_view db_name,       // ���
    int db_connection_timeout,      // ������� ����������� � ���� ������
    bool db_allow_exceptions,       // ���������� ��������� ���������� ��� ������ � ����� ������
    DBLogLevel db_log_level,        // ������� ����������� ���� ������
    int min_age,                    // ����������� ������� �����
    int max_age,                    // ������������ ������� �����
    std::string_view name_filter    // ������ �����
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

    auto loaded_rows = db.LoadRows<std::string, int>(query); // �������� optional, ����� ������������ ������ ������ ����� ����
    if (!loaded_rows) {
        return {};
    }

    vector<Person> persons;
    for (auto [name, age] : *loaded_rows) {
        persons.push_back({ std::move(name), age });
    }
    return persons;
}