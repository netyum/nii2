--TEST--
nii\base\Object Test
--FILE--
<?php

$component = new \nii\base\Component();

$array = [
   // 'nummm',
    'a' => 'test',
    'b' => 'c',
    'c' => 1
   // 'num'
];

//$component->ensureBehaviors();
//var_dump($component->behaviors());

//var_dump($component->_behaviors);
//var_dump($component->getBehaviors());
//attachBehaviors($array);

$component->__set('as Test', array('class'=>'test', "name"=>1111));
?>
--EXPECT--
nii\base\Object
