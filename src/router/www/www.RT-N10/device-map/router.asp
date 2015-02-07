<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<title>Untitled Document</title>
<link href="/NM_style.css" rel="stylesheet" type="text/css" />
<link href="/form_style.css" rel="stylesheet" type="text/css" />

<script type="text/javascript" src="/state.js"></script>
<script type="text/javascript" src="/general.js"></script>
<script type="text/javascript" src="formcontrol.js"></script>
<script type="text/javascript" src="/ajax.js"></script>
<script type="text/javascript" src="/mssid.js"></script>
<script>
var had_wrong_wep_key = false;

var wps_mode = '<% nvram_get_x("WLANConfig11b", "wps_mode"); %>';
var wps_method = '<% nvram_get_x("WLANConfig11b", "wps_method"); %>';
var wps_proc_status = '<% nvram_get_x("WLANConfig11b", "wps_proc_status"); %>';
var wps_config_state = '<% nvram_get_x("WLANConfig11b", "wps_config_state"); %>';
var wps_waiting = '<% nvram_get_x("WLANConfig11b", "wps_waiting"); %>';
var wps_timer_start = '<% nvram_get_x("WLANConfig11b", "wps_timer_start"); %>'; 
var ID_of_startPBC = null;

var secs;
var timerID = null;
var timerRunning = false;
var timeout = 1000;
var delay = 500;
var stopFlag=0;

function initial(){

	flash_button();
	loadXML();
	
	document.form.wl_ssid.value = decodeURIComponent(document.form.wl_ssid2.value);
	document.form.wl_wpa_psk.value = decodeURIComponent(document.form.wl_wpa_psk_org.value);
	document.form.wl_key1.value = decodeURIComponent(document.form.wl_key1_org.value);
	document.form.wl_key2.value = decodeURIComponent(document.form.wl_key2_org.value);
	document.form.wl_key3.value = decodeURIComponent(document.form.wl_key3_org.value);
	document.form.wl_key4.value = decodeURIComponent(document.form.wl_key4_org.value);
	
	//if(document.form.wl_wpa_psk.value.length <= 0)
	//	document.form.wl_wpa_psk.value = "Please type Password";
	
	//if(isModel() == "SnapAP" || isCard() == 'ralink')
	//	wl_auth_mode_reconf();
	
	wl_auth_mode_change(1);
	show_LAN_info();
	domore_create();
	
	if(wps_proc_status == "1" && wps_mode == "enabled"){
		PBC_shining();
	}

	if(free_mssid){
		$("Add-SSID-btn").style.visibility = "visible";
		if(free_mssid == 1){
			$("mssid_arrow").style.visibility = "hidden";
		}
	}
}

function PBC_shining(){
	$("wpsPBC_button").src = "/images/EZSetup_button_s.gif";
	$("wpsPBC_button").onmousedown = function(){};
}

function PBC_normal(){
	$("wpsPBC_button").src = "/images/EZSetup_button.gif";
	$("wpsPBC_button").onmousedown = function(){
			$("wpsPBC_button").src = "/images/EZSetup_button_0.gif";
		};
}

function domore_create(){
	var option_AP = new Array();
	option_AP[0] = document.createElement("option");
	option_AP[0].text = "<#menu5_7_4#>";
	option_AP[0].value = "../Main_WStatus_Content.asp"
	
	if(parent.wan_route_x == "IP_Bridged"){
		$("Router_domore").remove(6);
		$("Router_domore").remove(5);
		$("Router_domore").remove(4);
		$("Router_domore").options[3].value="../Advanced_APLAN_Content.asp";
		try{
    	$("Router_domore").add(option_AP[0],null);; // standards compliant
    }
  	catch(ex){
    	$("Router_domore").add(option_AP[0],4); // IE only
    }
	}
}

function show_LAN_info(){
	showtext($("LANIP"), '<% nvram_get_x("LANHostConfig", "lan_ipaddr_t"); %>');
	//showtext($("PINCode"), '<% nvram_get_x("", "secret_code"); %>'); //remove for height issue;
	showtext($("MAC"), '<% nvram_get_x("", "et0macaddr"); %>');
}

