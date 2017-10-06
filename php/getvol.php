<?php
	include 'top.phf';
	$uname = showHead("Volunteer");
	include 'skrsql.phf';
	$sql = loginMySQL($_SESSION['dbname'],$_SESSION['dbhost'],$_SESSION['uname'],$_SESSION['upass']);
	include 'nav.phf';
	showNav("Get/Add Volunteer",$_SESSION['uname'],"getvol.php",1,1);

function tableRowVol($display,$rid,$rgiven,$rprintname,$rfamily,$rbirth,$rcity,$rstate,$rmale,$rbirth,$remail,$rstreet,$rpost) {
	$namecomp = $rgiven;
	if($rprintname != '') {
		$namecomp = $namecomp . " \"" . $rprintname . "\"";
	}
	$namecomp = $namecomp . " " . $rfamily;
	$citycomp = $rcity . ", " . $rstate;
	$extra = '';
	if ('extended' == $display) {
		$extra = sprintf("<td>%s</td><td>%s</td><td>%s</td>",$rmale,$rbirth,$remail);
		$citycomp = sprintf("%s, %s, %s %s",$rstreet,$rcity,$rstate,$rpost);
	}
	printf("\t<tr><td>%s</td><td>%s</td><td>%s</td>%s</tr>\n",$rid,$namecomp,$citycomp,$extra);
## <a nohref=\"nohref\" onClick='return select_item(\"%s\")'>%s</a>		,$rid
}

function tableRowAddy($display,$rid,$rstreet,$rcity,$rstate,$rpost,$extra,$indent = 0) {
	while (0 < $indent) {
		print "\t"; $indent--;
	}
	printf("<tr><td>%s</td><td>%s</td><td>%s</td><td>%s</td><td>%s</td>%s</tr>\n",$rid,$rstreet,$rcity,$rstate,$rpost,$extra);
}

function makeAddress($sql,$qstreet,$qcity,$qstate,$qpost) {
			$st = "INSERT INTO address (street,city,state,post) VALUES ('$qstreet','$qcity','$qstate','$qpost');";
			# TODO: Parameterize
			$cmd = doSQL(1,$sql,$st,array([]));
			return $cmd->insert_id;
}

?>
		<script>
function select_item(item) {
	targetitem.value = item;
	top.close();
	return false;
}
function chooser(target,value) {
	targetitem = document.getElementById(target);
	targetitem.value = value;
	return true;
}
		</script>

		<noscript>
			<h1>JavaScript Disabled</h1>
			<p>This application will not work properly without JavaScript. Please enable it to continue.</p>
		</noscript>
