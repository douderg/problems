#include "problem.hpp"


int main(int argc, char** argv) {

    if (argc < 6) {
        std::cerr << R"~(Usage:
        schedule <employees> <senior-employees> <offices> <senior-offices> <days>
            employees           The total number of employees
            senior-employees    The number of employees that can work in the senior-only offices
            offices             The total number of offices
            senior-offices      The number of senior-only offices that can be reused multiple times
            days                The number of days for the schedule
)~";
        return EXIT_FAILURE;
    }

    try {
        size_t employees = std::atoi(argv[1]);
        size_t senior_employees = std::atoi(argv[2]);
        size_t offices = std::atoi(argv[3]);
        size_t senior_offices = std::atoi(argv[4]);
        size_t days = std::atoi(argv[5]);

        auto prob = problem()
            .with_employees(employees, senior_employees)
            .with_offices(offices, senior_offices)
            .for_days(days);
        
        prob.calculate();
        std::cout << prob;
    } catch (const std::runtime_error& err) {
        std::cerr << err.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
