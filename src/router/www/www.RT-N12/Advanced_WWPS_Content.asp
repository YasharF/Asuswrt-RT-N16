<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>
<head>
<meta http-equiv="X-UA-Compatible" content="IE=EmulateIE7"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<title>ASUS Wireless Router <#Web_Title#> - <#menu5_1_2#></title>
<link rel="stylesheet" type="text/css" href="index_style.css"> 
<link rel="stylesheet" type="text/css" href="form_style.css">
<script type="text/javascript" src="/state.js"></script>
<script type="text/javascript" src="/help.js"></script>
<script type="text/javascript" src="/general.js"></script>
<script type="text/javascript" src="/popup.js"></script>
<script type="text/javascript" src="/ajax.js"></script>
<script type="text/javascript" src="/detect.js"></script>
<script>
wan_route_x = '<% nvram_get_x("IPConnection", "wan_route_x"); %>';
wan_nat_x = '<% nvram_get_x("IPConnection", "wan_nat_x"); %>';
wan_proto = '<% nvram_get_x("Layer3Forwarding",  "wan_proto"); %>';

<% login_state_hook(); %>
var wireless = [<% wl_auth_list(); %>];	// [[MAC, associated, authorized], ...]

var wps_mode = "<% nvram_get_x("WLANConfig11b", "wps_mode"); %>";
var wps_method = "<% nvram_get_x("WLANConfig11b", "wps_method"); %>";
var wps_config_state = "<% nvram_get_x("WLANConfig11b", "wps_config_state"); %>";
var wps_proc_status = "<% nvram_get_x("WLANConfig11b", "wps_proc_status"); %>";
var wl_auth_mode = "<% nvram_get_x("WLANConfig11b", "wl_auth_mode"); %>";
var wps_client_role = "<% nvram_get_x("WLANConfig11b", "wps_client_role"); %>"; // 2008.06.05 Lock  add

var secs;
var timerID = null;
var timerRunning = false;
var timeout = 2000;
var delay = 1000;
var stopFlag = 0;

function initial(){
	show_banner(1);
	if(sw_mode == "2")
		show_menu(5,1,1);
	else
		show_menu(5,1,2);
	show_footer();
	
	enable_auto_hint(13, 0);
	loadXML();
}

function applyRule(){
	showLoading();
	stopFlag = 1;
	document.form.action_mode.value = " Apply ";
	document.form.current_page.value = "/Advanced_WWPS_Content.asp";
	document.form.next_page.value = "";
	
	document.form.submit();
}

function done_validating(action){
	refreshpage();
}

//<!--------------------WPS code by Jiahao---------------------->
function ValidateChecksum(PIN)
{
	var accum = 0;

	accum += 3 * (parseInt(PIN / 10000000) % 10);
	accum += 1 * (parseInt(PIN / 1000000) % 10);
	accum += 3 * (parseInt(PIN / 100000) % 10);
	accum += 1 * (parseInt(PIN / 10000) % 10);
	accum += 3 * (parseInt(PIN / 1000) % 10);
	accum += 1 * (parseInt(PIN / 100) % 10);
	accum += 3 * (parseInt(PIN / 10) % 10);
	accum += 1 * (parseInt(PIN / 1) % 10);

	return ((accum % 10) == 0);
}

function PIN_PBC_Check(){
	if(document.form.wps_sta_pin.value != ""){
		if(document.form.wps_sta_pin.value.length != 8 || !ValidateChecksum(document.form.wps_sta_pin.value)){
			alert("<#JS_InvalidPIN#>");
			document.form.wps_sta_pin.focus();
			document.form.wps_sta_pin.select();
			return false;
		}
	}	
	
	return true;
}

function resetWPS(){
	//document.form.wps_config_state.value = "0";
	//document.form.wl_auth_mode.value = "open";
	//document.form.wl_wep_x.value = "0";
	showLoading(2);
	setTimeout("loadXML();", 1000);
//	stopFlag = 1;
	document.form.current_page.value = "/Advanced_WWPS_Content.asp";
	document.form.next_page.value = "";

	document.form.action_script.value = "WPS_PIN_RESET";
	document.form.submit();
	refreshpage();
}

