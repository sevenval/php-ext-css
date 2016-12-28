--TEST--
Test the <url> token modifier in different contexts
--FILE--
<?php

class Test_CSS3ProcessorModifiers
{
	public static function test_url($aData)
	{
		var_dump($aData);

		$aInfo = $aData['info'] ?? [];

		if (isset($aData['context'])) {
			$aLast = end($aData['context']);

			if ($aLast['type'] === CSS3Processor::TYPE_AT_KEYWORD) {
				if ($aLast['value'] === '@import') {
					return ($aInfo['value'] ?? '') . 'http://example.com/css' . ($aInfo['value'] ?? '');
				} else if ($aLast['value'] === '@font-face') {
					return '"' . substr($aData['value'], $aInfo ? 1 : 0, $aInfo ? -1 : strlen($aData['value'])) . '"';
				}
			} else if ($aLast['type'] === CSS3Processor::TYPE_BR_CO) {
				return "'" . substr($aData['value'], $aInfo ? 1 : 0, $aInfo ? -1 : strlen($aData['value'])) . "'";
			}
		}
	}
}

$oProcessor = new \CSS3Processor();
$oProcessor->setModifier(CSS3Processor::TYPE_URL,  ['Test_CSS3ProcessorModifiers', 'test_url']);

var_dump($oProcessor->dump('
/* Expected: All <url> tokens link to "http://example.com/css", quotes are untouched */
@import "http://import_double_quotes";
@import \'http://import_single_quotes\';
@import url( "http://import_without_quotes"   );
@import url( \'http://import_without_quotes\' );
@import url( http://import_without_quotes     );

/* Expected: All <url> tokens are in double quotes */
@font-face {
	src: url( "http://font_double_quotes"   );
	src: url( \'http://font_single_quotes\' );
	src: url( http://font_without_quotes    );
}

/* Expected: All <url> tokens are untouched */
@media (foo: bar) {
	baz {
		background: url( "http://media_double_quotes");
		background: url(\'http://media_single_quotes\' );
		background: url( http://media_without_quotes );
	}
}

/* Expected: All <url> tokens are in single quotes */
x {
	background: url( "http://double_quotes"   );
	background: url( \'http://single_quotes\' );
	background: url( http://without_quotes    );
}
'));

?>
===DONE===
--EXPECT--
array(5) {
  ["type"]=>
  int(7)
  ["name"]=>
  string(3) "url"
  ["value"]=>
  string(29) ""http://import_double_quotes""
  ["info"]=>
  array(3) {
    ["flag"]=>
    int(6)
    ["name"]=>
    string(13) "at_url_string"
    ["value"]=>
    string(1) """
  }
  ["context"]=>
  array(1) {
    [0]=>
    array(4) {
      ["level"]=>
      int(1)
      ["type"]=>
      int(3)
      ["name"]=>
      string(10) "at_keyword"
      ["value"]=>
      string(7) "@import"
    }
  }
}
array(5) {
  ["type"]=>
  int(7)
  ["name"]=>
  string(3) "url"
  ["value"]=>
  string(29) "'http://import_single_quotes'"
  ["info"]=>
  array(3) {
    ["flag"]=>
    int(6)
    ["name"]=>
    string(13) "at_url_string"
    ["value"]=>
    string(1) "'"
  }
  ["context"]=>
  array(1) {
    [0]=>
    array(4) {
      ["level"]=>
      int(1)
      ["type"]=>
      int(3)
      ["name"]=>
      string(10) "at_keyword"
      ["value"]=>
      string(7) "@import"
    }
  }
}
array(5) {
  ["type"]=>
  int(7)
  ["name"]=>
  string(3) "url"
  ["value"]=>
  string(30) ""http://import_without_quotes""
  ["info"]=>
  array(3) {
    ["flag"]=>
    int(5)
    ["name"]=>
    string(6) "string"
    ["value"]=>
    string(1) """
  }
  ["context"]=>
  array(1) {
    [0]=>
    array(4) {
      ["level"]=>
      int(1)
      ["type"]=>
      int(3)
      ["name"]=>
      string(10) "at_keyword"
      ["value"]=>
      string(7) "@import"
    }
  }
}
array(5) {
  ["type"]=>
  int(7)
  ["name"]=>
  string(3) "url"
  ["value"]=>
  string(30) "'http://import_without_quotes'"
  ["info"]=>
  array(3) {
    ["flag"]=>
    int(5)
    ["name"]=>
    string(6) "string"
    ["value"]=>
    string(1) "'"
  }
  ["context"]=>
  array(1) {
    [0]=>
    array(4) {
      ["level"]=>
      int(1)
      ["type"]=>
      int(3)
      ["name"]=>
      string(10) "at_keyword"
      ["value"]=>
      string(7) "@import"
    }
  }
}
array(4) {
  ["type"]=>
  int(7)
  ["name"]=>
  string(3) "url"
  ["value"]=>
  string(28) "http://import_without_quotes"
  ["context"]=>
  array(1) {
    [0]=>
    array(4) {
      ["level"]=>
      int(1)
      ["type"]=>
      int(3)
      ["name"]=>
      string(10) "at_keyword"
      ["value"]=>
      string(7) "@import"
    }
  }
}
array(5) {
  ["type"]=>
  int(7)
  ["name"]=>
  string(3) "url"
  ["value"]=>
  string(27) ""http://font_double_quotes""
  ["info"]=>
  array(3) {
    ["flag"]=>
    int(5)
    ["name"]=>
    string(6) "string"
    ["value"]=>
    string(1) """
  }
  ["context"]=>
  array(2) {
    [0]=>
    array(4) {
      ["level"]=>
      int(2)
      ["type"]=>
      int(30)
      ["name"]=>
      string(5) "br_co"
      ["value"]=>
      string(1) "{"
    }
    [1]=>
    array(4) {
      ["level"]=>
      int(1)
      ["type"]=>
      int(3)
      ["name"]=>
      string(10) "at_keyword"
      ["value"]=>
      string(10) "@font-face"
    }
  }
}
array(5) {
  ["type"]=>
  int(7)
  ["name"]=>
  string(3) "url"
  ["value"]=>
  string(27) "'http://font_single_quotes'"
  ["info"]=>
  array(3) {
    ["flag"]=>
    int(5)
    ["name"]=>
    string(6) "string"
    ["value"]=>
    string(1) "'"
  }
  ["context"]=>
  array(2) {
    [0]=>
    array(4) {
      ["level"]=>
      int(2)
      ["type"]=>
      int(30)
      ["name"]=>
      string(5) "br_co"
      ["value"]=>
      string(1) "{"
    }
    [1]=>
    array(4) {
      ["level"]=>
      int(1)
      ["type"]=>
      int(3)
      ["name"]=>
      string(10) "at_keyword"
      ["value"]=>
      string(10) "@font-face"
    }
  }
}
array(4) {
  ["type"]=>
  int(7)
  ["name"]=>
  string(3) "url"
  ["value"]=>
  string(26) "http://font_without_quotes"
  ["context"]=>
  array(2) {
    [0]=>
    array(4) {
      ["level"]=>
      int(2)
      ["type"]=>
      int(30)
      ["name"]=>
      string(5) "br_co"
      ["value"]=>
      string(1) "{"
    }
    [1]=>
    array(4) {
      ["level"]=>
      int(1)
      ["type"]=>
      int(3)
      ["name"]=>
      string(10) "at_keyword"
      ["value"]=>
      string(10) "@font-face"
    }
  }
}
array(5) {
  ["type"]=>
  int(7)
  ["name"]=>
  string(3) "url"
  ["value"]=>
  string(28) ""http://media_double_quotes""
  ["info"]=>
  array(3) {
    ["flag"]=>
    int(5)
    ["name"]=>
    string(6) "string"
    ["value"]=>
    string(1) """
  }
  ["context"]=>
  array(3) {
    [0]=>
    array(4) {
      ["level"]=>
      int(3)
      ["type"]=>
      int(30)
      ["name"]=>
      string(5) "br_co"
      ["value"]=>
      string(1) "{"
    }
    [1]=>
    array(4) {
      ["level"]=>
      int(2)
      ["type"]=>
      int(30)
      ["name"]=>
      string(5) "br_co"
      ["value"]=>
      string(1) "{"
    }
    [2]=>
    array(4) {
      ["level"]=>
      int(1)
      ["type"]=>
      int(3)
      ["name"]=>
      string(10) "at_keyword"
      ["value"]=>
      string(6) "@media"
    }
  }
}
array(5) {
  ["type"]=>
  int(7)
  ["name"]=>
  string(3) "url"
  ["value"]=>
  string(28) "'http://media_single_quotes'"
  ["info"]=>
  array(3) {
    ["flag"]=>
    int(5)
    ["name"]=>
    string(6) "string"
    ["value"]=>
    string(1) "'"
  }
  ["context"]=>
  array(3) {
    [0]=>
    array(4) {
      ["level"]=>
      int(3)
      ["type"]=>
      int(30)
      ["name"]=>
      string(5) "br_co"
      ["value"]=>
      string(1) "{"
    }
    [1]=>
    array(4) {
      ["level"]=>
      int(2)
      ["type"]=>
      int(30)
      ["name"]=>
      string(5) "br_co"
      ["value"]=>
      string(1) "{"
    }
    [2]=>
    array(4) {
      ["level"]=>
      int(1)
      ["type"]=>
      int(3)
      ["name"]=>
      string(10) "at_keyword"
      ["value"]=>
      string(6) "@media"
    }
  }
}
array(4) {
  ["type"]=>
  int(7)
  ["name"]=>
  string(3) "url"
  ["value"]=>
  string(27) "http://media_without_quotes"
  ["context"]=>
  array(3) {
    [0]=>
    array(4) {
      ["level"]=>
      int(3)
      ["type"]=>
      int(30)
      ["name"]=>
      string(5) "br_co"
      ["value"]=>
      string(1) "{"
    }
    [1]=>
    array(4) {
      ["level"]=>
      int(2)
      ["type"]=>
      int(30)
      ["name"]=>
      string(5) "br_co"
      ["value"]=>
      string(1) "{"
    }
    [2]=>
    array(4) {
      ["level"]=>
      int(1)
      ["type"]=>
      int(3)
      ["name"]=>
      string(10) "at_keyword"
      ["value"]=>
      string(6) "@media"
    }
  }
}
array(5) {
  ["type"]=>
  int(7)
  ["name"]=>
  string(3) "url"
  ["value"]=>
  string(22) ""http://double_quotes""
  ["info"]=>
  array(3) {
    ["flag"]=>
    int(5)
    ["name"]=>
    string(6) "string"
    ["value"]=>
    string(1) """
  }
  ["context"]=>
  array(1) {
    [0]=>
    array(4) {
      ["level"]=>
      int(1)
      ["type"]=>
      int(30)
      ["name"]=>
      string(5) "br_co"
      ["value"]=>
      string(1) "{"
    }
  }
}
array(5) {
  ["type"]=>
  int(7)
  ["name"]=>
  string(3) "url"
  ["value"]=>
  string(22) "'http://single_quotes'"
  ["info"]=>
  array(3) {
    ["flag"]=>
    int(5)
    ["name"]=>
    string(6) "string"
    ["value"]=>
    string(1) "'"
  }
  ["context"]=>
  array(1) {
    [0]=>
    array(4) {
      ["level"]=>
      int(1)
      ["type"]=>
      int(30)
      ["name"]=>
      string(5) "br_co"
      ["value"]=>
      string(1) "{"
    }
  }
}
array(4) {
  ["type"]=>
  int(7)
  ["name"]=>
  string(3) "url"
  ["value"]=>
  string(21) "http://without_quotes"
  ["context"]=>
  array(1) {
    [0]=>
    array(4) {
      ["level"]=>
      int(1)
      ["type"]=>
      int(30)
      ["name"]=>
      string(5) "br_co"
      ["value"]=>
      string(1) "{"
    }
  }
}
string(877) "
/* Expected: All <url> tokens link to "http://example.com/css", quotes are untouched */
@import "http://example.com/css";
@import 'http://example.com/css';
@import url("http://example.com/css");
@import url('http://example.com/css');
@import url(http://example.com/css);

/* Expected: All <url> tokens are in double quotes */
@font-face {
	src: url("http://font_double_quotes");
	src: url("http://font_single_quotes");
	src: url("http://font_without_quotes");
}

/* Expected: All <url> tokens are untouched */
@media (foo: bar) {
	baz {
		background: url("http://media_double_quotes");
		background: url('http://media_single_quotes');
		background: url(http://media_without_quotes);
	}
}

/* Expected: All <url> tokens are in single quotes */
x {
	background: url('http://double_quotes');
	background: url('http://single_quotes');
	background: url('http://without_quotes');
}
"
===DONE===
