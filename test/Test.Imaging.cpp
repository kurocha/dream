
#include <UnitTest/UnitTest.h>

#include <Dream/Imaging/Image.h>
#include <Euclid/Numerics/Vector.IO.h>

#include <cstring>

namespace Dream
{
	namespace Imaging
	{
		UnitTest::Suite ImageTestSuite {
			"Dream::Imaging::Image",

			{"Image Reader/Writer",
				[](UnitTest::Examiner & examiner) {
					Ref<Image> image = new Image({8, 8}, PixelFormat::L, DataType::BYTE);

					examiner << "Image data was allocated" << std::endl;
					examiner.check_equal(image->buffer().size(), 8*8);

					image->fill(0xFF);

					examiner << "Image was filled with white" << std::endl;
					auto pixels = reader(*image);

					Vector<3, ByteT> pixel = pixels[Vec2(0, 0)], white(255, 255, 255);

					examiner.check_equal(pixel, white);

					Ref<Image> small_black_image = new Image({4, 4}, PixelFormat::L, DataType::BYTE);
					small_black_image->fill(0);

					examiner << "Copy black image into corner of white image" << std::endl;
					writer(*image).copy(reader(*small_black_image), 0, 0, small_black_image->size());

					const unsigned char black_row[] = {0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF};
					const unsigned char white_row[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

					for (std::size_t i = 0; i < 4; i += 1)
						examiner.check_equal(std::memcmp(pixels[Vec2(0, i)], black_row, 8), 0);

					for (std::size_t i = 4; i < 8; i += 1)
						examiner.check_equal(std::memcmp(pixels[Vec2(0, i)], white_row, 8), 0);
				}
			},

			{"Pixel Writer",
				[](UnitTest::Examiner & examiner) {
					Ref<Image> image = new Image({8, 8}, PixelFormat::RGB, DataType::BYTE);

					examiner << "Image data was allocated" << std::endl;
					examiner.check_equal(image->buffer().size(), 8*8*3);

					image->fill(0xFF);

					Vector<3, ByteT> color {0x11, 0x22, 0x33};
					writer(*image).set({0, 0}, color);

					Vector<3, ByteT> output;
					reader(*image).get<3>({0, 0}, output);

					examiner << "Check the colour is correct" << std::endl;
					examiner.check_equal(output, color);
				}
			}
		};
	}
}
