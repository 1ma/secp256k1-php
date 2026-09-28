--TEST--
Check if secp256k1 is loaded
--EXTENSIONS--
secp256k1
--FILE--
<?php
echo 'The extension "secp256k1" is available';
?>
--EXPECT--
The extension "secp256k1" is available