function show_wepkey_help(){
	if(document.form.wl_wep_x.value == 1)
		parent.showHelpofDrSurf(0, 12);
	else if(document.form.wl_wep_x.value == 2)
		parent.showHelpofDrSurf(0, 13);
}

function resetTimer()
{
	if (stopFlag==1)
	{
		stopFlag=0;
		InitializeTimer();
	}
}

function InitializeTimer()
{
	if(document.form.wl_auth_mode.value == "shared"
		|| document.form.wl_auth_mode.value == "wpa"
		|| document.form.wl_auth_mode.value == "wpa2"
		|| document.form.wl_auth_mode.value == "radius")
		return;

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
		msecs = msecs-500;
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
	var wpss=xmldoc.getElementsByTagName("wps");
	if(wpss == null || wpss[0] == null){
		stopFlag = 1;
		return;
	}

	var wps_infos=wpss[0].getElementsByTagName("wps_info");
	
	if(wps_mode == "disabled"){
		document.getElementById("WPS_PBCbutton_hint_span").innerHTML = "<a href='../Advanced_WWPS_Content.asp' target='_parent'><#WPS_PBCbutton_hint_notEnable#></a>"
		$("wpsPBC_button").title = "<#WPS_PBCbutton_hint_notEnable#>";
	}
	else{
		if (wps_infos[2].firstChild.nodeValue == "1"){
			document.getElementById("WPS_PBCbutton_hint_span").innerHTML = "<#WPS_PBCbutton_hint_waiting#>";
			if($("wpsPBC_button").src != "/images/EZSetup_button_s.gif")
				$("wpsPBC_button").src = "/images/EZSetup_button_s.gif";
		}
		else if (wps_infos[2].firstChild.nodeValue == "2"){
			document.getElementById("WPS_PBCbutton_hint_span").innerHTML = "<#WPS_PBCbutton_hint_success#>";
			PBC_normal();
		}
		else{
			$("WPS_PBCbutton_hint_span").innerHTML = "<#WPS_PBCbutton_hint_timeout#>";
			PBC_normal();
		}
	}
}

function submitForm(){
	var auth_mode = document.form.wl_auth_mode.value;
		
	if(!validate_string_ssid(document.form.wl_ssid))
		return false;
	
	if(auth_mode == "psk" || auth_mode == "wpa" || auth_mode == "wpa2"){
		if(!validate_psk(document.form.wl_wpa_psk))
			return false;
	}
	else{
		if(auth_mode!="radius" && !validate_wlkey(document.form.wl_asuskey1))
			return false;
	}
	
	stopFlag = 1;
	
	document.form.current_page.value = "";
	document.form.next_page.value = "/";
	document.form.action_mode.value = " Apply ";
	
	var wep11 = eval('document.form.wl_key'+document.form.wl_key.value);
	wep11.value = document.form.wl_asuskey1.value;
	
	if((auth_mode == "shared" || auth_mode == "wpa" || auth_mode == "wpa2" || auth_mode == "radius")
			&& document.form.wps_enable.value == "1"){
		document.form.wps_enable.value = "0";
		document.form.action_script.value = "WPS_apply";
	}
	document.form.wsc_config_state.value = "1";
	
	if(auth_mode == "wpa" || auth_mode == "wpa2" || auth_mode == "radius"){
		document.form.target = "";
		document.form.next_page.value = "/Advanced_WSecurity_Content.asp";
	}
	
	parent.showLoading(30);
	document.form.submit();
	
	return true;
}

function startPBCmethod(){
	stopFlag = 1;
	document.WPSForm.action_script.value = "WPS_push_button";
	document.WPSForm.current_page.value = "/";
	document.WPSForm.submit();	
}

