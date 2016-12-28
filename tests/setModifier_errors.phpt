--TEST--
Test CSS3Processor::setModifier() method (negative)
--FILE--
<?php

$oProcessor = new \CSS3Processor();

try {
	var_dump($oProcessor->setModifier(CSS3Processor::TYPE_FUNCTION, [$oProcessor, 'setModifier']));
} catch (Exception $e) {
	var_dump($e->getMessage());
}

try {
	var_dump($oProcessor->setModifier(CSS3Processor::TYPE_NUMBER, [$oProcessor, 'setModifier']));
} catch (Exception $e) {
	var_dump($e->getMessage());
}

try {
	var_dump($oProcessor->setModifier(12345, [$oProcessor, 'setModifier']));
} catch (Exception $e) {
	var_dump($e->getMessage());
}

?>
===DONE===
--EXPECT--
string(52) "Setting the modifier for an unmodifiable type failed"
string(52) "Setting the modifier for an unmodifiable type failed"
string(52) "Setting the modifier for an unmodifiable type failed"
===DONE===

