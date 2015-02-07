<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>
<head>
<meta http-equiv="X-UA-Compatible" content="IE=EmulateIE7"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<title>ASUS Wireless Router <#Web_Title#> - <#menu5_1_6#></title>
<link rel="stylesheet" type="text/css" href="index_style.css"> 
<link rel="stylesheet" type="text/css" href="form_style.css">
<script type="text/javascript" src="/state.js"></script>
<script type="text/javascript" src="/general.js"></script>
<script type="text/javascript" src="/help.js"></script>
<script type="text/javascript" src="/popup.js"></script>
<script type="text/javascript" src="/detect.js"></script>
<script>
wan_route_x = '<% nvram_get_x("IPConnection", "wan_route_x"); %>';
wan_nat_x = '<% nvram_get_x("IPConnection", "wan_nat_x"); %>';
wan_proto = '<% nvram_get_x("Layer3Forwarding",  "wan_proto"); %>';

<% login_state_hook(); %>
var wireless = [<% wl_auth_list(); %>];	// [[MAC, associated, authorized], ...]

function initial(){
	show_banner(1);
	if(sw_mode == "2"){
		show_menu(5,1,1);
		disableAdvFn(14);
	}
	else if(sw_mode == "3")
		show_menu(5,1,5);
	else
		show_menu(5,1,6);

	show_footer();
	
	enable_auto_hint(3, 19);

	load_body();
}

function applyRule(){
	if(validForm()){
		updateDateTime(document.form.current_page.value);
		
		showLoading();
		
		document.form.action_mode.value = " Apply ";
		document.form.next_page.value = "";
		
		document.form.submit();
	}
}

function validForm(){
	if(sw_mode != "2"){
		if(!validate_range(document.form.wl_frag, 256, 2346)
			|| !validate_range(document.form.wl_rts, 0, 2347)
			|| !validate_range(document.form.wl_dtim, 1, 255)
			|| !validate_range(document.form.wl_bcn, 1, 65535)
			)
			return false;
	}
	
	if(!validate_timerange(document.form.wl_radio_time_x_starthour, 0)
			|| !validate_timerange(document.form.wl_radio_time_x_startmin, 1)
			|| !validate_timerange(document.form.wl_radio_time_x_endhour, 2)
			|| !validate_timerange(document.form.wl_radio_time_x_endmin, 3)
			)
		return false;
	
	return true;
}

function done_validating(action){
	refreshpage();
}

function disableAdvFn(row){
	for(var i=row; i>=3; i--){
		$("WAdvTable").deleteRow(i);
	}
	$("WAdvTable").deleteRow(0);
}
</script>
</head>

<body onload="initial();" onunLoad="disable_auto_hint(3, 16);return unload_body();">
<div id="TopBanner"></div>

<div id="Loading" class="popup_bg"></div>

<iframe name="hidden_frame" id="hidden_frame" src="" width="0" height="0" frameborder="0"></iframe>
<form method="post" name="form" id="ruleForm" action="/start_apply.htm" target="hidden_frame">
<input type="hidden" name="productid" value="<% nvram_get_f("general.log","productid"); %>">
<input type="hidden" name="wan_route_x" value="<% nvram_get_x("IPConnection","wan_route_x"); %>">
<input type="hidden" name="wan_nat_x" value="<% nvram_get_x("IPConnection","wan_nat_x"); %>">

<input type="hidden" name="wl_gmode" value="<% nvram_get_x("WLANConfig11b","wl_gmode"); %>">
<input type="hidden" name="wl_gmode_protection_x" value="<% nvram_get_x("WLANConfig11b","wl_gmode_protection_x"); %>">

<input type="hidden" name="current_page" value="Advanced_WAdvanced_Content.asp">
<input type="hidden" name="next_page" value="SaveRestart.asp">
<input type="hidden" name="next_host" value="">
<input type="hidden" name="sid_list" value="WLANAuthentication11a;WLANConfig11b;LANHostConfig;">
<input type="hidden" name="group_id" value="">
<input type="hidden" name="modified" value="0">
<input type="hidden" name="action_mode" value="">
<input type="hidden" name="first_time" value="">
<input type="hidden" name="action_script" value="">
<input type="hidden" name="preferred_lang" id="preferred_lang" value="<% nvram_get_x("LANGUAGE", "preferred_lang"); %>">
<input type="hidden" name="wl_ssid2" value="<% nvram_char_to_ascii("WLANConfig11b", "wl_ssid"); %>">
<input type="hidden" name="firmver" value="<% nvram_get_x("",  "firmver"); %>">

