
#include <stdio.h>
#include "unity_fixture.h"
#include "unity.h"

void disableInterrupts(void)
{

}

void enableInterrupts(void)
{

}

void run_mpr121_tests(void)
{
	RUN_TEST_GROUP(mpr121);
}

int main(int argc, const char * argv[])
{
	return UnityMain(argc, argv, run_mpr121_tests);
}
    