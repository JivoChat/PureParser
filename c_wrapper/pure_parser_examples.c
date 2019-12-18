//
//  pure_parser_examples.c
//  PureParser
//
//  Created by Stan Potemkin on 05.12.2019.
//

#include "pure_parser.h"
#include <stdio.h>
#include <string.h>

#pragma mark - Local Types

typedef struct {
    const char *formula;
    char *output;
    const char *reference;
} test_result_t;

typedef void(*test_func_t)(test_result_t *);

typedef struct {
    const char *caption;
    test_func_t executor;
} test_meta_t;

#pragma mark - Local Helpers

static test_meta_t test_meta_make(const char *caption, test_func_t executor) {
    test_meta_t meta;
    meta.caption = caption;
    meta.executor = executor;
    return meta;
}

#pragma mark - Test Cases

static void test_Plain(test_result_t *result) {
    pure_config_t config;
    pure_config_set_default(&config);
    
    pure_parser_t parser;
    pure_parser_init(&parser, &config);
    
    const char *formula = "Hello world";
    result->formula = formula;
    result->reference = "Hello world";

    pure_parser_execute(&parser, result->formula, true, true, &result->output, NULL);
}

static void test_Variable(test_result_t *result) {
    pure_config_t config;
    pure_config_set_default(&config);
    
    pure_parser_t parser;
    pure_parser_init(&parser, &config);
    
    const char *formula = "Hello, $name";
    result->formula = formula;
    result->reference = "Hello, Stan";

    pure_parser_assign_var(&parser, "name", "Stan");
    pure_parser_execute(&parser, formula, true, true, &result->output, NULL);
}

static void test_VariableAndBlockWithVariable(test_result_t *result) {
    pure_config_t config;
    pure_config_set_default(&config);
    
    pure_parser_t parser;
    pure_parser_init(&parser, &config);
    
    const char *formula = "Please wait, $name: we're calling $[$anotherName ## another guy]";
    result->formula = formula;
    result->reference = "Please wait, Stan: we're calling Paul";

    pure_parser_assign_var(&parser, "name", "Stan");
    pure_parser_assign_var(&parser, "anotherName", "Paul");
    pure_parser_execute(&parser, result->formula, true, true, &result->output, NULL);
}

static void test_VariableAndBlockWithPlain(test_result_t *result) {
    pure_config_t config;
    pure_config_set_default(&config);
    
    pure_parser_t parser;
    pure_parser_init(&parser, &config);
    
    const char *formula = "Please wait, $name: we're calling $[$anotherName ## another guy]";
    result->formula = formula;
    result->reference = "Please wait, Stan: we're calling another guy";

    pure_parser_assign_var(&parser, "name", "Stan");
    pure_parser_execute(&parser, formula, true, true, &result->output, NULL);
}

static void test_BlockWithRich(test_result_t *result) {
    pure_config_t config;
    pure_config_set_default(&config);
    
    pure_parser_t parser;
    pure_parser_init(&parser, &config);
    
    const char *formula = "Congrats! You saved it $[in folder '$folder'].";
    result->formula = formula;
    result->reference = "Congrats! You saved it in folder 'Documents'.";

    pure_parser_assign_var(&parser, "folder", "Documents");
    pure_parser_execute(&parser, formula, true, true, &result->output, NULL);
}

static void test_InactiveBlock(test_result_t *result) {
    pure_config_t config;
    pure_config_set_default(&config);
    
    pure_parser_t parser;
    pure_parser_init(&parser, &config);
    
    const char *formula = "Congrats! You saved it $[in folder '$folder'].";
    result->formula = formula;
    result->reference = "Congrats! You saved it.";

    pure_parser_execute(&parser, result->formula, true, true, &result->output, NULL);
}

static void test_ComplexActiveAlias(test_result_t *result) {
    pure_config_t config;
    pure_config_set_default(&config);
    
    pure_parser_t parser;
    pure_parser_init(&parser, &config);
    
    const char *formula = "$[Agent $creatorName ## You] changed reminder $[«$comment»] $[:target: for $[$targetName ## you]] on $date at $time";
    result->formula = formula;
    result->reference = "You changed reminder «Check his payment» for you on today at 11:30 AM";

    pure_parser_assign_var(&parser, "comment", "Check his payment");
    pure_parser_assign_var(&parser, "date", "today");
    pure_parser_assign_var(&parser, "time", "11:30 AM");
    pure_parser_enable_alias(&parser, "target");
    pure_parser_execute(&parser, result->formula, true, true, &result->output, NULL);
}

static void test_ComplexInactiveAlias(test_result_t *result) {
    pure_config_t config;
    pure_config_set_default(&config);
    
    pure_parser_t parser;
    pure_parser_init(&parser, &config);
    
    const char *formula = "$[Agent $creatorName ## You] changed reminder $[«$comment»] $[:target: for $[$targetName ## you]] on $date at $time";
    result->formula = formula;
    result->reference = "You changed reminder «Check his payment» on today at 11:30 AM";

    pure_parser_assign_var(&parser, "comment", "Check his payment");
    pure_parser_assign_var(&parser, "date", "today");
    pure_parser_assign_var(&parser, "time", "11:30 AM");
    pure_parser_execute(&parser, result->formula, true, true, &result->output, NULL);
}

#pragma mark - Execute all Test Cases

#ifndef main_c
#define main_c main
#endif

int main_c() {
    #define declare_test_case(func) test_meta_make(#func, func)
    const test_meta_t all_tests[] = {
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
    
    const size_t number_of_tests = sizeof(all_tests) / sizeof(*all_tests);
    test_result_t result;
    
    for (const test_meta_t *test = all_tests, *end = &all_tests[number_of_tests]; test < end; test++) {
        test->executor(&result);
        
        printf("Testing '%s'\n", test->caption);
        printf("> Formula: %s\n", result.formula);

        if (strcmp(result.output, result.reference) == 0) {
            printf("[OK]\n");
        }
        else {
            printf("> Output: %s\n", result.output);
            printf("> Reference: %s\n", result.reference);
            printf("[Failed]\n");
            return 1;
        }
    }

    printf("== All test cases passed OK ==\n");
    return 0;
}
