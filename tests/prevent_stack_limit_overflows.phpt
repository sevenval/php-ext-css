--TEST--
Test the prevention of the stack limit overflows
--FILE--
<?php

$oProcessor = new \CSS3Processor();

// Rulesets and Tokens
$sCSS = str_repeat('a { x: y }', 500000);
var_dump(strlen($oProcessor->minify($sCSS)));

// Contexts
$sCSS = 'a ' . str_repeat('{', 1000000);
var_dump(strlen($oProcessor->minify($sCSS)));

// Declarations
$sCSS = 'a { ' . str_repeat('x:y;', 1000000) . ' }';
var_dump(strlen($oProcessor->minify($sCSS)));

?>
===DONE===
--EXPECT--
int(3000000)
int(0)
int(4000002)
===DONE===
