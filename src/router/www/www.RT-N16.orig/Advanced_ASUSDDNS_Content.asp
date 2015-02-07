﻿<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>
<head>
<meta http-equiv="X-UA-Compatible" content="IE=EmulateIE7"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<title>ASUS Wireless Router <#Web_Title#> - <#menu5_3_6#></title>
<link rel="stylesheet" type="text/css" href="index_style.css"> 
<link rel="stylesheet" type="text/css" href="form_style.css">

<script language="JavaScript" type="text/javascript" src="/state.js"></script>
<script language="JavaScript" type="text/javascript" src="/general.js"></script>
<script language="JavaScript" type="text/javascript" src="/popup.js"></script>
<script type="text/javascript" language="JavaScript" src="/help.js"></script>
<script type="text/javascript" language="JavaScript" src="/detect.js"></script>
<script>
wan_route_x = '<% nvram_get_x("IPConnection", "wan_route_x"); %>';
wan_nat_x = '<% nvram_get_x("IPConnection", "wan_nat_x"); %>';
wan_proto = '<% nvram_get_x("Layer3Forwarding",  "wan_proto"); %>';

<% login_state_hook(); %>
var wireless = [<% wl_auth_list(); %>];	// [[MAC, associated, authorized], ...]

function ddns_load_body(){
	var ddns_timeout = '<% nvram_get_ddns("LANHostConfig","ddns_timeout"); %>';
	var hostname_x = '<% nvram_get_x("LANHostConfig","ddns_hostname_x"); %>';
	var ddns_return_code = '<% nvram_get_ddns("LANHostConfig","ddns_return_code"); %>';
	var ddns_old_name = '<% nvram_get_x("LANHostConfig","ddns_old_name"); %>';
	var ddns_server_x = '<% nvram_get_x("LANHostConfig","ddns_server_x"); %>';
	
	hideLoading();
	if(ddns_return_code == ''){
		if(ddns_timeout == '1')
			alert("<#LANHostConfig_x_DDNS_alarm_1#>");
	}
	else if(ddns_return_code == 'register,-1')
		alert("<#LANHostConfig_x_DDNS_alarm_2#>");
	else if(ddns_return_code == 'register,200')
		alert("<#LANHostConfig_x_DDNS_alarm_3#>");
	else if(ddns_return_code == 'register,203')
		alert("<#LANHostConfig_x_DDNS_alarm_hostname#> '"+hostname_x+"' <#LANHostConfig_x_DDNS_alarm_registered#>");
	else if(ddns_return_code == 'register,220')
		alert("<#LANHostConfig_x_DDNS_alarm_4#>");
	else if(ddns_return_code == 'register,230')
		alert("<#LANHostConfig_x_DDNS_alarm_5#>");
	else if(ddns_return_code == 'register,233')
		alert("<#LANHostConfig_x_DDNS_alarm_hostname#> '"+hostname_x+"' <#LANHostConfig_x_DDNS_alarm_registered_2#> '"+ddns_old_name+"'.");
	else if(ddns_return_code == 'register,296')
		alert("<#LANHostConfig_x_DDNS_alarm_6#>");
	else if(ddns_return_code == 'register,297')
		alert("<#LANHostConfig_x_DDNS_alarm_7#>");
	else if(ddns_return_code == 'register,298')
		alert("<#LANHostConfig_x_DDNS_alarm_8#>");
	else if(ddns_return_code == 'register,299')
		alert("<#LANHostConfig_x_DDNS_alarm_9#>");
	else if(ddns_return_code == 'register,401')
		alert("<#LANHostConfig_x_DDNS_alarm_10#>");
	else if(ddns_return_code == 'register,407')
		alert("<#LANHostConfig_x_DDNS_alarm_11#>");
	else if(ddns_return_code == 'Time-out')
		alert("<#LANHostConfig_x_DDNS_alarm_12#>");
	
	if(ddns_server_x == "WWW.ASUS.COM" || ddns_server_x == ""){
		inputCtrl(document.form.ddns_username_x, 0);
		inputCtrl(document.form.ddns_passwd_x, 0);
		document.form.ddns_wildcard_x[0].disabled= 1;
		document.form.ddns_wildcard_x[1].disabled= 1;
		document.form.LANHostConfig_x_DDNSHostnameCheck_button.disabled = 0;
		showhide("link", 0);
		showhide("Hostname_Note", 1);
	}
	else{
		inputCtrl(document.form.ddns_username_x, 1);
		inputCtrl(document.form.ddns_passwd_x, 1);
		document.form.ddns_wildcard_x[0].disabled = 0;
		document.form.ddns_wildcard_x[1].disabled = 0;
		document.form.LANHostConfig_x_DDNSHostnameCheck_button.disabled = 1;
		showhide("Hostname_Note", 0);
	}
}

