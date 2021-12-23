#ifndef _TEST_RUNNER_H_
#define _TEST_RUNNER_H_

#include <stdlib.h>

#define TEST_CASE(name) { #name, name }

typedef int (*FuncPointer) ();

typedef struct TestInfo {
    char *description;
    FuncPointer func_ptr;
} TestInfo;

typedef struct TestSuite {
    char* name;
    TestInfo *tests;
    size_t amount_of_tests;
    FuncPointer setup;
    FuncPointer teardown;
} TestSuite;

typedef struct TestSuiteResult {
    size_t amount_of_passed;
} TestSuiteResult;

void run_test_suites(TestSuite test_suites[], size_t amount_of_test_suites);

#endif //_TEST_RUNNER_H_
