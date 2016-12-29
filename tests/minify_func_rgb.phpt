--TEST--
Test CSS3Processor::minify() method for CSS rgb() function
--FILE--
<?php

$oProcessor = new \CSS3Processor();

$sCSS = '
valid_rgb {
	color: rgb(0,0,0);
	color: rgb(0,0,0) xyz;
	color: rgb(0,0,0)xyz;

	color: rgb(100%,0%,0%);
	color: rgb(100%,0%,0%) xyz;
	color: rgb(100%,0%,0%)xyz;

	color: rgb( 1 , 2 , 3 );
	color: rgb( 1 , 2 , 3 ) xyz;
	color: rgb( 1 , 2 , 3 )xyz;

	color: rgb( /* 1 */ 255 /* 2 */ , /* 3 */ 0 /* 4 */ , /* 5 */ 0 /* 6 */ );
	color: rgb( /* 1 */ 255 /* 2 */ , /* 3 */ 0 /* 4 */ , /* 5 */ 0 /* 6 */ ) xyz;
	color: rgb( /* 1 */ 255 /* 2 */ , /* 3 */ 0 /* 4 */ , /* 5 */ 0 /* 6 */ )xyz;

	color: rgb( 1 , 2 , 3 )!important;
	color: rgb( 1 , 2 , 3 ) !important;
}

invalid_rgb {
	color: rgb( 100% , 0 , 0 );
	color: rgb( 100% , 0 , 0 ) xyz;
	color: rgb( 100% , 0 , 0 )xyz;

	color: rgb( 120% , 400 , 500 );
	color: rgb( 120% , 400 , 500 ) xyz;
	color: rgb( 120% , 400 , 500 )xyz;

	color: rgb( 255 , 0 , 0 , 1 );
	color: rgb( 255 , 0 , 0 , 1 ) xyz;
	color: rgb( 255 , 0 , 0 , 1 )xyz;
}

';

var_dump($oProcessor->minify($sCSS));

?>
===DONE===
--EXPECT--
string(225) "valid_rgb{color:#000;color:#000 xyz;color:#000 xyz;color:red;color:red xyz;color:red xyz;color:#010203;color:#010203 xyz;color:#010203 xyz;color:red;color:red xyz;color:red xyz;color:#010203!important;color:#010203!important}"
===DONE===
