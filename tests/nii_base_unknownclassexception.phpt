--TEST--
nii\base\UnknownClassException Test
--FILE--
<?php
$exception = new \nii\base\UnknownClassException();
print $exception->getName();
?>
--EXPECTF--
Unknown Class
