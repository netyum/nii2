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
	function getName() {
		return "this test class name method";
	}

	public function setValue($name) {
		echo $name."\n";
	}
}

$t = new Test();


//test __get

//*
echo $t->name;
//echo $t->sssss;
//*/

//test __set

/*
$t->value = "syang";
//echo $t->value."\n";
$t->age="@@@@@@";
*/

//test __isset
//var_dump($t->__isset('value'));
//echo "\n";
//var_dump($t->__isset('name'));

?>
--EXPECT--
nii\base\Object
