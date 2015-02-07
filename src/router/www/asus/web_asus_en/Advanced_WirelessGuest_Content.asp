<head>
<title>ZVMODELVZ Web Manager</title>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<link rel="stylesheet" type="text/css" href="style.css" media="screen"></link>
<script language="JavaScript" type="text/javascript" src="overlib.js"></script>
<script language="JavaScript" type="text/javascript" src="general.js"></script>
</head>  
<div id="overDiv" style="position:absolute; visibility:hidden; z-index:1000;"></div>    
<body onLoad="load_body()" onunLoad="return unload_body();">
<form method="GET" name="form" action="apply.cgi">
<!-- Table for the conntent page -->	    
<table width="666" border="0" cellpadding="0" cellspacing="0">     	      
    	
<input type="hidden" name="current_page" value="Advanced_WirelessGuest_Content.asp"><input type="hidden" name="next_page" value="SaveRestart.asp"><input type="hidden" name="next_host" value=""><input type="hidden" name="sid_list" value="WLANConfig11a;WLANConfig11b;"><input type="hidden" name="group_id" value=""><input type="hidden" name="modified" value="0"><input type="hidden" name="action_mode" value=""><input type="hidden" name="first_time" value=""><input type="hidden" name="action_script" value=""><input type="hidden" name="preferred_lang" value="<% nvram_get_x("","preferred_lang"); %>">
<tr>
<td>
<table width="666" border="1" cellpadding="0" cellspacing="0" bordercolor="E0E0E0">
<tr class="content_header_tr">
<td class="content_header_td_title" colspan="2"><#t1Wireless#> - <#t2Guest#></td>
</tr>
<tr>
<td class="content_desc_td" colspan="2"><#WLANConfig11b_GEnable_sectiondesc#>
         </td>
</tr>
<tr>
<td class="content_header_td"><#WLANConfig11b_GEnable_itemname#>
           </td><td class="content_input_td"><input type="radio" value="1" name="wl_guest_enable" class="content_input_fd" onClick="return change_common_radio(this, 'WLANConfig11b', 'wl_guest_enable', '1')" <% nvram_match_x("WLANConfig11b","wl_guest_enable", "1", "checked"); %>>Yes</input><input type="radio" value="0" name="wl_guest_enable" class="content_input_fd" onClick="return change_common_radio(this, 'WLANConfig11b', 'wl_guest_enable', '0')" <% nvram_match_x("WLANConfig11b","wl_guest_enable", "0", "checked"); %>>No</input></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_GSSID_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_GSSID_itemname#>
           </td><td class="content_input_td"><input type="text" maxlength="32" class="content_input_fd" size="32" name="wl_guest_ssid_1" value="<% nvram_get_x("WLANConfig11b","wl_guest_ssid_1"); %>" onKeyPress="return is_string(this)" onBlur="validate_string_ssid(this)"></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_GAuthenticationMethod_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_GAuthenticationMethod_itemname#>
           </td><td class="content_input_td"><select name="wl_guest_auth_mode_1" class="content_input_fd" onChange="return change_common(this, 'WLANConfig11b', 'wl_guest_auth_mode_1')"><option class="content_input_fd" value="open" <% nvram_match_x("WLANConfig11b","wl_guest_auth_mode_1", "open","selected"); %>>Open System or Shared Key</option><option class="content_input_fd" value="shared" <% nvram_match_x("WLANConfig11b","wl_guest_auth_mode_1", "shared","selected"); %>>Shared Key</option><option class="content_input_fd" value="psk" <% nvram_match_x("WLANConfig11b","wl_guest_auth_mode_1", "psk","selected"); %>>WPA-PSK</option></select></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_GWPAType_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_GWPAType_itemname#>
           </td><td class="content_input_td"><select name="wl_guest_crypto_1" class="content_input_fd" onChange="return change_common(this, 'WLANConfig11b', 'wl_guest_crypto_1')"><option class="content_input_fd" value="tkip" <% nvram_match_x("WLANConfig11b","wl_guest_crypto_1", "tkip","selected"); %>>TKIP</option><option class="content_input_fd" value="aes" <% nvram_match_x("WLANConfig11b","wl_guest_crypto_1", "aes","selected"); %>>AES</option><option class="content_input_fd" value="tkip+aes" <% nvram_match_x("WLANConfig11b","wl_guest_crypto_1", "tkip+aes","selected"); %>>TKIP+AES</option></select></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_Gx_PSKKey_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_Gx_PSKKey_itemname#>
           </td><td class="content_input_td"><script language="JavaScript" type="text/javascript" src="md5.js"></script><input type="password" maxlength="64" class="content_input_fd" size="32" name="wl_guest_wpa_psk_1" value="<% nvram_get_x("WLANConfig11b","wl_guest_wpa_psk_1"); %>" onKeyUp="return is_wlphrase('WLANConfig11b', 'wl_guest_wpa_psk_1', this)" onBlur="return validate_wlphrase('WLANConfig11b', 'wl_guest_wpa_psk_1', this)"></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_GWEPType_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_GWEPType_itemname#>
           </td><td class="content_input_td"><select name="wl_guest_wep_x_1" class="content_input_fd" onChange="return change_common(this, 'WLANConfig11b', 'wl_guest_wep_x_1')"><option class="content_input_fd" value="0" <% nvram_match_x("WLANConfig11b","wl_guest_wep_x_1", "0","selected"); %>>None</option><option class="content_input_fd" value="1" <% nvram_match_x("WLANConfig11b","wl_guest_wep_x_1", "1","selected"); %>>WEP-64bits</option><option class="content_input_fd" value="2" <% nvram_match_x("WLANConfig11b","wl_guest_wep_x_1", "2","selected"); %>>WEP-128bits</option></select></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_Gx_Phrase_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_Gx_Phrase_itemname#>
           </td><td class="content_input_td"><script language="JavaScript" type="text/javascript" src="md5.js"></script><input type="password" maxlength="64" class="content_input_fd" size="32" name="wl_guest_phrase_x_1" value="<% nvram_get_x("WLANConfig11b","wl_guest_phrase_x_1"); %>" onKeyUp="return is_wlphrase('WLANConfig11b', 'wl_guest_phrase_x_1', this)" onBlur="return validate_wlphrase('WLANConfig11b', 'wl_guest_phrase_x_1', this)"></td>
