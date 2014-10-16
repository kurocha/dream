
#include <UnitTest/UnitTest.hpp>
#include <Dream/Core/URI.hpp>

namespace Dream
{
	namespace Core
	{
		UnitTest::Suite URITestSuite {
			"Dream::Core::URI",

			{"Parsing HTTP URIs",
				[](UnitTest::Examiner & examiner) {
					URI a("http://user12:abc@www.google.com:80/blah?bob=2");
					examiner << "Scheme is correct";
					examiner.check(a.scheme() == "http");

					examiner << "User is correct";
					examiner.check(a.username() == "user12");
					
					examiner << "Password is correct";
					examiner.check(a.password() == "abc");

					examiner << "Hostname is correct";
					examiner.check(a.hostname() == "www.google.com");
					
					examiner << "Port is correct";
					examiner.check(a.port() == 80);
					
					examiner << "Path is correct";
					examiner.check(a.path() == "/blah");
					
					examiner << "Query is correct";
					examiner.check(a.query() == "?bob=2");

					URI a2("http://localhost");
					
					examiner << "scheme " << a2.scheme() << " should be 'http'";
					examiner.check(a2.scheme() == "http");
					
					examiner << "hostname " << a2.hostname() << " should be 'localhost'";
					examiner.check(a2.hostname() == "localhost");
				}
			},
			
			{"Parsing Mailto URIs",
				[](UnitTest::Examiner & examiner) {
					URI b("mailto:blah@blah.com");
					
					examiner << "Scheme is correct";
					examiner.check(b.scheme() == "mailto");
					
					examiner << "Path is correct";
					examiner.check(b.path() == "blah@blah.com");
				}
			},
			
			{"Parsing File URIs",
				[](UnitTest::Examiner & examiner) {
					//testing("File URI");
					URI c("file:/etc/fstab");
					
					examiner << "Scheme is correct";
					examiner.check(c.scheme() == "file");
					
					examiner << "Path is correct";
					examiner.check(c.path() == "/etc/fstab");

					//testing("Simple File URI");
					URI d("/etc/fstab");
					
					examiner << "Scheme is correct";
					examiner.check(d.scheme() == "");
					
					examiner << "Path is correct";
					examiner.check(d.path() == "/etc/fstab");

					//testing("Path Constructed URI");
					URI e("file", "/Apples/and/oranges");
					
					examiner << "Scheme is correct";
					examiner.check(e.scheme() == "file");
					
					examiner << "Path is correct";
					examiner.check(e.path() == "/Apples/and/oranges");
				}
			},
		};
	}
}