function wpsPBC(obj){
	obj.src = "/images/EZSetup_button_0.gif";
	
	if(wps_mode == "enabled"){
		if(wps_method == 1){ //If in unconfigured PIN mode
			if(wps_waiting == 1 && wps_timer_start == 1){ // If in proceeding
				stopFlag=1;
				parent.showLoading();
				document.stopPINForm.wps_config_command.value = "2"; //Interrupt PIN method
				document.stopPINForm.action_mode.value = "Update";
				document.stopPINForm.submit();
				clearTimeout(ID_of_startPBC);
				ID_of_startPBC = setTimeout("startPBCmethod();", 2000); // start the process of PIN mode after 2secs.
			}
			else{
				parent.showLoading(1);
				//startPBCmethod();
				clearTimeout(ID_of_startPBC);
				ID_of_startPBC = setTimeout("startPBCmethod();", 1);//*/
			}
		}
		else{
			document.WPSForm.pbc_overlap.value = "1";
			if(wps_waiting == 1 && wps_timer_start == 1){ // If in proceeding
				stopFlag=1;
				parent.showLoading(1);
				//startPBCmethod();
				clearTimeout(ID_of_startPBC);
				ID_of_startPBC = setTimeout("startPBCmethod();", 1);//*/
			}
			else{
				stopFlag = 1;
				parent.showLoading(8);
				//setTimeout("startPBCmethod();", 7000);
				clearTimeout(ID_of_startPBC);
				ID_of_startPBC = setTimeout("startPBCmethod();", 7000);//*/
			}
		}
	}
	else{
		$("wpsPBC_button").src = "/images/EZSetup_button.gif";
		//$("WPS_PBCbutton_hint_span").innerHTML = "<a href='../Advanced_WWPS_Content.asp' target='_parent'><#WPS_PBCbutton_hint_notEnable#></a>";
		return false;
	}
}

document.onclick = function(event)
{
	var obj;
	if(window.event)
	{
	  obj = window.event.srcElement;
	}
	else
	{
	  obj = event.target;
	}
	if(obj.id == "mssid_arrow" || obj.id == "ssid_check0" || obj.id == "ssid_check1" || obj.id == "ssid_check2" || obj.id == "ssid_check3")
		return;
	var topElement = "HTML";
	while(obj.tagName!=topElement) //檢查點擊物件的父物件(涵蓋範圍在本身之上)若為非觸發項，即return
	{
		obj = obj.parentNode;
		if(obj.id == "mssid_arrow"){
			return;
		}
	}
	//alert("jump:"+ obj.tagName);
	if(obj.id != "MSSIDList_Block")
	{
		hideMSSID_Block();
	}
}

