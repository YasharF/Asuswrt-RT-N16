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
    	
<input type="hidden" name="current_page" value="Main_GStatus_Content.asp"><input type="hidden" name="next_page" value="default value"><input type="hidden" name="next_host" value=""><input type="hidden" name="sid_list" value="default value"><input type="hidden" name="group_id" value=""><input type="hidden" name="modified" value="0"><input type="hidden" name="action_mode" value=""><input type="hidden" name="first_time" value=""><input type="hidden" name="action_script" value=""><input type="hidden" name="load_script" value="<% load_script("wan.sh"); %>"><input type="hidden" name="preferred_lang" value="<% nvram_get_x("","preferred_lang"); %>">
<tr>
<td>
<table width="666" border="1" cellpadding="0" cellspacing="0" bordercolor="E0E0E0">
<tr class="content_header_tr">
<td class="content_header_td_title" colspan="2"><#t1Status#> - <#t2Status#></td>
</tr>
<tr>
<td class="content_header_td_less" onMouseOver="return overlib('<#General_x_SystemUpTime_itemdesc#>', LEFT);" onMouseOut="return nd();"><#General_x_SystemUpTime_itemname#>
           </td><td class="content_input_td"><input type="text" size="48" class="content_input_fd" value="<% uptime(); %>" readonly="1"></td>
</tr>
</table>
</td>
</tr>
<tr>
<td>
<table width="666" border="1" cellpadding="0" cellspacing="0" bordercolor="E0E0E0">
<tr class="content_section_header_tr">
<td class="content_section_header_td" colspan="2"><#PPPConnection_x_WANType_statusname#></td>
</tr>
<tr>
<td class="content_header_td_less"><#PPPConnection_x_WANType_itemname#>
           </td><td class="content_input_td"><input type="text" size="36" class="content_input_fd" name="wan_proto_t" value="<% nvram_get_f("wan.log","wan_proto_t"); %>" readonly="1"></td>
</tr>
<tr>
<td class="content_header_td_less"><#PPPConnection_x_WANIPAddress_itemname#>
           </td><td class="content_input_td"><input type="text" size="36" class="content_input_fd" name="wan_ipaddr_t" value="<% nvram_get_f("wan.log","wan_ipaddr_t"); %>" readonly="1"></td>
</tr>
<tr>
<td class="content_header_td_less"><#PPPConnection_x_WANSubnetMask_itemname#>
           </td><td class="content_input_td"><input type="text" size="36" class="content_input_fd" name="wan_netmask_t" value="<% nvram_get_f("wan.log","wan_netmask_t"); %>" readonly="1"></td>
</tr>
<tr>
<td class="content_header_td_less"><#PPPConnection_x_WANGateway_itemname#>
           </td><td class="content_input_td"><input type="text" size="36" class="content_input_fd" name="wan_gateway_t" value="<% nvram_get_f("wan.log","wan_gateway_t"); %>" readonly="1"></td>
</tr>
<tr>
<td class="content_header_td_less"><#PPPConnection_x_WANDNSServer_itemname#>
           </td><td class="content_input_td"><input type="text" size="36" class="content_input_fd" name="wan_dns_t" value="<% nvram_get_f("wan.log","wan_dns_t"); %>" readonly="1"></td>
</tr>
<tr>
<td class="content_header_td_less"><#PPPConnection_x_WANLink_itemname#>
           </td><td class="content_input_td"><input type="text" size="36" class="content_input_fd" name="wan_status_t" value="<% nvram_get_f("wan.log","wan_status_t"); %>" readonly="1"></td>
</tr>
<tr>
<td class="content_header_td_less"><#PPPConnection_x_WANAction_itemname#>
           </td><td class="content_input_td"><input type="submit" maxlength="15" class="content_input_fd_ro" onClick="return onSubmitApply('dhcpc_release')" size="12" name="PPPConnection_x_WANAction_button" value="<#PPPConnection_x_WANAction_buttonname#>"><input type="submit" maxlength="15" class="content_input_fd_ro" onClick="return onSubmitApply('dhcpc_renew')" size="12" name="PPPConnection_x_WANAction_button1" value="<#PPPConnection_x_WANAction_button1name#>"></td>
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
<table width="666" border="1" cellpadding="0" cellspacing="0" bordercolor="E0E0E0">
<tr class="content_section_header_tr">
<td class="content_section_header_td" colspan="2"><#LANHostConfig_x_LANIPAddress_statusname#></td>
</tr>
<tr>
<td class="content_header_td_less"><#LANHostConfig_x_LANIPAddress_itemname#>
           </td><td class="content_input_td"><input type="text" size="36" class="content_input_fd" name="lan_ipaddr_t" value="<% nvram_get_f("lan.log","lan_ipaddr_t"); %>" readonly="1"></td>
</tr>
<tr>
<td class="content_header_td_less"><#LANHostConfig_x_LANSubnetMask_itemname#>
           </td><td class="content_input_td"><input type="text" size="36" class="content_input_fd" name="lan_netmask_t" value="<% nvram_get_f("lan.log","lan_netmask_t"); %>" readonly="1"></td>
</tr>
<tr>
<td class="content_header_td_less"><#LANHostConfig_x_LANGateway_itemname#>
           </td><td class="content_input_td"><input type="text" size="36" class="content_input_fd" name="lan_gateway_t" value="<% nvram_get_f("lan.log","lan_gateway_t"); %>" readonly="1"></td>
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
<table width="666" border="1" cellpadding="0" cellspacing="0" bordercolor="B0B0B0">
<tr bgcolor="#CCCCCC"><td colspan="3"><font face="arial" size="2"><b>&nbsp</b></font></td></tr>
<tr bgcolor="#FFFFFF">  
   <td height="25" width="34%">  
   </td>
   <td height="25" width="33%">  
   </td>
   <td height="25" width="33%">  
   <div align="center"><font face="Arial"> <input class=inputSubmit onMouseOut=buttonOut(this) onMouseOver="buttonOver(this)" onClick="onSubmitCtrl(this, ' Refresh ')" type="submit" value="<#CTL_refresh#>" name="action"></font></div> 
   </td>
</tr>
</table>
</td>
</tr>

</table>
</form>
</body>
