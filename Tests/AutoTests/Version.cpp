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

        CHECK(v1[0].first == 0);
        CHECK(v1[1].first == 0);
        CHECK(v1[2].first == 0);
        CHECK(v1[3].first == 0); //v1[3] is created here, set to 0 by default
        //etc.

		WkCocos::Download::Version v2("v0");

        CHECK( v2[0].first == 0);
        CHECK( v2[1].first == 0);
        CHECK( v2[2].first == 0);
        //etc.
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

		CHECK(v1[0].first == 0);
		CHECK(v1[1].first == 1);
		CHECK(v1[2].first == 2);

		WkCocos::Download::Version v2(to_string(v1));

		CHECK( v2 == v1 );

	}

    TEST(symmetric_split_join_4)
	{
	    WkCocos::Download::Version v1("v0.1.2.3");

		CHECK(v1[0].first == 0);
		CHECK(v1[1].first == 1);
		CHECK(v1[2].first == 2);
		CHECK(v1[3].first == 3);

		WkCocos::Download::Version v2(to_string(v1));

		CHECK( v2 == v1 );
	}

    //to_string
    TEST(toString_3)
	{
	    WkCocos::Download::Version v1("v0.1.2");

		CHECK( to_string(v1) == "v0.1.2" );
	}

    TEST(toString_4)
	{
	    WkCocos::Download::Version v1("v0.1.2.3");

		CHECK( to_string(v1) == "v0.1.2.3" );
	}

    TEST(toString_failconvert_3)
	{
	    WkCocos::Download::Version v1("v0.1.128454779843708");

		CHECK( to_string(v1) == "v0.1.128454779843708" );
	}

    TEST(toString_failconvert_4)
	{
	    WkCocos::Download::Version v1("v0.1.2.128454779843708");

		CHECK( to_string(v1) == "v0.1.2.128454779843708" );
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

    //Lexical comparison tests
    //equals
    TEST(Lexical_equals_3_case_1)
    {
        WkCocos::Download::Version v1("v128454779843704.1.2");
        WkCocos::Download::Version v2("v128454779843704.1.2");

        CHECK ( v1 == v2 );
    }

    TEST(Lexical_equals_3_case_2)
    {
        WkCocos::Download::Version v1("v0.128454779843708.2");
        WkCocos::Download::Version v2("v0.128454779843708.2");

        CHECK ( v1 == v2 );
    }

    TEST(Lexical_equals_3_case_3)
    {
        WkCocos::Download::Version v1("v0.1.128454779843716");
        WkCocos::Download::Version v2("v0.1.128454779843716");

        CHECK ( v1 == v2 );
    }

    TEST(Lexical_equals_3_case_4)
    {
        WkCocos::Download::Version v1("v0.128454779843708.128454779843716");
        WkCocos::Download::Version v2("v0.128454779843708.128454779843716");

        CHECK ( v1 == v2 );
    }

    TEST(Lexical_equals_3_case_5)
    {
        WkCocos::Download::Version v1("v128454779843704.128454779843708.2");
        WkCocos::Download::Version v2("v128454779843704.128454779843708.2");

        CHECK ( v1 == v2 );
    }

    TEST(Lexical_equals_3_case_6)
    {
        WkCocos::Download::Version v1("v128454779843704.1.128454779843716");
        WkCocos::Download::Version v2("v128454779843704.1.128454779843716");

        CHECK ( v1 == v2 );
    }

    TEST(Lexical_equals_3_case_7)
    {
        WkCocos::Download::Version v1("v128454779843704.128454779843708.128454779843716");
        WkCocos::Download::Version v2("v128454779843704.128454779843708.128454779843716");

        CHECK ( v1 == v2 );
    }

    //not equals
    TEST(Lexical_not_equals_3_case_1)
    {
        WkCocos::Download::Version v1("v0.1.2");
        WkCocos::Download::Version v2("v128454779843704.1.2");

        CHECK ( v1 != v2 );
    }

    TEST(Lexical_not_equals_3_case_2)
    {
        WkCocos::Download::Version v1("v128454779843704.1.2");
        WkCocos::Download::Version v2("v128454779843708.1.2");

        CHECK ( v1 != v2 );
    }

    TEST(Lexical_not_equals_3_case_3)
    {
        WkCocos::Download::Version v1("v1.0.2");
        WkCocos::Download::Version v2("v1.128454779843704.2");

        CHECK ( v1 != v2 );
    }

    TEST(Lexical_not_equals_3_case_4)
    {
        WkCocos::Download::Version v1("v1.128454779843704.2");
        WkCocos::Download::Version v2("v1.128454779843708.2");

        CHECK ( v1 != v2 );
    }

    TEST(Lexical_not_equals_3_case_5)
    {
        WkCocos::Download::Version v1("v1.2.0");
        WkCocos::Download::Version v2("v1.2.128454779843704");

        CHECK ( v1 != v2 );
    }

    TEST(Lexical_not_equals_3_case_6)
    {
        WkCocos::Download::Version v1("v1.2.128454779843704");
        WkCocos::Download::Version v2("v1.2.128454779843708");

        CHECK ( v1 != v2 );
    }

}
