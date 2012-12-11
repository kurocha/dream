
#include <UnitTest/UnitTest.h>

#include <Dream/Text/TextBuffer.h>

namespace Dream
{
	namespace Text
	{
		UnitTest::Suite ImageTestSuite {
			"Dream::Text::Image",

			{"Text Rasterization",
				[](UnitTest::Examiner & examiner) {
					Ref<Font> font = new Font("averia/Averia-Regular.ttf");

					font->set_pixel_size(32);

					Ref<TextBuffer> text_buffer = new TextBuffer(font);
					text_buffer->append_line("The quick brown fox jumped over the lazy dog!");
					text_buffer->append_line("A zoo keeper lost an elephant in the jungle?");

					bool regenerated = false;
					Ref<Image> image = text_buffer->render_text(regenerated);

					Ref<IData> data = Image::save_to_data(image);
					data->buffer()->write_to_file("test.png");
				}
			},
		};
	}
}
