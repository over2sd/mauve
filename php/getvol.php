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
?>
<script>
function select_item(item) {
	targetitem.value = item;
	top.close();
	return false;
}
</script>
<?php
	if(isset($_POST['stage']) && ($_POST['stage'] == 2)) {
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
			if ($wherelist > 1) {
				$query = $query . "v.family LIKE '" . $_POST['family'] . "' ";
			}
			if ($wherelist == 3) {
				$query = $query . "AND ";
			}
			if ($wherelist != 2) {
				$query = $query . "v.given LIKE '" . $_POST['given'] . "' ";
			}
		}
		$query = $query . "LIMIT 10;";
		$prep = $sql->prepare($query);
/* # For some reason, bind_param is not working on my development
 * # machine. I've seen evidence on forum posts that there might be a
 * # bug.
 * # Since this program should not be present on a public Web server,
 * # and since this program does not loan Web-server credentials to the
 * # user (they use their own SQL logins), I'm going to build this
 * # without parameterization. If someone can fix this, I'll gladly
 * # accept it. But since I can't get it to work, I'm leaving it undone,
 * # for now.
 * # TODO: Fix this so it uses parameterization.
		if ($wherelist == 3) {
			echo "-=3=-";
			if (!$prep->bind_param('ss',$first,$second)) {
				echo "Preparation failed: " . $prep->errno . ", " . $prep->error;
			}
		} else {
			if (!$prep->bind_param('s',$only)) {
				echo "Preparation failed: " . $prep->errno . ", " . $prep->error;
			}
			if ($wherelist == 2) {
				echo "=22= " . $_POST['family'] . " ";
				$only = $_POST['family'];
			} else {
				echo "=21= " . $_POST['given'] . " ";
				$only = $_POST['given'];
			}
		}
		$first = $_POST['family'];
		$second = $_POST['given'];
		$only = $first;
# */
#	var_dump($prep);
# echo "Your query: '$query'<br />";
		echo $prep->error . "\n<br />";
		if (!$prep->execute()) {
			echo "Execution failed: " . $prep->errno . ", " . $prep->error;
		} else {
			if (!$prep->store_result()) {
				echo "Failed to store results: " . $prep->errno . ", " . $prep->error;
			} else {
#		echo "!";
				$prep->bind_result($rid,$rgiven,$rprintname,$rfamily,$rbirth,$rcity,$rstate);
				echo "<table>
			<tr><th>ID</th><th>Name</th><th>Location</th></tr>\n";
#	var_dump($prep);
				while ($prep->fetch()) {
#		echo "#";
					tableRowVol($display,$rid,$rgiven,$rprintname,$rfamily,$rbirth,$rcity,$rstate);
				}
				echo "\n</table>\n";
			}
		}
		$prep->close();
	} elseif(isset($_POST['stage']) && ($_POST['stage'] == 3))
		{
		$display = $_POST['formtype'];
		$st = "INSERT INTO volunteer SET";
		$ready = 0;
		# given, family, male, birth, (vaddress), email, street, city, state, post
		if (isset($_POST['birth']) && isset($_POST['family']) && isset($_POST['given'])) {
			if (($_POST['given'] != '') && ($_POST['family'] != '') && ($_POST['birth'] != '')) {
				$st = $st . " given = '" . $_POST['given'] . "', family = '" . $_POST['family'] . "', birth = '" . $_POST['birth'] . "',";
				$ready = 1;
			} else {
				echo "<h1>Error!</h1>\n\t\t<p>A required field (given,family,birth) is empty (''). Please fill these fields before submitting record!</p>\n";
			}
		
			if (isset($_POST['email']) && ($_POST['email'] != '')) {
				$st = $st . " email = '" . $_POST['email'] . "',";
			}
			if (isset($_POST['male']) && ($_POST['male'] != '')) {
				$st = $st . " male = '" . $_POST['male'] . "',";
			}
		} else {
			echo "<h1>Error!</h1>\n\t\t<p>A required field (given,family,birth) is missing. Please fill these fields before submitting record!</p>\n";
		}
		if (1 == $ready) {
			$st = rtrim($st,', ') . ";";
			echo "<p>Your query will be: $st</p>\n";
		}

/*
		# Address checker
		if (isset($_POST['street']) && isset($_POST['city']) && isset($_POST['state']) && isset($_POST['post'])) {
			if (($_POST['street'] != '') && ($_POST['city'] != '') && ($_POST['state'] != '') && ($_POST['post'] != '')) {
				$sta = "SELECT * FROM address WHERE street LIKE '" . substr($_POST['street'],0,3) . "%' AND city LIKE '" . substr($_POST['city'],0,3) . "%' AND state='" . strtoupper($_POST['state']) . "' AND post='" . $_POST['post'] . "' LIMIT 25;";
				echo "<p>Your Query will be: $sta</p>\n";
				$prep = $sql->prepare($sta);
				# TODO: Parameterize, if possible, when time permits.
				$prep->execute();
var_dump($prep);
				echo "<p>I found " . $prep->num_rows . " matching records.</p>\n";
			} else {
				echo "<h1>Error!</h1>\n\t\t<p>A required field (street,city,state,post) is empty (''). Please fill these fields before submitting record!</p>\n";
			}
		} else {
			echo "<h1>Error!</h1>\n\t\t<p>A required field (street,city,state,post) is missing. Please fill these fields before submitting record!</p>\n";
		}

# */







/*
				echo "<table>
			<tr><th>ID</th><th>Name</th><th>Location</th></tr>\n";
#	var_dump($prep);
				while ($prep->fetch()) {
#		echo "#";
					tableRowVol($display,$rid,$rgiven,$rprintname,$rfamily,$rbirth,$rcity,$rstate);
				}
				echo "\n</table>\n";
 */
	}

	$male = "Y";
	if (isset($_POST['male']) && ($_POST['male'] != '')) {
		$male = substr($_POST['male'],0,1);
	}
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
Birth Date: <input type="text" maxlength="10" name="birth" size="6" value="<?php echo $_POST['birth']; ?>" />
Email: <input type="text" name="email" size="30" value="<?php echo $_POST['email']; ?>" /><br />
Street: <input type="text" name="street" size="60" value="<?php echo $_POST['street']; ?>" /><br />
City: <input type="text" name="city" value="<?php echo $_POST['city']; ?>" />
State: <input type="text" name="state" size="3" value="<?php echo $_POST['state']; ?>" />
Postal code: <input type="text" name="post" size="5" value="<?php echo $_POST['post']; ?>" />
		<input type="submit" value="Add" />
	</form>
	</div>

	<noscript>
		<h1>JavaScript Disabled</h1>
		<p>This application will not work properly without JavaScript. Please enable it to continue.</p>
	</noscript>


<?php
	include 'bottom.phf';
	showFoot('2017.10.04b');
?>
