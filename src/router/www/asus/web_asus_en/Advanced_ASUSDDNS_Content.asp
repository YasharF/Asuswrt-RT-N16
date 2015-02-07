<head>
<title>ZVMODELVZ Web Manager</title>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<link rel="stylesheet" type="text/css" href="style.css" media="screen"></link>
<script language="JavaScript" type="text/javascript" src="overlib.js"></script>
<script language="JavaScript" type="text/javascript" src="general.js"></script>
<!--2007.03.25 Yau add for asus ddns-->
<script>
<!--
function ddns_load_body()
{
var ddns_timeout = '<% nvram_get_ddns("LANHostConfig","ddns_timeout"); %>';
var hostname_x = '<% nvram_get_x("LANHostConfig","ddns_hostname_x"); %>';
var ddns_return_code = '<% nvram_get_ddns("LANHostConfig","ddns_return_code"); %>';
var ddns_old_name = '<% nvram_get_x("LANHostConfig","ddns_old_name"); %>';
var ddns_server_x = '<% nvram_get_x("LANHostConfig","ddns_server_x"); %>';

if(ddns_return_code == '')
{
	if(ddns_timeout == '1')
		alert("<#LANHostConfig_x_DDNS_alarm_1#>");
}
else if(ddns_return_code == 'register,-1')
        alert("<#LANHostConfig_x_DDNS_alarm_2#>");
else if(ddns_return_code == 'register,200')
        alert("<#LANHostConfig_x_DDNS_alarm_3#>");
else if(ddns_return_code == 'register,203')
        alert("<#LANHostConfig_x_DDNS_alarm_hostname#> '"+ hostname_x +"' <#LANHostConfig_x_DDNS_alarm_registered#>");
else if(ddns_return_code == 'register,220')
        alert("<#LANHostConfig_x_DDNS_alarm_4#>");
else if(ddns_return_code == 'register,230')
        alert("<#LANHostConfig_x_DDNS_alarm_5#>");
else if(ddns_return_code == 'register,233')
        alert("<#LANHostConfig_x_DDNS_alarm_hostname#> '"+ hostname_x +"' <#LANHostConfig_x_DDNS_alarm_registered_2#> '"+ddns_old_name+"'.");
else if(ddns_return_code == 'register,296')
        alert("<#LANHostConfig_x_DDNS_alarm_6#>");
else if(ddns_return_code == 'register,297')
        alert("<#LANHostConfig_x_DDNS_alarm_7#>");
else if(ddns_return_code == 'register,298')
        alert("<#LANHostConfig_x_DDNS_alarm_8#>");
else if(ddns_return_code == 'register,299')
        alert("<#LANHostConfig_x_DDNS_alarm_9#>");
else if(ddns_return_code == 'register,401')
        alert("<#LANHostConfig_x_DDNS_alarm_10#>");
else if(ddns_return_code == 'register,407')
        alert("<#LANHostConfig_x_DDNS_alarm_11#>");
else if(ddns_return_code == 'Time-out')
        alert("<#LANHostConfig_x_DDNS_alarm_12#>");

if (ddns_server_x == "WWW.ASUS.COM" || ddns_server_x == "")
{inputCtrl(document.form.ddns_username_x, 0);
inputCtrl(document.form.ddns_passwd_x, 0);
document.form.ddns_wildcard_x[0].disabled= 1;
document.form.ddns_wildcard_x[1].disabled= 1;
document.form.LANHostConfig_x_DDNSHostnameCheck_button.disabled= 0;
showhide("link", 0);
showhide("Hostname_Note", 1);
}
else
{inputCtrl(document.form.ddns_username_x, 1);
inputCtrl(document.form.ddns_passwd_x, 1);
document.form.ddns_wildcard_x[0].disabled= 0;
document.form.ddns_wildcard_x[1].disabled= 0;
document.form.LANHostConfig_x_DDNSHostnameCheck_button.disabled= 1;
showhide("Hostname_Note", 0);
}

}
-->
</script>
</head>  
<div id="overDiv" style="position:absolute; visibility:hidden; z-index:1000;"></div>    
<body onLoad="ddns_load_body()" onunLoad="return unload_body();">
<form method="GET" name="form" action="apply.cgi">
<!-- Table for the conntent page -->	    
<table width="666" border="0" cellpadding="0" cellspacing="0">     	      
    	
<input type="hidden" name="current_page" value="Advanced_ASUSDDNS_Content.asp"><input type="hidden" name="next_page" value="SaveRestart.asp"><input type="hidden" name="next_host" value=""><input type="hidden" name="sid_list" value="LANHostConfig;"><input type="hidden" name="group_id" value=""><input type="hidden" name="modified" value="0"><input type="hidden" name="action_mode" value=""><input type="hidden" name="first_time" value=""><input type="hidden" name="action_script" value=""><input type="hidden" name="preferred_lang" value="<% nvram_get_x("","preferred_lang"); %>">
<tr>
<td>
<table width="666" border="1" cellpadding="0" cellspacing="0" bordercolor="E0E0E0">
<tr class="content_section_header_tr">
<td class="content_section_header_td" colspan="2"><#LANHostConfig_x_DDNSEnable_sectionname#>
            </td>
</tr>
<tr>
<td class="content_desc_td" colspan="2"><#LANHostConfig_x_DDNSEnable_sectiondesc#>
         </td>
