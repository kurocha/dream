
#include <UnitTest/UnitTest.hpp>
#include <Dream/Association.hpp>

namespace Dream
{
	class Foo : public Object
	{
	public:
		int x;
	};

	UnitTest::Suite AssociationTestSuite {
		"Dream::Association",

		{"Construction",
			[](UnitTest::Examiner & examiner) {
				Association<Foo> weak_foo;

				{
					Reference<Foo> foo(new Foo);
					foo->x = 10;

					weak_foo = foo;

					examiner << "Weak foo is valid" << std::endl;
					examiner.check(weak_foo);
				}

				examiner << "Weak foo is invalid as foo has gone out of scope" << std::endl;
				examiner.check(!weak_foo);
			}
		},

		{"Locking",
			[](UnitTest::Examiner & examiner) {
				Association<Foo> weak_foo;

				Reference<Foo> foo = new Foo;
				foo->x = 20;

				weak_foo = foo;

				// Make a new reference to the weak foo.
				Reference<Foo> other_foo;
				other_foo = weak_foo;

				foo.clear();

				examiner << "Foo has been cleared" << std::endl;
				examiner.check(!foo);

				examiner << "Other foo was assigned" << std::endl;
				examiner.check(other_foo);

				examiner << "Weak foo is still valid" << std::endl;
				examiner.check(weak_foo);

				examiner << "Value of x is 20" << std::endl;
				examiner.check_equal(other_foo->x, 20);
			}
		},
	};
}
