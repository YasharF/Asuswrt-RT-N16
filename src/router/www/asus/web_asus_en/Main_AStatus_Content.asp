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
    	
<input type="hidden" name="current_page" value="Main_AStatus_Content.asp"><input type="hidden" name="next_page" value="default value"><input type="hidden" name="next_host" value=""><input type="hidden" name="sid_list" value="default value"><input type="hidden" name="group_id" value=""><input type="hidden" name="modified" value="0"><input type="hidden" name="action_mode" value=""><input type="hidden" name="first_time" value=""><input type="hidden" name="action_script" value=""><input type="hidden" name="load_script" value="<% load_script("printer.sh"); %>"><input type="hidden" name="preferred_lang" value="<% nvram_get_x("","preferred_lang"); %>">
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
<td class="content_section_header_td" colspan="2"><#PrinterStatus_x_PrinterModel_statusname#></td>
</tr>
<tr>
<td class="content_header_td_less" onMouseOver="return overlib('<#PrinterStatus_x_PrinterModel_itemdesc#>', LEFT);" onMouseOut="return nd();"><#PrinterStatus_x_PrinterModel_itemname#>
           </td><td class="content_input_td"><input type="text" size="36" class="content_input_fd" name="" value="<% nvram_get_f("printer_status.log","printer_model_t"); %>" readonly="1"></td>
</tr>
<tr>
<td class="content_header_td_less" onMouseOver="return overlib('<#PrinterStatus_x_PrinterStatus_itemdesc#>', LEFT);" onMouseOut="return nd();"><#PrinterStatus_x_PrinterStatus_itemname#>
           </td><td class="content_input_td"><input type="text" size="36" class="content_input_fd" name="" value="<% nvram_get_f("printer_status.log","printer_status_t"); %>" readonly="1"></td>
</tr>
<tr>
<td class="content_header_td_less" onMouseOver="return overlib('<#PrinterStatus_x_PrinterUser_itemdesc#>', LEFT);" onMouseOut="return nd();"><#PrinterStatus_x_PrinterUser_itemname#>
           </td><td class="content_input_td"><input type="text" size="36" class="content_input_fd" name="" value="<% nvram_get_f("printer_status.log","printer_user_t"); %>" readonly="1"></td>
</tr>
<tr>
<td class="content_header_td_less"><#PrinterStatus_x_PrinterAction_itemname#>
           </td><td class="content_input_td"><input type="submit" maxlength="15" class="content_input_fd_ro" onClick="return onSubmitApply('lpr_remove')" size="12" name="PrinterStatus_x_PrinterAction_button" value="<#PrinterStatus_x_PrinterAction_buttonname#>"></td>
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
