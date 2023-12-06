#include <iostream>
#include <iomanip>
#include <vector>

int MAX_ID = 100000;                                                                                    // max user id
int MAX_PAGE = 1000;                                                                                    // max page  

class BookManager {
public:
    BookManager()
        : id_to_read_progress_(MAX_ID + 1, -1)
        , pages_to_read_count_(MAX_PAGE, 0)
    {}

    void Proceed() {                                                                                    // proceeding commands
        int command_count;
        std::cin >> command_count;

        for (size_t i = 0; i < command_count; ++i) {
            std::string command;
            std::cin >> command;

            if (command == "READ") { Read(); }
            if (command == "CHEER") { Cheer(); }
        }
    }

    void Read() {
        int id, page;
        std::cin >> id >> page;
        for (int i = id_to_read_progress_[id] + 1; i <= page; ++i) { ++pages_to_read_count_[i]; }       // changing the status of previous pages
        id_to_read_progress_[id] = page;                                                                // recording the progress of a given user
    }

    void Cheer() {
        int id;
        std::cin >> id;
        if (id_to_read_progress_[id] == -1) {                                                           // no read pages for this user
            std::cout << 0 << std::endl;                                                                // means 0 % readers read less
            return;
        }
        if (pages_to_read_count_[0] == 1) {                                                             // only 1 user
            std::cout << 1 << std::endl;                                                                // means 100 % of readers read less
            return;
        }
        std::cout
            << std::setprecision(6)
            << static_cast <double> (pages_to_read_count_[0]                                            // count of users who read at least first page (i.e. all users)
                - pages_to_read_count_[id_to_read_progress_[id]])                                       // minus count of users who read pages equal as user
            / static_cast <double> (pages_to_read_count_[0] - 1)                                        // divide by count of users who read at least first page (i.e. all users), except the user
            << std::endl;
    }

private:
    std::vector <int> id_to_read_progress_;                                                             // (index + 1 == user id), (value == page which user read)
    std::vector <int> pages_to_read_count_;                                                             // (index + 1 == page number), (value == number of users read this page)
};

int main() {
    BookManager book_manager;
    book_manager.Proceed();

    return 0;
}