// autotest.cpp
#include "UnitTest++/UnitTest++.h"
#include "UnitTest++/TestReporterStdout.h"

#include <string>

int main(int argc, char** argv)
{
	if (argc > 1)
	{
		//if first arg is "suite", we search for suite names instead of test names
		const bool bSuite = strcmp("suite", argv[1]) == 0;

		if (bSuite)
		{ //running test suite
			std::vector<char const * > suites;
			for (int i = 2; i < argc; ++i)
			{
				suites.push_back(argv[i]);
			}
			return UnitTest::RunAllTestsInSuites(suites);
		}
		else
		{ // running tests
			std::vector<char const * > tests;
			for (int i = 1; i < argc; ++i)
			{
				tests.push_back(argv[i]);
			}
			return UnitTest::RunAllNamedTests(tests);
		}
	}
	else
	{
		return UnitTest::RunAllTests();
	}
}
