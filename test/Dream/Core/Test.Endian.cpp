
#include <UnitTest/UnitTest.hpp>
#include <Dream/Core/Endian.hpp>

namespace Dream
{
	namespace Core
	{
		
		template <Endian store_t>
		struct OrderedTestStruct {
			Ordered<uint32_t, store_t> value;
		};
		
		UnitTest::Suite EndianTestSuite {
			"Dream::Core::Endian",

			{"Conversions",
				[](UnitTest::Examiner & examiner) {
					uint32_t a, b, c;
					uint64_t d, e;

					Endian other_endian = LITTLE;

					if (host_endian() == LITTLE) {
						other_endian = BIG;
					}

					//testing("Endian Conversions");

					// 32-bit value
					a = 0x10203040;

					order_copy(a, b, host_endian(), other_endian);
					examiner << "32-bit endian conversion is correct";
					examiner.check(b == 0x40302010);

					a = 100;
					order_copy(a, b, host_endian(), other_endian);
					order_copy(b, c, other_endian, host_endian());
					examiner << "Converted values are the same";
					examiner.check(a == c);

					endian_decode(b, other_endian, host_endian());
					examiner << "Correctly converted in-place";
					examiner.check(a == b);

					order_copy(a, b, LITTLE, LITTLE);
					examiner << "Values are not converted when endian is the same";
					examiner.check(a == b);

					order_copy(a, b, BIG, BIG);
					examiner << "Values are not converted when endian is the same";
					examiner.check(a == b);

					order_copy(a, b, LITTLE, BIG);
					examiner << "Values are not converted when endian is the same";
					examiner.check(a != b);

					d = 0x1020304050607080ull;
					order_copy(d, e, host_endian(), other_endian);
					examiner << "64-bit endian conversion is correct";
					examiner.check(e == 0x8070605040302010ull);

					//testing("Ordered Structs");

					OrderedTestStruct<LITTLE> ls;
					OrderedTestStruct<BIG> bs;

					ls.value = 0x10203040;
					bs.value = 0x10203040;

					if (host_endian() == LITTLE) {
						examiner << "Value is stored in native order";
						examiner.check(ls.value.base == 0x10203040);
						examiner << "Value is stored in converted order";
						examiner.check(bs.value.base == 0x40302010);
					} else {
						examiner << "Value is stored in native order";
						examiner.check(bs.value.base == 0x10203040);
						examiner << "Value is stored in converted order";
						examiner.check(ls.value.base == 0x40302010);
					}

					a = ls.value;
					b = bs.value;

					examiner << "Converted values are same";
					examiner.check(a == b);
					examiner << "Equality operator works";
					examiner.check(ls.value == b);
					examiner << "Equality operator works";
					examiner.check(bs.value == a);
				}
			},
		};
	}
}