function resetTimer(){
	if(stopFlag == 1){
		stopFlag = 0;
		InitializeTimer();
	}
}

function InitializeTimer()
{
	msecs = timeout;
	StopTheClock();
	StartTheTimer();
}

function StopTheClock()
{
	if(timerRunning)
		clearTimeout(timerID);
	timerRunning = false;
}

function StartTheTimer(){
	if(msecs == 0){
		StopTheClock();
		
		if(stopFlag == 1)
			return;
		
		updateWPS();
		msecs = timeout;
		StartTheTimer();
	}
	else{
		msecs = msecs - 500;
		timerRunning = true;
		timerID = setTimeout("StartTheTimer();", delay);
	}
}

function updateWPS()
{
	var ie = window.ActiveXObject;

	if (ie)
		makeRequest_ie('/WPS_info.asp');
	else
		makeRequest('/WPS_info.asp');
}

function loadXML()
{
	updateWPS();
	InitializeTimer();
}

function refresh_wpsinfo(xmldoc){
	var wpss = xmldoc.getElementsByTagName("wps");
	if(wpss == null || wpss[0] == null){
		stopFlag = 1;
		return;
	}
	
	var wps_infos = wpss[0].getElementsByTagName("wps_info");
	show_wps_status(wps_infos);
}

function show_wps_status(wps_infos_1){
	if(wps_mode == "enabled"){
		$("wps_mode_word").innerHTML = "<#btn_Enabled#>"
		$("enableWPSbtn").value = "<#btn_disable#>"
		
		
		if(wps_infos_1[1].firstChild.nodeValue == "1"){  						//In configured mode			
			if(wps_infos_1[1].firstChild.nodeValue != document.form.wps_config_state.value){					
					showLoading(2);
					location.href="/Advanced_WWPS_Content.asp";					
			}			
						
			$("devicePIN_tr").style.display = "";
			$("pin_tr").style.display = "";
			
			$("wps_sta_pin").style.display = "block";
			$("Reset_OOB").style.display = "block";
			
			if(wps_infos_1[2].firstChild.nodeValue == "1"){
				disableBtn(1);
				$("wps_state_td").innerHTML = "<#WPS_PBCbutton_hint_waiting#>"; // it will become enrollee within 2 mins.
			}
			else{
				disableBtn(0);
				$("wps_state_td").innerHTML = "<#wps_state_configured#>";
			}			
			
		}
		else{
			$("wps_state_td").innerHTML = "<#wps_state_unconfigured#>"
			$("devicePIN_tr").style.display = "";
			
			if(wps_infos_1[2].firstChild.nodeValue == "1"){ 		//wps_proc_status == 1 -> idle or config fail;
				$("wps_state_td").innerHTML = "<#wps_state_unconfigured#>"
				disableBtn(1);
			}
			else if(wps_infos_1[2].firstChild.nodeValue == "2"){
				$("wps_state_td").innerHTML = "<#WPS_state_configured_success#>"
				disableBtn(0);
			}
			else{
				$("wps_state_td").innerHTML = "<#WPS_state_Timeout_unconfigured#>"
				disableBtn(0);
			}
		}		
	}
	else{
		stopFlag = 1;
		
		if(wl_auth_mode == "shared" || wl_auth_mode == "wpa" || wl_auth_mode == "wpa2" || wl_auth_mode == "radius"){
			$("enableWPSbtn").disabled = 1;
			$("wps_mode_hint").innerHTML = "<#wsc_mode_hint1#><a href=\"Advanced_Wireless_Content.asp\"> <#menu5_1_1#></a> <#wsc_mode_hint2#>"
		}

		$("addEnrolleebtn").style.display = "none";
		$("wps_mode_word").innerHTML = "<#btn_Disabled#>"
		$("devicePIN_tr").style.display = "none";
		$("pin_tr").style.display = "none";		
		$("wps_state_td").innerHTML = "<#wps_state_not_enabled#>";
		$("enableWPSbtn").value = "<#btn_Enable#>"
	}
}

