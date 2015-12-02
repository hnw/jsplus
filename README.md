# jsplus [![Build Status](https://travis-ci.org/hnw/jsplus.svg?branch=master)](https://travis-ci.org/hnw/jsplus)

A PHP extension which changes behavior of '+' operator as string concatetation when either 1 operand is a string (like JavaScript).

# Attention

This extension is JUST experimental. Do NOT use on production environment.

# Requirement

- PHP 7.0+

# Installation

```
git clone https://github.com/hnw/jsplus.git
cd jsplus
phpize
./configure
make
make install
```

## Ini settings

### jsplus.enabled

`jsplus.enabled` can be set to `0` to disable jsplus extension. (default: `1`)

## Example

```php
<?php
$abc="abc";
$twelve=12;
var_dump($abc+"def");   // string(6) "abcdef"
var_dump($twelve+"34"); // string(4) "1234"
var_dump($twelve+34);   // int(46)
```

## License

The MIT License

Copyright (c) 2015 Yoshio HANAWA

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
