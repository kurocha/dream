
#include <UnitTest/UnitTest.hpp>
#include <Dream/Core/Data.hpp>

#include <string.h>

namespace Dream
{
	namespace Core
	{
		UnitTest::Suite DataTestSuite {
			"Dream::Core::Data",

			{"Construction",
				[](UnitTest::Examiner & examiner) {
					const char * data = "First things first -- but not necessarily in that order.";

					//testing("Construction");

					Shared<StaticBuffer> sb = new StaticBuffer(StaticBuffer::for_cstring(data, false));
					Ref<IData> a = new BufferedData(sb);

					examiner << "Data length is correct";
					examiner.check(a->buffer()->size() == strlen(data));
				}
			},
		};
	}
}
