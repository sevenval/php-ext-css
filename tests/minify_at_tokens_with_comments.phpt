--TEST--
Test CSS3Processor::minify() method for "@import" and "@namespace" tokens with leading comments
--FILE--
<?php

class CSS3ProcessorModifiers
{
	public static function urls(array $aData)
    {
		if (strpos($aData['value'], '.css')) {
			return strtolower($aData['value']);
		} else {
			return str_replace('FONT', '/path/to/FONT', $aData['value']);
		}
	}

	public static function comments(array $aData)
    {
		if (strpos($aData['value'], 'REMOVE')) {
			return '';
		} else if (strpos($aData['value'], 'MODIFY')) {
			return str_replace(' MODIFY', '', $aData['value']);
		} else if (strpos($aData['value'], 'DO NOT TOUCH')) {
			if (isset($aData['context']) && ($aLast = end($aData['context']))) {
				if ($aLast['name'] === 'at_keyword' && $aLast['value'] === '@font-face') {
					return $aData['value'];
				}
			}
		}
	}
}

$oProcessor = new \CSS3Processor();

$sCSS = '
/* REMOVE Simple-IMPORT comment */
@import "STRING.css";

/* MODIFY IMPORT comment inside the URL function */
@import url( "URL.css" );

/* MODIFY NAMESPACE comment inside the URL function */
@namespace prefix url(www.example.com);

/* DO NOT TOUCH the FONT-FACE comment, but only if inside the FONT-FACE-BLOCK */
@font-face { font-family: "Font-Name"; /* DO NOT TOUCH the comment inside the FONT-FACE */ src: url("FONT.ttf") }
';

$oProcessor->setModifier(\CSS3Processor::TYPE_URL, ['CSS3ProcessorModifiers', 'urls']);
$oProcessor->setModifier(\CSS3Processor::TYPE_COMMENT, ['CSS3ProcessorModifiers', 'comments']);

var_dump($oProcessor->minify($sCSS));

?>
===DONE===
--EXPECT--
string(288) "@import"string.css";/* IMPORT comment inside the URL function */@import url("url.css");/* NAMESPACE comment inside the URL function */@namespace prefix url(www.example.com);@font-face{font-family:"Font-Name";/* DO NOT TOUCH the comment inside the FONT-FACE */src:url("/path/to/FONT.ttf")}"
===DONE===