</tr>
<tr>
<td class="content_header_td"><#WLANConfig11b_GWEPKey1_itemname#>
           </td><td class="content_input_td"><input type="pssword" maxlength="32" class="content_input_fd" size="32" name="wl_guest_key1_1" value="<% nvram_get_x("WLANConfig11b","wl_guest_key1_1"); %>" onBlur="return validate_wlkey(this, 'WLANConfig11b')" onKeyPress="return is_wlkey(this, 'WLANConfig11b')" onKeyUp="return change_wlkey(this, 'WLANConfig11b')"></td>
</tr>
<tr>
<td class="content_header_td"><#WLANConfig11b_GWEPKey2_itemname#>
           </td><td class="content_input_td"><input type="pssword" maxlength="32" class="content_input_fd" size="32" name="wl_guest_key2_1" value="<% nvram_get_x("WLANConfig11b","wl_guest_key2_1"); %>" onBlur="return validate_wlkey(this, 'WLANConfig11b')" onKeyPress="return is_wlkey(this, 'WLANConfig11b')" onKeyUp="return change_wlkey(this, 'WLANConfig11b')"></td>
</tr>
<tr>
<td class="content_header_td"><#WLANConfig11b_GWEPKey3_itemname#>
           </td><td class="content_input_td"><input type="pssword" maxlength="32" class="content_input_fd" size="32" name="wl_guest_key3_1" value="<% nvram_get_x("WLANConfig11b","wl_guest_key3_1"); %>" onBlur="return validate_wlkey(this, 'WLANConfig11b')" onKeyPress="return is_wlkey(this, 'WLANConfig11b')" onKeyUp="return change_wlkey(this, 'WLANConfig11b')"></td>
