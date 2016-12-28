--TEST--
Test the handling of non UTF-8 CSS strings
--FILE--
<?php

try {
	(new \CSS3Processor())->dump(html_entity_decode("k&ouml;ln", ENT_COMPAT, 'ISO-8859-1'));
} catch (Exception $ex) {
	var_dump($ex->getMessage());
}

try {
	(new \CSS3Processor())->minify(html_entity_decode("k&ouml;ln", ENT_COMPAT, 'ISO-8859-1'));
} catch (Exception $ex) {
	var_dump($ex->getMessage());
}

?>
===DONE===
--EXPECT--
string(49) "extcss3: Invalid or corrupt UTF-8 string detected"
string(49) "extcss3: Invalid or corrupt UTF-8 string detected"
===DONE===
