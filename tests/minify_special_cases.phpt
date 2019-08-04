--TEST--
Test CSS3Processor::minify() method with special cases
--FILE--
<?php

$oProcessor = new \CSS3Processor();

$sCSS = '
@keyframes fade {
	0%   { opacity: 0;   }
	50%  { opacity: 0.5  }
	100% { opacity: 1.0; }
}
';
var_dump($oProcessor->minify($sCSS));

$sCSS = '
@media screen AND (min-width:1000px) {
	body{
		font-size:16pt
	}
}
';
var_dump($oProcessor->minify($sCSS));

$sCSS = "
x { content: '\\000A\\000B' }

y { background: url('wall\
paper.jpg') }

#\\000031st { color: red } #\\00032nd { color: blue; } .\\0033rd { color: green }";
var_dump($oProcessor->minify($sCSS));

$sCSS = 'x{y:1px\9}';
var_dump($oProcessor->minify($sCSS));

$sCSS = '@media (min-width:30em) and (max-height:60em) { a:b }';
var_dump($oProcessor->minify($sCSS));

$sCSS = ':not( x ) foo {a:b;}';
var_dump($oProcessor->minify($sCSS));

$sCSS = ' <!-- @media screen { <!-- x { a:b; } --> } --> ';
var_dump($oProcessor->minify($sCSS));

$sCSS = 'x {a: b c "Open Sans" , Arial} y { a: func(b) "s3"; }';
var_dump($oProcessor->minify($sCSS));

?>
===DONE===
--EXPECT--
string(60) "@keyframes fade{0%{opacity:0}50%{opacity:.5}100%{opacity:1}}"
string(58) "@media screen AND (min-width:1000px){body{font-size:16pt}}"
string(105) "x{content:'\A\B'}y{background:url('wallpaper.jpg')}#\31st{color:red}#\32nd{color:blue}.\33rd{color:green}"
string(10) "x{y:1px\9}"
string(48) "@media(min-width:30em)and (max-height:60em){a:b}"
string(16) ":not(x) foo{a:b}"
string(28) "@media screen{<!--x{a:b}-->}"
string(43) "x{a:b c "Open Sans",Arial}y{a:func(b) "s3"}"
===DONE===
