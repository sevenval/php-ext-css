--TEST--
Test CSS3Processor::minify() method with color declarations
--FILE--
<?php

$oProcessor = new \CSS3Processor();

$sCSS = '
x {
	boder-color: red red red black;
}
';
var_dump($oProcessor->minify($sCSS));

?>
===DONE===
--EXPECT--
string(31) "x{boder-color:red red red #000}"
===DONE===
