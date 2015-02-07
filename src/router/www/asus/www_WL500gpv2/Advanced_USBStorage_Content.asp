<head>
<title>ASUS Wireless Router Web Manager</title>
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
<input type="hidden" name="current_page" value="Advanced_USBStorage_Content.asp"><input type="hidden" name="next_page" value="Advanced_WebCam_Content.asp"><input type="hidden" name="next_host" value=""><input type="hidden" name="sid_list" value="PrinterStatus;"><input type="hidden" name="group_id" value=""><input type="hidden" name="modified" value="0"><input type="hidden" name="action_mode" value=""><input type="hidden" name="first_time" value=""><input type="hidden" name="action_script" value=""><input type="hidden" name="preferred_lang" value="<% nvram_get_x("","preferred_lang"); %>">
<tr>
<td>
<table width="666" border="1" cellpadding="0" cellspacing="0" bordercolor="E0E0E0">
<tr class="content_header_tr">
<td class="content_header_td_title" colspan="2"><#t1USB#> - <#t2FTP#></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#PrinterStatus_x_FEject_itemdesc#>', LEFT);" onMouseOut="return nd();"><#PrinterStatus_x_FEject_itemname#>
</td><td class="content_input_td"><input type="hidden" maxlength="15" class="content_input_fd_ro" size="12" name="" value="<% nvram_get_f("ddns.log","DDNSStatus"); %>"><input type="submit" maxlength="15" class="content_input_fd_ro" onClick="return onSubmitApply('eject-usb.sh')" size="12" name="PrinterStatus_x_FEject_button" value="<#PrinterStatus_x_FEject_buttonname#>"></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#PrinterStatus_x_FIsFtpServer_itemdesc#>', LEFT);" onMouseOut="return nd();"><#PrinterStatus_x_FIsFtpServer_itemname#>
</td><td class="content_input_td"><input type="radio" value="1" name="usb_ftpenable_x" class="content_input_fd" onClick="return change_common_radio(this, 'PrinterStatus', 'usb_ftpenable_x', '1')" <% nvram_match_x("PrinterStatus","usb_ftpenable_x", "1", "checked"); %>>Yes</input><input type="radio" value="0" name="usb_ftpenable_x" class="content_input_fd" onClick="return change_common_radio(this, 'PrinterStatus', 'usb_ftpenable_x', '0')" <% nvram_match_x("PrinterStatus","usb_ftpenable_x", "0", "checked"); %>>No</input></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#PrinterStatus_x_FIsAnonymous_itemdesc#>', LEFT);" onMouseOut="return nd();"><#PrinterStatus_x_FIsAnonymous_itemname#>
</td><td class="content_input_td"><input type="radio" value="1" name="usb_ftpanonymous_x" class="content_input_fd" onClick="return change_common_radio(this, 'PrinterStatus', 'usb_ftpanonymous_x', '1')" <% nvram_match_x("PrinterStatus","usb_ftpanonymous_x", "1", "checked"); %>>Yes</input><input type="radio" value="0" name="usb_ftpanonymous_x" class="content_input_fd" onClick="return change_common_radio(this, 'PrinterStatus', 'usb_ftpanonymous_x', '0')" <% nvram_match_x("PrinterStatus","usb_ftpanonymous_x", "0", "checked"); %>>No</input><a href="javascript:openLink('x_FIsAnonymous')" class="content_input_link" name="x_FIsAnonymous_link"><#PrinterStatus_x_FIsAnonymous_linkname#>
</a></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#PrinterStatus_x_FIsSuperuser_itemdesc#>', LEFT);" onMouseOut="return nd();"><#PrinterStatus_x_FIsSuperuser_itemname#>
</td><td class="content_input_td"><input type="radio" value="1" name="usb_ftpsuper_x" class="content_input_fd" onClick="return change_common_radio(this, 'PrinterStatus', 'usb_ftpsuper_x', '1')" <% nvram_match_x("PrinterStatus","usb_ftpsuper_x", "1", "checked"); %>>Yes</input><input type="radio" value="0" name="usb_ftpsuper_x" class="content_input_fd" onClick="return change_common_radio(this, 'PrinterStatus', 'usb_ftpsuper_x', '0')" <% nvram_match_x("PrinterStatus","usb_ftpsuper_x", "0", "checked"); %>>No</input><a href="javascript:openLink('x_FIsSuperuser')" class="content_input_link" name="x_FIsSuperuser_link"><#PrinterStatus_x_FIsSuperuser_linkname#>
</a></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#PrinterStatus_x_FPort_itemdesc#>', LEFT);" onMouseOut="return nd();"><#PrinterStatus_x_FPort_itemname#>
</td><td class="content_input_td"><input type="text" maxlength="5" size="5" name="usb_ftpport_x" class="content_input_fd" value="<% nvram_get_x("PrinterStatus", "usb_ftpport_x"); %>" onBlur="validate_range(this, 1, 65535)" onKeyPress="return is_number(this)"></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#PrinterStatus_x_FMaxUsers_itemdesc#>', LEFT);" onMouseOut="return nd();"><#PrinterStatus_x_FMaxUsers_itemname#>
</td><td class="content_input_td"><input type="text" maxlength="5" size="5" name="usb_ftpmax_x" class="content_input_fd" value="<% nvram_get_x("PrinterStatus", "usb_ftpmax_x"); %>" onBlur="validate_range(this, 1, 12)" onKeyPress="return is_number(this)"></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#PrinterStatus_x_FLoginTimeout_itemdesc#>', LEFT);" onMouseOut="return nd();"><#PrinterStatus_x_FLoginTimeout_itemname#>
</td><td class="content_input_td"><input type="text" maxlength="5" class="content_input_fd" size="5" name="usb_ftptimeout_x" value="<% nvram_get_x("PrinterStatus","usb_ftptimeout_x"); %>" onBlur="validate_range(this, 0, 65535)" onKeyPress="return is_number(this)"></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#PrinterStatus_x_FStayTimeout_itemdesc#>', LEFT);" onMouseOut="return nd();"><#PrinterStatus_x_FStayTimeout_itemname#>
</td><td class="content_input_td"><input type="text" maxlength="5" class="content_input_fd" size="5" name="usb_ftpstaytimeout_x" value="<% nvram_get_x("PrinterStatus","usb_ftpstaytimeout_x"); %>" onBlur="validate_range(this, 0, 65535)" onKeyPress="return is_number(this)"></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#PrinterStatus_x_FInitScript_itemdesc#>', LEFT);" onMouseOut="return nd();"><#PrinterStatus_x_FInitScript_itemname#>
</td><td class="content_input_td"><input type="text" maxlength="256" class="content_input_fd" size="32" name="usb_ftpscript_x" value="<% nvram_get_x("PrinterStatus","usb_ftpscript_x"); %>" onKeyPress="return is_string(this)" onBlur="validate_string(this)"></td>
</tr>
</table>
</td>
</tr>
<tr>
<td>
<table width="666" border="2" cellpadding="0" cellspacing="0" bordercolor="E0E0E0">
<tr class="content_list_header_tr">
<td class="content_list_header_td" width="60%" id="x_FUserList"><#PrinterStatus_x_FUserList_groupitemdesc#>
<input type="hidden" name="usb_ftpnum_x_0" value="<% nvram_get_x("PrinterStatus", "usb_ftpnum_x"); %>" readonly="1"></td><td width="10%">
<div align="center">
<input class="inputSubmit" type="submit" onMouseOut="buttonOut(this)" onMouseOver="buttonOver(this)" onClick="return markGroup(this, 'x_FUserList', 32, ' Add ');" name="x_FUserList" value="<#CTL_add#>" size="12">
</div>
</td><td width="10%">
<div align="center">
<input class="inputSubmit" type="submit" onMouseOut="buttonOut(this)" onMouseOver="buttonOver(this)" onClick="return markGroup(this, 'x_FUserList', 32, ' Del ');" name="x_FUserList" value="<#CTL_del#>" size="12">
</div>
</td><td width="5%"></td>
</tr>
<table class="content_list_table" width="640" border="0" cellspacing="0" cellpadding="0">
<tr>
<td colspan="3" height="10"></td>
</tr>
<tr>
<td colspan="3">
<div align="center">
<table class="content_list_value_table" border="1" cellspacing="0" cellpadding="0">
<tr>
<td></td><td class="content_list_field_header_td" colspan=""><#PrinterStatus_x_FUserName_itemname#>
</td><td class="content_list_field_header_td" colspan=""><#PrinterStatus_x_FUserPass_itemname#>
</td><td class="content_list_field_header_td" colspan=""><#PrinterStatus_x_FUserLogin_itemname#>
</td><td class="content_list_field_header_td" colspan=""><#PrinterStatus_x_FUserRight_itemname#>
</td><td></td>
</tr>
<tr>
<td></td><td class="content_list_input_td" colspan=""><input type="text" maxlength="16" size="16" name="usb_ftpusername_x_0" class="content_input_list_fd" onKeyPress="return is_string(this)"></td><td class="content_list_input_td" colspan=""><input type="text" maxlength="16" size="16" name="usb_ftppasswd_x_0" class="content_input_list_fd" onKeyPress="return is_string(this)"></td><td class="content_list_input_td" colspan=""><input type="text" maxlength="5" style="font-family: fixedsys; font-size: 10pt;" size="5" name="usb_ftpmaxuser_x_0" onKeyPress="return is_number(this)"></td><td class="content_list_input_td" colspan=""><select name="usb_ftprights_x_0" class="content_input_list_fd"><option value="Read/Write/Erase" <% nvram_match_list_x("PrinterStatus","usb_ftprights_x", "Read/Write/Erase","selected", 0); %>>Read/Write/Erase</option><option value="Read/Write" <% nvram_match_list_x("PrinterStatus","usb_ftprights_x", "Read/Write","selected", 0); %>>Read/Write</option><option value="Read Only" <% nvram_match_list_x("PrinterStatus","usb_ftprights_x", "Read Only","selected", 0); %>>Read Only</option><option value="View Only" <% nvram_match_list_x("PrinterStatus","usb_ftprights_x", "View Only","selected", 0); %>>View Only</option><option value="Private" <% nvram_match_list_x("PrinterStatus","usb_ftprights_x", "Private","selected", 0); %>>Private</option><option value="Private(WO)" <% nvram_match_list_x("PrinterStatus","usb_ftprights_x", "Private(WO)","selected", 0); %>>Private(WO)</option></select></td>
</tr>
<tr>
<td></td><td colspan="10"><select size="8" name="x_FUserList_s" multiple="true" style="font-family: 'fixedsys'; font-size: '8pt'">
<% nvram_get_table_x("PrinterStatus","x_FUserList"); %>
</select></td>
</tr>
</table>
</div>
</td>
</tr>
<tr>
<td colspan="3" height="10"></td>
</tr>
</table>
</table>
</td>
</tr>
<tr>
<td>
<table width="666" border="2" cellpadding="0" cellspacing="0" bordercolor="E0E0E0">
<tr class="content_list_header_tr">
<td class="content_list_header_td" width="60%" id="x_FBanIPList"><#PrinterStatus_x_FBanIPList_groupitemdesc#>
<input type="hidden" name="usb_bannum_x_0" value="<% nvram_get_x("PrinterStatus", "usb_bannum_x"); %>" readonly="1"></td><td width="10%">
<div align="center">
<input class="inputSubmit" type="submit" onMouseOut="buttonOut(this)" onMouseOver="buttonOver(this)" onClick="return markGroup(this, 'x_FBanIPList', 16, ' Add ');" name="x_FBanIPList" value="<#CTL_add#>" size="12">
</div>
</td><td width="10%">
<div align="center">
<input class="inputSubmit" type="submit" onMouseOut="buttonOut(this)" onMouseOver="buttonOver(this)" onClick="return markGroup(this, 'x_FBanIPList', 16, ' Del ');" name="x_FBanIPList" value="<#CTL_del#>" size="12">
</div>
</td><td width="5%"></td>
</tr>
<table class="content_list_table" width="640" border="0" cellspacing="0" cellpadding="0">
<tr>
<td colspan="3" height="10"></td>
</tr>
<tr>
<td colspan="3">
<div align="center">
<table class="content_list_value_table" border="1" cellspacing="0" cellpadding="0">
<tr>
<td></td><td class="content_list_field_header_td" colspan=""><#PrinterStatus_x_FBanIP_itemname#>
</td><td></td>
</tr>
<tr>
<td></td><td class="content_list_input_td" colspan=""><input type="text" maxlength="15" class="content_input_list_fd" size="24" name="usb_ftpbanip_x_0" onKeyPress="return is_iprange(this)" onKeyUp="change_iprange(this)"></td>
</tr>
<tr>
<td></td><td colspan="10"><select size="8" name="x_FBanIPList_s" multiple="true" style="font-family: 'fixedsys'; font-size: '8pt'">
<% nvram_get_table_x("PrinterStatus","x_FBanIPList"); %>
</select></td>
</tr>
</table>
</div>
</td>
</tr>
<tr>
<td colspan="3" height="10"></td>
</tr>
</table>
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
