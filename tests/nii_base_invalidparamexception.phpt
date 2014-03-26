--TEST--
nii\base\InvalidParamException Test
--FILE--
<?php
$exception = new \nii\base\InvalidParamException();
print $exception->getName();
?>
--EXPECTF--
Invalid Parameter
