#pragma once

#include <vector>
#include <optional>
#include <iostream>

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

std::ostream& operator<<(std::ostream& os, const problem& p);