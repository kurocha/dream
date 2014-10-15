#include <UnitTest/UnitTest.hpp>
#include <Dream/Core/Value.h>

namespace Dream
{
	namespace Core
	{
		UnitTest::Suite ValueTestSuite {
			"Dream::Value",

			{"Typed Values",
				[](UnitTest::Examiner & examiner) {
					TypedValue<int> int_value (10);

					examiner << "Typeinfo is same";
					examiner.check(int_value.value_type() == typeid(5));
					
					examiner << "Size is same";
					examiner.check(int_value.size() == sizeof(5));

					TypedValue<unsigned> unsigned_value (10);
					ITypedValue * q1 = &int_value;
					ITypedValue * q2 = &unsigned_value;

					examiner << "Values are equal";
					examiner.check(q1->extract<int>() == q2->extract<unsigned>());

					q1->set<int>(5);
					q2->set<unsigned>(5);

					examiner << "Values are equal";
					examiner.check(q1->extract<int>() == q2->extract<unsigned>());

					bool exception_thrown = false;

					try {
						q1->set<bool>(true);
					} catch (ConversionError & err)   {
						exception_thrown = true;
					}

					examiner << "Exception thrown when type is not correct";
					examiner.check(exception_thrown);
				}
			},
			
			{"Generic Values",
				[](UnitTest::Examiner & examiner) {
					Value v1(5), v2(10);
					Value v3;

					examiner << "Value is set correctly";
					examiner.check(v1.extract<int>() == 5);
					
					examiner << "Value is set correctly";
					examiner.check(v2.extract<int>() == 10);

					examiner << "Value is defined";
					examiner.check(v1.defined());
					
					examiner << "Value is defined";
					examiner.check(v2.defined());

					examiner << "Value is undefined";
					examiner.check(!v3.defined());
					
					examiner << "Value is undefined";
					examiner.check(v3.undefined());

					v1.set(5);
					v2.set(5);

					examiner << "Values are equal";
					examiner.check(v1 == v2);
					
					examiner << "Values are not equal";
					examiner.check(!(v1 == v3));
				}
			},

			{"Input and Output",
				[](UnitTest::Examiner & examiner) {
					// We need to set these so that we know we are working with integers.
					Value v1(0), v2(0);
					
					std::stringstream b1, b2;

					b1 << 15;
					b1 >> v1;

					examiner << "Value was parsed correctly";
					examiner.check(v1.extract<int>() == 15);

					b2 << v1;
					b2 >> v2;

					examiner << "Values are equal";
					examiner.check(v1 == v2);
				}
			},

			{"Pointers",
				[](UnitTest::Examiner & examiner) {
					Value v1, v2, v3;
					
					int i = 1;
					float f = 1.0;
					v1.set(&i);
					v2.set(&i);
					v3.set(&f);

					examiner << "Values are equal";
					examiner.check(v1 == v2);
					
					examiner << "Values are not equal";
					examiner.check(!(v2 == v3));
				}
			},
		};
	}
}
