--TEST--
Test CSS3Processor::dump() method (empty string)
--FILE--
<?php

var_dump((new \CSS3Processor())->dump(''));

?>
===DONE===
--EXPECT--
string(0) ""
===DONE===
