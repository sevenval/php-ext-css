--TEST--
Test CSS3Processor::__construct() method
--FILE--
<?php

$oProcessor = new \CSS3Processor();

var_dump(get_class($oProcessor));

?>
===DONE===
--EXPECT--
string(13) "CSS3Processor"
===DONE===
