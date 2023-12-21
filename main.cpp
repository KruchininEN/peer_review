#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>

using namespace std;

class Domain {
public:
    Domain(const string& line)
        : domain_(line)
    {
        reverse(domain_.begin(), domain_.end());
        domain_.push_back('.');
    }

    string GetDomain() const { return domain_; }

    // comparison operators overload
    bool operator== (const Domain& other) const { return GetDomain() == other.GetDomain(); }
    bool operator<  (const Domain& other) const { return GetDomain() <  other.GetDomain(); }
    bool operator>  (const Domain& other) const { return other.GetDomain() < GetDomain(); }

    // subdomain check
    bool IsSubdomain(const Domain& other) const {
        return name_.find(other.name_) == 0;
    }

private:
    string domain_;
};

class DomainChecker {
public:

    template<typename Iterator>
    DomainChecker(Iterator begin, Iterator end)
        : forbidden_domains_(begin, end)
    {
        // sort domains in ascending order to conveniently exclude subdomains
        sort
        (
            forbidden_domains_.begin(),
            forbidden_domains_.end(),
            [](const Domain& lhs, const Domain& rhs) {
                return lhs.GetDomain() < rhs.GetDomain();
            }
        );

        // exclude subdomains
        auto it = unique
        (
            forbidden_domains_.begin(),
            forbidden_domains_.end(),
            [](const Domain& lhs, const Domain& rhs) {
                return rhs.IsSubdomain(lhs);
            }
        );

        // remove empty positions
        forbidden_domains_.erase (it, forbidden_domains_.end());
    }

    bool IsForbidden(const Domain& domain) const {
        auto it = upper_bound
        (
            forbidden_domains_.begin(),
            forbidden_domains_.end(),
            domain,
            [](const Domain& lhs, const Domain& rhs) { 
                return lhs < rhs; 
            }
        );

        if (it == forbidden_domains_.begin()) {
            return false; 
        }
        else {
            Domain prev_domain = *(it - 1);
            return domain.IsSubdomain(prev_domain);
        }
    }

private:
    vector<Domain> forbidden_domains_;
};

template <typename Number>
vector<Domain> ReadDomains(istream& input, Number dom_count) {
    vector<Domain> domains;
    domains.reserve(dom_count);
    string domain;

    for (size_t i = 0; i < dom_count; ++i) {
        getline(input, domain);
        domains.push_back(domain);
    }
    return domains;
}

template <typename Number>
Number ReadNumberOnLine(istream& input) {
    string line;
    getline(input, line);

    Number num;
    istringstream(line) >> num;

    return num;
}

int main() {
    const vector<Domain> forbidden_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    const vector<Domain> test_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    for (const Domain& domain : test_domains) {
        cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }
}