
#include <UnitTest/UnitTest.hpp>
#include <Dream/Core/Buffer.hpp>

#include <string.h>

namespace Dream
{
	namespace Core
	{
		UnitTest::Suite BufferTestSuite {
			"Dream::Core::Buffer",

			{"StaticBuffer",
				[](UnitTest::Examiner & examiner) {
					StaticBuffer buf = StaticBuffer::for_cstring("Bobby");

					uint8_t v;
					buf.read(2, v);

					examiner << "Read value is equal";
					examiner.check_equal(v, 'b');
				}
			},
			
			{"DynamicBuffer",
				[](UnitTest::Examiner & examiner) {
					const char * data = "Human resources are human first, and resources second.";
					unsigned data_length = strlen(data);

					//testing("Construction");

					DynamicBuffer a(100), b;

					examiner << "Size set by constructor";
					examiner.check(a.size() == 100);
					examiner << "Size set by constructor";
					examiner.check(b.size() == 0);

					examiner << "Sized construtor is not empty";
					examiner.check(!a.empty());
					examiner << "Default construtor is empty";
					examiner.check(b.empty());

					examiner << "Capacity >= size";
					examiner.check(a.capacity() >= a.size());
					examiner << "Capacity >= size";
					examiner.check(b.capacity() >= b.size());

					//testing("Assigning Data");

					std::size_t prev_capacity = a.capacity();
					a.resize(data_length);
					a.assign((const ByteT*)data, (const ByteT*)data + data_length);

					// Performance check
					examiner << "Don't realloc if size is within capacity";
					examiner.check(a.capacity() == prev_capacity);

					b.resize(data_length);
					b.assign((const ByteT*)data, (const ByteT*)data + data_length);

					examiner << "Data and size is the same";
					examiner.check(a == b);
					examiner << "Data indexing is correct";
					examiner.check(a[5] == data[5]);

					//testing("Clearing buffers");

					a.clear();
					examiner << "Cleared buffer is empty";
					examiner.check(a.empty());

					examiner << "Buffers are different after being cleared";
					examiner.check(a != b);

					b.clear();
					examiner << "Cleared buffers are equivalent";
					examiner.check(a == b);

					//testing("Capacity");

					a.reserve(200);
					examiner << "Reserved capacity for 200 bytes";
					examiner.check(a.capacity() == 200);

					b.reserve(400);
					examiner << "Reserved capacity for 400 bytes";
					examiner.check(b.capacity() == 400);

					a.resize(600);
					examiner << "Size increase was successful";
					examiner.check(a.size() == 600);
					examiner << "Capacity increased after size increase";
					examiner.check(a.capacity() >= 600);

					a.expand(100);

					examiner << "Size expansion was successful";
					examiner.check(a.size() == 700);

					//testing("Appending");

					a.resize(0);
					a.append(5, (const ByteT *)"abcde");
					a.append(5, (const ByteT *)"abcde");

					examiner << "Size is correct after appending 10 characters";
					examiner.check(a.size() == 10);
					examiner << "Character is correct";
					examiner.check(a[1] == 'b');
				}
			},
			
			{"PackedBuffer",
				[](UnitTest::Examiner & examiner) {
					const char * data = "Packed Buffer.";
					unsigned data_length = strlen(data);

					PackedBuffer * buffer;

					//testing("Construction");

					buffer = PackedBuffer::new_buffer(data_length);
					examiner << "Buffer was created successfully";
					examiner.check(buffer != NULL);

					//testing("Assigning Data");

					buffer->assign((const ByteT*)data, (const ByteT*)data + data_length, 0);
					examiner << "Data size is consistent";
					examiner.check(buffer->size() == data_length);
					
					examiner << "Data is correct";
					for (unsigned i = 0; i < buffer->size(); i += 1) {
						examiner.check((*buffer)[i] == data[i]);
					}
				}
			},
			
			{"Reading and Writing",
				[](UnitTest::Examiner & examiner) {
					Path tmp_path = "junk.dat";
					const char * data = "When the only tool you have is a hammer, you tend to treat everything as if it were a nail.";
					unsigned data_length = strlen(data);

					//testing("Writing");

					PackedBuffer * write_buffer;
					write_buffer = PackedBuffer::new_buffer(data_length);

					write_buffer->assign((const ByteT*)data, (const ByteT*)data + data_length, 0);

					write_buffer->write_to_file(tmp_path);

					//testing("Reading");
					FileBuffer read_buffer(tmp_path);

					std::string write_string(write_buffer->begin(), write_buffer->end());
					std::string read_string(read_buffer.begin(), read_buffer.end());

					examiner << "Data size is consistent";
					examiner.check(write_buffer->size() == read_buffer.size());
					examiner << "Data is correct";
					examiner.check(write_buffer->checksum() == read_buffer.checksum());

					examiner << "Data string is equal";
					examiner.check(write_string == read_string);

					write_buffer->hexdump(std::cout);
					read_buffer.hexdump(std::cout);

					// Remove the temporary file
					//tmp_path.remove();
				}
			}
		};
	}
}
