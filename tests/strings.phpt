--TEST--
Test the handling of <string> tokens
--FILE--
<?php

$oProcessor = new \CSS3Processor();

var_dump($oProcessor->dump("'valid string'"));
var_dump($oProcessor->dump('"valid string"'));
var_dump($oProcessor->dump("'valid string\\"));
var_dump($oProcessor->dump('"valid string\\'));

var_dump($oProcessor->dump("'valid string without ending code point"));
var_dump($oProcessor->dump('"valid string without ending code point'));
var_dump($oProcessor->dump("'valid multiline\\
text'"));
var_dump($oProcessor->dump('"valid multiline\\
text"'));

var_dump($oProcessor->dump("'bad string + \\n +
something' + valid string"));
var_dump($oProcessor->dump('"bad string + \n +
something" + valid string'));

var_dump($oProcessor->dump("'string with escaped \\xEF\\xBF\\xBD'"));
var_dump($oProcessor->dump('"string with escaped \xEF\xBF\xBD"'));

var_dump($oProcessor->dump("'string without replacements: \\0020AC \\20ACxy MAX:\\10FFFF BEFORE-SURROGATE:\\D7FF AFTER-SURROGATE:\\E000'"));
var_dump($oProcessor->dump('"string without replacements: \\0020AC \\20ACxy MAX:\\10FFFF BEFORE-SURROGATE:\\D7FF AFTER-SURROGATE:\\E000"'));
var_dump($oProcessor->dump("'string with replacements: \\0x \\0  \\00  \\000  \\0000  \\00000  \\000000  \\0000000 OVERFLOW:\\1A0000  SURR-FIRST:\\D800  SURR-LAST:\\DFFF'"));
var_dump($oProcessor->dump('"string with replacements: \\0x \\0  \\00  \\000  \\0000  \\00000  \\000000  \\0000000 OVERFLOW:\\1A0000  SURR-FIRST:\\D800  SURR-LAST:\\DFFF"'));

var_dump($oProcessor->dump("'\\0\\0\\0\\0\\0'"));
var_dump($oProcessor->dump('"\\0\\0\\0\\0\\0"'));
var_dump($oProcessor->dump('"\\000000\\000000\\000000\\000000\\000000"'));
var_dump($oProcessor->dump("'\\000000\\000000\\000000\\000000\\000000'"));

var_dump($oProcessor->dump("'\0\0\0\0\0'"));

?>
===DONE===
--EXPECT--
string(14) "'valid string'"
string(14) ""valid string""
string(14) "'valid string\"
string(14) ""valid string\"
string(39) "'valid string without ending code point"
string(39) ""valid string without ending code point"
string(23) "'valid multiline\
text'"
string(23) ""valid multiline\
text""
string(44) "'bad string + \n +
something' + valid string"
string(44) ""bad string + \n +
something" + valid string"
string(34) "'string with escaped \xEF\xBF\xBD'"
string(34) ""string with escaped \xEF\xBF\xBD""
string(103) "'string without replacements: \0020AC \20ACxy MAX:\10FFFF BEFORE-SURROGATE:\D7FF AFTER-SURROGATE:\E000'"
string(103) ""string without replacements: \0020AC \20ACxy MAX:\10FFFF BEFORE-SURROGATE:\D7FF AFTER-SURROGATE:\E000""
string(103) "'string with replacements: �x � � � � � � �0 OVERFLOW:� SURR-FIRST:� SURR-LAST:�'"
string(103) ""string with replacements: �x � � � � � � �0 OVERFLOW:� SURR-FIRST:� SURR-LAST:�""
string(17) "'�����'"
string(17) ""�����""
string(17) ""�����""
string(17) "'�����'"
string(17) "'�����'"
===DONE===
