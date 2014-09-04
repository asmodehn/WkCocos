// autotest.cpp
#include "UnitTest++/UnitTest++.h"

#include "WkCocos/StrongBox/StrongBox.h"

TEST(FailSpectacularly)
{
	CHECK(false);
}

int main()
{
	return UnitTest::RunAllTests();
}
