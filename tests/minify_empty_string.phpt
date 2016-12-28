--TEST--
Test CSS3Processor::minify() method (empty string)
--FILE--
<?php

$oProcessor = new \CSS3Processor();

var_dump($oProcessor->minify(''));
var_dump($oProcessor->minify('', []));
var_dump($oProcessor->minify('', ['--x']));

?>
===DONE===
--EXPECT--
string(0) ""
string(0) ""
string(0) ""
===DONE===
