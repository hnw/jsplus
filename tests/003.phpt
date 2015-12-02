--TEST--
Check for strings are not concatenated by '+' operator when jsplus.enabled=false
--INI--
jsplus.enabled=false
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
int(7)
int(7)
