<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<title>Add New Account</title>
<link rel="stylesheet" href="../form_style.css"  type="text/css" />

<script type="text/javascript" src="/state.js"></script>
<script type="text/javascript" src="/jquery.js"></script>
<script type="text/javascript" src="/general.js"></script>
<script type="text/javascript" src="formcontrol.js"></script>
<script type="text/javascript" src="/mssid.js"></script>
<script type="text/javascript">

var $j = jQuery.noConflict();
var idx = 0; //index of free ssid;

function submitForm(){
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
		if(auth_mode!="radius" && !validate_wlkey(document.form.wl_asuskey1))
			return false;
	}

	//var wep11 = eval('document.form.wl_key'+document.form.wl_key.value);
	//wep11.value = document.form.wl_asuskey1.value;

	$j("#ssid").attr("name","mbss"+idx+"_ssid");
	$j("#auth_mode").attr("name","mbss"+idx+"_auth_mode");
	
	$j("#wep_x").attr("name","mbss"+idx+"_wep_x");
	$j("#key1").attr("name","mbss"+idx+"_key1");
	$j("#key2").attr("name","mbss"+idx+"_key2");
	$j("#key3").attr("name","mbss"+idx+"_key3");
	$j("#key4").attr("name","mbss"+idx+"_key4");
	
	//var mbss_wep = eval('document.form.mbss_'+idx+'_key'+ document.form.wl_key.value);
	//mbss_wep.value = document.form.wl_asuskey1.value;
	
	$j("#key"+document.form.wl_key.value).val(document.form.wl_asuskey1.value);
	$j("#key").attr("name","mbss"+idx+"_key");
	
	$j("#crypto").attr("name","mbss"+idx+"_crypto");
	$j("#wpa_psk").attr("name","mbss"+idx+"_wpa_psk");
	
	$j("#mbss_enable0").attr("name","mbss"+idx+"_enabled");
	$j("#mbss_enable1").attr("name","mbss"+idx+"_enabled");
	
	//$j("#network").attr("name","mbss"+idx+"_network");
	set_mssid_network(document.form.mssid_network);
	$j("#nolan").attr("name","mbss"+idx+"_nolan");
	$j("#nowan").attr("name","mbss"+idx+"_nowan");
	
	$j("#priority").attr("name","mbss"+idx+"_priority");
	
	$j("#mbss_closed0").attr("name","mbss"+idx+"_closed");
	$j("#mbss_closed1").attr("name","mbss"+idx+"_closed");
	
	$j("#wpa_mode").attr("name","mbss"+idx+"_wpa_mode");
	
	//alert(eval('document.form.mbss'+idx+'_ssid').value)
	
	parent.hidePop('OverlayMask'); 
	parent.showLoading();
	document.form.flag.value = "mssid";
	document.form.action_mode.value = " Update ";
	//document.form.submit();
	$("add_mssid_form").submit(); // use id instead of name to prevent from duplicate error.
}
function initial(){
	document.form.wl_auth_mode.selectedIndex = 0;
	change_auth_mode(document.form.wl_auth_mode);
	document.form.wl_ssid.focus();
		
	mssidlist[0][2] = decodeURIComponent(document.form.wl_ssid2.value);
	mssidlist[1][2] = decodeURIComponent(document.form.mbss1_ssid2.value);
	mssidlist[2][2] = decodeURIComponent(document.form.mbss2_ssid2.value);
	mssidlist[3][2] = decodeURIComponent(document.form.mbss3_ssid2.value);
	
	idx = get_free_mssid();
}

</script>
</head>

<body onload="initial();">
<iframe name="hidden_frame" id="hidden_frame" width="0" height="0" frameborder="0"></iframe>
<form method="post" id="add_mssid_form" name="form" action="/start_apply2.htm">
<input type="hidden" name="current_page" value="">
<input type="hidden" name="next_page" value="/">
<input type="hidden" name="sid_list" value="WLANConfig11b;">
<input type="hidden" name="group_id" value="">
<input type="hidden" name="action_mode" value="">
<input type="hidden" name="productid" value="<% nvram_get_x("",  "productid"); %>">
<input type="hidden" name="flag" value="">