function applyRule(){
	if(validForm()){
		with(document.form){
			if(ddns_enable_x[0].checked && ddns_server_x.value == 'WWW.ASUS.COM' && ddns_hostname_x.value.length > '.asuscomm.com'.length
				&& !confirm('<#ddnsname_computername#>'))
				return;
		}
		
		showLoading();
		
		document.form.action_mode.value = " Apply ";
		document.form.current_page.value = "/as.asp";
		document.form.next_page.value = "";
		
		document.form.submit();
	}
}

function validForm(){
	if(document.form.ddns_enable_x[0].checked){
		if(document.form.ddns_server_x.value == "WWW.ASUS.COM"){
			if(!validate_string(document.form.ddns_hostname_x)
					|| !validate_ddns_hostname(document.form.ddns_hostname_x))
				return false;
		}
		else{
			if(!validate_string(document.form.ddns_username_x)
					|| !validate_string(document.form.ddns_passwd_x))
				return false;
		}
		
		if(document.form.ddns_hostname_x.value == ""){
			alert("<#LANHostConfig_x_DDNS_alarm_14#>");
			document.form.ddns_hostname_x.focus();
			document.form.ddns_hostname_x.select();
			return false;
		}
		else if(document.form.ddns_server_x.selectedIndex != 0 && document.form.ddns_username_x.value == ""){
			alert("<#QKSet_account_nameblank#>");
			document.form.ddns_username_x.focus();
			document.form.ddns_username_x.select();
			return false;
		}
		else if(document.form.ddns_server_x.selectedIndex != 0 && document.form.ddns_passwd_x.value == ""){
			alert("<#File_Pop_content_alert_desc6#>");
			document.form.ddns_passwd_x.focus();
			document.form.ddns_passwd_x.select();
			return false;
		}
		else
			return true;
	}
	else
		return true;
}
</script>
</head>

<body onload="show_banner(1); show_menu(5,3,6); show_footer();ddns_load_body();" onunLoad="return unload_body();">
<div id="TopBanner"></div>

<div id="Loading" class="popup_bg"></div>

<iframe name="hidden_frame" id="hidden_frame" src="" width="0" height="0" frameborder="0"></iframe>
<form method="post" name="form" action="/start_apply.htm" target="hidden_frame">
<input type="hidden" name="productid" value="<% nvram_get_f("general.log","productid"); %>">
<input type="hidden" name="wan_route_x" value="<% nvram_get_x("IPConnection","wan_route_x"); %>">
<input type="hidden" name="wan_nat_x" value="<% nvram_get_x("IPConnection","wan_nat_x"); %>">

<input type="hidden" name="current_page" value="Advanced_ASUSDDNS_Content.asp">
<input type="hidden" name="next_page" value="">
<input type="hidden" name="next_host" value="">
<input type="hidden" name="sid_list" value="LANHostConfig;">
<input type="hidden" name="group_id" value="">
<input type="hidden" name="modified" value="0">
<input type="hidden" name="action_mode" value="">
<input type="hidden" name="first_time" value="">
<input type="hidden" name="action_script" value="">

<input type="hidden" name="flag" value="">

<input type="hidden" name="preferred_lang" id="preferred_lang" value="<% nvram_get_x("LANGUAGE", "preferred_lang"); %>">
<input type="hidden" name="ssid_acsii" value="<% nvram_char_to_ascii("WLANConfig11b",  "wl_ssid"); %>">
<input type="hidden" name="firmver" value="<% nvram_get_x("",  "firmver"); %>">

