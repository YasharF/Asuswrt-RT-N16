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
    	
<input type="hidden" name="current_page" value="Advanced_WAdvanced_Content.asp"><input type="hidden" name="next_page" value="SaveRestart.asp"><input type="hidden" name="next_host" value=""><input type="hidden" name="sid_list" value="WLANAuthentication11a;WLANConfig11b;"><input type="hidden" name="group_id" value=""><input type="hidden" name="modified" value="0"><input type="hidden" name="action_mode" value=""><input type="hidden" name="first_time" value=""><input type="hidden" name="action_script" value=""><input type="hidden" name="preferred_lang" value="<% nvram_get_x("","preferred_lang"); %>">
<tr>
<td>
<table width="666" border="1" cellpadding="0" cellspacing="0" bordercolor="E0E0E0">
<tr class="content_header_tr">
<td class="content_header_td_title" colspan="2"><#t1Wireless#> - <#t2Advanced#></td>
</tr>
<input type="hidden" name="wl_gmode" value="<% nvram_get_x("WLANConfig11b","wl_gmode"); %>"><input type="hidden" name="wl_gmode_protection_x" value="<% nvram_get_x("WLANConfig11b","wl_gmode_protection_x"); %>">
<tr>
<td class="content_desc_td" colspan="2"><#WLANConfig11b_display5_sectiondesc#>
         </td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_x_BlockBCSSID_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_x_BlockBCSSID_itemname#>
           </td><td class="content_input_td"><input type="radio" value="1" name="wl_closed" class="content_input_fd" onClick="return change_common_radio(this, 'WLANConfig11b', 'wl_closed', '1')" <% nvram_match_x("WLANConfig11b","wl_closed", "1", "checked"); %>>Yes</input><input type="radio" value="0" name="wl_closed" class="content_input_fd" onClick="return change_common_radio(this, 'WLANConfig11b', 'wl_closed', '0')" <% nvram_match_x("WLANConfig11b","wl_closed", "0", "checked"); %>>No</input></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_x_IsolateAP_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_x_IsolateAP_itemname#>
           </td><td class="content_input_td"><input type="radio" value="1" name="wl_ap_isolate" class="content_input_fd" onClick="return change_common_radio(this, 'WLANConfig11b', 'wl_ap_isolate', '1')" <% nvram_match_x("WLANConfig11b","wl_ap_isolate", "1", "checked"); %>>Yes</input><input type="radio" value="0" name="wl_ap_isolate" class="content_input_fd" onClick="return change_common_radio(this, 'WLANConfig11b', 'wl_ap_isolate', '0')" <% nvram_match_x("WLANConfig11b","wl_ap_isolate", "0", "checked"); %>>No</input></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_DataRateAll_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_DataRateAll_itemname#>
           </td><td class="content_input_td"><select name="wl_rate" class="content_input_fd" onChange="return change_common(this, 'WLANConfig11b', 'wl_rate')"><option class="content_input_fd" value="0" <% nvram_match_x("WLANConfig11b","wl_rate", "0","selected"); %>>Auto</option><option class="content_input_fd" value="1000000" <% nvram_match_x("WLANConfig11b","wl_rate", "1000000","selected"); %>>1</option><option class="content_input_fd" value="2000000" <% nvram_match_x("WLANConfig11b","wl_rate", "2000000","selected"); %>>2</option><option class="content_input_fd" value="5500000" <% nvram_match_x("WLANConfig11b","wl_rate", "5500000","selected"); %>>5.5</option><option class="content_input_fd" value="6000000" <% nvram_match_x("WLANConfig11b","wl_rate", "6000000","selected"); %>>6</option><option class="content_input_fd" value="9000000" <% nvram_match_x("WLANConfig11b","wl_rate", "9000000","selected"); %>>9</option><option class="content_input_fd" value="11000000" <% nvram_match_x("WLANConfig11b","wl_rate", "11000000","selected"); %>>11</option><option class="content_input_fd" value="12000000" <% nvram_match_x("WLANConfig11b","wl_rate", "12000000","selected"); %>>12</option><option class="content_input_fd" value="18000000" <% nvram_match_x("WLANConfig11b","wl_rate", "18000000","selected"); %>>18</option><option class="content_input_fd" value="24000000" <% nvram_match_x("WLANConfig11b","wl_rate", "24000000","selected"); %>>24</option><option class="content_input_fd" value="36000000" <% nvram_match_x("WLANConfig11b","wl_rate", "36000000","selected"); %>>36</option><option class="content_input_fd" value="48000000" <% nvram_match_x("WLANConfig11b","wl_rate", "48000000","selected"); %>>48</option><option class="content_input_fd" value="54000000" <% nvram_match_x("WLANConfig11b","wl_rate", "54000000","selected"); %>>54</option></select></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_DataRate_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_DataRate_itemname#>
           </td><td class="content_input_td"><select name="wl_rateset" class="content_input_fd" onChange="return change_common(this, 'WLANConfig11b', 'wl_rateset')"><option class="content_input_fd" value="default" <% nvram_match_x("WLANConfig11b","wl_rateset", "default","selected"); %>>Default</option><option class="content_input_fd" value="all" <% nvram_match_x("WLANConfig11b","wl_rateset", "all","selected"); %>>All</option><option class="content_input_fd" value="12" <% nvram_match_x("WLANConfig11b","wl_rateset", "12","selected"); %>>1, 2 Mbps</option></select></td>
