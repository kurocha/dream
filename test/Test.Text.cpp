
#include <UnitTest/UnitTest.h>

#include <Dream/Text/TextBuffer.h>
#include <Dream/Resources/Loader.h>
#include <Dream/Core/Path.h>

namespace Dream
{
	namespace Text
	{
		UnitTest::Suite ImageTestSuite {
			"Dream::Text::Image",

			{"Text Rasterization",
				[](UnitTest::Examiner & examiner) {
					using namespace Dream::Events::Logging;

					Ref<Resources::Loader> loader = new Resources::Loader();
					loader->add_loader(new Font::Loader);

					Ref<Font> font = loader->load<Font>("../share/dream/fonts/averia/Averia-Regular.ttf");

					font->set_pixel_size(32);

					Ref<TextBuffer> text_buffer = new TextBuffer(font);
					text_buffer->append_line("The quick brown fox jumped over the lazy dog!");
					text_buffer->append_line("A zoo keeper lost an elephant in the jungle?");

					bool regenerated = false;
					Ref<Image> image = text_buffer->render_text(regenerated);

					examiner << "Font cache was regenerated." << std::endl;
					examiner.check(regenerated);

					Ref<IData> data = Image::save_to_data(image);
					//data->buffer()->write_to_file("test.png");
					
					examiner << "Rasterised text was composited correctly." << std::endl;
					examiner.check_equal(data->buffer()->checksum(), 808266);
				}
			},
		};
	}
}
