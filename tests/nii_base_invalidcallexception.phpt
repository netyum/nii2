--TEST--
nii\base\InvalidCallException Test
--FILE--
<?php
$exception = new \nii\base\InvalidCallException();
print $exception->getName();
?>
--EXPECTF--
Invalid Call
