#include <stdio.h>
#include "unity_fixture.h"
#include "unity.h"


void disableInterrupts(void)
{

}

void enableInterrupts(void)
{

}

void run_{{cookiecutter.module_name}}_tests(void)
{
	RUN_TEST_GROUP({{cookiecutter.module_name}});
}

int main(int argc, const char * argv[])
{
	return UnityMain(argc, argv, run_{{cookiecutter.module_name}}_tests);
}