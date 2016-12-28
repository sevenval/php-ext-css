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

?>
===DONE===
--EXPECT--
string(60) "@keyframes fade{0%{opacity:0}50%{opacity:.5}100%{opacity:1}}"
string(58) "@media screen AND (min-width:1000px){body{font-size:16pt}}"
string(105) "x{content:'\A\B'}y{background:url('wallpaper.jpg')}#\31st{color:red}#\32nd{color:blue}.\33rd{color:green}"
===DONE===
