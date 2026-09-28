--TEST--
test1() Basic test
--EXTENSIONS--
secp256k1
--FILE--
<?php
$ret = test1();

var_dump($ret);
?>
--EXPECT--
The extension secp256k1 is loaded and working!
NULL
