# Testing Plan: ASSIGN with Operations

## ASSIGN + ADD
- [*] variable + variable
- [*] variable + value
- [*] variable + function
- [*] value + variable
- [*] value + value
- [*] value + function
- [*] function + variable
- [*] function + value
- [*] function + function

## ASSIGN + SUB
- [*] variable - variable
- [*] variable - value
- [*] variable - function
- [*] value - variable
- [*] value - value
- [*] value - function
- [*] function - variable
- [*] function - value
- [*] function - function

## ASSIGN + MUL
- [*] variable * variable
- [*] variable * value
- [*] variable * function
- [*] value * variable
- [*] value * value
- [*] value * function
- [*] function * variable
- [*] function * value
- [*] function * function

## ASSIGN + DIV
- [*] variable / variable
- [*] variable / value
- [*] variable / function
- [*] value / variable
- [*] value / value
- [*] value / function
- [*] function / variable
- [*] function / value
- [*] function / function

## ASSIGN + MOD
- [*] variable % variable
- [*] variable % value
- [*] variable % function
- [*] value % variable
- [*] value % value
- [*] value % function
- [*] function % variable
- [*] function % value
- [*] function % function
- [ ] Modulo by zero edge cases

## ASSIGN + LESS
- [*] variable < variable
- [*] variable < value
- [*] variable < function
- [*] value < variable
- [*] value < value
- [*] value < function
- [*] function < variable
- [*] function < value
- [*] function < function

## ASSIGN + LESS_EQUAL
- [*] variable <= variable
- [*] variable <= value
- [*] variable <= function
- [*] value <= variable
- [*] value <= value
- [*] value <= function
- [*] function <= variable
- [*] function <= value
- [*] function <= function

## ASSIGN + MORE
- [*] variable > variable
- [*] variable > value
- [*] variable > function
- [*] value > variable
- [*] value > value
- [*] value > function
- [*] function > variable
- [*] function > value
- [*] function > function

## ASSIGN + MORE_EQUAL
- [*] variable >= variable
- [*] variable >= value
- [*] variable >= function
- [*] value >= variable
- [*] value >= value
- [*] value >= function
- [*] function >= variable
- [*] function >= value
- [*] function >= function

## ASSIGN + AND
- [*] variable && variable
- [*] variable && value
- [*] variable && function
- [*] value && variable
- [*] value && value
- [*] value && function
- [*] function && variable
- [*] function && value
- [*] function && function
- [*] Short-circuit evaluation

## ASSIGN + OR
- [*] variable || variable
- [*] variable || value
- [*] variable || function
- [*] value || variable
- [*] value || value
- [*] value || function
- [*] function || variable
- [*] function || value
- [*] function || function
- [*] Short-circuit evaluation

## ASSIGN + NOT
- [*] !variable
- [*] !value
- [*] !function
- [*] Double negation: !!variable

## ASSIGN + EQUAL
- [*] variable == variable
- [*] variable == value
- [*] variable == function
- [*] value == variable
- [*] value == value
- [*] value == function
- [*] function == variable
- [*] function == value
- [*] function == function

## ASSIGN + NOT_EQUAL
- [*] variable != variable
- [*] variable != value
- [*] variable != function
- [*] value != variable
- [*] value != value
- [*] value != function
- [*] function != variable
- [*] function != value
- [*] function != function

## Edge Cases & Combined Operations
- [ ] Chained assignments: a = b = c
- [ ] Complex expressions: a = (b + c) * (d - e)
- [ ] Nested function calls: a = f(g(h()))
- [ ] Type mismatches (if applicable)
- [ ] Null/undefined values (if applicable)
- [ ] Operator precedence in assignments
- [ ] Assignment with side effects