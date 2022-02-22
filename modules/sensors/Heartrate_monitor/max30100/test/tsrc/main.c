
#include <stdio.h>
#include "unity_fixture.h"
#include "unity.h"

void disableInterrupts(void)
{

}

void enableInterrupts(void)
{

}

void run_max30100_tests(void)
{
	RUN_TEST_GROUP(max30100);
}

int main(int argc, const char * argv[])
{
	return UnityMain(argc, argv, run_max30100_tests);
}
    