--TEST--
nii\di\Container Test
--FILE--
<?php

class Test {
	
	public $m;

	public function a() {

		echo "this Test\n";
	}
}

$container = new \nii\di\Container();
$container->set('test', '\Test');
$container->set('Test', [
	'm' => 'mysql:host=127.0.0.1;dbname=demo',
]);


//$container->clear('name');
//$container->has('name');
?>
--EXPECT--