function disableBtn(X){
	$("addEnrolleebtn").disabled = X;
	$("Reset_OOB").disabled = X;	
}

function enableWPS(){
	var confirm_string = "";

	if(wps_mode == "enabled"){
		confirm_string = "<#WPS_disable_confirm#>";
	}
	else if(wps_mode == "disabled"){
		confirm_string = "<#WPS_enable_confirm#>"
	}
	else
		alert("Exceptional WPS error, Please root it!")	
	
	if(confirm(confirm_string)){
		if(wps_mode == "enabled"){
			document.form.wps_mode.value = "disabled";
		}
		else if(wps_mode == "disabled"){
			document.form.wps_mode.value = "enabled";
		}
		applyRule();
	}
	else
		return false;
}

function configCommand(){
	
	if(wps_config_state == 1){
		if(document.form.wps_sta_pin.value == ""){
			document.form.wps_client_role.value = "registrar";
		}
		else{
			if(!PIN_PBC_Check()){
				return false;				
			}
			document.form.action_script.value = "WPS_unconfig_addenr";
		}		
	}
	else if(wps_config_state == 0){
		if(document.form.wps_sta_pin.value != ""){			
			if(!PIN_PBC_Check()){
				return false;
			}
			else{
				document.form.action_script.value = "WPS_unconfig_addenr";
				document.form.wps_client_role.value = "enrollee";	
			}
		}
		else {
			document.form.wps_client_role.value = "registrar";
		}
	}

	document.form.wps_method.value = "1";
	document.form.wps_config_command.value = "1";

	showLoading(2);
	setTimeout("loadXML();", 3000);
	document.form.action_mode.value = "Update";
	document.form.current_page.value = "/Advanced_WWPS_Content.asp";
	document.form.next_page.value = "";
	
	document.form.submit();
}
</script>
</head>

<body onload="initial();" onunLoad="disable_auto_hint(13, 0);return unload_body();">
<div id="TopBanner"></div>

<div id="Loading" class="popup_bg"></div>

<iframe name="hidden_frame" id="hidden_frame" src="" width="0" height="0" frameborder="0"></iframe>
<form method="POST" name="form" id="ruleForm" action="/start_apply.htm" target="hidden_frame">

<table class="content" align="center" cellpadding="0" cellspacing="0">
	<tr>
		<td width="23">&nbsp;</td>		
		<td valign="top" width="202">
		<div  id="mainMenu"></div>
		<div  id="subMenu"></div>	
		</td>
    <td valign="top">
	<div id="tabMenu" class="submenuBlock"></div><br />
		<!--===================================Beginning of Main Content===========================================-->
<input type="hidden" name="current_page" value="/Advanced_WWPS_Content.asp">
<input type="hidden" name="next_page" value="">
<input type="hidden" name="next_host" value="">
<input type="hidden" name="sid_list" value="WLANConfig11b;">
<input type="hidden" name="group_id" value="">
<input type="hidden" name="modified" value="0">
<input type="hidden" name="action_mode" value="">
<input type="hidden" name="first_time" value="">
<input type="hidden" name="action_script" value="">
<input type="hidden" name="preferred_lang" id="preferred_lang" value="<% nvram_get_x("LANGUAGE", "preferred_lang"); %>">
<input type="hidden" name="wl_ssid2" value="<% nvram_char_to_ascii("WLANConfig11b",  "wl_ssid"); %>">
<input type="hidden" name="firmver" value="<% nvram_get_x("",  "firmver"); %>">

<input type="hidden" name="wps_mode" value="<% nvram_get_x("WLANConfig11b", "wps_mode"); %>">
<input type="hidden" name="wps_config_state" value="<% nvram_get_x("WLANConfig11b", "wps_config_state"); %>">
<input type="hidden" name="wps_method" value="<% nvram_get_x("WLANConfig11b", "wps_method"); %>">
<input type="hidden" name="wps_config_command" value="<% nvram_get_x("WLANConfig11b", "wps_config_command"); %>">
<input type="hidden" name="wps_client_role" value="<% nvram_get_x("WLANConfig11b", "wps_client_role"); %>">

