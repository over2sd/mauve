
<?php
	include 'top.phf';
	$uname = showHead("Registration");
	include 'skrsql.phf';
	$sql = loginMySQL($_SESSION['dbname'],$_SESSION['dbhost'],$_SESSION['uname'],$_SESSION['upass']);
	include 'nav.phf';
	showNav("Register a team",$_SESSION['uname'],"register.php",1,1);
?>
	<div class="mform">
		<form method='post' action="register.php?stage=2" onSubmit="return validate(this)">
			<div>
				Contact type:  
				<input type="radio" name="contype" value="e" onclick="document.getElementById('timeofcontact').readOnly = false;" /> Email  
				<input type="radio" name="contype" value="p" onclick="document.getElementById('timeofcontact').readOnly = true;" checked="checked" /> Phone  
				<input type="radio" name="contype" value="m" onclick="document.getElementById('timeofcontact').readOnly = true;" /> Mail  
				<input type="radio" name="contype" value="v" onclick="document.getElementById('timeofcontact').readOnly = true;" /> Visit
			</div>
			<p>Date: <input type="text" maxlength="10" name="contact" value="<?php echo date("Y-m-d"); ?>" style="width: 6em;" />
			Time: <input type="text" name="contime" maxlength="8" id="timeofcontact" value="<?php echo date("H:i:s",time()); ?>" style="width: 4.5em;" readonly="true" />
			</p>
			<p>Leader ID: <input type="text" size="4" name="leadname" id="leaderid" /> <span id="leadername"></span><!-- <input type="button" value="Choose/Add" onClick='targetitem = document.getElementById("leaderid"); dataitem = window.open("getvol.php","dataitem","toolbar=no,menubar=no,scrollbars=yes"); dataitem.targetitem = targetitem' />--></p>
			
		<script type="text/javascript">
		function validate(form) {
			fail = validateCondate(form.contact.value)
			fail += validateContime(form.contime.value)
			fail += validateType(form.contype.value)
			fail += validateLeadnum(form.leader.value)
			fail += validateLeadname(form.leadname.value)
			fail += validateAddress(form.address.value)
			fail += validatePhone(form.phone.value)
			fail += validateEmail(form.email.value)
			fail += validateOrg(form.orgnum.value)
			fail += validateOrgname(form.orgnam.value)
			fail += validatePhone(form.orgpho.value)
			fail += validateAddress(form.orgadd.value)
			fail += validateTeamsize(form.tsize.value)
			fail += validateWeek(form.week1.value)
			fail += validateWeek(form.week2.value)
			fail += validateWeek(form.week3.value)
			if (fail == "") return true
			else { alert(fail); return false }
		
		}
		document.write("<input type='submit' value='Register' />");
		</script>
		</form>
	</div>
	<noscript>
		<h1>JavaScript Disabled</h1>
		<p>This application will not work properly without JavaScript. Please enable it to continue.</p>
	</noscript>


<?php
	include 'bottom.phf';
	showFoot('2017.10.03b');
?>