function applyForm(){
	
	var mbss = new Array(3);
	var mbss_idx = ["ssid", "enabled", "auth_mode", "wep_x", "key", "key1", "key2", "key3", "key4", "crypto", "wpa_psk", "wpa_mode", "nolan", "nowan", "priority", "closed", "time"];
	
	var auth_mode = document.form.wl_auth_mode.value;
		
	if(!validate_string_ssid(document.form.wl_ssid))
		return false;
		
	if(!mssid_duplicate(document.form.wl_ssid))
		return false;
		
	if(auth_mode == "psk" || auth_mode == "wpa" || auth_mode == "wpa2"){
		if(!validate_psk(document.form.wl_wpa_psk))
			return false;
	}
	else{
		if(!validate_wlkey(document.form.wl_asuskey1))
			return false;
	}
	
	if(ssid_order != 0){
		save_temp_ssid_setting(ssid_order);
		//將wl_回填;
		var wl_info = "";
		for(var i=0; i<mssidlist[0].length; i++){
			wl_info += "\n wl_" + mbss_idx[i];
			wl_info += ": " + mssidlist[0][i+2];
		}
		alert(wl_info);
		
		document.form.wl_ssid.value = mssidlist[0][2];

		document.form.wl_auth_mode.name = "x_auth_mode";
		document.form.wl_wep_x.name = "x_wep_x";
		document.form.wl_key.name = "x_auth_mode";
		document.form.wl_crypto.name = "x_crypto";
		
		var wl_auth_mode_obj = document.createElement("input");
		var wl_wep_x_obj = document.createElement("input");
		var wl_key_obj = document.createElement("input");
		var wl_crypto_obj = document.createElement("input");
								
		wl_auth_mode_obj.type = "hidden";
		wl_auth_mode_obj.name = "wl_auth_mode";
		wl_auth_mode_obj.value= mssidlist[0][4];
		$("form").appendChild(wl_auth_mode_obj);

		wl_wep_x_obj.type = "hidden";
		wl_wep_x_obj.name = "wl_wep_x";
		wl_wep_x_obj.value= mssidlist[0][5];
		$("form").appendChild(wl_wep_x_obj);

		wl_key_obj.type = "hidden";
		wl_key_obj.name = "wl_key";
		wl_key_obj.value= mssidlist[0][6];
		$("form").appendChild(wl_key_obj);
		
		wl_crypto_obj.type = "hidden";
		wl_crypto_obj.name = "wl_crypto";
		wl_crypto_obj.value= mssidlist[0][11];
		$("form").appendChild(wl_crypto_obj);
		
		document.form.wl_key1.value = mssidlist[0][7];
		document.form.wl_key2.value = mssidlist[0][8];
		document.form.wl_key3.value = mssidlist[0][9];
		document.form.wl_key4.value = mssidlist[0][10];
		//$("key"+mssidlist[0][6]).value = mssidlist[0][6+parseInt(mssidlist[0][6])];
		//alert("mssidlist[0][6]: "+mssidlist[0][6]);
		//alert("$(keyX).value: "+$("key"+mssidlist[0][6]).value);
		
		document.form.wl_wpa_psk.value = mssidlist[0][12];
		document.form.wl_wpa_mode.value = mssidlist[0][13];
		
		/*
		if(mssidlist[0][4]=="open"){
			document.form.wl_auth_mode.selectedIndex = 0;
			document.form.wl_wep_x.selectedIndex = parseInt(mssidlist[0][5]);	
			if(mssidlist[0][5] != "0"){
				document.form.wl_key.selectedIndex = mssidlist[0][6]-1;
				document.form.wl_key1.value = mssidlist[0][7];
				document.form.wl_key2.value = mssidlist[0][8];
				document.form.wl_key3.value = mssidlist[0][9];
				document.form.wl_key4.value = mssidlist[0][10];
			}
		}
		else if(mssidlist[0][4]=="shared"){
			document.form.wl_auth_mode.selectedIndex = 1;
			document.form.wl_wep_x.selectedIndex = parseInt(mssidlist[0][5])-1;	
			document.form.wl_key.selectedIndex = mssidlist[0][6]-1;
		}
		else if(mssidlist[0][4]=="psk"){
			document.form.wl_auth_mode.selectedIndex = 2;
		}
		else{
			try{
	    	$("auth_mode").add(am_option[0],null); // standards compliant
	    	$("auth_mode").add(am_option[1],null);
	    	$("auth_mode").add(am_option[2],null);
	    	$("auth_mode").add(am_option[3],null);
	    }
	  	catch(ex){
	  		$("auth_mode").add(am_option[0],5);// IE only
	  		$("auth_mode").add(am_option[1],6);
	  		$("auth_mode").add(am_option[2],7);
	  		$("auth_mode").add(am_option[3],8);
	    }
			if(mssidlist[0][4]=="wpa"){
				document.form.wl_auth_mode.selectedIndex = 5;
			}
			else if(mssidlist[0][4]=="wpa2"){
				document.form.wl_auth_mode.selectedIndex = 6;
			}		
			else if(mssidlist[0][4]=="radius"){
				document.form.wl_auth_mode.selectedIndex = 8;
			}
		}
		
		if(mssidlist[0][11] == "tkip")
			document.form.wl_crypto.selectedIndex = 0;
		else if(mssidlist[0][11] == "aes")
			document.form.wl_crypto.selectedIndex = 1;
		else if(mssidlist[0][11] == "tkip+aes")
			document.form.wl_crypto.selectedIndex = 2;
		
		document.form.wl_wpa_psk.value = mssidlist[0][12];
		document.form.wl_wpa_mode.value = mssidlist[0][13];
		alert("回填wl_ssid完畢:\n");
		*/
	}
	else{
		var wep11 = eval('document.form.wl_key'+document.form.wl_key.value);
		wep11.value = document.form.wl_asuskey1.value;
	}
	
	mbss[0] = new Array(mbss_idx.length);
	mbss[1] = new Array(mbss_idx.length);
	mbss[2] = new Array(mbss_idx.length);

	for(var i=1; i<(2-free_mssid); i++){
		for(var j=0; j<mbss_idx.length; j++){
			mbss[i-1][j] = document.createElement("input");
			mbss[i-1][j].type = "hidden";
			mbss[i-1][j].name = "mbss"+i+"_"+mbss_idx[j];
			mbss[i-1][j].value= mssidlist[i][j+2];
			$("form").appendChild(mbss[i-1][j]);
		}
		alert("mbss"+i+": " + "\nssid: "+ mssidlist[i][2] + "\nenabled: "+ mssidlist[i][3]+ "\nauth_mode: "+ mssidlist[i][4]+ "\nwep_x: "+ mssidlist[i][5]+ "\ncrypto: "+ mssidlist[i][11]+ "\nwpa_psk: "+ mssidlist[0][12]+ "\nwpa_mode: "+ mssidlist[i][13]);
	}
	//alert("wl_ssid:\nssid: "+ document.form.wl_ssid.value + "\nenabled: "+ mssidlist[0][3]+ "\nauth_mode: "+ document.form.wl_auth_mode.value+ "\nwep_x: "+ document.form.wl_wep_x.value+ "\nkey: "+ document.form.wl_key.value+ "\nkey1: "+ document.form.wl_key1.value+ "\nkey2: "+ document.form.wl_key2.value+ "\nkey3: "+ document.form.wl_key3.value+ "\ncrypto: "+ document.form.wl_crypto.value+ "\nwpa_psk: "+ document.form.wl_wpa_psk.value+ "\nwpa_mode: "+ document.form.wl_wpa_mode.value);
	
	var mbss_info = "";
	for(var i=0; i<mssidlist[1].length; i++){
		mbss_info += "\n wl_" + mbss_idx[i];
		mbss_info += ": " + mssidlist[1][i+2];
	}
	alert(mbss_info);

	stopFlag = 1;
	document.form.wps_config_state.value = "1";
	
	if((auth_mode == "shared" || auth_mode == "wpa" || auth_mode == "wpa2" || auth_mode == "radius" ||
			( auth_mode == "open" && wl_wep_x != "0" ))
			&& document.form.wps_mode.value == "enabled")
		document.form.wps_mode.value = "disabled";	
	
	parent.showLoading();
	document.form.current_page.value = "";
	document.form.next_page.value = "/";
	document.form.flag.value = "mssid";
	document.form.action_mode.value = " Apply ";
	document.form.submit();
	
	//return true;
}

