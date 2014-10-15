# Dream

Dream is a cross-platform game development framework built from the ground up to modern and portable. It has the following features:

* Reference counted pointers for resource/memory management.
* Event driven networking and rendering engine.
* Vector, Matrix and Quaternion implementation for mathematics.
* Sphere, Line, Plane, Frustrum, AlignedBox, Triangle for geometry calculations.
* Robust message based network framework.
* Loading and saving images (PNG, JPEG) of various channels and bit depths.
* Flexible text rendering using FreeType2.
* Positional audio using using OpenAL and Ogg Vorbis.
* High performance graphics using OpenGL (Support for OpenGL3.2+ and OpenGL2.0ES+)

[![Build Status](https://travis-ci.org/dream-framework/dream.svg?branch=master)](https://travis-ci.org/dream-framework/dream)

## Installation

The dream framework is assembled using [teapot](http://www.kyusu.org) which simplifies cross-platform development and deployment.

## Compatibility

This library has been designed for cross-platform compilation. It currently has support for the following targets:

* Mac OS X [Full]
* iPhone [Full]
* Linux [Partial]
* Android NDK [Partial]
* Windows [Unsupported at this time]

## Contributing

1. Fork it
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Add some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create new Pull Request

### Desired Features

* Best possible out of the box support for major platforms including iOS, Android, Windows, Mac OS X and Linux.
* More unit tests for existing functionality.
* Support latest OpenGL functionality.

## License

Released under the MIT license.

Copyright, 2006, 2012, 2014, by [Samuel G. D. Williams](http://www.codeotaku.com/samuel-williams).

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.