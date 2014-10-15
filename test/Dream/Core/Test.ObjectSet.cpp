
#include <UnitTest/UnitTest.hpp>
#include <Dream/Core/ObjectSet.h>

namespace Dream
{
	namespace Core
	{
		class TestResource : public Object {
		};
		
		UnitTest::Suite ObjectSetTestSuite {
			"Dream::Core::ObjectSet",

			{"Object insertion",
				[](UnitTest::Examiner & examiner) {
					Ref<ObjectSet> objects(new ObjectSet);

					ObjectSet::ObjectID oid1 = objects->insert(new TestResource);
					ObjectSet::ObjectID oid2 = objects->insert(new TestResource);
					ObjectSet::ObjectID oid3 = objects->insert(new TestResource);
					ObjectSet::ObjectID oid4 = objects->insert(new TestResource);

					examiner << "Object set contains 4 objects";
					examiner.check(objects->size() == 4);

					objects->erase(oid3);

					examiner << "Object set contains 3 objects";
					examiner.check(objects->size() == 3);

					ObjectSet::ObjectID oid5 = objects->insert(new TestResource);

					examiner << "Object set contains 4 objects";
					examiner.check(objects->size() == 4);
					
					examiner << "Object was placed into previous slot";
					examiner.check(oid3.identity() == oid5.identity());
				}
			},
		};
	}
}
