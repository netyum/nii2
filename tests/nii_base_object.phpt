--TEST--
nii\base\Object Test
--FILE--
<?php

/*
print \nii\base\Object::className();

$o = new \nii\base\Object();
$o->__call('Hello', array('2'));
*/


class Test extends \nii\base\Object
{
	public $max;
	function getName() {
		return "this test class name method";
	}

	public function setValue($name) {
		var_dump($name)."\n";
	}
}

$t = new Test();


//test __get

//*
//echo $t->name;
//echo $t->sssss;
//*/

//test __set


//$t->value = "syang";
//echo $t->value."\n";
//$t->age="@@@@@@";


//test __isset
//var_dump($t->__isset('value'));
//echo "\n";
//var_dump($t->__isset('name'));

//test __unset
//$t->__unset('value');
//$t->__unset('name');

//test canSetProperty

//var_dump($t->canSetProperty('value'));
//var_dump($t->canSetProperty('name'));
//var_dump($t->canSetProperty('max', false));

//test canGetProperty

//var_dump($t->canGetProperty('value'));
//var_dump($t->canGetProperty('name'));
//var_dump($t->canGetProperty('max'));

//test hasProperty
var_dump($t->hasProperty('value'));
var_dump($t->hasProperty('name'));
var_dump($t->hasProperty('max'));
var_dump($t->hasProperty('ffffff'));
?>
--EXPECT--
nii\base\Object
