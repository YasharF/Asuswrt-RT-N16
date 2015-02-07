<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<title>Mod New Account</title>
<link rel="stylesheet" href="../form_style.css"  type="text/css">

<script type="text/javascript" src="../state.js"></script>
<script type="text/javascript">
var selectedAccount = parent.getSelectedAccount();

function initial(){
	$("new_account").value = selectedAccount;
	
	showtext($("selected_account"), selectedAccount);
	
	clickevent();
}

function clickevent(){
	$("Submit").onclick = function(){
			if(validForm()){
				$("account").value = selectedAccount;
				
				parent.showLoading();
				document.modifyAccountForm.submit();
				parent.hidePop("apply");
			}
		};
}

function validForm(){
	if($("new_account").value.length > 0)
		$("new_account").value = trim($("new_account").value);
	if($("new_password").value.length > 0)
		$("new_password").value = trim($("new_password").value);
	$("confirm_password").value = trim($("confirm_password").value);
	
	// new_account name
	if($("new_account").value.length > 0){
		if(trim($("new_account").value).length > 20){
			alert("<#File_Pop_content_alert_desc3#>");
			$("new_account").focus();
			return false;
		}
		
		var re = new RegExp("[^a-zA-Z0-9-]+","gi");
		if(re.test($("new_account").value)){
			alert("<#File_Pop_content_alert_desc4#>");
			$("new_account").focus();
			return false;
		}
		
		if(checkDuplicateName($("new_account").value, parent.get_accounts()) &&
				$("new_account").value != selectedAccount){
			alert("<#File_Pop_content_alert_desc5#>");
			$("new_account").focus();
			return false;
		}
	}
	
	// password
	if($("new_password").value != $("confirm_password").value){
		alert("<#File_Pop_content_alert_desc7#>");
		
		if($("new_password").value.length <= 0)
			$("new_password").focus();
		else
			$("confirm_password").focus();
		return false;
	}
	
	if($("new_account").value.length <= 0 && $("new_password").value.length <= 0){
		alert("並無輸入新的帳號或新的密碼!!");
		
		return false;
	}
		
	return true;
}

function checkDuplicateName(newname, teststr){
	var existing_string = teststr.join(',');
	existing_string = "," + existing_string + ",";
	var newstr = "," + trim(newname) + ","; 

	var re = new RegExp(newstr,"gi")
	var matchArray =  existing_string.match(re);
	if (matchArray != null)
		return true;
	else
		return false;
}
</script>
</head>

<body onLoad="initial();">
<form method="post" name="modifyAccountForm" action="modify_account.asp" target="hidden_frame">
<input name="account" id="account" type="hidden" value="">
	<table width="90%" class="popTable" border="0" align="center" cellpadding="0" cellspacing="0">
	<thead>
    <tr>
      <td colspan="2"><span><#ModAccountTitle#>: </span><span id="selected_account"></span><img src="../images/button-close.gif" onClick="parent.hidePop('OverlayMask');"></td>
    </tr>
    </thead>	
	<tbody>
    <tr valign="middle">
      <td height="30" colspan="2" class="hint_word"><#ModAccountAlert#></td>
    </tr>
    <tr>
      <th><#AiDisk_Account#>: </th>
      <td ><input name="new_account" id="new_account" type="text"></td>
    </tr>
    <tr>
      <th><#ModAccountPassword#>: </th>
      <td><input name="new_password" id="new_password" type="password"></td>
    </tr>
    <tr>
      <th><#Confirmpassword#>: </th>
      <td><input id="confirm_password" type="password"></td>
    </tr>
	</tbody>	
    <tr bgcolor="#E6E6E6">
      <th colspan="2" align="right"><input id="Submit" type="button" class="button" value="<#CTL_modify#>"></th>
    </tr>
  </table>
</form>
</body>
</html>
