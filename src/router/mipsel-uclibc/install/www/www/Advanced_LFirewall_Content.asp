<head>
<title>WL500g.Deluxe Web Manager</title>
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
<input type="hidden" name="current_page" value="Advanced_LFirewall_Content.asp"><input type="hidden" name="next_page" value="Advanced_DMZIP_Content.asp"><input type="hidden" name="next_host" value=""><input type="hidden" name="sid_list" value="FirewallConfig;"><input type="hidden" name="group_id" value=""><input type="hidden" name="modified" value="0"><input type="hidden" name="action_mode" value=""><input type="hidden" name="first_time" value=""><input type="hidden" name="action_script" value=""><input type="hidden" name="preferred_lang" value="<% nvram_get_x("","preferred_lang"); %>">
<tr>
<td>
<table width="666" border="1" cellpadding="0" cellspacing="0" bordercolor="E0E0E0">
<tr class="content_header_tr">
<td class="content_header_td_title" colspan="2"><#t1IFW#> - t2WZFilter</td>
</tr>
<tr>
<td class="content_desc_td" colspan="2"><#FirewallConfig_WanLocalFirewallEnable_sectiondesc#>
</td>
</tr>
<tr>
<td class="content_header_td"><#FirewallConfig_WanLocalFirewallEnable_itemname#>
</td><td class="content_input_td"><input type="radio" value="1" name="" class="content_input_fd" onClick="return change_common_radio(this, 'FirewallConfig', '', '1')" <% nvram_match_x("FirewallConfig","", "1", "checked"); %>>Yes</input><input type="radio" value="0" name="" class="content_input_fd" onClick="return change_common_radio(this, 'FirewallConfig', '', '0')" <% nvram_match_x("FirewallConfig","", "0", "checked"); %>>No</input></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#FirewallConfig_WanLocalActiveDate_itemdesc#>', LEFT);" onMouseOut="return nd();"><#FirewallConfig_WanLocalActiveDate_itemname#>
</td><td class="content_input_td"><input type="hidden" maxlength="7" class="content_input_fd" size="7" name="" value="<% nvram_get_x("FirewallConfig",""); %>">
<p style="word-spacing: 0; margin-top: 0; margin-bottom: 0">
<input type="checkbox" class="content_input_fd" name="_Sun" onChange="return changeDate();">Sun</input><input type="checkbox" class="content_input_fd" name="_Mon" onChange="return changeDate();">Mon</input><input type="checkbox" class="content_input_fd" name="_Tue" onChange="return changeDate();">Tue</input><input type="checkbox" class="content_input_fd" name="_Wed" onChange="return changeDate();">Wed</input>
</p>
<input type="checkbox" class="content_input_fd" name="_Thu" onChange="return changeDate();">Thu</input><input type="checkbox" class="content_input_fd" name="_Fri" onChange="return changeDate();">Fri</input><input type="checkbox" class="content_input_fd" name="_Sat" onChange="return changeDate();">Sat</input></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#FirewallConfig_WanLocalActiveTime_itemdesc#>', LEFT);" onMouseOut="return nd();"><#FirewallConfig_WanLocalActiveTime_itemname#>
</td><td class="content_input_td"><input type="hidden" maxlength="11" class="content_input_fd" size="11" name="" value="<% nvram_get_x("FirewallConfig",""); %>"><input type="text" maxlength="2" class="content_input_fd" size="2" name="_starthour" onKeyPress="return is_number(this)" onBlur="return validate_timerange(this, 0)">:
<input type="text" maxlength="2" class="content_input_fd" size="2" name="_startmin" onKeyPress="return is_number(this)" onBlur="return validate_timerange(this, 1)">-
<input type="text" maxlength="2" class="content_input_fd" size="2" name="_endhour" onKeyPress="return is_number(this)" onBlur="return validate_timerange(this, 2)">:
<input type="text" maxlength="2" class="content_input_fd" size="2" name="_endmin" onKeyPress="return is_number(this)" onBlur="return validate_timerange(this, 3)"></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#FirewallConfig_WanLocalDefaultAct_itemdesc#>', LEFT);" onMouseOut="return nd();"><#FirewallConfig_WanLocalDefaultAct_itemname#>
</td><td class="content_input_td"><select name="" class="content_input_fd" onChange="return change_common(this, 'FirewallConfig', '')"><option class="content_input_fd" value="DROP" <% nvram_match_x("FirewallConfig","", "DROP","selected"); %>>DROP</option><option class="content_input_fd" value="ACCEPT" <% nvram_match_x("FirewallConfig","", "ACCEPT","selected"); %>>ACCEPT</option></select></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#FirewallConfig_WanLocalICMP_itemdesc#>', LEFT);" onMouseOut="return nd();"><#FirewallConfig_WanLocalICMP_itemname#>
</td><td class="content_input_td"><input type="text" maxlength="32" class="content_input_fd" size="32" name="" value="<% nvram_get_x("FirewallConfig",""); %>" onBlur="return validate_portlist(this, '')" onKeyPress="return is_portlist(this)"></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#FirewallConfig_WanLocalLog_itemdesc#>', LEFT);" onMouseOut="return nd();"><#FirewallConfig_WanLocalLog_itemname#>
</td><td class="content_input_td"><select name="" class="content_input_fd" onChange="return change_common(this, 'FirewallConfig', '')"><option class="content_input_fd" value="None" <% nvram_match_x("FirewallConfig","", "None","selected"); %>>None</option><option class="content_input_fd" value="Dropped" <% nvram_match_x("FirewallConfig","", "Dropped","selected"); %>>Dropped</option><option class="content_input_fd" value="Accepted" <% nvram_match_x("FirewallConfig","", "Accepted","selected"); %>>Accepted</option><option class="content_input_fd" value="Both" <% nvram_match_x("FirewallConfig","", "Both","selected"); %>>Both</option></select></td>
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
<table width="666" border="2" cellpadding="0" cellspacing="0" bordercolor="E0E0E0">
<tr class="content_list_header_tr">
<td class="content_list_header_td" width="60%" id="WLocalFilterList"><#FirewallConfig_WLocalFilterList_groupitemdesc#>
<input type="hidden" name="WanLocalRuleCount_0" value="<% nvram_get_x("FirewallConfig", "WanLocalRuleCount"); %>" readonly="1"></td><td width="10%">
<div align="center">
<input class="inputSubmit" type="submit" onMouseOut="buttonOut(this)" onMouseOver="buttonOver(this)" onClick="return markGroup(this, 'WLocalFilterList', 32, ' Add ');" name="WLocalFilterList" value="<#CTL_add#>" size="12">
</div>
</td><td width="10%">
<div align="center">
<input class="inputSubmit" type="submit" onMouseOut="buttonOut(this)" onMouseOver="buttonOver(this)" onClick="return markGroup(this, 'WLocalFilterList', 32, ' Del ');" name="WLocalFilterList" value="<#CTL_del#>" size="12">
</div>
</td><td width="10%">
<div align="center">
<input class="inputSubmit" type="button" onMouseOut="buttonOut(this)" onMouseOver="buttonOver(this)" onClick="return openHelp(this, 'FilterHelp');" name="WLocalFilterList" value="<#CTL_help#>" size="12">
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
<td></td><td class="content_list_field_header_td" colspan=""><#FirewallConfig_WanLocalSrcIP_itemname#>
</td><td class="content_list_field_header_td" colspan=""><#FirewallConfig_WanLocalDstPort_itemname#>
</td><td class="content_list_field_header_td" colspan=""><#FirewallConfig_WanLocalProFlag_itemname#>
</td><td></td>
</tr>
<tr>
<td></td><td class="content_list_input_td" colspan=""><input type="text" maxlength="15" class="content_input_list_fd" size="14" name="_0" onKeyPress="return is_iprange(this)" onKeyUp="change_iprange(this)"></td><td class="content_list_input_td" colspan=""><input type="text" maxlength="11" class="content_input_list_fd" size="10" name="_0" value="" onKeyPress="return is_portrange(this)"></td><td class="content_list_input_td" colspan=""><select name="_0" class="content_input_list_fd"><option value="TCP" <% nvram_match_list_x("FirewallConfig","", "TCP","selected", 0); %>>TCP</option><option value="TCP ALL" <% nvram_match_list_x("FirewallConfig","", "TCP ALL","selected", 0); %>>TCP ALL</option><option value="TCP SYN" <% nvram_match_list_x("FirewallConfig","", "TCP SYN","selected", 0); %>>TCP SYN</option><option value="TCP ACK" <% nvram_match_list_x("FirewallConfig","", "TCP ACK","selected", 0); %>>TCP ACK</option><option value="TCP FTN" <% nvram_match_list_x("FirewallConfig","", "TCP FTN","selected", 0); %>>TCP FTN</option><option value="TCP RST" <% nvram_match_list_x("FirewallConfig","", "TCP RST","selected", 0); %>>TCP RST</option><option value="TCP URG" <% nvram_match_list_x("FirewallConfig","", "TCP URG","selected", 0); %>>TCP URG</option><option value="TCP PSH" <% nvram_match_list_x("FirewallConfig","", "TCP PSH","selected", 0); %>>TCP PSH</option><option value="UDP" <% nvram_match_list_x("FirewallConfig","", "UDP","selected", 0); %>>UDP</option></select></td>
</tr>
<tr>
<td></td><td colspan="10"><select size="8" name="WLocalFilterList_s" multiple="true" style="font-family: 'fixedsys'; font-size: '8pt'">
<% nvram_get_table_x("FirewallConfig","WLocalFilterList"); %>
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
