#include "unity.h"
#include "unity_fixture.h"
#include <stdio.h>

void disableInterrupts(void)
{
}

void enableInterrupts(void)
{
}

void run_tmp468_tests(void)
{
    RUN_TEST_GROUP(tmp468);
}

int main(int argc, const char *argv[])
{
    return UnityMain(argc, argv, run_tmp468_tests);
}