--TEST--
secp256k1 phpinfo output
--EXTENSIONS--
secp256k1
--FILE--
<?php
ob_start();
phpinfo(INFO_MODULES);
$info = ob_get_clean();

var_dump(strpos($info, 'secp256k1 support') !== false);
?>
--EXPECT--
bool(true)
