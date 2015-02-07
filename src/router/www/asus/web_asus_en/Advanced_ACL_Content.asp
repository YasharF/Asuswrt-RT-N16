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
    	
<input type="hidden" name="current_page" value="Advanced_ACL_Content.asp"><input type="hidden" name="next_page" value="SaveRestart.asp"><input type="hidden" name="next_host" value=""><input type="hidden" name="sid_list" value="DeviceSecurity11a;"><input type="hidden" name="group_id" value=""><input type="hidden" name="modified" value="0"><input type="hidden" name="action_mode" value=""><input type="hidden" name="first_time" value=""><input type="hidden" name="action_script" value=""><input type="hidden" name="preferred_lang" value="<% nvram_get_x("","preferred_lang"); %>">
<tr>
<td>
<table width="666" border="1" cellpadding="0" cellspacing="0" bordercolor="E0E0E0">
<tr class="content_header_tr">
<td class="content_header_td_title" colspan="2"><#t1Wireless#> - <#t2ACL#></td>
</tr>
<tr>
<td class="content_desc_td" colspan="2"><#DeviceSecurity11a_display1_sectiondesc#>
         </td>
</tr>
<tr>
<td class="content_header_td"><#DeviceSecurity11a_ACLMethod_itemname#>
           </td><td class="content_input_td"><select name="wl_macmode" class="content_input_fd" onChange="return change_common(this, 'DeviceSecurity11a', 'wl_macmode')"><option class="content_input_fd" value="disabled" <% nvram_match_x("DeviceSecurity11a","wl_macmode", "disabled","selected"); %>>Disable</option><option class="content_input_fd" value="allow" <% nvram_match_x("DeviceSecurity11a","wl_macmode", "allow","selected"); %>>Accept</option><option class="content_input_fd" value="deny" <% nvram_match_x("DeviceSecurity11a","wl_macmode", "deny","selected"); %>>Reject</option></select></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#DeviceSecurity11a_ApplyDevices_itemdesc#>', LEFT);" onMouseOut="return nd();"><#DeviceSecurity11a_ApplyDevices_itemname#>
           </td><td class="content_input_td"><select name="wl_macapply_x" class="content_input_fd" onChange="return change_common(this, 'DeviceSecurity11a', 'wl_macapply_x')"><option class="content_input_fd" value="Both" <% nvram_match_x("DeviceSecurity11a","wl_macapply_x", "Both","selected"); %>>Both</option><option class="content_input_fd" value="802.11a only" <% nvram_match_x("DeviceSecurity11a","wl_macapply_x", "802.11a only","selected"); %>>802.11a only</option><option class="content_input_fd" value="802.11g only" <% nvram_match_x("DeviceSecurity11a","wl_macapply_x", "802.11g only","selected"); %>>802.11g only</option></select></td>
</tr>
</table>
</td>
</tr>
<tr>
<td>
<table width="666" border="2" cellpadding="0" cellspacing="0" bordercolor="E0E0E0">
<tr class="content_list_header_tr">
<td class="content_list_header_td" width="60%" id="ACLList"><#DeviceSecurity11a_ACLList_groupitemdesc#>
         <input type="hidden" name="wl_macnum_x_0" value="<% nvram_get_x("DeviceSecurity11a", "wl_macnum_x"); %>" readonly="1"></td><td width="10%">
<div align="center">
<input class="inputSubmit" type="submit" onMouseOut="buttonOut(this)" onMouseOver="buttonOver(this)" onClick="return markGroup(this, 'ACLList', 64, ' Add ');" name="ACLList" value="<#CTL_add#>" size="12">
</div>
</td><td width="10%">
<div align="center">
<input class="inputSubmit" type="submit" onMouseOut="buttonOut(this)" onMouseOver="buttonOver(this)" onClick="return markGroup(this, 'ACLList', 64, ' Del ');" name="ACLList" value="<#CTL_del#>" size="12">
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
<td></td><td class="content_list_field_header_td" colspan=""><#DeviceSecurity11a_MacAddr_itemname#>	                
           	        </td><td></td>
</tr>
<tr>
<td></td><td class="content_list_input_td" colspan=""><input type="text" maxlength="12" class="content_input_list_fd" size="32" name="wl_maclist_x_0" onKeyPress="return is_hwaddr()"></td>
</tr>
<tr>
<td></td><td colspan="10"><select size="8" name="ACLList_s" multiple="true" style="font-family: 'fixedsys'; font-size: '8pt'">
<% nvram_get_table_x("DeviceSecurity11a","ACLList"); %>
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
