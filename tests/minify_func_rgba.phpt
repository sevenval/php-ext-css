--TEST--
Test CSS3Processor::minify() method for CSS rgb[a]() function
--SKIPIF--
<?php if(true) echo 'skip'; ?>
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

valid_rgba {
	color: rgba( 255 , 0 , /* Comment */ 0 , 1 );
	color: rgba( 255 , 0 , /* Comment */ 0 , 1 ) 80%;
	color: rgba( 255 , 0 , /* Comment */ 0 , 1 )80%

	color: rgba( 255 , 0 , 0 , .1 );
	color: rgba( 255 , 0 , 0 , .1 ) 80%;
	color: rgba( 255 , 0 , 0 , .1 )80%;

	color: rgba( 100% , 0% , 0% , 10% );
	color: rgba( 100% , 0% , 0% , 10% ) 80%;
	color: rgba( 100% , 0% , 0% , 10% )80%;

	color: rgba( 1 , 2 , 3 , 1 )!important;
	color: rgba( 1 , 2 , 3 , 1 ) !important;

	color: rgba( 100% , 0% , 0% , 10% ) /* 1234567890 */80%;
	color: rgba( 100% , 0% , 0% , 10% )/* 1234567890 */80%;
	color: rgba( 100% , 0% , 0% , 10% ) /* 1234567890 */ 80%;
	color: rgba( 100% , 0% , 0% , 10% )
}

invalid_rgba {
	color: rgba( 255 , 0  0 , 1 );
	color: rgba( 255 , 0  0 , 1 ) /* 1234567890 */foo;
	color: rgba( 255 , 0  0 , 1 )/* 1234567890 */foo;

	color: 1pt rgba( 255 , 0 , 0 );
	color: 1pt rgba( 255 , 0 , 0 ) 1px;
	color: 1pt rgba( 255 , 0 , 0 )1px;

	color: 1pt rgba( 255 , 0 , 0 
}

';

var_dump($oProcessor->minify($sCSS));

?>
===DONE===
--EXPECT--
string(499) "valid_rgb{color:#000;color:#000 xyz;color:#000 xyz;color:red;color:red xyz;color:red xyz;color:#010203;color:#010203 xyz;color:#010203 xyz;color:red;color:red xyz;color:red xyz;color:#010203!important;color:#010203!important}valid_rgba{color:red;color:red 80%;color:red 80% color: #ff00001a;color:#ff00001a 80%;color:#ff00001a 80%;color:#ff00001a;color:#ff00001a 80%;color:#ff00001a 80%;color:#010203ff!important;color:#010203ff!important;color:#ff00001a 80%;color:#ff00001a 80%;color:#ff00001a 80%}"
===DONE===
