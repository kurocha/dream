
#include <UnitTest/UnitTest.hpp>
#include <Dream/Assertion.hpp>

namespace Dream
{
	UnitTest::Suite AssertionTestSuite {
		"Dream::Assertion",

		{"Throw Exception",
			[](UnitTest::Examiner & examiner) {
				bool exception_thrown = false;

				exception_thrown = false;
				try {
					DREAM_ASSERT(1 == 0);
				} catch (AssertionError & err)   {
					exception_thrown = true;
				}

				examiner << "Assertion should fail";
				examiner.check(exception_thrown);

				exception_thrown = false;
				try {
					DREAM_ASSERT(1 == 1);
				} catch (AssertionError & err)   {
					exception_thrown = true;
				}

				examiner << "Assertion was okay";
				examiner.check(!exception_thrown);
			}
		},
	};
}
