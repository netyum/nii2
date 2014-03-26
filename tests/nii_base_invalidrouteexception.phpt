--TEST--
nii\base\InvalidRouteException Test
--FILE--
<?php
$exception = new \nii\base\InvalidRouteException();
print $exception->getName();
?>
--EXPECTF--
Invalid Route
