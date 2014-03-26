--TEST--
nii\base\UnknownPropertyException Test
--FILE--
<?php
$exception = new \nii\base\UnknownPropertyException();
print $exception->getName();
?>
--EXPECTF--
Unknown Property
