--TEST--
Test CSS3Processor::minify() method with [***]-selectors
--FILE--
<?php

$oProcessor = new \CSS3Processor();

$sCSS = '

/* Attribute exists */
a [ attribute-exists ] {
	x: y;
}

/* Attribute has this exact value */
a[ attribute = "exact-value" ] {
	x: y;
}

/* Attribute value contains this value somewhere in it */
a[ attribute *= "contains-this" ] {
	x: y;
}

/* Attribute has this value in a space-separated list somewhere */
a [ attribute ~= "space-separated" ] {
	x: y;
}

/* Attribute value starts with this */
a [ attribute ^= "starts-with" ] {
	x: y;
}

/* Attribute value has this in a dash-separated list somewhere */
a [ attribute |= "dash-separated" ] {
	x: y;
}

/* Attribute value ends with this */
a [ data-value $= "ends-with" ] {
	x: y;
}



/* Attribute has this exact value */
a[ attribute = "exact value" ] {
	x: y;
}

/* Attribute value contains this value somewhere in it */
a[ attribute *= "contains this" ] {
	x: y;
}

/* Attribute has this value in a space-separated list somewhere */
a [ attribute ~= "space separated" ] {
	x: y;
}

/* Attribute value starts with this */
a [ attribute ^= "starts with" ] {
	x: y;
}

/* Attribute value has this in a dash-separated list somewhere */
a [ attribute |= "dash separated" ] {
	x: y;
}

/* Attribute value ends with this */
a [ data-value $= "ends with" ] {
	x: y;
}

';
var_dump($oProcessor->minify($sCSS));

?>
===DONE===
--EXPECT--
string(419) "a [attribute-exists]{x:y}a[attribute=exact-value]{x:y}a[attribute*=contains-this]{x:y}a [attribute~=space-separated]{x:y}a [attribute^=starts-with]{x:y}a [attribute|=dash-separated]{x:y}a [data-value$=ends-with]{x:y}a[attribute="exact value"]{x:y}a[attribute*="contains this"]{x:y}a [attribute~="space separated"]{x:y}a [attribute^="starts with"]{x:y}a [attribute|="dash separated"]{x:y}a [data-value$="ends with"]{x:y}"
===DONE===
