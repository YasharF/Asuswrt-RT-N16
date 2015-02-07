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
    	
<input type="hidden" name="current_page" value="Advanced_DMZDHCP_Content.asp"><input type="hidden" name="next_page" value="Advanced_DMZDWFilter_Content.asp"><input type="hidden" name="next_host" value=""><input type="hidden" name="sid_list" value="LANHostConfig;"><input type="hidden" name="group_id" value=""><input type="hidden" name="modified" value="0"><input type="hidden" name="action_mode" value=""><input type="hidden" name="first_time" value=""><input type="hidden" name="action_script" value=""><input type="hidden" name="preferred_lang" value="<% nvram_get_x("","preferred_lang"); %>">
<tr>
<td>
<table width="666" border="1" cellpadding="0" cellspacing="0" bordercolor="E0E0E0">
<tr class="content_header_tr">
<td class="content_header_td_title" colspan="2"><#t1WFW#> - <#t2WDHCP#></td>
</tr>
<tr>
<td class="content_desc_td" colspan="2"><#LANHostConfig_DmzDHCPServerConfigurable_sectiondesc#>
         </td>
</tr>
<tr>
<td class="content_header_td"><#LANHostConfig_DmzDHCPServerConfigurable_itemname#>
           </td><td class="content_input_td"><input type="radio" value="1" name="" class="content_input_fd" onClick="return change_common_radio(this, 'LANHostConfig', '', '1')" <% nvram_match_x("LANHostConfig","", "1", "checked"); %>>Yes</input><input type="radio" value="0" name="" class="content_input_fd" onClick="return change_common_radio(this, 'LANHostConfig', '', '0')" <% nvram_match_x("LANHostConfig","", "0", "checked"); %>>No</input></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#LANHostConfig_DmzDomainName_itemdesc#>', LEFT);" onMouseOut="return nd();"><#LANHostConfig_DmzDomainName_itemname#>
           </td><td class="content_input_td"><input type="text" maxlength="32" class="content_input_fd" size="32" name="" value="<% nvram_get_x("LANHostConfig",""); %>" onKeyPress="return is_string(this)" onBlur="validate_string(this)"></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#LANHostConfig_DmzMinAddress_itemdesc#>', LEFT);" onMouseOut="return nd();"><#LANHostConfig_DmzMinAddress_itemname#>
           </td><td class="content_input_td"><input type="text" maxlength="15" class="content_input_fd" size="15" name="" value="<% nvram_get_x("LANHostConfig",""); %>" onBlur="return validate_ipaddr(this, '')" onKeyPress="return is_ipaddr(this)" onKeyUp="change_ipaddr(this)"></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#LANHostConfig_DmzMaxAddress_itemdesc#>', LEFT);" onMouseOut="return nd();"><#LANHostConfig_DmzMaxAddress_itemname#>
           </td><td class="content_input_td"><input type="text" maxlength="15" class="content_input_fd" size="15" name="" value="<% nvram_get_x("LANHostConfig",""); %>" onBlur="return validate_ipaddr(this, '')" onKeyPress="return is_ipaddr(this)" onKeyUp="change_ipaddr(this)"></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#LANHostConfig_DmzLeaseTime_itemdesc#>', LEFT);" onMouseOut="return nd();"><#LANHostConfig_DmzLeaseTime_itemname#>
           </td><td class="content_input_td"><input type="text" maxlength="10" class="content_input_fd" size="10" name="" value="<% nvram_get_x("LANHostConfig",""); %>" onBlur="validate_range(this, 0, 4294967295)" onKeyPress="return is_number(this)"></td>
</tr>
<tr class="content_section_header_tr">
<td class="content_section_header_td" colspan="2"><#LANHostConfig_x_DmzLDNSServer1_sectionname#>
            </td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#LANHostConfig_x_DmzLDNSServer1_itemdesc#>', LEFT);" onMouseOut="return nd();"><#LANHostConfig_x_DmzLDNSServer1_itemname#>
           </td><td class="content_input_td"><input type="text" maxlength="15" class="content_input_fd" size="15" name="" value="<% nvram_get_x("LANHostConfig",""); %>" onBlur="return validate_ipaddr(this, '')" onKeyPress="return is_ipaddr(this)" onKeyUp="change_ipaddr(this)"></td>
