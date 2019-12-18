//
// Created by Stan Potemkin on 30.11.2019.
//

#include "PureParser.hpp"
#include <string>
#include <map>
#include <set>
#include <vector>
#include <iostream>

#pragma mark - Local Types

typedef struct {
    std::string formula;
    std::string output;
    std::string reference;
} test_result_t;

typedef test_result_t(*test_func_t)(void);

typedef struct {
    std::string caption;
    test_func_t executor;
} test_meta_t;

#pragma mark - Test Cases

static test_result_t test_Plain() {
    PureParser parser;
    const std::string formula = "Hello world";

    const std::string output = parser.execute(formula, true, true);
    const std::string reference = "Hello world";
    
    return test_result_t {
        .formula = formula,
        .output = output,
        .reference = reference
    };
}

static test_result_t test_Variable() {
    PureParser parser;
    const std::string formula = "Hello, $name";
    
    parser.assignVariable("name", "Stan");

    const std::string output = parser.execute(formula, true, true);
    const std::string reference = "Hello, Stan";
    
    return test_result_t {
        .formula = formula,
        .output = output,
        .reference = reference
    };
}

static test_result_t test_VariableAndBlockWithVariable() {
    PureParser parser;
    const std::string formula = "Please wait, $name: we're calling $[$anotherName ## another guy]";
    
    parser.assignVariable("name", "Stan");
    parser.assignVariable("anotherName", "Paul");
    
    const std::string output = parser.execute(formula, true, true);
    const std::string reference = "Please wait, Stan: we're calling Paul";
    
    return test_result_t {
        .formula = formula,
        .output = output,
        .reference = reference
    };
}

static test_result_t test_VariableAndBlockWithPlain() {
    PureParser parser;
    const std::string formula = "Please wait, $name: we're calling $[$anotherName ## another guy]";
    
    parser.assignVariable("name", "Stan");
    
    const std::string output = parser.execute(formula, true, true);
    const std::string reference = "Please wait, Stan: we're calling another guy";
    
    return test_result_t {
        .formula = formula,
        .output = output,
        .reference = reference
    };
}

static test_result_t test_BlockWithRich() {
    PureParser parser;
    const std::string formula = "Congrats! You saved it $[in folder '$folder'].";
    
    parser.assignVariable("folder", "Documents");
    
    const std::string output = parser.execute(formula, true, true);
    const std::string reference = "Congrats! You saved it in folder 'Documents'.";
    
    return test_result_t {
        .formula = formula,
        .output = output,
        .reference = reference
    };
}

static test_result_t test_InactiveBlock() {
    PureParser parser;
    const std::string formula = "Congrats! You saved it $[in folder '$folder'].";
    
    const std::string output = parser.execute(formula, true, true);
    const std::string reference = "Congrats! You saved it.";
    
    return test_result_t {
        .formula = formula,
        .output = output,
        .reference = reference
    };
}
    
static test_result_t test_ComplexActiveAlias() {
    PureParser parser;
    const std::string formula = "$[Agent $creatorName ## You] changed reminder $[«$comment»] $[:target: for $[$targetName ## you]] on $date at $time";
    
    parser.assignVariable("comment", "Check his payment");
    parser.assignVariable("date", "today");
    parser.assignVariable("time", "11:30 AM");
    parser.enableAlias("target");

    const std::string output = parser.execute(formula, true, true);
    const std::string reference = "You changed reminder «Check his payment» for you on today at 11:30 AM";
    
    return test_result_t {
        .formula = formula,
        .output = output,
        .reference = reference
    };
}

static test_result_t test_ComplexInactiveAlias() {
    PureParser parser;
    const std::string formula = "$[Agent $creatorName ## You] changed reminder $[«$comment»] $[:target: for $[$targetName ## you]] on $date at $time";

    parser.assignVariable("comment", "Check his payment");
    parser.assignVariable("date", "today");
    parser.assignVariable("time", "11:30 AM");

    const std::string output = parser.execute(formula, true, true);
    const std::string reference = "You changed reminder «Check his payment» on today at 11:30 AM";

    return test_result_t {
        .formula = formula,
        .output = output,
        .reference = reference
    };
}

#pragma mark - Execute all Test Cases

#ifndef main_cpp
#define main_cpp main
#endif

int main_cpp() {
    #define declare_test_case(func) test_meta_t{.caption=#func,.executor=func}
    const std::vector<test_meta_t> all_tests {
        declare_test_case(test_Plain),
        declare_test_case(test_Variable),
        declare_test_case(test_VariableAndBlockWithVariable),
        declare_test_case(test_VariableAndBlockWithPlain),
        declare_test_case(test_BlockWithRich),
        declare_test_case(test_InactiveBlock),
        declare_test_case(test_ComplexActiveAlias),
        declare_test_case(test_ComplexInactiveAlias)
    };
    #undef declare_test_case

    for (auto &test : all_tests) {
        const test_result_t result = test.executor();
        std::cout << "Testing '" << test.caption <<  "'" << std::endl;
        std::cout << "> Formula: " << result.formula << std::endl;

        if (result.output == result.reference) {
            std::cout << "[OK]" << std::endl;
        }
        else {
            std::cout << "> Output: " << result.output << std::endl;
            std::cout << "> Reference: " << result.reference << std::endl;
            std::cout << "[Failed]" << std::endl;
            return 1;
        }
        
        std::cout << std::endl;
    }

    std::cout << "== All test cases passed OK ==" << std::endl;
    return 0;
}
