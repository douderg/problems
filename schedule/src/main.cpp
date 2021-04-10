#include <iostream>
#include <vector>
#include <iomanip>
#include <optional>
#include <numeric>
#include <algorithm>
#include <set>

class problem {
public:

    problem& with_offices(size_t total, size_t senior_only);

    problem& with_employees(size_t total, size_t seniors);

    problem& for_days(size_t count);

    void calculate();

    const std::vector<std::vector<std::optional<size_t>>>& solution() const;

    size_t employee_count() const;

    size_t senior_employee_count() const;

private:
    void validate_solution();

    size_t employees, offices, senior_offices, senior_employees, mixed_offices, days;
    std::vector<std::vector<std::optional<size_t>>> solution_;
};

problem& problem::with_offices(size_t total, size_t senior_only) {
    offices = total;
    senior_offices = senior_only;
    mixed_offices = total - senior_only;
    return *this;
}

problem& problem::with_employees(size_t total, size_t seniors) {
    employees = total;
    senior_employees = seniors;
    return *this;
}

problem& problem::for_days(size_t count) {
    days = count;
    return *this;
}

size_t problem::employee_count() const {
    return employees;
}

size_t problem::senior_employee_count() const {
    return senior_employees;
}

void problem::calculate() {
    std::vector<std::optional<size_t>> ids(offices);
    size_t id=0;
    for (auto& v:ids) {
        v=id++;
    }
    std::vector<size_t> days_off(employees);
    std::fill(days_off.begin(), days_off.end(), days);
    std::vector<std::set<size_t>> worked_before(mixed_offices);

    std::vector<size_t> employee(employees);
    std::iota(employee.begin(), employee.end(), 0);
    
    for (size_t i = 0; i < days; ++i) {
        std::vector<size_t> today = employee;
        std::sort(today.begin(), today.end(), [&](size_t x, size_t y) -> bool { return days_off[x] > days_off[y]; });

        std::vector<std::optional<size_t>> working(offices);
        size_t senior_workers = 0;
        size_t assigned = 0;
        std::vector<bool> office_full(mixed_offices);
        for (size_t j = 0; j < employees && assigned < mixed_offices; ++j) {
            bool found = false;
            if (today[j] < senior_employees && senior_workers < senior_offices) {
                working[senior_workers] = today[j];
                --days_off[today[j]];
                ++senior_workers;
            } else {
                if (days_off[today[j]] < days_off[today[0]]) {
                    continue;
                }
                for (size_t k = 0; k < mixed_offices; ++k) {
                    if (office_full[k]) {
                        continue;
                    }
                    auto it = worked_before[k].find(today[j]);
                    if (it == worked_before[k].end()) {
                        worked_before[k].insert(today[j]);
                        working[k + senior_offices] = today[j];
                        office_full[k] = true;
                        --days_off[today[j]];
                        ++assigned;
                        break;
                    }
                }
            }
        }
        solution_.push_back(working);
    }
    validate_solution();
}

void problem::validate_solution() {
    std::vector<std::set<size_t>> office_assignments(mixed_offices);
    for (size_t i = 0; i < days; ++i) {
        for (size_t j = 0; j < mixed_offices; ++j) {
            auto value = solution_[i][j + senior_offices];
            if (!value) {
                continue;
            }
            auto it = office_assignments[j].find(*value);
            if (it != office_assignments[j].end()) {
                throw std::runtime_error("duplicate entry found");
            }
        }
    }
}

const std::vector<std::vector<std::optional<size_t>>>& problem::solution() const {
    return solution_;
}

std::ostream& operator<<(std::ostream& os, const problem& p) {
    auto solution = p.solution();

    std::vector<size_t> days_off(p.employee_count());
    std::fill(days_off.begin(), days_off.end(), solution.size());
    for (const auto& day : solution) {
        for (const auto& employee : day) {
            if (employee) {
                --days_off[*employee];
            }
        }
    }

    os << "Schedule: \n";
    for (const auto& day : solution) {
        if (day[0]) {
            os << std::setw(3) << *day[0];
        } else {
            os << std::setw(3) << "";
        }
        for (size_t i = 1; i < day.size(); ++i) {
            if (day[i]) {
                os << "," << std::setw(3) << *day[i];
            } else {
                os << "," << std::setw(3) << "";
            }
        }
        os << "\n";
    }

    os << "Days off per employee:\n";
    for (size_t i = 0; i < days_off.size(); ++i) {
        std::string label = i < p.senior_employee_count() ? " (senior): " : " (junior): ";
        os << i << label << days_off[i] << "\n";
    }
    
    return os;
}


int main(int argc, char** argv) {

    size_t employees = 40;
    size_t offices = 28;
    size_t senior_employees = 6;
    size_t senior_offices = 1;
    size_t days = 32;

    std::cout << "enter number of employees\n";
    std::cin >> employees;
    std::cout << "enter number of senior employees\n";
    std::cin >> senior_employees;
    std::cout << "enter number of offices\n";
    std::cin >> offices;
    std::cout << "enter number of senior-only offices\n";
    std::cin >> senior_offices;
    std::cout << "enter number of days\n";
    std::cin >> days;

    auto prob = problem()
        .with_employees(employees, senior_employees)
        .with_offices(offices, senior_offices)
        .for_days(days);

    try {
        prob.calculate();
        std::cout << prob;
    } catch (const std::runtime_error& err) {
        std::cerr << err.what() << "\n";
    }

    return EXIT_SUCCESS;
}
