
#include <UnitTest/UnitTest.hpp>
#include <Dream/Core/Strings.hpp>

namespace Dream
{
	namespace Core
	{
		UnitTest::Suite StringsTestSuite {
			"Dream::Core::Strings",

			{"Formatting",
				[](UnitTest::Examiner & examiner) {
					examiner << "String is centered";
					examiner.check(center("Apple", 10, ' ') == "   Apple  ");
					
					examiner << "String is centered";
					examiner.check(center("Apple", 11, ' ') == "   Apple   ");
					
					examiner << "String is centered";
					examiner.check(center("Apple", 11, '-') == "---Apple---");
				}
			},
		};
	}
}
