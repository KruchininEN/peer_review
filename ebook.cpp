#include <iostream>
#include <iomanip>
#include <vector>

int MAX_ID = 100000;    // max user id
int MAX_PAGE = 1000;    // max page  

class BookManager {
public:
    BookManager()
        : id_to_read_progress_(MAX_ID + 1, -1)
        , pages_to_read_count_(MAX_PAGE, 0)
    {}

    // proceeding commands
    void Proceed(std::istream& input) {
        int command_count;
        input >> command_count;

        for (size_t i = 0; i < command_count; ++i) {
            std::string command;
            std::cin >> command;

            if (command == "READ") { Read(input); }
            if (command == "CHEER") { Cheer(input); }
        }
    }

    void Read(std::istream& input) {
        int id, page;
        input >> id >> page;
        for (int i = id_to_read_progress_[id] + 1; i <= page; ++i) {
            // changing the status of previous pages
            ++pages_to_read_count_[i];
        }

        id_to_read_progress_[id] = page; // recording the progress of a given user
    }

    void Cheer(std::istream& input) {
        int id;
        input >> id;
        if (id_to_read_progress_[id] == -1) {   // no read pages for this user
            std::cout << 0 << std::endl;        // means 0 % readers read less
            return;
        }
        if (pages_to_read_count_[0] == 1) { // only 1 user
            std::cout << 1 << std::endl;    // means 100 % of readers read less
            return;
        }
        std::cout
            << std::setprecision(6)
            // count of users who read at least first page (i.e. all users)
            << static_cast <double> (pages_to_read_count_[0]
                // minus count of users who read pages equal as user
                - pages_to_read_count_[id_to_read_progress_[id]])
            // divide by count of users who read at least first page (i.e. all users), except the user
            / static_cast <double> (pages_to_read_count_[0] - 1)
            << std::endl;
    }

private:
    // (index + 1 == user id), (value == page which user read)
    std::vector <int> id_to_read_progress_;
    // (index + 1 == page number), (value == number of users read this page)
    std::vector <int> pages_to_read_count_;
};

int main() {
    BookManager book_manager;
    book_manager.Proceed(std::cin);

    return 0;
}