</tr>
<tr>
<td class="content_header_td"><#WLANConfig11b_GWEPKey4_itemname#>
           </td><td class="content_input_td"><input type="pssword" maxlength="32" class="content_input_fd" size="32" name="wl_guest_key4_1" value="<% nvram_get_x("WLANConfig11b","wl_guest_key4_1"); %>" onBlur="return validate_wlkey(this, 'WLANConfig11b')" onKeyPress="return is_wlkey(this, 'WLANConfig11b')" onKeyUp="return change_wlkey(this, 'WLANConfig11b')"></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_GWEPDefaultKey_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_GWEPDefaultKey_itemname#>
           </td><td class="content_input_td"><select name="wl_guest_key_1" class="content_input_fd" onChange="return change_common(this, 'WLANConfig11b', 'wl_guest_key_1')"><option class="content_input_fd" value="1" <% nvram_match_x("WLANConfig11b","wl_guest_key_1", "1","selected"); %>>Key1</option><option class="content_input_fd" value="2" <% nvram_match_x("WLANConfig11b","wl_guest_key_1", "2","selected"); %>>Key2</option><option class="content_input_fd" value="3" <% nvram_match_x("WLANConfig11b","wl_guest_key_1", "3","selected"); %>>Key3</option><option class="content_input_fd" value="4" <% nvram_match_x("WLANConfig11b","wl_guest_key_1", "4","selected"); %>>Key4</option></select></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_Gx_Rekey_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_Gx_Rekey_itemname#>
           </td><td class="content_input_td"><input type="text" maxlength="5" size="5" name="wl_guest_wpa_gtk_rekey_1" class="content_input_fd" value="<% nvram_get_x("WLANConfig11b", "wl_guest_wpa_gtk_rekey_1"); %>" onBlur="validate_range(this, 0, 86400)" onKeyPress="return is_number(this)"></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_GIPRouters_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_GIPRouters_itemname#>
           </td><td class="content_input_td"><input type="text" maxlength="15" class="content_input_fd" size="15" name="lan1_ipaddr" value="<% nvram_get_x("WLANConfig11b","lan1_ipaddr"); %>" onBlur="return validate_ipaddr(this, 'lan1_ipaddr')" onKeyPress="return is_ipaddr(this)" onKeyUp="change_ipaddr(this)"></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_GSubnetMask_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_GSubnetMask_itemname#>
           </td><td class="content_input_td"><input type="text" maxlength="15" class="content_input_fd" size="15" name="lan1_netmask" value="<% nvram_get_x("WLANConfig11b","lan1_netmask"); %>" onBlur="return validate_ipaddr(this, 'lan1_netmask')" onKeyPress="return is_ipaddr(this)" onKeyUp="change_ipaddr(this)"></td>
</tr>
<tr>
<td class="content_header_td"><#WLANConfig11b_GDHCPServerConfigurable_itemname#>
           </td><td class="content_input_td"><input type="radio" value="1" name="dhcp1_enable_x" class="content_input_fd" onClick="return change_common_radio(this, 'WLANConfig11b', 'dhcp1_enable_x', '1')" <% nvram_match_x("WLANConfig11b","dhcp1_enable_x", "1", "checked"); %>>Yes</input><input type="radio" value="0" name="dhcp1_enable_x" class="content_input_fd" onClick="return change_common_radio(this, 'WLANConfig11b', 'dhcp1_enable_x', '0')" <% nvram_match_x("WLANConfig11b","dhcp1_enable_x", "0", "checked"); %>>No</input></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_GMinAddress_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_GMinAddress_itemname#>
           </td><td class="content_input_td"><input type="text" maxlength="15" class="content_input_fd" size="15" name="dhcp1_start" value="<% nvram_get_x("WLANConfig11b","dhcp1_start"); %>" onBlur="return validate_ipaddr(this, 'dhcp1_start')" onKeyPress="return is_ipaddr(this)" onKeyUp="change_ipaddr(this)"></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_GMaxAddress_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_GMaxAddress_itemname#>
           </td><td class="content_input_td"><input type="text" maxlength="15" class="content_input_fd" size="15" name="dhcp1_end" value="<% nvram_get_x("WLANConfig11b","dhcp1_end"); %>" onBlur="return validate_ipaddr(this, 'dhcp1_end')" onKeyPress="return is_ipaddr(this)" onKeyUp="change_ipaddr(this)"></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_GLeaseTime_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_GLeaseTime_itemname#>
           </td><td class="content_input_td"><input type="text" maxlength="5" size="5" name="lan1_lease" class="content_input_fd" value="<% nvram_get_x("WLANConfig11b", "lan1_lease"); %>" onBlur="validate_range(this, 1, 86400)" onKeyPress="return is_number(this)"></td>
