
#include <UnitTest/UnitTest.hpp>
#include <Dream/Core/Dictionary.h>

namespace Dream
{
	namespace Core
	{
		UnitTest::Suite DictionaryTestSuite {
			"Dream::Association",

			{"Construction",
				[](UnitTest::Examiner & examiner) {
					//testing("Serialization");

					Ref<Dictionary> dict1 = new Dictionary;

					dict1->set("Key1", 5);
					dict1->set("Key2", "Apples");

					int key1 = dict1->get<int>("Key1");

					examiner << "Value is equal";
					examiner.check(key1 == 5);

					Ref<IData> data = dict1->serialize();

					Ref<Dictionary> dict2(new Dictionary);

					dict2->deserialize(data);

					examiner << "Values are equal";
					examiner.check(dict1->get<int>("Key1") == dict2->get<int>("Key1"));

					dict1->set("Dictionary Name", "dict1");

					std::cout << "Dict 1" << std::endl;
					dict1->debug(std::cout);

					std::cout << "Dict 2" << std::endl;
					dict2->debug(std::cout);
				}
			},
		};
	}
}
