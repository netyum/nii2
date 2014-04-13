--TEST--
nii\base\Object Test
--FILE--
<?php

$container = new \nii\di\Container();


$container->clear('name');
?>
--EXPECT--
nii\base\Object