</tr>
</table>
</td>
</tr>
<tr>
<td>
<table width="666" border="1" cellpadding="0" cellspacing="0" bordercolor="E0E0E0">
<input type="hidden" name="lan_ipaddr" value="<% nvram_get_x("WLANConfig11b","lan_ipaddr"); %>"><input type="hidden" name="lan_netmask" value="<% nvram_get_x("WLANConfig11b","lan_netmask"); %>">
</table>
</td>
</tr>
<tr>
<td>
<table width="666" border="2" cellpadding="0" cellspacing="0" bordercolor="E0E0E0"></table>
</td>
</tr>
<tr>
<td>
<table width="666" border="2" cellpadding="0" cellspacing="0" bordercolor="E0E0E0"></table>
</td>
</tr>
<tr>
<td>
<table width="666" border="2" cellpadding="0" cellspacing="0" bordercolor="E0E0E0"></table>
</td>
</tr>
<tr>
<td>
<table width="666" border="1" cellpadding="0" cellspacing="0" bordercolor="E0E0E0">
<input type="hidden" name="wan_ipaddr" value="<% nvram_get_x("IPConnection","wan_ipaddr"); %>"><input type="hidden" name="wan_netmask" value="<% nvram_get_x("IPConnection","wan_netmask"); %>">
</table>
</td>
</tr>
<tr>
<td>
<table width="666" border="2" cellpadding="0" cellspacing="0" bordercolor="E0E0E0"></table>
</td>
</tr>
<tr>
<td>
<table width="666" border="2" cellpadding="0" cellspacing="0" bordercolor="E0E0E0"></table>
</td>
</tr>
<tr>
<td>
<table width="666" border="2" cellpadding="0" cellspacing="0" bordercolor="E0E0E0"></table>
</td>
</tr>
<tr>
<td>
<table width="666" border="2" cellpadding="0" cellspacing="0" bordercolor="E0E0E0"></table>
</td>
</tr>
<tr>
<td>
<table width="666" border="1" cellpadding="0" cellspacing="0" bordercolor="E0E0E0">
<input type="hidden" name="wan_proto" value="<% nvram_get_x("Layer3Forwarding","wan_proto"); %>">
</table>
</td>
</tr>
<tr>
<td>		
<table width="666" border="1" cellpadding="0" cellspacing="0" bordercolor="B0B0B0">
<tr bgcolor="#CCCCCC"><td colspan="3"><font face="arial" size="2"><b>&nbsp</b></font></td></tr>
<tr bgcolor="#FFFFFF">  
   <td id ="Confirm" height="25" width="34%">  
   <div align="center"><font face="Arial"> <input class=inputSubmit onMouseOut=buttonOut(this) onMouseOver="buttonOver(this)" onClick="return onSubmitCtrl(this, ' Restore ');" type="submit" value=" <#CTL_restore#> " name="action"></font></div> 
   </td>  
   <td height="25" width="33%">  
   <div align="center"><font face="Arial"> <input class=inputSubmit onMouseOut=buttonOut(this) onMouseOver="buttonOver(this)" onClick="return onSubmitCtrl(this, ' Finish ');" type="submit" value=" <#CTL_finish#> " name="action"></font></div> 
   </td>
   <td height="25" width="33%">  
   <div align="center"><font face="Arial"> <input class=inputSubmit onMouseOut=buttonOut(this) onMouseOver="buttonOver(this)" onClick="return onSubmitCtrl(this, ' Apply ');" type="submit" value=" <#CTL_apply#> " name="action"></font></div> 
   </td>    
</tr>
</table>
</td>
</tr>

<tr>
<td>
<table width="666" border="1" cellpadding="0" cellspacing="0" bordercolor="B0B0B0">
<tr>
    <td colspan="2" width="616" height="25" bgcolor="#FFBB00"></td> 
</tr>                   
<tr bgcolor="#FFFFFF">
    <td class="content_header_td_15" align="left"><#CTL_restore#>: </td>
    <td class="content_input_td_padding" align="left"><#CTL_desc_restore#></td>
</tr>
<tr bgcolor="#FFFFFF">
    <td class="content_header_td_15" align="left"><#CTL_finish#>: </td>
    <td class="content_input_td_padding" align="left"><#CTL_desc_finish#></td>
</tr>
<tr bgcolor="#FFFFFF">
    <td class="content_header_td_15" align="left"><#CTL_apply#>: </td>
    <td class="content_input_td_padding" align="left"><#CTL_desc_apply#></td>
</tr>
</table>
</td>
</tr>

</table>
</form>
</body>