</script>
</head>

<body class="statusbody" onload="initial();">
<iframe name="hidden_frame" id="hidden_frame" width="0" height="0" frameborder="0"></iframe>
<form method="post" name="form" id="form" action="/start_apply2.htm">
<input type="hidden" name="current_page" value="">
<input type="hidden" name="next_page" value="">
<input type="hidden" name="sid_list" value="WLANConfig11b;">
<input type="hidden" name="group_id" value="">
<input type="hidden" name="action_mode" value="">
<input type="hidden" name="action_script" value="">
<input type="hidden" name="productid" value="<% nvram_get_x("",  "productid"); %>">
<input type="hidden" name="flag" value="">

<input type="hidden" name="wps_mode" value="<% nvram_get_x("WLANConfig11b", "wps_mode"); %>">
<input type="hidden" name="wps_config_state" value="<% nvram_get_x("WLANConfig11b", "wps_config_state"); %>">
<input type="hidden" name="wl_wpa_mode" value="<% nvram_get_x("WLANConfig11b", "wl_wpa_mode"); %>">
<input type="hidden" name="wl_key1" id="key1" value="">
<input type="hidden" name="wl_key2" id="key2" value="">
<input type="hidden" name="wl_key3" id="key3" value="">
<input type="hidden" name="wl_key4" id="key4" value="">

<input type="hidden" name="wl_ssid2" value="<% nvram_char_to_ascii("WLANConfig11b", "wl_ssid"); %>">
<input type="hidden" name="wl_wpa_psk_org" value="<% nvram_char_to_ascii("WLANConfig11b", "wl_wpa_psk"); %>">
<input type="hidden" name="wl_key_type" value="<% nvram_get_x("WLANConfig11b","wl_key_type"); %>"><!--Lock Add 1125 for ralink platform-->
<input type="hidden" name="wl_key1_org" value="<% nvram_char_to_ascii("WLANConfig11b", "wl_key1"); %>">
<input type="hidden" name="wl_key2_org" value="<% nvram_char_to_ascii("WLANConfig11b", "wl_key2"); %>">
<input type="hidden" name="wl_key3_org" value="<% nvram_char_to_ascii("WLANConfig11b", "wl_key3"); %>">
<input type="hidden" name="wl_key4_org" value="<% nvram_char_to_ascii("WLANConfig11b", "wl_key4"); %>">