<table class="content" align="center" cellpadding="0" cellspacing="0">
	<tr>
		<td width="23">&nbsp;</td>
		
		<td valign="top" width="202">				
		<div  id="mainMenu"></div>	
		<div  id="subMenu"></div>		
		</td>				
		
    <td valign="top">
		<div id="tabMenu" class="submenuBlock">
		</div><br />
		<!--===================================Beginning of Main Content===========================================-->
<table width="98%" border="0" align="center" cellpadding="0" cellspacing="0">
	<tr>
		<td align="left" valign="top" >
          
		<table width="550" border="0" align="center" cellpadding="5" cellspacing="0" class="FormTitle">
			<thead>
			<tr>
				<td><#menu5_3#> - <#menu5_3_6#></td>
			</tr>
			</thead>	
            <tr>
              <td bgcolor="#FFFFFF"><#LANHostConfig_x_DDNSEnable_sectiondesc#></td>
            </tr>
            <tr>
				<td bgcolor="#FFFFFF">
		<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3"  class="FormTable">
				<input type="hidden" name="wl_gmode_protection_x" value="<% nvram_get_x("WLANConfig11b","wl_gmode_protection_x"); %>">
			<tr>
				<th width="200"><#LANHostConfig_x_DDNSEnable_itemname#></th>
				<td>
				<input type="radio" value="1" name="ddns_enable_x"onClick="return change_common_radio(this, 'LANHostConfig', 'ddns_enable_x', '1')" <% nvram_match_x("LANHostConfig","ddns_enable_x", "1", "checked"); %>><#checkbox_Yes#>
				<input type="radio" value="0" name="ddns_enable_x"onClick="return change_common_radio(this, 'LANHostConfig', 'ddns_enable_x', '0')" <% nvram_match_x("LANHostConfig","ddns_enable_x", "0", "checked"); %>><#checkbox_No#>
			</tr>
			<tr>
				<th><#LANHostConfig_x_DDNSServer_itemname#></th>
				<td>
                  <select name="ddns_server_x"class="input"onchange="return change_common(this, 'LANHostConfig', 'ddns_server_x')">
                    <!-- 2007.03.20 Yau add "WWW.ASUS.COM" -->
                    <option value="WWW.ASUS.COM" <% nvram_match_x("LANHostConfig","ddns_server_x", "WWW.ASUS.COM","selected"); %>>WWW.ASUS.COM</option>
                    <option value="WWW.DYNDNS.ORG" <% nvram_match_x("LANHostConfig","ddns_server_x", "WWW.DYNDNS.ORG","selected"); %>>WWW.DYNDNS.ORG</option>
                    <option value="WWW.DYNDNS.ORG(CUSTOM)" <% nvram_match_x("LANHostConfig","ddns_server_x", "WWW.DYNDNS.ORG(CUSTOM)","selected"); %>>WWW.DYNDNS.ORG(CUSTOM)</option>
                    <option value="WWW.DYNDNS.ORG(STATIC)" <% nvram_match_x("LANHostConfig","ddns_server_x", "WWW.DYNDNS.ORG(STATIC)","selected"); %>>WWW.DYNDNS.ORG(STATIC)</option>
                    <option value="WWW.TZO.COM" <% nvram_match_x("LANHostConfig","ddns_server_x", "WWW.TZO.COM","selected"); %>>WWW.TZO.COM</option>
                    <option value="WWW.ZONEEDIT.COM" <% nvram_match_x("LANHostConfig","ddns_server_x", "WWW.ZONEEDIT.COM","selected"); %>>WWW.ZONEEDIT.COM</option>
                  </select>
                  <div id="link">
