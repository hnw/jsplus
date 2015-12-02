--TEST--
Check for strings are concatenated by '+' operator
--SKIPIF--
<?php
	extension_loaded('jsplus') or die('skip jsplus not available');
?>
--FILE--
<?php
$w=2;
$x=3;
$y="4";
$z="5";
var_dump($w+$x);
var_dump($x+$y);
var_dump($y+$w);
var_dump($y+$z);
var_dump($w+3);
var_dump($x+"4");
var_dump($y+2);
var_dump($y+"5");
var_dump(2+$x);
var_dump(3+$y);
var_dump("4"+$w);
var_dump("4"+$z);
$w+=$x;
var_dump($w);
$x+=$y;
var_dump($x);
$y+=$z;
var_dump($y);
$z+=6;
var_dump($z);
var_dump(7+8);
var_dump("7"+8);
var_dump(7+"8");
var_dump("7"+"8");
--EXPECT--
int(5)
string(2) "34"
string(2) "42"
string(2) "45"
int(5)
string(2) "34"
string(2) "42"
string(2) "45"
int(5)
string(2) "34"
string(2) "42"
string(2) "45"
int(5)
string(2) "34"
string(2) "45"
string(2) "56"
int(15)
string(2) "78"
string(2) "78"
string(2) "78"