<input type="hidden" name="nolan" id="nolan" value="0">
<input type="hidden" name="nowan" id="nowan" value="0">

<!--table class="mssid-toolbar" align="center" cellpadding="4" cellspacing="0">

</table-->

<table width="290" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="table1px">
  <tr>
    <th id="toolbar-th">
    	SSID
    	<input id="Add-SSID-btn" type="button" class="sbtn" value="<#CTL_add#>" onclick="add_ssid_event();" style="visibility:hidden;">
    </th>
    <td id="toolbar-td">
    	<div id="mssid_arrow" onclick="pullMSSIDList(this);" title="Select Multi-SSID"></div>
		  <div id="MSSIDList_Block" class="MSSIDList_Block">
				<div id="other_ssid1" title="1"></div>
				<!--[if lte IE 6.5]><iframe class="hackiframe2"></iframe><![endif]-->
		  </div>	
		  <div id="ssid_input" class="input" abbr="0" style="padding:1px; width:158px; height:20px;">
      	<input type="text" id="ssid" name="wl_ssid" value="<% nvram_get_x("WLANConfig11b", "wl_ssid"); %>" maxlength="32" size="22" autocomplete="off"  onClick="hideMSSID_Block();" class="nocssinput" onblur="if(!mssid_duplicate(this)){return false;};" />
      	<input type="checkbox" name="ssid_check" id="ssid_check0" style="float:right;" onclick="" disabled="disabled" checked>
      </div>
    </td>
  </tr>
  <!--tr>
    <td colspan="2"><input id="Add-SSID-btn" type="button" class="button" value="<#CTL_add#> SSID" onclick="add_ssid_event();" style="margin-left:110px;"></td>
  <tr-->
  <tr>
    <th width="95"><#WLANConfig11b_AuthenticationMethod_itemname#></th>
    <td>
      <select id="auth_mode" name="wl_auth_mode" class="input" onchange="change_auth_mode(this);">
				<option value="open" <% nvram_match_x("WLANConfig11b","wl_auth_mode", "open","selected"); %>>Open System</option>
				<option value="shared" <% nvram_match_x("WLANConfig11b","wl_auth_mode", "shared","selected"); %>>Shared Key</option>
				<option value="psk" <% nvram_double_match_x("WLANConfig11b", "wl_auth_mode", "psk", "WLANConfig11b", "wl_wpa_mode", "1", "selected"); %>>WPA-Personal</option>
				<option value="psk" <% nvram_double_match_x("WLANConfig11b", "wl_auth_mode", "psk", "WLANConfig11b", "wl_wpa_mode", "2", "selected"); %>>WPA2-Personal</option>
				<option value="psk" <% nvram_double_match_x("WLANConfig11b", "wl_auth_mode", "psk", "WLANConfig11b", "wl_wpa_mode", "0", "selected"); %>>WPA-Auto-Personal</option>
				<option value="wpa" <% nvram_double_match_x("WLANConfig11b", "wl_auth_mode", "wpa", "WLANConfig11b", "wl_wpa_mode", "3", "selected"); %>>WPA-Enterprise</option>
				<option value="wpa2" <% nvram_match_x("WLANConfig11b", "wl_auth_mode", "wpa2", "selected"); %>>WPA2-Enterprise</option>
				<option value="wpa" <% nvram_double_match_x("WLANConfig11b", "wl_auth_mode", "wpa", "WLANConfig11b", "wl_wpa_mode", "4", "selected"); %>>WPA-Auto-Enterprise</option>
				<option value="radius" <% nvram_match_x("WLANConfig11b","wl_auth_mode", "radius","selected"); %>>Radius with 802.1x</option>
	  	</select>
    </td>
  </tr>
</table>

