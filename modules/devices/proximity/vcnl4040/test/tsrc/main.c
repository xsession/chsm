
#include <stdio.h>
#include "unity_fixture.h"
#include "unity.h"

void disableInterrupts(void)
{

}

void enableInterrupts(void)
{

}

void run_vcnl4040_tests(void)
{
	RUN_TEST_GROUP(vcnl4040);
}

int main(int argc, const char * argv[])
{
	return UnityMain(argc, argv, run_vcnl4040_tests);
}
    