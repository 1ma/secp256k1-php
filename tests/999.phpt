--TEST--
secp256k1_test() outputs expected message
--EXTENSIONS--
secp256k1
--FILE--
<?php
secp256k1_test();
?>
--EXPECT--
The extension secp256k1 is loaded and working!
