### Description

Evaluates a schedule for employees under the following constraints:

- Each employee may only work once in each office
- Each office holds up to one employee each day
- Some offices may only be used by senior employees
    - Senior offices may be reused by the same employee multiple times
- The schedule must be as fair as possible

### Usage

```
schedule <employees> <senior-employees> <offices> <senior-offices> <days>
    employees           The total number of employees
    senior-employees    The number of employees that can work in the senior-only offices
    offices               The total number of offices
    senior-offices        The number of senior-only offices that can be reused multiple times
    days                The number of days for the schedule
```

### Example

Calling `schedule 33 6 28 1 32` using a terminal will produce the schedule in the following format:

|senior office 1|senior office 2| ... |office 1|office 2|...|
|---|---|---|---|---|---|
|day 1|employee id|employee id|employee id|...||
|day 2|employee id|employee id|...|||
|...||||||

A report for the day offs of each employee follows for validating results

```
Schedule: 
  0, 25, 17, 18, 19, 20, 21, 22, 23, 24, 16, 26, 27, 28, 29, 30, 31, 32, 15, 14, 13, 12, 11, 10,  9,  8,  7,  6
  1,  2,  3,  4,  5, 32, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,  0, 19, 18, 17, 16, 15, 14, 13, 12, 11
...
Days off per employee:
0 (senior): 5
1 (senior): 5
2 (senior): 5
...
```