<input type="hidden" name="wl_wpa_mode" id="wpa_mode" value="">
<input type="hidden" name="wl_key1" id="key1" value="">
<input type="hidden" name="wl_key2" id="key2" value="">
<input type="hidden" name="wl_key3" id="key3" value="">
<input type="hidden" name="wl_key4" id="key4" value="">

<input type="hidden" name="wl_ssid2" value="<% nvram_char_to_ascii("WLANConfig11b", "wl_ssid"); %>">
<input type="hidden" id="ssid2_mbss1" name="mbss1_ssid2" value="<% nvram_char_to_ascii("WLANConfig11b", "mbss1_ssid"); %>">
<input type="hidden" id="ssid2_mbss2" name="mbss2_ssid2" value="<% nvram_char_to_ascii("WLANConfig11b", "mbss2_ssid"); %>">
<input type="hidden" id="ssid2_mbss3" name="mbss3_ssid2" value="<% nvram_char_to_ascii("WLANConfig11b", "mbss3_ssid"); %>">

<input type="hidden" name="wl_wpa_psk_org" value="<% nvram_char_to_ascii("WLANConfig11b", "wl_wpa_psk"); %>">
<input type="hidden" name="wl_key_type" value="<% nvram_get_x("WLANConfig11b","wl_key_type"); %>"><!--Lock Add 1125 for ralink platform-->
<input type="hidden" name="wl_key1_org" value="<% nvram_char_to_ascii("WLANConfig11b", "wl_key1"); %>">
<input type="hidden" name="wl_key2_org" value="<% nvram_char_to_ascii("WLANConfig11b", "wl_key2"); %>">
<input type="hidden" name="wl_key3_org" value="<% nvram_char_to_ascii("WLANConfig11b", "wl_key3"); %>">
<input type="hidden" name="wl_key4_org" value="<% nvram_char_to_ascii("WLANConfig11b", "wl_key4"); %>">

<input type="hidden" name="nolan" id="nolan" value="0">
<input type="hidden" name="nowan" id="nowan" value="0">

<table width="95%" class="popTable" border="0" align="center" cellpadding="0" cellspacing="0">
	<!--thead>
		<tr><td colspan="2"><H2><#CTL_add#> SSID</H2><div class="popup_close_x"><a href="javascript:parent.hidePop('OverlayMask');"></a></div></td></tr>
	</thead-->
	<thead>
   	<tr>
     	<td colspan="2"><span><#CTL_add#> SSID</span>
     		<div class="popup_close_x"><a href="javascript:parent.hidePop('OverlayMask');"></a></div>
     		<!--img src="../images/button-close.gif" onClick="parent.hidePop('OverlayMask');"-->
     	</td>
    </tr>
	</thead>
	<tbody>
		<tr>
			<th width="40%"><#MSSID_profile#></th>
			<td>
				<input type="radio" name="SSID_profile" value="1" onclick="change_profile(this);"><#mssid_guest#>
				<input type="radio" name="SSID_profile" value="2" onclick="change_profile(this);">VIP
				<input type="radio" name="SSID_profile" value="0" onclick="change_profile(this);" checked><#mssid_customize#>
	    </td>
		</tr>		
		<tr>
			<th>SSID</th>
			<td>
				<input type="text" id="ssid" name="wl_ssid" value="" maxlength="32" size="22" autocomplete="off" class="input"/>
			</td>
		</tr>
		<tr>
			<td colspan="2"><div class="s-line"></div></td>
		</tr>
  	<tr>
    	<th width="40%"><#WLANConfig11b_AuthenticationMethod_itemname#></th>
    	<td>
      	<select id="auth_mode" name="wl_auth_mode" class="input" onchange="change_auth_mode(this);">
					<option value="open" selected>Open System</option>
					<option value="shared">Shared Key</option>
					<option value="psk">WPA-Personal</option>
					<option value="psk">WPA2-Personal</option>
					<option value="psk">WPA-Auto-Personal</option>
	  		</select>
    	</td>
  	</tr>		
	</tbody>
