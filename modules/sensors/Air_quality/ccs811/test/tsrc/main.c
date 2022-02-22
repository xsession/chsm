
#include <stdio.h>
#include "unity_fixture.h"
#include "unity.h"

void disableInterrupts(void)
{

}

void enableInterrupts(void)
{

}

void run_ccs811_tests(void)
{
	RUN_TEST_GROUP(ccs811);
}

int main(int argc, const char * argv[])
{
	return UnityMain(argc, argv, run_ccs811_tests);
}
    