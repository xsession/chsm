#include "unity.h"
#include "unity_fixture.h"
#include <stdio.h>

void disableInterrupts(void)
{
}

void enableInterrupts(void)
{
}

void run_sht30_tests(void)
{
    RUN_TEST_GROUP(sht30);
}

int main(int argc, const char *argv[])
{
    return UnityMain(argc, argv, run_sht30_tests);
}