<div id='all_related_wep' style='display:none;'>
<table width="290" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="table1px">
  <tr>
	<th width="95"><#WLANConfig11b_WEPType_itemname#></th>
	<td>
	  <select name="wl_wep_x" id="wep_x" class="input" onchange="change_wlweptype(this);">
			<option value="0" <% nvram_match_x("WLANConfig11b", "wl_wep_x", "0", "selected"); %>>None</option>
			<option value="1" <% nvram_match_x("WLANConfig11b", "wl_wep_x", "1", "selected"); %>>WEP-64bits</option>
			<option value="2" <% nvram_match_x("WLANConfig11b", "wl_wep_x", "2", "selected"); %>>WEP-128bits</option>
	  </select>
	</td>
  </tr>
</table>

<div id='all_wep_key' style='display:none;'>
<table width="290" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="table1px">
  <tr>
    <th width="95"><#WLANConfig11b_WEPDefaultKey_itemname#></th>
    <td>
      <select name="wl_key" id="key" class="input">
        <option value="1" <% nvram_match_x("WLANConfig11b", "wl_key", "1", "selected"); %>>Key1</option>
        <option value="2" <% nvram_match_x("WLANConfig11b", "wl_key", "2", "selected"); %>>Key2</option>
        <option value="3" <% nvram_match_x("WLANConfig11b", "wl_key", "3", "selected"); %>>Key3</option>
        <option value="4" <% nvram_match_x("WLANConfig11b", "wl_key", "4", "selected"); %>>Key4</option>
      </select>
    </td>
  </tr>
  <tr>
    <th width="95"><#WLANConfig11b_WEPKey_itemname#></th>
    <td>
      <input type="text" name="wl_asuskey1" onKeyUp="return change_wlkey(this, 'WLANConfig11b');" value="" size="22" class="input"/>
    </td>
  </tr>
</table>
</div>
</div>

<div id='wl_crypto' style='display:none;'>
<table width="290" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="table1px">
  <tr>
	<th width="95"><#WLANConfig11b_WPAType_itemname#></th>
	<td>
	  <select id="crypto" name="wl_crypto" class="input" onchange="wl_auth_mode_change(0);">
			<option value="tkip" <% nvram_match_x("WLANConfig11b", "wl_crypto", "tkip", "selected"); %>>TKIP</option>
			<option value="aes" <% nvram_match_x("WLANConfig11b", "wl_crypto", "aes", "selected"); %>>AES</option>
			<option value="tkip+aes" <% nvram_match_x("WLANConfig11b", "wl_crypto", "tkip+aes", "selected"); %>>TKIP+AES</option>
	  </select>
	</td>
  </tr>
</table>
</div>

<div id='wl_wpa_psk' style='display:none'>
<table width="290" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="table1px">
  <tr>
    <th width="95"><#WPA-PSKKey#></th>
    <td>
      <input type="text" id="wpa_psk" name="wl_wpa_psk" value="" size="22" maxlength="64" class="input"/>
    </td>
  </tr>
</table>
</div>

