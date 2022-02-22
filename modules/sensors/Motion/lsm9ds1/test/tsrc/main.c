
#include <stdio.h>
#include "unity_fixture.h"
#include "unity.h"

void disableInterrupts(void)
{

}

void enableInterrupts(void)
{

}

void run_lsm9ds1_tests(void)
{
	RUN_TEST_GROUP(lsm9ds1);
}

int main(int argc, const char * argv[])
{
	return UnityMain(argc, argv, run_lsm9ds1_tests);
}
    