</tr>
<tr>
<td class="content_header_td"><#LANHostConfig_x_DDNSEnable_itemname#>
           </td><td class="content_input_td"><input type="radio" value="1" name="ddns_enable_x" class="content_input_fd" onClick="return change_common_radio(this, 'LANHostConfig', 'ddns_enable_x', '1')" <% nvram_match_x("LANHostConfig","ddns_enable_x", "1", "checked"); %>>Yes</input><input type="radio" value="0" name="ddns_enable_x" class="content_input_fd" onClick="return change_common_radio(this, 'LANHostConfig', 'ddns_enable_x', '0')" <% nvram_match_x("LANHostConfig","ddns_enable_x", "0", "checked"); %>>No</input></td>
</tr>
<tr>
<td class="content_header_td"><#LANHostConfig_x_DDNSServer_itemname#></td>
<td class="content_input_td">
	<select name="ddns_server_x" class="content_input_fd" onChange="return change_common(this, 'LANHostConfig', 'ddns_server_x')">
<!-- 2007.03.20 Yau add "WWW.ASUS.COM" -->
	    <option class="content_input_fd" value="WWW.ASUS.COM" <% nvram_match_x("LANHostConfig","ddns_server_x", "WWW.ASUS.COM","selected"); %>>WWW.ASUS.COM</option>
            <option class="content_input_fd" value="WWW.DYNDNS.ORG" <% nvram_match_x("LANHostConfig","ddns_server_x", "WWW.DYNDNS.ORG","selected"); %>>WWW.DYNDNS.ORG</option>
            <option class="content_input_fd" value="WWW.DYNDNS.ORG(CUSTOM)" <% nvram_match_x("LANHostConfig","ddns_server_x", "WWW.DYNDNS.ORG(CUSTOM)","selected"); %>>WWW.DYNDNS.ORG(CUSTOM)</option>
            <option class="content_input_fd" value="WWW.DYNDNS.ORG(STATIC)" <% nvram_match_x("LANHostConfig","ddns_server_x", "WWW.DYNDNS.ORG(STATIC)","selected"); %>>WWW.DYNDNS.ORG(STATIC)</option>
	    <option class="content_input_fd" value="WWW.TZO.COM" <% nvram_match_x("LANHostConfig","ddns_server_x", "WWW.TZO.COM","selected"); %>>WWW.TZO.COM</option>
	    <option class="content_input_fd" value="WWW.ZONEEDIT.COM" <% nvram_match_x("LANHostConfig","ddns_server_x", "WWW.ZONEEDIT.COM","selected"); %>>WWW.ZONEEDIT.COM</option>
	</select>
	<div id="link">
        <a href="javascript:openLink('x_DDNSServer')" class="content_input_link" name="x_DDNSServer_link">
	    <#LANHostConfig_x_DDNSServer_linkname#>
	</a>
	</div>
</td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#LANHostConfig_x_DDNSUserName_itemdesc#>', LEFT);" onMouseOut="return nd();"><#LANHostConfig_x_DDNSUserName_itemname#>
           </td><td class="content_input_td"><input type="text" maxlength="32" class="content_input_fd" size="32" name="ddns_username_x" value="<% nvram_get_x("LANHostConfig","ddns_username_x"); %>" onKeyPress="return is_string(this)" onBlur="validate_string(this)"></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#LANHostConfig_x_DDNSPassword_itemdesc#>', LEFT);" onMouseOut="return nd();"><#LANHostConfig_x_DDNSPassword_itemname#>
           </td><td class="content_input_td"><input type="password" maxlength="64" class="content_input_fd" size="32" name="ddns_passwd_x" value="<% nvram_get_x("LANHostConfig","ddns_passwd_x"); %>" onBlur="validate_string(this)"></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#LANHostConfig_x_DDNSHostNames_itemdesc#>', LEFT);" onMouseOut="return nd();"><#LANHostConfig_x_DDNSHostNames_itemname#>
           </td><td class="content_input_td"><input type="text" maxlength="63" class="content_input_fd" size="32" name="ddns_hostname_x" value="<% nvram_get_x("LANHostConfig","ddns_hostname_x"); %>" onKeyPress="return is_string(this)" onBlur="validate_string(this)">
           <input type="submit" maxlength="15" class="content_input_fd_ro" onClick="return onSubmitApply('hostname_check')" size="12" name="LANHostConfig_x_DDNSHostnameCheck_button" value="<#LANHostConfig_x_DDNSHostnameCheck_buttonname#>"><BR><div id="Hostname_Note"><#LANHostConfig_x_DDNSHostNames_Note#></div></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#LANHostConfig_x_DDNSWildcard_itemdesc#>', LEFT);" onMouseOut="return nd();"><#LANHostConfig_x_DDNSWildcard_itemname#>
           </td><td class="content_input_td"><input type="radio" value="1" name="ddns_wildcard_x" class="content_input_fd" onClick="return change_common_radio(this, 'LANHostConfig', 'ddns_wildcard_x', '1')" <% nvram_match_x("LANHostConfig","ddns_wildcard_x", "1", "checked"); %>>Yes</input><input type="radio" value="0" name="ddns_wildcard_x" class="content_input_fd" onClick="return change_common_radio(this, 'LANHostConfig', 'ddns_wildcard_x', '0')" <% nvram_match_x("LANHostConfig","ddns_wildcard_x", "0", "checked"); %>>No</input></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#LANHostConfig_x_DDNSStatus_itemdesc#>', LEFT);" onMouseOut="return nd();"><#LANHostConfig_x_DDNSStatus_itemname#>
           </td><td class="content_input_td"><input type="hidden" maxlength="15" class="content_input_fd_ro" size="12" name="" value="<% nvram_get_f("ddns.log","DDNSStatus"); %>"><input type="submit" maxlength="15" class="content_input_fd_ro" onClick="return onSubmitApply('ddnsclient')" size="12" name="LANHostConfig_x_DDNSStatus_button" value="<#LANHostConfig_x_DDNSStatus_buttonname#>"></td>
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