<?php
	$showforms = 1;
	$male = "Y";
	if (isset($_POST['male']) && ($_POST['male'] != '')) {
		$male = substr($_POST['male'],0,1);
		if ("M" == $male || "m" == $male) { $male = "Y"; }
	} # used by multiple stages

	if(isset($_POST['stage']) && ($_POST['stage'] == 2))
		{ # Stage 2 (search volunteer)
		$display = $_POST['formtype'];
		$query = "SELECT v.ID,v.given,v.printname,v.family,v.birth,a.city,a.state FROM volunteer v INNER JOIN address a ON v.vaddress=a.id ";
		$wherelist = 0;
		if (isset($_POST['given']) && ($_POST['given'] != "")) {
			$wherelist += 1;
		}
		if (isset($_POST['family']) && ($_POST['family'] != "")) {
			$wherelist += 2;
		}
		if ($wherelist) {
			$query = $query . "WHERE ";
			$qstr = "'";
			if ($wherelist > 1) {
				$query = $query . "v.family LIKE '" . $_POST['family'] . "' ";
				$qstr = $qstr . $_POST['family'] . "'";
			}
			if ($wherelist == 3) {
				$query = $query . "AND ";
				$qstr = $qstr . " AND '";
			}
			if ($wherelist != 2) {
				$query = $query . "v.given LIKE '" . $_POST['given'] . "' ";
				$qstr = $qstr . $_POST['given'] . "'";
			}
		}
		$query = $query . "LIMIT 10;";
		$prep = doSQL(1,$sql,$query,array([]));
		$prep->bind_result($rid,$rgiven,$rprintname,$rfamily,$rbirth,$rcity,$rstate);
		print "<p>Search results for $qstr:</p><table>\n\t\t\t<tr><th>ID</th><th>Name</th><th>Location</th></tr>\n";
#	var_dump($prep);
		while ($prep->fetch()) {
#		echo "#";
			tableRowVol($display,$rid,$rgiven,$rprintname,$rfamily,$rbirth,$rcity,$rstate);
		}
		printf("\n\t\t\t<tr><td colspan=\"3\" class=\"c\">%d record%s matched.</td></tr>",$prep->num_rows,(1 == $prep->num_rows ? "" : "s"));
		print "\n\t\t</table>\n";
		$prep->close();
	} elseif(isset($_POST['stage']) && ($_POST['stage'] == 3))
		{ # Stage 3 (insert volunteer)
		$display = $_POST['formtype'];
		$ready = 0;

		# Address checker
		$addhere = isset($_POST['vaddress']);
		$vaddress = $_POST['vaddress'];
		if ($addhere && (-1 == $vaddress)) {
			if (isset($_POST['street']) && isset($_POST['city']) && isset($_POST['state']) && isset($_POST['post'])) {
				if (($_POST['street'] != '') && ($_POST['city'] != '') && ($_POST['state'] != '') && ($_POST['post'] != '')) {
					$qstreet = substr($_POST['street'],0,3) . "%";
					$qcity = substr($_POST['city'],0,3) . "%";
					$qstate = strtoupper($_POST['state']);
					$qpost = $_POST['post'];
					$sta = "SELECT * FROM address WHERE street LIKE '$qstreet' AND city LIKE '$qcity' AND state='$qstate' AND post='$qpost' LIMIT 25;";
#					echo "<p>Your Query will be: $sta</p>\n";
					$prep = doSQL(1,$sql,$sta,array([]));
					if ($prep->num_rows) {
						$prep->bind_result($rid,$rstreet,$rcity,$rstate,$rpost);
						$qstr = "'$qstreet in $qcity, $qstate $qpost'";
						print "<p>Search results for $qstr reveal a possible duplication:</p>";
?>

		<form method='post' action="getvol.php" onSubmit="return validate(this)">
			<input type="hidden" name="given" value="<?php if ("3" == $_POST['stage']) echo $_POST['given']; ?>" />
			<input type="hidden" name="family" value="<?php if ("3" == $_POST['stage']) echo $_POST['family']; ?>" /> 
			<input type="hidden" name="formtype" value="extended" />
			<input type="hidden" name="stage" value="3" />
			<input type="hidden" id="vaddress" name="vaddress" value="-1" />
			<input type="hidden" name="male" value="<?php echo $male; ?>" />
			<input type="hidden" maxlength="10" name="birth" value="<?php echo $_POST['birth']; ?>" />
			<input type="hidden" name="email" size="30" value="<?php echo $_POST['email']; ?>" />
			<input type="hidden" name="street" size="60" value="<?php echo $_POST['street']; ?>" />
			<input type="hidden" name="city" value="<?php echo $_POST['city']; ?>" />
			<input type="hidden" name="state" size="3" value="<?php echo $_POST['state']; ?>" />
			<input type="hidden" name="post" size="5" value="<?php echo $_POST['post']; ?>" />
<?php
						print "\t\t\t<table>\n\t\t\t\t<tr><th>ID</th><th>Street</th><th>City</th><th>State</th><th>Postal Code</th><th>Choice</th></tr>\n";
						while ($prep->fetch()) {
							$extra = "<td><input type=\"submit\" value=\"Use\" onClick=\"chooser('vaddress',$rid);\" /></td>";
							tableRowAddy($display,$rid,$rstreet,$rcity,$rstate,$rpost,$extra,4);
						}
						tableRowAddy($display,"New",$_POST['street'],$_POST['city'],$_POST['state'],$_POST['post'],"<td><input type=\"submit\" value=\"New\" onClick=\"chooser('vaddress',0);\" /></td>",4);
						printf("\n\t\t\t\t<tr><td colspan=\"7\" class=\"c\">%d record%s matched.</td></tr>",$prep->num_rows,(1 == $prep->num_rows ? "" : "s"));
						print "\n\t\t\t</table>\n";
						print "\t\t</form>\n";
						$prep->close();
					} else { # no duplicate found; make address
						$qstreet = $_POST['street'];
						$qcity = $_POST['city'];
						$qstate = strtoupper($_POST['state']);
						$qpost = $_POST['post'];
						$vaddress = makeAddress($sql,$qstreet,$qcity,$qstate,$qpost);
						print "<p>Address created. ID: $vaddress</p>\n";
						$addhere = 1;
						$ready = 1;
					}
				} else { # not fields filled
					print "<h1>Error!</h1>\n\t\t<p>A required field (street,city,state,post) is empty (''). Please fill these fields before submitting record!</p>\n";
				}
			} else {
				print "<h1>Error!</h1>\n\t\t<p>A required field (street,city,state,post) is missing. Please fill these fields before submitting record!</p>\n";
			}
		} else if ($addhere && (0 == $vaddress)) { # end of no vaddress; insert vaddress
			$qstreet = $_POST['street'];
			$qcity = $_POST['city'];
			$qstate = strtoupper($_POST['state']);
			$qpost = $_POST['post'];
			$vaddress = makeAddress($qstreet,$qcity,$qstate,$qpost);
			print "\t\t<p>Address created. ID: $vaddress</p>\n";
			$ready = 1;
		} else { # end of insert vaddress
			$ready = 1; # vaddress not -1 or 0, so should be a valid ID.
			print "\t\t<p>Using vaddress ID: $vaddress.</p>\n";
		}
		$given = '';
		$family = '';
		if (1 == $ready) { # insert volunteer
			$st = "INSERT INTO volunteer SET";
			$ready = 0;
		# given, family, male, birth, (vaddress), email, street, city, state, post
			if (isset($_POST['birth']) && isset($_POST['family']) && isset($_POST['given'])) {
				if (($_POST['given'] != '') && ($_POST['family'] != '') && ($_POST['birth'] != '')) {
					$given = $_POST['given'];
					$family = $_POST['family'];
					$st = $st . " given = '$given', family = '$family', birth = '" . $_POST['birth'] . "',";
					$ready = 1;
				} else {
					print "<h1>Error!</h1>\n\t\t<p>A required field (given,family,birth) is empty (''). Please fill these fields before submitting record!</p>\n";
					$ready = 0;
				}
				$st = $st . " vaddress = '$vaddress', male = '$male',";
				if (isset($_POST['email']) && ($_POST['email'] != '')) {
					$st = $st . " email = '" . $_POST['email'] . "',";
				}
			} else {
				print "<h1>Error!</h1>\n\t\t<p>A required field (given,family,birth) is missing. Please fill these fields before submitting record!</p>\n";
				$ready = 0;
			}
			if (1 == $ready) {
				$st = rtrim($st,', ') . ";";
#				echo "<p>Your query will be: $st</p>\n";
				# TODO: Parameterize
				$cmd = doSQL(1,$sql,$st,array([]));
				$volid = $cmd->insert_id;
				print "<div class=\"notice\">\n\t\t\t<h2>Success!</h2>\n\t\t<p>Record created for volunteer $given $family. ID: $volid</p>\n\t\t</div>";
			}
#		} else { # end of insert volunteer
		}
	} # end of stage 3
	if (1 == $showforms) {
?>

	<div class="mform">
		<form method='post' action="getvol.php" onSubmit="return validate(this)">
			<p>Enter search values. Enter at least three letters of one of these fields. You may use a wildcard (e.g., "Smi%" for Smith).</p>
			Given name: <input type="text" name="given" value="<?php if ("2" == $_POST['stage']) echo $_POST['given']; ?>" />
			Family name: <input type="text" name="family" value="<?php if ("2" == $_POST['stage']) echo $_POST['family']; ?>" /> 
			<input type="hidden" name="formtype" value="search" />
			<input type="hidden" name="stage" value="2" />
			<input type="submit" value="Search" />
		</form>
	</div>
<br />
	<div class="mform">
		<form method='post' action="getvol.php" onSubmit="return validate(this)">
			<p>Enter all possible values to add a volunteer.</p>
			Given name: <input type="text" name="given" value="<?php if ("3" == $_POST['stage']) echo $_POST['given']; ?>" />
			Family name: <input type="text" name="family" value="<?php if ("3" == $_POST['stage']) echo $_POST['family']; ?>" /> 
			<input type="hidden" name="formtype" value="extended" /><br />
			<input type="hidden" name="stage" value="3" />
			Male: <input type="text" name="male" value="<?php echo $male; ?>" size="2" />
			Birth Date: <input type="text" maxlength="10" name="birth" size="7" value="<?php echo $_POST['birth']; ?>" />
			Email: <input type="text" name="email" size="30" value="<?php echo $_POST['email']; ?>" /><br />
			<input type="hidden" name="vaddress" value="-1" />
			Street: <input type="text" name="street" size="60" value="<?php echo $_POST['street']; ?>" /><br />
			City: <input type="text" name="city" value="<?php echo $_POST['city']; ?>" />
			State: <input type="text" name="state" size="3" value="<?php echo $_POST['state']; ?>" />
			Postal code: <input type="text" name="post" size="5" value="<?php echo $_POST['post']; ?>" />
			<input type="submit" value="Add" />
		</form>
	</div>

<?php
	}
	include 'bottom.phf';
	showFoot('2017.10.04b');
?>
