#include <stdio.h>
#include "test_runner.h"

#define PASS_COLOR "\033[0;32m"
#define FAIL_COLOR "\033[0;31m"
#define PARTIAL_FAIL_COLOR "\e[0;33m"
#define DEFAULT_COLOR "\033[0m"

#define PASS_STR "PASSED"
#define FAIL_STR "FAILED"

void print_passing_str(int result);

int run_test(TestInfo test_info, int number, size_t amount_of_tests);

void setup() {
    setvbuf(stdout, NULL, _IONBF, 0);
}

void set_color(char *color) {
    printf("%s", color);
}

void print_in_color(char *text, char *color) {
    set_color(color);
    printf("%s", text);
    set_color(DEFAULT_COLOR);
}

void print_test_passed(size_t passed, size_t amount) {
    set_color(passed == amount ? PASS_COLOR : PARTIAL_FAIL_COLOR);
    printf("%lu\\%lu tests passed \n", passed, amount);
    set_color(DEFAULT_COLOR);
}

void print_suite_result(TestSuite testSuite, TestSuiteResult testSuiteResult) {
    printf("%s: ", testSuite.name);
    print_test_passed(testSuiteResult.amount_of_passed,
                      testSuite.amount_of_tests);
}

void print_summary(TestSuite test_suites[], TestSuiteResult *test_results,
                   size_t amount_of_test_suites) {
    printf("************************************************\n");
    printf("Finished running all tests !\n");
    printf("Summary:\n");

    size_t total_tests = 0;
    size_t total_passed = 0;

    for (size_t i = 0; i < amount_of_test_suites; i++) {
        print_suite_result(test_suites[i], test_results[i]);
        total_tests += test_suites[i].amount_of_tests;
        total_passed += test_results[i].amount_of_passed;
    }

    printf("\nTotal: ");
    print_test_passed(total_passed, total_tests);
    printf("************************************************\n");
}

TestSuiteResult run_test_suite(TestSuite test_suite) {
    int passed_count = 0;
    for (int i = 0; i < test_suite.amount_of_tests; i++) {
        if (test_suite.setup != NULL) test_suite.setup();
        if (run_test(test_suite.tests[i], i + 1, test_suite.amount_of_tests)) {
            passed_count++;
        }
        if (test_suite.teardown != NULL) test_suite.teardown();
    }

    return (TestSuiteResult) {passed_count};
}


void run_test_suites(TestSuite test_suites[], size_t amount_of_test_suites) {
    setup();
    printf("Running Tests:\n");
    printf("=============\n");

    TestSuiteResult *test_results = malloc(
            amount_of_test_suites * sizeof(TestSuiteResult));
    if (!test_results) {
        fprintf(stderr, "Allocation failed");
        return;
    }

    for (size_t i = 0; i < amount_of_test_suites; i++) {
        printf("(%lu\\%lu) Running Test Suite - %s:\n", i + 1,
               amount_of_test_suites, test_suites[i].name);
        test_results[i] = run_test_suite(test_suites[i]);
        printf("\n");
        print_suite_result(test_suites[i], test_results[i]);
        printf("====================\n");
    }

    print_summary(test_suites, test_results, amount_of_test_suites);

    free(test_results);
}

int run_test(TestInfo test_info, int number, size_t amount_of_tests) {
    printf("    (%d\\%lu) %s - ", number, amount_of_tests,
           test_info.description);
    int result = test_info.func_ptr();
    print_passing_str(result);
    printf("\n");
    return result;
}

void print_passing_str(int result) {
    print_in_color(result ? PASS_STR : FAIL_STR,
                   result ? PASS_COLOR : FAIL_COLOR);
}