<input type="hidden" name="wl_auth_mode" value="<% nvram_get_x("WLANConfig11b", "wl_auth_mode"); %>">
<input type="hidden" name="wl_wep_x" value="<% nvram_get_x("WLANConfig11b", "wl_wep_x"); %>">

<table width="98%" border="0" align="center" cellpadding="0" cellspacing="0">
	<tr>
		<td valign="top" >
		
<table width="500" border="0" align="center" cellpadding="5" cellspacing="0" class="FormTitle" table>
	<thead>
	<tr>
		<td><#menu5_1#> - <#t2WPS#></td>
	</tr>
	</thead>
	<tbody>
	<tr>
		<td bgcolor="#FFFFFF"><#WLANConfig11b_display6_sectiondesc#></td>
	</tr>
	</tbody>	
	<tr>
	  <td bgcolor="#FFFFFF">
		<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3"  class="FormTable">
			<tr>
			  <th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(13,1);"><#WLANConfig11b_x_WPS_itemname#></a></th>
			  <td>
			    <span style="color:#000;" id="wps_mode_word"></span>&nbsp;&nbsp;<input class="button" type="button" onClick="enableWPS();" name="enableWPSbtn" id="enableWPSbtn" value="">
				<br><span id="wps_mode_hint"></span>
		  	  </td>
			</tr>
			<tr>
			<th><#WLANConfig11b_x_WPSConf_itemname#></th>
			<td width="300" id="wps_state_td">&nbsp;</td>
			</tr>			
			<tr id="devicePIN_tr">
			  <th>
			  	<span id="devicePIN_name"><a class="hintstyle" href="javascript:void(0);" onclick="openHint(13,4);"><#WLANConfig11b_x_DevicePIN_itemname#></a></span>			  
			  </th>
			  <td>
			  	<input id="devicePIN" value="<% nvram_get_x("WLANConfig11b", "wps_device_pin"); %>" type="text" onmouseover="this.select();" class="devicepin" name="devicePIN" readonly="1" style="float:left;"></input>
			  </td>
			</tr>
			<tr id="pin_tr">
				<th>
			  	<span id="wpsPIN_name"><a class="hintstyle" href="javascript:void(0);" onclick="openHint(13,3);"><#WLANConfig11b_x_WPSPIN_itemname#></a></span>
			  </th>
			  <td>
			  	<input id="wps_sta_pin" type="text" size="8" maxlength="8"  class="inputpin" name="wps_sta_pin" value=""  <% nvram_match_x("WLANConfig11b","wps_method", "1", "checked"); %> style="float:left;">
			  				  	
			  </td>
			</tr>
			<tr>
				<td colspan="2" align="right">								
					<table>
						<tr>
						<td><input class="sbtn" type="button" onClick="configCommand();" id="addEnrolleebtn" name="addEnrolleebtn"  value="<#CTL_Add_enrollee#>"></td>
						<td><input class="button" type="button" onClick="resetWPS();" id="Reset_OOB" name="Reset_OOB" value="<#CTL_Reset_OOB#>" style="display:none; padding:0 0.3em 0 0.3em;" ></td>
						</tr>
					</table>
				</td>					
			</tr>
		</table>
	  </td>
	</tr>
</table>		
					
		</td>
</form>
		
          <td id="help_td" style="width:15px;" valign="top">
<form name="hint_form"></form>
            <div id="helpicon" onClick="openHint(0,0);" title="Click to open AiHelp">
            	<img src="images/help.gif"/>
            </div>
            <div id="hintofPM"  style="display:none; ">
              <table width="100%" cellpadding="0" cellspacing="1" class="Help" bgcolor="#999999">
			  	<thead>
                <tr>
                  <td><div id="helpname" class="AiHintTitle"></div><a href="javascript:void(0);" onclick="closeHint()" ><img src="images/button-close.gif" class="closebutton" /></a></td>
                </tr>
				</thead>				
                <tr>
                  <td valign="top" >
  				    <div class="hint_body2" id="hint_body"></div>
  				    <iframe id="statusframe" name="statusframe" class="statusframe" src="" frameborder="0"></iframe>
				</td>
                </tr>
              </table>
          </div>
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