</tr>
<tr>
<td class="content_header_td"><#WLANConfig11b_x_Frag_itemname#>
           </td><td class="content_input_td"><input type="text" maxlength="5" size="5" name="wl_frag" class="content_input_fd" value="<% nvram_get_x("WLANConfig11b", "wl_frag"); %>" onBlur="validate_range(this, 256, 2346)" onKeyPress="return is_number(this)"></td>
</tr>
<tr>
<td class="content_header_td"><#WLANConfig11b_x_RTS_itemname#>
           </td><td class="content_input_td"><input type="text" maxlength="5" size="5" name="wl_rts" class="content_input_fd" value="<% nvram_get_x("WLANConfig11b", "wl_rts"); %>" onBlur="validate_range(this, 0, 2347)" onKeyPress="return is_number(this)"></td>
</tr>
<tr>
<td class="content_header_td"><#WLANConfig11b_x_DTIM_itemname#>
           </td><td class="content_input_td"><input type="text" maxlength="5" size="5" name="wl_dtim" class="content_input_fd" value="<% nvram_get_x("WLANConfig11b", "wl_dtim"); %>" onBlur="validate_range(this, 1, 255)" onKeyPress="return is_number(this)"></td>
</tr>
<tr>
<td class="content_header_td"><#WLANConfig11b_x_Beacon_itemname#>
           </td><td class="content_input_td"><input type="text" maxlength="5" size="5" name="wl_bcn" class="content_input_fd" value="<% nvram_get_x("WLANConfig11b", "wl_bcn"); %>" onBlur="validate_range(this, 1, 65535)" onKeyPress="return is_number(this)"></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_x_FrameBurst_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_x_FrameBurst_itemname#>
           </td><td class="content_input_td"><select name="wl_frameburst" class="content_input_fd" onChange="return change_common(this, 'WLANConfig11b', 'wl_frameburst')"><option class="content_input_fd" value="off" <% nvram_match_x("WLANConfig11b","wl_frameburst", "off","selected"); %>>Disabled</option><option class="content_input_fd" value="on" <% nvram_match_x("WLANConfig11b","wl_frameburst", "on","selected"); %>>Enabled</option></select></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_x_ExtendMode_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_x_ExtendMode_itemname#>
           </td><td class="content_input_td"><select name="wl_mode_ex" class="content_input_fd" onChange="return change_common(this, 'WLANConfig11b', 'wl_mode_ex')"><option class="content_input_fd" value="ap" <% nvram_match_x("WLANConfig11b","wl_mode_ex", "ap","selected"); %>>AP or WDS</option><option class="content_input_fd" value="sta" <% nvram_match_x("WLANConfig11b","wl_mode_ex", "sta","selected"); %>>Station</option><option class="content_input_fd" value="wet" <% nvram_match_x("WLANConfig11b","wl_mode_ex", "wet","selected"); %>>Ethernet Bridge</option></select></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_x_RadioEnable_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_x_RadioEnable_itemname#>
           </td><td class="content_input_td"><input type="radio" value="1" name="wl_radio_x" class="content_input_fd" onClick="return change_common_radio(this, 'WLANConfig11b', 'wl_radio_x', '1')" <% nvram_match_x("WLANConfig11b","wl_radio_x", "1", "checked"); %>>Yes</input><input type="radio" value="0" name="wl_radio_x" class="content_input_fd" onClick="return change_common_radio(this, 'WLANConfig11b', 'wl_radio_x', '0')" <% nvram_match_x("WLANConfig11b","wl_radio_x", "0", "checked"); %>>No</input></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_x_RadioEnableDate_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_x_RadioEnableDate_itemname#>
           </td><td class="content_input_td"><input type="hidden" maxlength="7" class="content_input_fd" size="7" name="wl_radio_date_x" value="<% nvram_get_x("WLANConfig11b","wl_radio_date_x"); %>">