</tr>
<tr>
<td class="content_header_td"><#LANHostConfig_x_DmzLDNSServer2_itemname#>
           </td><td class="content_input_td"><input type="text" maxlength="15" class="content_input_fd_ro" size="15" name="" value="<% nvram_get_f("FirewallConfig","DmzIP"); %>" readonly="1"></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#LANHostConfig_x_DmzWINSServer_itemdesc#>', LEFT);" onMouseOut="return nd();"><#LANHostConfig_x_DmzWINSServer_itemname#>
           </td><td class="content_input_td"><input type="text" maxlength="15" class="content_input_fd" size="15" name="" value="<% nvram_get_x("LANHostConfig",""); %>" onBlur="return validate_ipaddr(this, '')" onKeyPress="return is_ipaddr(this)" onKeyUp="change_ipaddr(this)"></td>
</tr>
<tr class="content_section_header_tr">
<td class="content_section_header_td" colspan="2"><#LANHostConfig_DmzManualDHCPEnable_sectionname#>
            </td>
</tr>
<tr>
<td class="content_header_td"><#LANHostConfig_DmzManualDHCPEnable_itemname#>
           </td><td class="content_input_td"><input type="radio" value="1" name="" class="content_input_fd" onClick="return change_common_radio(this, 'LANHostConfig', '', '1')" <% nvram_match_x("LANHostConfig","", "1", "checked"); %>>Yes</input><input type="radio" value="0" name="" class="content_input_fd" onClick="return change_common_radio(this, 'LANHostConfig', '', '0')" <% nvram_match_x("LANHostConfig","", "0", "checked"); %>>No</input></td>
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
<table width="666" border="2" cellpadding="0" cellspacing="0" bordercolor="E0E0E0">
<tr class="content_list_header_tr">
<td class="content_list_header_td" width="60%" id="DmzManualDHCPList"><#LANHostConfig_DmzManualDHCPList_groupitemdesc#>
         <input type="hidden" name="DmzManualDHCPCount_0" value="<% nvram_get_x("LANHostConfig", "DmzManualDHCPCount"); %>" readonly="1"></td><td width="10%">
<div align="center">
<input class="inputSubmit" type="submit" onMouseOut="buttonOut(this)" onMouseOver="buttonOver(this)" onClick="return markGroup(this, 'DmzManualDHCPList', 8, ' Add ');" name="DmzManualDHCPList" value="<#CTL_add#>" size="12">
</div>
</td><td width="10%">
<div align="center">
<input class="inputSubmit" type="submit" onMouseOut="buttonOut(this)" onMouseOver="buttonOver(this)" onClick="return markGroup(this, 'DmzManualDHCPList', 8, ' Del ');" name="DmzManualDHCPList" value="<#CTL_del#>" size="12">
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
<td></td><td class="content_list_field_header_td" colspan=""><#LANHostConfig_DmzManualMac_itemname#>	                
           	        </td><td class="content_list_field_header_td" colspan=""><#LANHostConfig_DmzManualIP_itemname#>	                
           	        </td><td></td>
</tr>
<tr>
<td></td><td class="content_list_input_td" colspan=""><input type="text" maxlength="12" class="content_input_list_fd" size="12" name="_0" onKeyPress="return is_hwaddr()"></td><td class="content_list_input_td" colspan=""><input type="text" maxlength="15" class="content_input_list_fd" size="14" name="_0" onKeyPress="return is_ipaddr(this)" onKeyUp="change_ipaddr(this)"></td>
</tr>
<tr>
<td></td><td colspan="10"><select size="4" name="DmzManualDHCPList_s" multiple="true" style="font-family: 'fixedsys'; font-size: '8pt'">
<% nvram_get_table_x("LANHostConfig","DmzManualDHCPList"); %>
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
