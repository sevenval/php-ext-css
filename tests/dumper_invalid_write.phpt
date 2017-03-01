--TEST--
Test invalid write (buffer overflow)
--FILE--
<?php

$oProcessor = new \CSS3Processor();
$oProcessor->setModifier(CSS3Processor::TYPE_URL, function(array $aData)
{
		return str_pad($aData['value'], 1024 * 1024, 'x');
});

$sCSS = 'a{x:url(x)} b{x:url(y)} c{x:url(z)}';

var_dump(strlen($sCSS));
var_dump(strlen($oProcessor->dump($sCSS)));

/*
 * 1024 * 1024 * 3 - 3	= 3145725
 *						  +
 * strlen($sCSS)		= 35
 * ==============================
 *						= 3145760
 */

?>
===DONE===
--EXPECT--
int(35)
int(3145760)
===DONE===