<a href="javascript:openLink('x_DDNSServer')" class="content_input_link" name="x_DDNSServer_link">
<#LANHostConfig_x_DDNSServer_linkname#>
</a>
</div>
				</td>
			</tr>
			<tr>
				<th><#LANHostConfig_x_DDNSUserName_itemname#></th>
				<td><input type="text" maxlength="32" class="input" size="32" name="ddns_username_x" value="<% nvram_get_x("LANHostConfig","ddns_username_x"); %>" onKeyPress="return is_string(this)"></td>
			</tr>
			<tr>
				<th><#LANHostConfig_x_DDNSPassword_itemname#></th>
				<td><input type="password" maxlength="64" class="input" size="32" name="ddns_passwd_x" value="<% nvram_get_x("LANHostConfig","ddns_passwd_x"); %>"></td>
			</tr>
			<tr>
				<th><#LANHostConfig_x_DDNSHostNames_itemname#></th>
				<td>
				<input type="text" maxlength="63" class="input" size="32" name="ddns_hostname_x" value="<% nvram_get_x("LANHostConfig","ddns_hostname_x"); %>" onKeyPress="return is_string(this)">
				<input type="submit" maxlength="15" class="button" onClick="showLoading();return onSubmitApply('hostname_check');" size="12" name="LANHostConfig_x_DDNSHostnameCheck_button" value="<#LANHostConfig_x_DDNSHostnameCheck_buttonname#>"><br />
				<div id="Hostname_Note"><span><#LANHostConfig_x_DDNSHostNames_Note#></span></div>
				</td>
			</tr>
			<tr>
				<th><#LANHostConfig_x_DDNSWildcard_itemname#></th>
				<td><input type="radio" value="1" name="ddns_wildcard_x" onClick="return change_common_radio(this, 'LANHostConfig', 'ddns_wildcard_x', '1')" <% nvram_match_x("LANHostConfig","ddns_wildcard_x", "1", "checked"); %>><#checkbox_Yes#>
				<input type="radio" value="0" name="ddns_wildcard_x" onClick="return change_common_radio(this, 'LANHostConfig', 'ddns_wildcard_x', '0')" <% nvram_match_x("LANHostConfig","ddns_wildcard_x", "0", "checked"); %>><#checkbox_No#>
				</td>
			</tr>
			<tr>
				<th><#LANHostConfig_x_DDNSStatus_itemname#></th>
				<td><input type="hidden" maxlength="15" class="button" size="12" name="" value="<% nvram_get_f("ddns.log","DDNSStatus"); %>">
				  <input type="submit" maxlength="15" class="button" onclick="showLoading();return onSubmitApply('ddnsclient');" size="12" name="LANHostConfig_x_DDNSStatus_button" value="<#LANHostConfig_x_DDNSStatus_buttonname#>" /></td>
			</tr>
			<tr align="right">
				<td colspan="2">
				   <input class="button" onclick="applyRule();" type="button" value="<#CTL_apply#>" /></td>
			</tr>															
		</table>
			  </td>
              </tr>
            </table>
		  </td>
</form>

	<td id="help_td" style="width:15px;" valign="top">
		  
	  <div id="helpicon" onClick="openHint(0,0);" title="<#Help_button_default_hint#>"><img src="images/help.gif" /></div>
	  <div id="hintofPM" style="display:none;">
<form name="hint_form"></form>
	    <table width="100%" cellpadding="0" cellspacing="1" class="Help" bgcolor="#999999">
		  <thead>
		  <tr>
			<td>
			  <div id="helpname" class="AiHintTitle"></div>
			  <a href="javascript:;" onclick="closeHint()" ><img src="images/button-close.gif" class="closebutton" /></a>
			</td>
		  </tr>
		  </thead>
		  
		  <tr>				
			<td valign="top" >
			  <div class="hint_body2" id="hint_body"></div>
			  <iframe id="statusframe" name="statusframe" class="statusframe" src="" frameborder="0"></iframe>
			</td>
		  </tr>
		</table>
	  </div><!--End of hintofPM-->
		  </td>
        </tr>
      </table>				
		<!--===================================Ending of Main Content===========================================-->		
	</td>
		
    <td width="10" align="center" valign="top">&nbsp;</td>
	</tr>
</table>

<div id="footer"></div>
</body>
</html>
