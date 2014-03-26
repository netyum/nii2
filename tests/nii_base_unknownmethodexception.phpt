--TEST--
nii\base\UnknownMethodException Test
--FILE--
<?php
$exception = new \nii\base\UnknownMethodException();
print $exception->getName();
?>
--EXPECTF--
Unknown Method