<table width="290" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="table1px">
  <!--tr>
    <th width="95"><#Wireless_Radio#></th>
	<td>
	  <input type="radio" name="wl_radio_x" value="1" <% nvram_match_x("WLANConfig11b", "wl_radio_x", "1", "checked"); %>>on
	  <input type="radio" name="wl_radio_x" value="0" <% nvram_match_x("WLANConfig11b", "wl_radio_x", "0", "checked"); %>>off
	</td>
  </tr>
  <tr id="Enable_ssid_tr" style="display:none;">
    <th width="95">Enable SSID:</th>
		<td>
	  	<input type="radio" name="wl_ssid_x_enable" value="1" checked>On
	  	<input type="radio" name="wl_ssid_x_enable" value="0" >Off
		</td>
  </tr-->
  <tr id="mssid_network_tr" style="display:none;">
    <th width="95">使用網路權限:</th>
		<td>
	  	<select name="mssid_network" class="input" onchange="set_mssid_network(this);">
				<option value="0">Both</option>
				<option value="1">LAN Only</option>
				<option value="2">Internet Only</option>
	  	</select>
		</td>
  </tr>
  <tr id="mssid_qos_tr" style="display:none;">
    <th width="95">使用頻寬優先權:</th>
		<td>
	  	<select id="priority" name="priority" class="input">
				<option value="1"><#Priority_Level_2#></option>
				<option value="0"><#Priority_Level_3#></option>
	  	</select>
		</td>
  </tr>
  <tr id="mssid_closed_tr" style="display:none;">
    <th width="95"><#WLANConfig11b_x_BlockBCSSID_itemname#></th>
		<td>
			<input type="radio" value="1" id="mbss_closed1" name="closed">是
			<input type="radio" value="0" id="mbss_closed0" name="closed" checked>否
		</td>
  </tr>
  <!--tr id="mssid_time_tr" style="display:none;">
    <th width="95">有效時間</th>
		<td>
	  	<select id="mssid_time" name="mssid_time" class="input">
				<option value="2">2</option>
				<option value="4">4</option>
				<option value="6">6</option>
				<option value="8">8</option>
				<option value="10">10</option>
				<option value="12">12</option>
				<option value="14">14</option>
				<option value="16">16</option>
				<option value="18">18</option>
				<option value="20">20</option>
				<option value="22">22</option>
				<option value="24">24</option>																				
	  	</select> 小時
		</td>
  </tr-->
  <tr>
    <td colspan="2" style="width:210px;">
    	<!--input id="saveSecurity" type="button" class="button" value="<#CTL_onlysave#>" onclick="submitForm();" style="float:right;"-->
    	<input id="applySecurity" type="button" class="button" value="<#CTL_onlysave#>" onclick="applyForm();" style="margin-left:110px;">
    </td>
  <tr>
    <th width="95"><#LAN_IP#></th>
    <td id="LANIP"></td>
  </tr>
  <!--tr>remove for Height issue;
    <th width="95"><#PIN_code#></th>
    <td id="PINCode"></td>
  </tr-->
  <tr>
    <th width="95"><#MAC_Address#></th>
    <td id="MAC"></td>
  </tr>
  <tr>
    <th width="95">EZSetup</th>
    <td>
	<img src="/images/EZSetup_button.gif" style="cursor:pointer;" align="absmiddle" title="<#WPS_PBCbutton_hint#>" id="wpsPBC_button" onClick="wpsPBC(this);">
	<span id="WPS_PBCbutton_hint_span"></span>
	</td>
	</tr>
</table>
</form>

<select id="Router_domore" class="domore" onchange="domore_link(this);">
	<option><#MoreConfig#>...</option>
	<option value="../Advanced_Wireless_Content.asp"><#menu5_1#>-<#menu5_1_1#></option>
	<option value="../Advanced_WWPS_Content.asp"><#menu5_1_2#></option>
	<option value="../Advanced_LAN_Content.asp"><#menu5_2_1#></option>
	<option value="../Advanced_DHCP_Content.asp"><#menu5_2_2#></option>
	<option value="../Advanced_GWStaticRoute_Content.asp"><#menu5_2_3#></option>
	<option value="../Main_LogStatus_Content.asp"><#menu5_7_2#></option>
</select>

<form method="post" name="WPSForm" id="WPSForm" action="/start_apply.htm">
	<input type="hidden" name="current_page" value="">
	<input type="hidden" name="next_page" value="">
	<input type="hidden" name="sid_list" value="WLANConfig11b;">
	<input type="hidden" name="group_id" value="">
	<input type="hidden" name="action_mode" value="">
	<input type="hidden" name="action_script" value="">
	<input type="hidden" name="wps_method" value="<% nvram_get_x("WLANConfig11b", "wps_method"); %>">
	<input type="hidden" name="wps_pbc_force" value="<% nvram_get_x("WLANConfig11b", "wps_pbc_force"); %>">
	<input type="hidden" name="pbc_overlap" value="<% nvram_get_x("WLANConfig11b", "pbc_overlap"); %>">
</form>

<form method="post" name="stopPINForm" id="stopPINForm" action="/start_apply.htm" target="hidden_frame">
	<input type="hidden" name="current_page" value="">
	<input type="hidden" name="sid_list" value="WLANConfig11b;">
	<input type="hidden" name="group_id" value="">
	<input type="hidden" name="action_mode" value="">
	<input type="hidden" name="wps_config_command" value="<% nvram_get_x("WLANConfig11b", "wps_config_command"); %>">
</form>
</body>
</html>