</table>

<div id='all_related_wep' style='display:none;'>
<table width="95%" class="popTable" border="0" align="center" cellpadding="0" cellspacing="0">
  <tr>
	<th width="40%"><#WLANConfig11b_WEPType_itemname#></th>
	<td>
	  <select name="wl_wep_x" id="wep_x" class="input" onchange="change_wlweptype(this);">
		<option value="0">None</option>
		<option value="1">WEP-64bits</option>
		<option value="2">WEP-128bits</option>
	  </select>
	</td>
  </tr>
</table>

<div id='all_wep_key' style='display:none;'>
<table width="95%" class="popTable" border="0" align="center" cellpadding="0" cellspacing="0">
  <tr>
    <th width="40%"><#WLANConfig11b_WEPDefaultKey_itemname#></th>
    <td>
      <select id="key" name="wl_key" class="input" onchange="show_key();">
        <option value="1" selected>Key1</option>
        <option value="2">Key2</option>
        <option value="3">Key3</option>
        <option value="4">Key4</option>
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
<table width="95%" class="popTable" border="0" align="center" cellpadding="0" cellspacing="0">
  <tr>
	<th width="40%"><#WLANConfig11b_WPAType_itemname#></th>
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
<table width="95%" class="popTable" border="0" align="center" cellpadding="0" cellspacing="0">
  <tr>
    <th width="40%"><#WPA-PSKKey#></th>
    <td>
      <input type="text" id="wpa_psk" name="wl_wpa_psk" value="" size="22" maxlength="64" class="input"/>
    </td>
  </tr>
</table>
</div>

<table width="95%" class="popTable" border="0" align="center" cellpadding="0" cellspacing="0">
	<tr><td colspan="2" height="0"><div class="s-line"></div></td></tr>
	<tbody>
  <tr id="Enable_ssid_tr">
    <th width="40%"><#btn_Enable#> SSID</th>
		<td>
	  	<input id="mbss_enable1" type="radio" name="enabled" value="1" checked /><#checkbox_Yes#>
	  	<input id="mbss_enable0" type="radio" name="enabled" value="0" /><#checkbox_No#>
		</td>
  </tr>
  <tr id="mssid_network_tr">
    <th><#mssid_network_access#></th>
		<td>
	  	<select name="mssid_network" class="input">
				<option value="0">Both</option>
				<option value="1">LAN Only</option>
				<option value="2">Internet Only</option>
	  	</select>	
		</td>
  </tr>
  <tr id="mssid_qos_tr">
    <th><#mssid_qos_priority#></th>
		<td>
	  	<select id="priority" name="priority" class="input">
				<option value="1"><#Priority_Level_2#></option>
				<option value="0"><#Priority_Level_3#></option>
	  	</select>
		</td>
  </tr>
  <tr id="mssid_closed_tr">
    <th><#WLANConfig11b_x_BlockBCSSID_itemname#></th>
		<td>
			<input type="radio" value="1" id="mbss_closed1" name="closed" class="input"><#checkbox_Yes#>
			<input type="radio" value="0" id="mbss_closed0" name="closed" class="input" checked><#checkbox_No#>
		</td>
  </tr>
  <!--tr>
    <th width="95"><#mssid_time#></th>
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
				<option value="24" selected>24</option>
	  	</select><#Hour#>
		</td>
  </tr-->    
</tbody>  
    <tr bgcolor="#FFFFFF" style="border-bottom:1px solid #666">
    	<td colspan="2" align="right">
    		<input id="Submit" type="button" class="button" value="<#CTL_add#>" onclick="submitForm();">
    		<input type="button" class="button" value="<#CTL_Cancel#>" onClick="parent.hidePop('OverlayMask');">
    	</td>
    </tr>
</table>
</form>
</body>
</html>
