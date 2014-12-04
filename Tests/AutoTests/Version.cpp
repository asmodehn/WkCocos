// autotest.cpp
#include "UnitTest++/UnitTest++.h"

#include "WkCocos/Download/Version.h"

#include <string>

SUITE(Version)
{
    //default equals
    TEST(default_null)
	{
	    WkCocos::Download::Version v1;
	    std::vector<unsigned long> v1s = v1.split();

		CHECK(v1s[0] == 0);

		WkCocos::Download::Version v2("v0");

		CHECK( v2 == v1);

		WkCocos::Download::Version v3("v0.0");

		CHECK( v3 == v1);

		WkCocos::Download::Version v4("v0.0.0");

		CHECK( v4 == v1);

	}

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

    //toString
    TEST(toString_3)
	{
	    WkCocos::Download::Version v1("v0.1.2");

		CHECK( v1.toString() == "v0.1.2");
	}

    TEST(toString_4)
	{
	    WkCocos::Download::Version v1("v0.1.2.3");

		CHECK( v1.toString() == "v0.1.2.3");
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

    TEST(not_equals_3_case_4)
    {
        WkCocos::Download::Version v1("v0.1.2");
        WkCocos::Download::Version v2("v0.9.9");

        CHECK ( v1 != v2 );
    }

    TEST(not_equals_3_case_5)
    {
        WkCocos::Download::Version v1("v0.1.2");
        WkCocos::Download::Version v2("v9.9.9");

        CHECK ( v1 != v2 );
    }

    TEST(not_equals_3_case_6)
    {
        WkCocos::Download::Version v1("v0.1.2");
        WkCocos::Download::Version v2("v9.9.2");

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

    TEST(not_equals_4_case_5)
    {
        WkCocos::Download::Version v1("v0.1.2.3");
        WkCocos::Download::Version v2("v0.1.9.9");

        CHECK ( v1 != v2 );
    }

    TEST(not_equals_4_case_6)
    {
        WkCocos::Download::Version v1("v0.1.2.3");
        WkCocos::Download::Version v2("v0.9.9.3");

        CHECK ( v1 != v2 );
    }

    TEST(not_equals_4_case_7)
    {
        WkCocos::Download::Version v1("v0.1.2.3");
        WkCocos::Download::Version v2("v9.9.2.3");

        CHECK ( v1 != v2 );
    }

    TEST(not_equals_4_case_8)
    {
        WkCocos::Download::Version v1("v0.1.2.3");
        WkCocos::Download::Version v2("v9.9.9.3");

        CHECK ( v1 != v2 );
    }

    TEST(not_equals_4_case_9)
    {
        WkCocos::Download::Version v1("v0.1.2.3");
        WkCocos::Download::Version v2("v0.9.9.9");

        CHECK ( v1 != v2 );
    }

    TEST(not_equals_4_case_10)
    {
        WkCocos::Download::Version v1("v0.1.2.3");
        WkCocos::Download::Version v2("v9.9.9.9");

        CHECK ( v1 != v2 );
    }


    //less // pattern 1 : +1
    TEST(less_3_case_1)
    {
        WkCocos::Download::Version v1("v1.2.3");
        WkCocos::Download::Version v2("v2.2.3");

        CHECK ( v1 < v2 );
        CHECK ( v1 <= v2 );
        CHECK ( v2 > v1 );
        CHECK ( v2 >= v1 );
    }

    TEST(less_3_case_2)
    {
        WkCocos::Download::Version v1("v1.2.3");
        WkCocos::Download::Version v2("v1.3.3");

        CHECK ( v1 < v2 );
        CHECK ( v1 <= v2 );
        CHECK ( v2 > v1 );
        CHECK ( v2 >= v1 );
    }

    TEST(less_3_case_3)
    {
        WkCocos::Download::Version v1("v1.2.3");
        WkCocos::Download::Version v2("v1.2.4");

        CHECK ( v1 < v2 );
        CHECK ( v1 <= v2 );
        CHECK ( v2 > v1 );
        CHECK ( v2 >= v1 );
    }

    //less // pattern 2 : +1 +1
    TEST(less_3_case_4)
    {
        WkCocos::Download::Version v1("v1.2.3");
        WkCocos::Download::Version v2("v2.3.3");

        CHECK ( v1 < v2 );
        CHECK ( v1 <= v2 );
        CHECK ( v2 > v1 );
        CHECK ( v2 >= v1 );
    }

    TEST(less_3_case_5)
    {
        WkCocos::Download::Version v1("v1.2.3");
        WkCocos::Download::Version v2("v1.3.4");

        CHECK ( v1 < v2 );
        CHECK ( v1 <= v2 );
        CHECK ( v2 > v1 );
        CHECK ( v2 >= v1 );
    }

    //less // pattern 2 : +1 +1 +1
    TEST(less_3_case_6)
    {
        WkCocos::Download::Version v1("v1.2.3");
        WkCocos::Download::Version v2("v2.3.4");

        CHECK ( v1 < v2 );
        CHECK ( v1 <= v2 );
        CHECK ( v2 > v1 );
        CHECK ( v2 >= v1 );
    }
    //less //pattern 2 : +1 -1
    TEST(less_3_case_7)
    {
        WkCocos::Download::Version v1("v1.2.3");
        WkCocos::Download::Version v2("v2.1.3");

        CHECK ( v1 < v2 );
        CHECK ( v1 <= v2 );
        CHECK ( v2 > v1 );
        CHECK ( v2 >= v1 );
    }

    TEST(less_3_case_8)
    {
        WkCocos::Download::Version v1("v1.2.3");
        WkCocos::Download::Version v2("v1.3.2");

        CHECK ( v1 < v2 );
        CHECK ( v1 <= v2 );
        CHECK ( v2 > v1 );
        CHECK ( v2 >= v1 );
    }

    //less //pattern 3 : +1 -1 -1
    TEST(less_3_case_9)
    {
        WkCocos::Download::Version v1("v1.2.3");
        WkCocos::Download::Version v2("v2.1.2");

        CHECK ( v1 < v2 );
        CHECK ( v1 <= v2 );
        CHECK ( v2 > v1 );
        CHECK ( v2 >= v1 );
    }

    //less //pattern 4 : +1 +1 -1
    TEST(less_3_case_10)
    {
        WkCocos::Download::Version v1("v1.2.3");
        WkCocos::Download::Version v2("v2.3.2");

        CHECK ( v1 < v2 );
        CHECK ( v1 <= v2 );
        CHECK ( v2 > v1 );
        CHECK ( v2 >= v1 );
    }

    //less //multiple digits
    TEST(less_3_case_11)
    {
        WkCocos::Download::Version v1("v1.2.3");
        WkCocos::Download::Version v2("v11.2.3");

        CHECK ( v1 < v2 );
        CHECK ( v1 <= v2 );
        CHECK ( v2 > v1 );
        CHECK ( v2 >= v1 );
    }

    TEST(less_3_case_12)
    {
        WkCocos::Download::Version v1("v1.2.3");
        WkCocos::Download::Version v2("v1.12.3");

        CHECK ( v1 < v2 );
        CHECK ( v1 <= v2 );
        CHECK ( v2 > v1 );
        CHECK ( v2 >= v1 );
    }

    TEST(less_3_case_13)
    {
        WkCocos::Download::Version v1("v1.2.3");
        WkCocos::Download::Version v2("v1.2.13");

        CHECK ( v1 < v2 );
        CHECK ( v1 <= v2 );
        CHECK ( v2 > v1 );
        CHECK ( v2 >= v1 );
    }

    TEST(less_3_case_14)
    {
        WkCocos::Download::Version v1("v1.2.3");
        WkCocos::Download::Version v2("v1.12.13");

        CHECK ( v1 < v2 );
        CHECK ( v1 <= v2 );
        CHECK ( v2 > v1 );
        CHECK ( v2 >= v1 );
    }

    TEST(less_3_case_15)
    {
        WkCocos::Download::Version v1("v1.2.3");
        WkCocos::Download::Version v2("v11.12.3");

        CHECK ( v1 < v2 );
        CHECK ( v1 <= v2 );
        CHECK ( v2 > v1 );
        CHECK ( v2 >= v1 );
    }

    TEST(less_3_case_16)
    {
        WkCocos::Download::Version v1("v1.2.3");
        WkCocos::Download::Version v2("v11.12.13");

        CHECK ( v1 < v2 );
        CHECK ( v1 <= v2 );
        CHECK ( v2 > v1 );
        CHECK ( v2 >= v1 );
    }

    //less 4 // TODO : review like less for 3
    TEST(less_4_case_1)
    {
        WkCocos::Download::Version v1("v0.1.2.3");
        WkCocos::Download::Version v2("v1.1.2.3");

        CHECK ( v1 < v2 );
        CHECK ( v2 > v1 );
    }

    TEST(less_4_case_2)
    {
        WkCocos::Download::Version v1("v0.1.2.3");
        WkCocos::Download::Version v2("v0.2.2.3");

        CHECK ( v1 < v2 );
        CHECK ( v2 > v1 );
    }

    TEST(less_4_case_3)
    {
        WkCocos::Download::Version v1("v0.1.2.3");
        WkCocos::Download::Version v2("v0.1.3.3");

        CHECK ( v1 < v2 );
        CHECK ( v2 > v1 );
    }

    TEST(less_4_case_4)
    {
        WkCocos::Download::Version v1("v0.1.2.3");
        WkCocos::Download::Version v2("v0.1.2.4");

        CHECK ( v1 < v2 );
        CHECK ( v2 > v1 );
    }

    TEST(less_4_case_5)
    {
        WkCocos::Download::Version v1("v0.1.2.3");
        WkCocos::Download::Version v2("v0.1.3.2");

        CHECK ( v1 < v2 );
        CHECK ( v2 > v1 );
    }

    TEST(less_4_case_6)
    {
        WkCocos::Download::Version v1("v0.1.2.3");
        WkCocos::Download::Version v2("v0.2.1.2");

        CHECK ( v1 < v2 );
        CHECK ( v2 > v1 );
    }

    TEST(less_4_case_7)
    {
        WkCocos::Download::Version v1("v0.1.2.3");
        WkCocos::Download::Version v2("v1.0.1.2");

        CHECK ( v1 < v2 );
        CHECK ( v2 > v1 );
    }

    TEST(less_4_case_8)
    {
        WkCocos::Download::Version v1("v0.1.2.3");
        WkCocos::Download::Version v2("v1.2.1.2");

        CHECK ( v1 < v2 );
        CHECK ( v2 > v1 );
    }

    TEST(less_4_case_9)
    {
        WkCocos::Download::Version v1("v0.1.2.3");
        WkCocos::Download::Version v2("v1.2.3.2");

        CHECK ( v1 < v2 );
        CHECK ( v2 > v1 );
    }

    TEST(less_4_case_10)
    {
        WkCocos::Download::Version v1("v0.1.2.3");
        WkCocos::Download::Version v2("v1.2.3.4");

        CHECK ( v1 < v2 );
        CHECK ( v2 > v1 );
    }
}
