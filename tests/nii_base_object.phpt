--TEST--
nii\base\Object Test
--FILE--
<?php


print \nii\base\Object::className();

$o = new \nii\base\Object();
$o->__call('Hello', array('2'));


/*
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

echo $t->name."\n";
//echo $t->value."\n";
echo $t->age."\n";
*/

?>
--EXPECT--
nii\base\Object
