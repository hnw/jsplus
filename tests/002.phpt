--TEST--
Check for strings are concatenated by '+' operator
--SKIPIF--
<?php
	extension_loaded('jsplus') or die('skip jsplus not available');
?>
--FILE--
<?php
$x="3";
$y="4";
var_dump($x+$y);
$y+=$x;
var_dump($y);
--EXPECT--
string(2) "34"
string(2) "43"
