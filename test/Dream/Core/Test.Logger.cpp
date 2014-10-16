
#include <UnitTest/UnitTest.hpp>
#include <Dream/Logger.hpp>

namespace Dream
{
	namespace Core
	{
		UnitTest::Suite LoggerTestSuite {
			"Dream::Logger",

			{"Output",
				[](UnitTest::Examiner & examiner) {
					using namespace Dream::Core::Logging;
					
					console()->set_thread_name("Unit Tests");
					
					log("The console output is allocated:", console());
					
					examiner.check(console());
				}
			},
		};
	}
}
