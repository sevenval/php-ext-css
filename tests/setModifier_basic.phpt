--TEST--
Test CSS3Processor::setModifier() method
--FILE--
<?php

$oProcessor = new \CSS3Processor();

var_dump($oProcessor->setModifier(CSS3Processor::TYPE_STRING, [$oProcessor, 'setModifier']));
var_dump($oProcessor->setModifier(CSS3Processor::TYPE_BAD_STRING, [$oProcessor, 'setModifier']));
var_dump($oProcessor->setModifier(CSS3Processor::TYPE_URL,  [$oProcessor, 'setModifier']));
var_dump($oProcessor->setModifier(CSS3Processor::TYPE_BAD_URL,  [$oProcessor, 'setModifier']));
var_dump($oProcessor->setModifier(CSS3Processor::TYPE_COMMENT, [$oProcessor, 'setModifier']));

?>
===DONE===
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
===DONE===