<p style="word-spacing: 0; margin-top: 0; margin-bottom: 0">
<input type="checkbox" class="content_input_fd" name="wl_radio_date_x_Sun" onChange="return changeDate();">Sun</input><input type="checkbox" class="content_input_fd" name="wl_radio_date_x_Mon" onChange="return changeDate();">Mon</input><input type="checkbox" class="content_input_fd" name="wl_radio_date_x_Tue" onChange="return changeDate();">Tue</input><input type="checkbox" class="content_input_fd" name="wl_radio_date_x_Wed" onChange="return changeDate();">Wed</input>
</p>
<input type="checkbox" class="content_input_fd" name="wl_radio_date_x_Thu" onChange="return changeDate();">Thu</input><input type="checkbox" class="content_input_fd" name="wl_radio_date_x_Fri" onChange="return changeDate();">Fri</input><input type="checkbox" class="content_input_fd" name="wl_radio_date_x_Sat" onChange="return changeDate();">Sat</input></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_x_RadioEnableTime_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_x_RadioEnableTime_itemname#>
           </td><td class="content_input_td"><input type="hidden" maxlength="11" class="content_input_fd" size="11" name="wl_radio_time_x" value="<% nvram_get_x("WLANConfig11b","wl_radio_time_x"); %>"><input type="text" maxlength="2" class="content_input_fd" size="2" name="wl_radio_time_x_starthour" onKeyPress="return is_number(this)" onBlur="return validate_timerange(this, 0)">:
                <input type="text" maxlength="2" class="content_input_fd" size="2" name="wl_radio_time_x_startmin" onKeyPress="return is_number(this)" onBlur="return validate_timerange(this, 1)">-
                <input type="text" maxlength="2" class="content_input_fd" size="2" name="wl_radio_time_x_endhour" onKeyPress="return is_number(this)" onBlur="return validate_timerange(this, 2)">:
                <input type="text" maxlength="2" class="content_input_fd" size="2" name="wl_radio_time_x_endmin" onKeyPress="return is_number(this)" onBlur="return validate_timerange(this, 3)"></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_x_RadioPower_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_x_RadioPower_itemname#>
           </td><td class="content_input_td"><input type="text" maxlength="5" size="5" name="wl_radio_power_x" class="content_input_fd" value="<% nvram_get_x("WLANConfig11b", "wl_radio_power_x"); %>" onBlur="validate_range(this, 1, 84)" onKeyPress="return is_number(this)"></td>
</tr>
<tr>
<td class="content_header_td"><#WLANConfig11b_x_WMM_itemname#>
           </td><td class="content_input_td"><select name="wl_wme" class="content_input_fd" onChange="return change_common(this, 'WLANConfig11b', 'wl_wme')"><option class="content_input_fd" value="off" <% nvram_match_x("WLANConfig11b","wl_wme", "off","selected"); %>>Disabled</option><option class="content_input_fd" value="on" <% nvram_match_x("WLANConfig11b","wl_wme", "on","selected"); %>>Enabled</option></select></td>
</tr>
<tr>
<td class="content_header_td"><#WLANConfig11b_x_NOACK_itemname#>
           </td><td class="content_input_td"><select name="wl_wme_no_ack" class="content_input_fd" onChange="return change_common(this, 'WLANConfig11b', 'wl_wme_no_ack')"><option class="content_input_fd" value="off" <% nvram_match_x("WLANConfig11b","wl_wme_no_ack", "off","selected"); %>>Disabled</option><option class="content_input_fd" value="on" <% nvram_match_x("WLANConfig11b","wl_wme_no_ack", "on","selected"); %>>Enabled</option></select></td>
</tr>
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
