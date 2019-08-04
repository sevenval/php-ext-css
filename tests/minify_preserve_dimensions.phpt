--TEST--
Test CSS3Processor::minify() method (preserving dimensions)
--FILE--
<?php

var_dump((new \CSS3Processor())->minify('div { height: calc(20px  -  (0%  +  0px))  foo  bar; height:  0%; }'));

?>
===DONE===
--EXPECT--
string(53) "div{height:calc(20px - (0% + 0px)) foo bar;height:0%}"
===DONE===
