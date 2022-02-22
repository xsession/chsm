
#include <stdio.h>
#include "unity_fixture.h"
#include "unity.h"

void disableInterrupts(void)
{

}

void enableInterrupts(void)
{

}

void run_apds9960_tests(void)
{
	RUN_TEST_GROUP(apds9960);
}

int main(int argc, const char * argv[])
{
	return UnityMain(argc, argv, run_apds9960_tests);
}
    