<input type="hidden" name="wl_radio_date_x" value="<% nvram_get_x("WLANConfig11b","wl_radio_date_x"); %>">
<input type="hidden" name="wl_radio_time_x" value="<% nvram_get_x("WLANConfig11b","wl_radio_time_x"); %>">

<table class="content" align="center" cellpadding="0" cellspacing="0">
	<tr>
		<td width="23">&nbsp;</td>
		
		<td valign="top" width="202">				
		<div  id="mainMenu"></div>	
		<div  id="subMenu"></div>		
		</td>				
		
    <td valign="top">
	<div id="tabMenu" class="submenuBlock"></div>
		<br />
		<!--===================================Beginning of Main Content===========================================-->
<table width="98%" border="0" align="center" cellpadding="0" cellspacing="0">
	<tr>
		<td valign="top" >
		
<table width="98%" border="0" align="center" cellpadding="5" cellspacing="0" class="FormTitle" table>
	<thead>
	<tr>
		<td><#menu5_1#> - <#menu5_1_6#></td>
	</tr>
	</thead>
	<tbody>
	<tr>
		<td bgcolor="#FFFFFF"><#WLANConfig11b_display5_sectiondesc#></td>
	</tr>
	</tbody>	
	<tr>
	  <td bgcolor="#FFFFFF">
		<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3"  class="FormTable" id="WAdvTable">
			<tr>
			  <th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(3, 1);"><#WLANConfig11b_x_RadioEnable_itemname#></a></th>
			  <td>
			  	<input type="radio" value="1" name="wl_radio_x" class="input" onClick="return change_common_radio(this, 'WLANConfig11b', 'wl_radio_x', '1')" <% nvram_match_x("WLANConfig11b","wl_radio_x", "1", "checked"); %>><#checkbox_Yes#>
			    <input type="radio" value="0" name="wl_radio_x" class="input" onClick="return change_common_radio(this, 'WLANConfig11b', 'wl_radio_x', '0')" <% nvram_match_x("WLANConfig11b","wl_radio_x", "0", "checked"); %>><#checkbox_No#>
			  </td>
			</tr>
			<tr>
			  <th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(3, 2);"><#WLANConfig11b_x_RadioEnableDate_itemname#></a></th>
			  <td>
				<input type="checkbox" class="input" name="wl_radio_date_x_Sun" onChange="return changeDate();">Sun
				<input type="checkbox" class="input" name="wl_radio_date_x_Mon" onChange="return changeDate();">Mon
				<input type="checkbox" class="input" name="wl_radio_date_x_Tue" onChange="return changeDate();">Tue
				<input type="checkbox" class="input" name="wl_radio_date_x_Wed" onChange="return changeDate();">Wed
				<input type="checkbox" class="input" name="wl_radio_date_x_Thu" onChange="return changeDate();">Thu
				<input type="checkbox" class="input" name="wl_radio_date_x_Fri" onChange="return changeDate();">Fri
				<input type="checkbox" class="input" name="wl_radio_date_x_Sat" onChange="return changeDate();">Sat			  
			  </td>
			</tr>
			<tr>
			  <th><a class="hintstyle"  href="javascript:void(0);" onClick="openHint(3, 3);"><#WLANConfig11b_x_RadioEnableTime_itemname#></a></th>
			  <td>
			  	<input type="text" maxlength="2" class="input" size="2" name="wl_radio_time_x_starthour" onKeyPress="return is_number(this)">:
					<input type="text" maxlength="2" class="input" size="2" name="wl_radio_time_x_startmin" onKeyPress="return is_number(this)">-
					<input type="text" maxlength="2" class="input" size="2" name="wl_radio_time_x_endhour" onKeyPress="return is_number(this)">:
					<input type="text" maxlength="2" class="input" size="2" name="wl_radio_time_x_endmin" onKeyPress="return is_number(this)">
				</td>
			</tr>
		
			<tr>
			  <th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(3, 5);"><#WLANConfig11b_x_IsolateAP_itemname#></a></th>
			  <td>
				<input type="radio" value="1" name="wl_ap_isolate" class="input" onClick="return change_common_radio(this, 'WLANConfig11b', 'wl_ap_isolate', '1')" <% nvram_match_x("WLANConfig11b","wl_ap_isolate", "1", "checked"); %>>Yes
				<input type="radio" value="0" name="wl_ap_isolate" class="input" onClick="return change_common_radio(this, 'WLANConfig11b', 'wl_ap_isolate', '0')" <% nvram_match_x("WLANConfig11b","wl_ap_isolate", "0", "checked"); %>>No
			  </td>
			</tr>

			<tr>
				<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(3, 7);"><#WLANConfig11b_MultiRateAll_itemname#></a></th>
				<td>
					<select name="wl_mrate" class="input" onChange="return change_common(this, 'WLANConfig11b', 'wl_mrate')">
						<option value="0" <% nvram_match_x("WLANConfig11b", "wl_mrate", "0", "selected"); %>>Auto</option>
						<option value="1000000" <% nvram_match_x("WLANConfig11b", "wl_mrate", "1000000", "selected"); %>>1</option>
						<option value="2000000" <% nvram_match_x("WLANConfig11b", "wl_mrate", "2000000", "selected"); %>>2</option>
						<option value="5500000" <% nvram_match_x("WLANConfig11b", "wl_mrate", "5500000", "selected"); %>>5.5</option>
						<option value="6000000" <% nvram_match_x("WLANConfig11b", "wl_mrate", "6000000", "selected"); %>>6</option>
						<option value="9000000" <% nvram_match_x("WLANConfig11b", "wl_mrate", "9000000", "selected"); %>>9</option>
						<option value="11000000" <% nvram_match_x("WLANConfig11b", "wl_mrate", "11000000", "selected"); %>>11</option>
						<option value="12000000" <% nvram_match_x("WLANConfig11b", "wl_mrate", "12000000", "selected"); %>>12</option>
						<option value="18000000" <% nvram_match_x("WLANConfig11b", "wl_mrate", "18000000", "selected"); %>>18</option>
						<option value="24000000" <% nvram_match_x("WLANConfig11b", "wl_mrate", "24000000", "selected"); %>>24</option>
						<option value="36000000" <% nvram_match_x("WLANConfig11b", "wl_mrate", "36000000", "selected"); %>>36</option>
						<option value="48000000" <% nvram_match_x("WLANConfig11b", "wl_mrate", "48000000", "selected"); %>>48</option>
						<option value="54000000" <% nvram_match_x("WLANConfig11b", "wl_mrate", "54000000", "selected"); %>>54</option>
					</select>
				</td>
			</tr>
		
			<tr>
			  <th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(3, 8);"><#WLANConfig11b_DataRate_itemname#></a></th>
				<td>
					<select name="wl_rateset" class="input" onChange="return change_common(this, 'WLANConfig11b', 'wl_rateset')">
			   			<option value="default" <% nvram_match_x("WLANConfig11b","wl_rateset", "default","selected"); %>>Default</option>
				   		<option value="all" <% nvram_match_x("WLANConfig11b","wl_rateset", "all","selected"); %>>All</option>
					   	<option value="12" <% nvram_match_x("WLANConfig11b","wl_rateset", "12","selected"); %>>1, 2 Mbps</option>
					</select>
				</td>
			</tr>
			<tr>
			  <th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(3, 9);"><#WLANConfig11b_x_Frag_itemname#></a></th>
			  	<td>
			  		<input type="text" maxlength="5" size="5" name="wl_frag" class="input" value="<% nvram_get_x("WLANConfig11b", "wl_frag"); %>" onKeyPress="return is_number(this)" onChange="page_changed()" onBlur="validate_range(this, 256, 2346)">
				</td>
			</tr>
			<tr>
			  <th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(3, 10);"><#WLANConfig11b_x_RTS_itemname#></a></th>
			  	<td>
			  		<input type="text" maxlength="5" size="5" name="wl_rts" class="input" value="<% nvram_get_x("WLANConfig11b", "wl_rts"); %>" onKeyPress="return is_number(this)">
			  	</td>
			</tr>
			<tr>
			  <th><a class="hintstyle"  href="javascript:void(0);" onClick="openHint(3, 11);"><#WLANConfig11b_x_DTIM_itemname#></a></th>
				<td>
			  		<input type="text" maxlength="5" size="5" name="wl_dtim" class="input" value="<% nvram_get_x("WLANConfig11b", "wl_dtim"); %>" onKeyPress="return is_number(this)"  onBlur="validate_range(this, 1, 255)">
				</td>
			  
			</tr>
			<tr>
			  <th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(3, 12);"><#WLANConfig11b_x_Beacon_itemname#></a></th>
				<td>
					<input type="text" maxlength="5" size="5" name="wl_bcn" class="input" value="<% nvram_get_x("WLANConfig11b", "wl_bcn"); %>" onKeyPress="return is_number(this)" onBlur="validate_range(this, 1, 65535)">
				</td>
			</tr>
			<tr>
				<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(3, 13);"><#WLANConfig11b_x_FrameBurst_itemname#></a></th>
				<td>
					<select name="wl_frameburst" class="input">
						<option value="off" <% nvram_match_x("WLANConfig11b","wl_frameburst", "off","selected"); %>><#WLANConfig11b_WirelessCtrl_buttonname#></option>
						<option value="on" <% nvram_match_x("WLANConfig11b","wl_frameburst", "on","selected"); %>><#WLANConfig11b_WirelessCtrl_button1name#></option>
					</select>
				</td>
			</tr>			
			<tr>
				<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(3, 20);"><#WLANConfig11n_PremblesType_itemname#></a></th>
				<td>
					<select name="wl_plcphdr" class="input">
						<option value="short" <% nvram_match_x("WLANConfig11b","wl_plcphdr", "short","selected"); %>>Short</option>
						<option value="long" <% nvram_match_x("WLANConfig11b","wl_plcphdr", "long","selected"); %>>Long</option>
					</select>
				</td>
			</tr>

			<tr>
				<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(3, 19);"><#WLANConfig11b_x_HT_OpMode_itemname#></a></th>
				<td>
					<select name="wl_mimo_preamble"  class="input" onChange="return change_common(this, 'WLANConfig11b', 'wl_mimo_preamble')">
						<option value="gf" <% nvram_match_x("WLANConfig11b","wl_mimo_preamble", "gf","selected"); %>>Green Field</option>
						<option value="mm" <% nvram_match_x("WLANConfig11b","wl_mimo_preamble", "mm","selected"); %>>Mixed Mode</option>
					</select>				
				</td>
			</tr>
			
			<tr>
			  <th><a class="hintstyle"  href="javascript:void(0);" onClick="openHint(3, 14);"><#WLANConfig11b_x_WMM_itemname#></a></th>
			  <td>
				<select name="wl_wme" class="input" onChange="return change_common(this, 'WLANConfig11b', 'wl_wme')">
			  	  <option value="off" <% nvram_match_x("WLANConfig11b","wl_wme", "off","selected"); %>><#WLANConfig11b_WirelessCtrl_buttonname#></option>
			  	  <option value="on" <% nvram_match_x("WLANConfig11b","wl_wme", "on","selected"); %>><#WLANConfig11b_WirelessCtrl_button1name#></option>
				</select>
			  </td>
			</tr>
			<tr>
			  <th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(3,15);"><#WLANConfig11b_x_NOACK_itemname#></a></th>
			  <td>
				<select name="wl_wme_no_ack"class="input" onChange="return change_common(this, 'WLANConfig11b', 'wl_wme_no_ack')">
			  	  <option value="off" <% nvram_match_x("WLANConfig11b","wl_wme_no_ack", "off","selected"); %>><#WLANConfig11b_WirelessCtrl_buttonname#></option>
			  	  <option value="on" <% nvram_match_x("WLANConfig11b","wl_wme_no_ack", "on","selected"); %>><#WLANConfig11b_WirelessCtrl_button1name#></option>
				</select>
			  </td>
			</tr>
			<tr align="right">
				<td colspan="2"><input class="button" onclick="applyRule();" type="button" value="<#CTL_apply#>"/></td>
			</tr>
		</table>
		</td>
	</tr>
</table>		
</td>
</form>

	<!--==============Beginning of hint content=============-->
	<td id="help_td" style="width:15px;" valign="top">
<form name="hint_form"></form>
	  <div id="helpicon" onClick="openHint(0, 0);" title="<#Help_button_default_hint#>">
		<img src="images/help.gif">
	  </div>
	  
	  <div id="hintofPM" style="display:none;">
		<table width="100%" cellpadding="0" cellspacing="1" class="Help" bgcolor="#999999">
		  <thead>
		  <tr>
			<td>
			  <div id="helpname" class="AiHintTitle"></div>
			  <a href="javascript:closeHint();"><img src="images/button-close.gif" class="closebutton" /></a>
			</td>
		  </tr>
		  </thead>
		  
		  <tbody>
		  <tr>
			<td valign="top">
			  <div id="hint_body" class="hint_body2"></div>
			  <iframe id="statusframe" name="statusframe" class="statusframe" src="" frameborder="0"></iframe>
			</td>
		  </tr>
		  </tbody>
		</table>
	  </div>
	</td>
	<!--==============Ending of hint content=============-->
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
