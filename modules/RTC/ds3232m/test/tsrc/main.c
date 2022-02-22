
#include <stdio.h>
#include "unity_fixture.h"
#include "unity.h"

void disableInterrupts(void)
{

}

void enableInterrupts(void)
{

}

void run_ds3232m_tests(void)
{
	RUN_TEST_GROUP(ds3232m);
}

int main(int argc, const char * argv[])
{
	return UnityMain(argc, argv, run_ds3232m_tests);
}
    