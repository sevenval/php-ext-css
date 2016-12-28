--TEST--
Test the preprocessing
--FILE--
<?php

$oProcessor = new \CSS3Processor();

var_dump($oProcessor->dump("a -\0- b-\r\n-x-\f-y-\r-z"));

?>
===DONE===
--EXPECT--
string(21) "a -�- b-
-x-
-y-
-z"
===DONE===
