--TEST--
Check for strings are not concatenated by '+' operator when strplus.enabled=false
--INI--
strplus.enabled=false
--SKIPIF--
<?php
	extension_loaded('strplus') or die('skip strplus not available');
?>
--FILE--
<?php
$x="3";
$y="4";
var_dump($x+$y);
$y+=$x;
var_dump($y);
--EXPECT--
int(7)
int(7)
