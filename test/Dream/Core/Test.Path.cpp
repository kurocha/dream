
#include <UnitTest/UnitTest.hpp>
#include <UnitTest/Expectation.hpp>
#include <Dream/Core/Path.h>

namespace Dream
{
	namespace Core
	{
		Path p1("/apples/oranges"), p2("grapefruit/"), p3("bananna/peach"), p4("files/image.jpeg"), p5("bob"), p6("/dole");
		
		UnitTest::Suite PathTestSuite {
			"Dream::Core::Path",

			{"Directories",
				[](UnitTest::Examiner & examiner) {
					examiner << p1 << " is absolute";
					examiner.check(p1.is_absolute());
					
					examiner << p2 << " is relative";
					examiner.check(!p2.is_absolute());
					
					examiner << p2 << " is a directory";
					examiner.check(p2.is_directory());
					
					examiner << p3 << " is not a directory";
					examiner.check(!p3.is_directory());
				}
			},
			
			{"Local Paths",
				[](UnitTest::Examiner & examiner) {
					examiner.expect(p1.to_local_path()) == "/apples/oranges";
					
					examiner.expect(p2.to_local_path()) == "grapefruit/";
					
					examiner.expect(p3.to_local_path())== "bananna/peach";
				}
			},
			
			{"Path Manipulation",
				[](UnitTest::Examiner & examiner) {
					Path::NameComponents name_components = p4.last_name_components();
					
					examiner.expect(name_components.basename) == "image";
					examiner.expect(name_components.extension) == "jpeg";

					examiner.expect(p5 + p4) == "bob/files/image.jpeg";

					Path p7 = p1 + "../john.png";

					examiner.expect(p7.simplify()) == "/apples/john.png";
				}
			},
		};
	}
}
