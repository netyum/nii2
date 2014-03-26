--TEST--
nii\base\Exception Test
--FILE--
<?php
$exception = new \nii\base\Exception();
print $exception->getName();
?>
--EXPECTF--
Exception
