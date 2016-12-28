--TEST--
Test CSS3Processor::minify() method for "@charset", "@import" and "@namespace" tokens
--FILE--
<?php

$oProcessor = new \CSS3Processor();

$aTests = [
	[
		'in' => '@charset "UTF-8"; @import "import.css"; @namespace url(www.example.com);',
		'out' => '@charset "UTF-8";@import"import.css";@namespace url(www.example.com);'
	],
	[
		'in' => ' @charset "UTF-8"; @import "import.css"; @namespace url(www.example.com);',
		'out' => '@import"import.css";@namespace url(www.example.com);'
	],
	[
		'in' => '@import "import.css"; @charset "UTF-8"; @namespace url(www.example.com);',
		'out' => '@import"import.css";@namespace url(www.example.com);'
	],
	[
		'in' => '@namespace url(www.example.com); @charset "UTF-8"; @import "import.css"; @namespace url(www.example.com);',
		'out' => '@namespace url(www.example.com);@namespace url(www.example.com);'
	],
	[
		'in' => '@foo; @charset "UTF-8"; @import "import.css"; @namespace url(www.example.com);',
		'out' => '@foo;'
	],
];

foreach ($aTests as $aTest) {
	if ($oProcessor->minify($aTest['in']) !== $aTest['out']) {
		echo "FAILURE: {$aTest['in']}\n";
	}
}

echo "SUCCESS\n";

?>
===DONE===
--EXPECT--
SUCCESS
===DONE===
