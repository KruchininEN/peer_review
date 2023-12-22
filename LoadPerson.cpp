struct DBParameters
{
    std::string_view db_name,       // ���
    int db_connection_timeout,      // ������� ����������� � ���� ������
    bool db_allow_exceptions,       // ���������� ��������� ���������� ��� ������ � ����� ������
    DBLogLevel db_log_level,        // ������� ����������� ���� ������
    int min_age,                    // ����������� ������� �����
    int max_age,                    // ������������ ������� �����
    std::string_view name_filter    // ������ �����

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