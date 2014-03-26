--TEST--
nii\base\InvalidConfigException Test
--FILE--
<?php
$exception = new \nii\base\InvalidConfigException();
print $exception->getName();
?>
--EXPECTF--
Invalid Configuration
