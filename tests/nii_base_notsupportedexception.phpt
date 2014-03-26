--TEST--
nii\base\NotSupportedException Test
--FILE--
<?php
$exception = new \nii\base\NotSupportedException();
print $exception->getName();
?>
--EXPECTF--
Not Supported
