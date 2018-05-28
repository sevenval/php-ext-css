--TEST--
Test CSS3Processor::setNotifier() method
--FILE--
<?php
class Test_Notifier
{
	public static function A(array $aData)
	{
		var_dump('A: ' . $aData['value']);
	}
	public static function B(array $aData)
	{
		var_dump('B: ' . $aData['value']);
		return 'foo';
	}
}
$oProcessor = new \CSS3Processor();
var_dump($oProcessor->setNotifier(CSS3Processor::TYPE_AT_KEYWORD, ['Test_Notifier', 'A']));
var_dump($oProcessor->setNotifier(CSS3Processor::TYPE_AT_KEYWORD, ['Test_Notifier', 'B']));
$oProcessor->dump('@charset "UTF-8";');
?>
===DONE===
--EXPECT--
bool(true)
bool(true)
string(11) "A: @charset"
string(11) "B: @charset"
===DONE===
