
#include <UnitTest/UnitTest.hpp>
#include <Dream/Reference.hpp>

#include <set>

namespace Dream
{
	UnitTest::Suite ReferenceTestSuite {
		"Dream::Reference",

		{"Shared Object",
			[](UnitTest::Examiner & examiner) {
				Ref<SharedObject> s1, s2, s3;

				examiner << "Null objects are equal";
				examiner.check(s1 == s2);

				s1 = new SharedObject;

				examiner << "Null and non-null objects are not equal";
				examiner.check(s1 != s2);

				s2 = new SharedObject;
				s3 = new SharedObject;

				examiner << "Objects are equal";
				examiner.check(s1 == s1);
				
				examiner << "Objects are not equal";
				examiner.check(s1 != s2);

				std::set<Ref<SharedObject>> objects;
				objects.insert(s1);
				objects.insert(s2);
				objects.insert(s3);

				examiner << "Set contains correct number of objects";
				examiner.check(objects.size() == 3);
			}
		},
		
		{"Shared Values",
			[](UnitTest::Examiner & examiner) {
				Shared<int> s1(new int), s2, s3;

				s2 = s1;

				*s1 = 10;

				examiner << "Value was not the same!";
				examiner.check(*s2 == 10);
			}
		}
	};
}
