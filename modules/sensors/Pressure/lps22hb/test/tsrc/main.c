
#include <stdio.h>
#include "unity_fixture.h"
#include "unity.h"

void disableInterrupts(void)
{

}

void enableInterrupts(void)
{

}

void run_lps22hb_tests(void)
{
	RUN_TEST_GROUP(lps22hb);
}

int main(int argc, const char * argv[])
{
	return UnityMain(argc, argv, run_lps22hb_tests);
}
    