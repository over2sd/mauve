<?php
# Testbed
	header("Expires: Tue, 01 Jan 2000 00:00:00 GMT");
	header("Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT");
	header("Cache-Control: no-store, no-cache, must-revalidate, max-age=0");
	header("Cache-Control: post-check=0, pre-check=0", false);
	header("Pragma: no-cache");
	include 'top.phf';
	$uname = showHead("Testing");
	include 'skrsql.phf';
	$sql = loginMySQL($_SESSION['dbname'],$_SESSION['dbhost'],$_SESSION['uname'],$_SESSION['upass']);
	include 'nav.phf';
	showNav("Test a function",$_SESSION['uname'],"testbed.php",1,1);






?>



<?php
	include 'bottom.phf';
	showFoot('2017.10.02b');
?>
