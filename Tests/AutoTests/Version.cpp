// autotest.cpp
#include "UnitTest++/UnitTest++.h"

#include "WkCocos/Download/Comp/DataLoad.h"

#include <string>

SUITE(Version)
{
    //symmetric split join constructor
	TEST(symmetric_split_join_3)
	{
	    WkCocos::Download::Version v1("v0.1.2");
	    std::vector<unsigned long> v1s = v1.split();

		CHECK(v1s[0] == 0);
		CHECK(v1s[1] == 1);
		CHECK(v1s[2] == 2);

		WkCocos::Download::Version v2(v1s);

		CHECK( v2 == v1);

	}

    TEST(symmetric_split_join_4)
	{
	    WkCocos::Download::Version v1("v0.1.2.3");
	    std::vector<unsigned long> v1s = v1.split();

		CHECK(v1s[0] == 0);
		CHECK(v1s[1] == 1);
		CHECK(v1s[2] == 2);
		CHECK(v1s[3] == 3);

		WkCocos::Download::Version v2(v1s);

		CHECK( v2 == v1);
	}

    //equals
    TEST(equals_3_case_1)
    {
        WkCocos::Download::Version v1("v0.1.2");
        WkCocos::Download::Version v2("v0.1.2");

        CHECK ( v1 == v2 );
    }

    TEST(equals_4_case_1)
    {
        WkCocos::Download::Version v1("v0.1.2.3");
        WkCocos::Download::Version v2("v0.1.2.3");

        CHECK ( v1 == v2 );
    }

    //not equals
    TEST(not_equals_3_case_1)
    {
        WkCocos::Download::Version v1("v0.1.2");
        WkCocos::Download::Version v2("v9.1.2");

        CHECK ( v1 != v2 );
    }

    TEST(not_equals_3_case_2)
    {
        WkCocos::Download::Version v1("v0.1.2");
        WkCocos::Download::Version v2("v0.9.2");

        CHECK ( v1 != v2 );
    }

    TEST(not_equals_3_case_3)
    {
        WkCocos::Download::Version v1("v0.1.2");
        WkCocos::Download::Version v2("v0.1.9");

        CHECK ( v1 != v2 );
    }

    TEST(not_equals_4_case_1)
    {
        WkCocos::Download::Version v1("v0.1.2.3");
        WkCocos::Download::Version v2("v9.1.2.3");

        CHECK ( v1 != v2 );
    }

    TEST(not_equals_4_case_2)
    {
        WkCocos::Download::Version v1("v0.1.2.3");
        WkCocos::Download::Version v2("v0.9.2.3");

        CHECK ( v1 != v2 );
    }

    TEST(not_equals_4_case_3)
    {
        WkCocos::Download::Version v1("v0.1.2.3");
        WkCocos::Download::Version v2("v0.1.9.3");

        CHECK ( v1 != v2 );
    }

    TEST(not_equals_4_case_4)
    {
        WkCocos::Download::Version v1("v0.1.2.3");
        WkCocos::Download::Version v2("v0.1.2.9");

        CHECK ( v1 != v2 );
    }

    //less
    TEST(less_3_case_1)
    {
        WkCocos::Download::Version v1("v0.1.2");
        WkCocos::Download::Version v2("v1.1.2");

        CHECK ( v1 < v2 );
    }

    TEST(less_3_case_2)
    {
        WkCocos::Download::Version v1("v0.1.2");
        WkCocos::Download::Version v2("v0.2.2");

        CHECK ( v1 < v2 );
    }

    TEST(less_3_case_3)
    {
        WkCocos::Download::Version v1("v0.1.2");
        WkCocos::Download::Version v2("v0.1.3");

        CHECK ( v1 < v2 );
    }

    TEST(less_3_case_4)
    {
        WkCocos::Download::Version v1("v0.1.2");
        WkCocos::Download::Version v2("v0.2.1");

        CHECK ( v1 < v2 );
    }

    TEST(less_3_case_5)
    {
        WkCocos::Download::Version v1("v0.1.2");
        WkCocos::Download::Version v2("v1.0.1");

        CHECK ( v1 < v2 );
    }

    TEST(less_3_case_6)
    {
        WkCocos::Download::Version v1("v0.1.2");
        WkCocos::Download::Version v2("v1.2.1");

        CHECK ( v1 < v2 );
    }

    TEST(less_3_case_7)
    {
        WkCocos::Download::Version v1("v0.1.2");
        WkCocos::Download::Version v2("v1.2.3");

        CHECK ( v1 < v2 );
    }

    //less 4
    TEST(less_4_case_1)
    {
        WkCocos::Download::Version v1("v0.1.2.3");
        WkCocos::Download::Version v2("v1.1.2.3");

        CHECK ( v1 < v2 );
    }

    TEST(less_4_case_2)
    {
        WkCocos::Download::Version v1("v0.1.2.3");
        WkCocos::Download::Version v2("v0.2.2.3");

        CHECK ( v1 < v2 );
    }

    TEST(less_4_case_3)
    {
        WkCocos::Download::Version v1("v0.1.2.3");
        WkCocos::Download::Version v2("v0.1.3.3");

        CHECK ( v1 < v2 );
    }

    TEST(less_4_case_4)
    {
        WkCocos::Download::Version v1("v0.1.2.3");
        WkCocos::Download::Version v2("v0.1.2.4");

        CHECK ( v1 < v2 );
    }

    TEST(less_4_case_5)
    {
        WkCocos::Download::Version v1("v0.1.2.3");
        WkCocos::Download::Version v2("v0.1.3.2");

        CHECK ( v1 < v2 );
    }

    TEST(less_4_case_6)
    {
        WkCocos::Download::Version v1("v0.1.2.3");
        WkCocos::Download::Version v2("v0.2.1.2");

        CHECK ( v1 < v2 );
    }

    TEST(less_4_case_7)
    {
        WkCocos::Download::Version v1("v0.1.2.3");
        WkCocos::Download::Version v2("v1.0.1.2");

        CHECK ( v1 < v2 );
    }

    TEST(less_4_case_8)
    {
        WkCocos::Download::Version v1("v0.1.2.3");
        WkCocos::Download::Version v2("v1.2.1.2");

        CHECK ( v1 < v2 );
    }

    TEST(less_4_case_9)
    {
        WkCocos::Download::Version v1("v0.1.2.3");
        WkCocos::Download::Version v2("v1.2.3.2");

        CHECK ( v1 < v2 );
    }

    TEST(less_4_case_10)
    {
        WkCocos::Download::Version v1("v0.1.2.3");
        WkCocos::Download::Version v2("v1.2.3.4");

        CHECK ( v1 < v2 );
    }
}
