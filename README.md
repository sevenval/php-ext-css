# `extcss3`

[![Software license][ico-license]](LICENSE)
[![Build status][ico-travis]][link-travis]

`extcss3` is a fast PHP7 extension for the handling of CSS3 strings (see
[W3C Candidate Recommendation](https://www.w3.org/TR/css-syntax-3/)). It supports
preprocessing, tokenizing and minifying. Furthermore, `extcss3` implements an API
that can be used to analyse, augment or correct the style sheet while it is being
processed.

## Library Features

* Written in pure C99.
* Doesn't require any external libraries.
* Implements an intervention API.
* PHP 7, PHP 7.1 and PHP 7.2 ready.

## Minifying Features

* Removes unnecessary whitespace, comments and semicolons.
* Removes invalid or empty declarations and qualified rules.
* Color name transformations (e.g. `MediumSpringGreen` to `#00FA9A`).
* Hexadecimal color transformations (e.g. `#FF0000` to `red`).
* Function transformations (e.g. `rgb(255, 255, 255)` to `#FFF`).
* Minifying of numeric values (e.g. `005` to `5` or `0.1em` to `.1em`).
* Optional: Removal of vendor-prefixed declarations.

## Current Limitations

* The CSS string must be UTF-8 encoded.
* String and URL tokens are returned to the registered callbacks with quotes ("xxx"
  or 'xxx') as given in the original CSS string.
* The code compiles and runs on Linux systems. Other platforms have not been tested.

## PHP Class (`CSS3Processor`)

### CSS3Processor::__construct()

* Constructs a new `CSS3Processor` object.
* Throws exceptions on errors.

```php
public CSS3Processor::__construct(void) : CSS3Processor
```

### CSS3Processor::setModifier()

* Registers a callback for the given token `$type` that will be called during tokenisation.
  The callback gets an info array of the current token and context.
* The callback should return a string with a (new) value that replaces the given
  token in the result string. If the return value is not a string, the original
  value is left unmodified.
* The parameter `$type` can be set to any of the `extcss3` Type Constants listed
  below that are marked as `modifiable`.
* Only one modifier callback per token type is possible. Any subsequent call to
  `::setModifier()` replaces previously set callbacks for the same type.
* Returns `true` on success.
* Throws exceptions on errors.

```php
public CSS3Processor::setModifier(int $type, callable $callback) : bool
```

### CSS3Processor::dump()

* Applies preprocessing and the registered modifiers to `$css` and returns the
  resulting string.
* Throws exceptions on errors.

```php
public CSS3Processor::dump(string $css) : string
```

### CSS3Processor::minify()

* Returns the minimized result string considering the registered modifiers and the
  blacklist of vendor prefixes given in the `$vendors` array.
* Throws exceptions on errors.

```php
public CSS3Processor::minify(string $css [, array $vendors ]) : string
```

### PHP Class Constants

#### Type Constants

<pre>
* `CSS3Processor::TYPE_IDENT`               1
* `CSS3Processor::TYPE_FUNCTION`            2
* `CSS3Processor::TYPE_AT_KEYWORD`          3
* `CSS3Processor::TYPE_HASH`                4
* `CSS3Processor::TYPE_STRING`              5 (modifiable)
* `CSS3Processor::TYPE_BAD_STRING`          6 (modifiable)
* `CSS3Processor::TYPE_URL`                 7 (modifiable)
* `CSS3Processor::TYPE_BAD_URL`             8 (modifiable)
* `CSS3Processor::TYPE_DELIM`               9
* `CSS3Processor::TYPE_NUMBER`             10
* `CSS3Processor::TYPE_PERCENTAGE`         11
* `CSS3Processor::TYPE_DIMENSION`          12
* `CSS3Processor::TYPE_UNICODE_RANGE`      13
* `CSS3Processor::TYPE_INCLUDE_MATCH`      14
* `CSS3Processor::TYPE_DASH_MATCH`         15
* `CSS3Processor::TYPE_PREFIX_MATCH`       16
* `CSS3Processor::TYPE_SUFFIX_MATCH`       17
* `CSS3Processor::TYPE_SUBSTR_MATCH`       18
* `CSS3Processor::TYPE_COLUMN`             19
* `CSS3Processor::TYPE_WS`                 20
* `CSS3Processor::TYPE_CDO`                21
* `CSS3Processor::TYPE_CDC`                22
* `CSS3Processor::TYPE_COLON`              23
* `CSS3Processor::TYPE_SEMICOLON`          24
* `CSS3Processor::TYPE_COMMA`              25
* `CSS3Processor::TYPE_BR_RO`              26
* `CSS3Processor::TYPE_BR_RC`              27
* `CSS3Processor::TYPE_BR_SO`              28
* `CSS3Processor::TYPE_BR_SC`              29
* `CSS3Processor::TYPE_BR_CO`              30
* `CSS3Processor::TYPE_BR_CC`              31
* `CSS3Processor::TYPE_COMMENT`            32 (modifiable)
* `CSS3Processor::TYPE_EOF`                33
</pre>

#### Flag Constants

<pre>
* `CSS3Processor::FLAG_ID`                  1
* `CSS3Processor::FLAG_UNRESTRICTED`        2
* `CSS3Processor::FLAG_INTEGER`             3
* `CSS3Processor::FLAG_NUMBER`              4
* `CSS3Processor::FLAG_STRING`              5
* `CSS3Processor::FLAG_AT_URL_STRING`       6
</pre>

#### Exception Code Constants

<pre>
* `CSS3Processor::ERR_MEMORY`               1
* `CSS3Processor::ERR_BYTES_CORRUPTION`     2
* `CSS3Processor::ERR_NULL_PTR`             3
* `CSS3Processor::ERR_INV_PARAM`            4
</pre>

### Examples

```php
$css = file_get_contents('style.css');

try {
    $proc = new \CSS3Processor();

    $min = $proc->minify($css, ['-ms', '-moz', '-o']);
} catch (Exception $e) {
    ...
}
```

See the tests for more examples.

[ico-license]: https://img.shields.io/github/license/mashape/apistatus.svg
[ico-travis]: https://travis-ci.org/sevenval/php-ext-css.svg?branch=master
[link-travis]: https://travis-ci.org/sevenval/php-ext-css
