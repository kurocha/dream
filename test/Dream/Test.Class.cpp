
#include <UnitTest/UnitTest.hpp>
#include <Dream/Association.hpp>

namespace Dream
{
	UnitTest::Suite ClassTestSuite {
		"Dream::Class",

		{"Object Cache",
			[](UnitTest::Examiner & examiner) {
				Ref<ObjectCache<Object>> object_cache = new ObjectCache<Object>;

				{
					Ref<Object> key = new Object;
					Ref<Object> value = new Object;

					object_cache->set(key, value);

					examiner << "Object was found in cache";
					examiner.check(object_cache->lookup(key) == value);
					
					examiner << "Cache size is correct";
					examiner.check(object_cache->objects().size() == 1);

					examiner << "Key is referenced locally only";
					examiner.check(key->reference_count() == 1);
					
					examiner << "Value is referenced locally and in cache";
					examiner.check(value->reference_count() == 2);

					key = nullptr;

					examiner << "Value has been purged from cache";
					examiner.check(value->reference_count() == 1);

					value = nullptr;
				}

				// Now that we are out of the scope where key exists, the object has been removed from the cache, since key was automatically deleted.
				examiner << "Object has been removed";
				examiner.check(object_cache->objects().size() == 0);
			}
		},
	};
}
