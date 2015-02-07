<head>
<title>ZVMODELVZ Web Manager</title>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<link rel="stylesheet" type="text/css" href="style.css" media="screen"></link>
<script language="JavaScript" type="text/javascript" src="overlib.js"></script>
<script language="JavaScript" type="text/javascript" src="general.js"></script>
<script language="JavaScript">
function validate_ports()
{
	if (eval(document.form.apps_dl_share_port_from.value)+50!=eval(document.form.apps_dl_share_port_to.value))
	{
		i=eval(document.form.apps_dl_share_port_from.value);
		j=eval(document.form.apps_dl_share_port_from.value)+50;
		while (j>60000)
		{
			i-=100;
			j-=100;
		}
		document.form.apps_dl_share_port_from.value=i;
		document.form.apps_dl_share_port_to.value=j;
	}
}
</script>
</head>  
<div id="overDiv" style="position:absolute; visibility:hidden; z-index:1000;"></div>    
<body onLoad="load_body()" onunLoad="return unload_body();">
<form method="GET" name="form" action="apply.cgi">
	<input type="hidden" name="sh_num_0" value="<% nvram_get_x("Storage", "sh_num"); %>" readonly="1">
<!-- Table for the conntent page -->	    
<table width="666" border="0" cellpadding="0" cellspacing="0">     	      
    	
<input type="hidden" name="current_page" value="Advanced_StorageBasicConfig_Content.asp">
<input type="hidden" name="next_page" value="Advanced_StorageRight_Content.asp">
<input type="hidden" name="next_host" value="">
<input type="hidden" name="sid_list" value="Storage;">
<input type="hidden" name="group_id" value="">
<input type="hidden" name="modified" value="0">
<input type="hidden" name="action_mode" value="">
<input type="hidden" name="first_time" value="">
<input type="hidden" name="action_script" value="">
<input type="hidden" name="preferred_lang" value="<% nvram_get_x("","preferred_lang"); %>">

<tr>
<td>
<!-- Start of table A -->
<table width="666" border="1" cellpadding="0" cellspacing="0" bordercolor="E0E0E0">

<tr class="content_header_tr">
<td class="content_header_td_title" colspan="2"><#t1USB#> - <#t2TBC#></td>
</tr>

<tr>
<td class="content_desc_td" colspan="2"><#USB_BasicConfig_sectiondesc#></td>
</tr>

<tr>
<td class="content_header_td" onMouseOver="return overlib('<#JS_basiconfig1#>', LEFT);" onMouseOut="return nd();">
<#BasicConfig_EnableDownloadMachine_itemname#></td>
<td class="content_input_td">
	<input type="radio" value="1" name="apps_dl" <% nvram_match_x("Storage", "apps_dl", "1", "checked"); %>>Yes</input>
	<input type="radio" value="0" name="apps_dl" <% nvram_match_x("Storage", "apps_dl", "0", "checked"); %>>No</input>
</td>
</tr>

<tr>
<td class="content_header_td" onMouseOver="return overlib('<#JS_basiconfig3#>', LEFT);" onMouseOut="return nd();">
<#BasicConfig_EnableDownloadShare_itemname#></td>
<td class="content_input_td">
	<input type="radio" value="1" name="apps_dl_share" <% nvram_match_x("Storage", "apps_dl_share", "1", "checked"); %>>Yes</input>
	<input type="radio" value="0" name="apps_dl_share" <% nvram_match_x("Storage", "apps_dl_share", "0", "checked"); %>>No</input>
</td>
</tr>

<tr>
<td class="content_header_td" onMouseOver="return overlib('<#JS_basiconfig0#>', LEFT);" onMouseOut="return nd();">
<#BasicConfig_DefaultPortRangeforDownloadShare_itemname#></td>
<td class="content_input_td">
	&nbsp;From <input type="text" maxlength="5" size="5" name="apps_dl_share_port_from" value="<% nvram_get_x("Storage", "apps_dl_share_port_from"); %>" onBlur="validate_range(this, 1025, 60000);validate_ports()" onKeyPress="return is_number(this)">
	&nbsp;To <input type="text" maxlength="5" size="5" name="apps_dl_share_port_to" value="<% nvram_get_x("Storage", "apps_dl_share_port_to"); %>" onBlur="validate_range(this, 1025, 60000);validate_ports()" onKeyPress="return is_number(this)">
</td>
</tr>

<tr>
<td class="content_header_td" onMouseOver="return overlib('<#JS_basiconfig4#>', LEFT);" onMouseOut="return nd();">
<#BasicConfig_DefaultSeedTimeforBT_itemname#></td>
<td class="content_input_td">
	<input type="text" maxlength="5" size="5" name="apps_dl_seed" value="<% nvram_get_x("Storage", "apps_dl_seed"); %>" onBlur="validate_range(this, 24, 168)" onKeyPress="return is_number(this)">
</td>
</tr>

<tr>
<td class="content_header_td" onMouseOver="return overlib('<#JS_basiconfig8#>', LEFT);" onMouseOut="return nd();">
<#BasicConfig_EnableMediaServer_itemname#></td>
<td class="content_input_td">
	<input type="radio" value="1" name="apps_dms" <% nvram_match_x("Storage", "apps_dms", "1", "checked"); %>>Yes</input>
	<input type="radio" value="0" name="apps_dms" <% nvram_match_x("Storage", "apps_dms", "0", "checked"); %>>No</input>
</td>
</tr>

<!--
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#JS_basiconfig9#>', LEFT);" onMouseOut="return nd();">
<#BasicConfig_MediaServer_USBPort_itemname#></td>
<td class="content_input_td">
	<input type="radio" value="1" name="apps_dms_usb_port" <% nvram_match_x("Storage", "apps_dms_usb_port", "1", "checked"); %>>Lower Port </input>
	<input type="radio" value="2" name="apps_dms_usb_port" <% nvram_match_x("Storage", "apps_dms_usb_port", "2", "checked"); %>>Upper Port </input>
</td>
</tr>
-->

</table>
<!-- End of table A -->
<tr>
<td>

<tr>
<td height="3"></td>
</tr>


<tr>
<td>
<!-- Start of table E -->
<table width="666" border="1" cellpadding="0" cellspacing="0" bordercolor="B0B0B0">
<tr bgcolor="#CCCCCC"><td colspan="3"><font face="arial" size="2"><b>&nbsp;</b></font></td></tr>
<tr bgcolor="#FFFFFF">  
<td id ="Confirm" height="25" width="34%">  
<div align="center">
	<font face="Arial">
	<input class=inputSubmit onMouseOut=buttonOut(this) onMouseOver="buttonOver(this)" onClick="onSubmitCtrl(this, ' Restore ')" type="submit" value=" <#CTL_restore#> " name="action">
	</font>
</div> 
</td>  
<td height="25" width="33%">  
<div align="center">
	<font face="Arial">
	<input class=inputSubmit onMouseOut=buttonOut(this) onMouseOver="buttonOver(this)" onClick="onSubmitCtrl(this, ' Finish ')" type="submit" value=" <#CTL_finish#> " name="action">
	</font>
</div> 
</td>
<td height="25" width="33%">  
<div align="center">
	<font face="Arial">
	<input class=inputSubmit onMouseOut=buttonOut(this) onMouseOver="buttonOver(this)" onClick="onSubmitCtrl(this, ' Apply ')" type="submit" value=" <#CTL_apply#> " name="action">
	</font>
</div> 
</td>    
</tr>
</table>
<!-- End of table E -->
</td>
</tr>

<tr>
<td>
<!-- Start of table F -->
<table width="666" border="1" cellpadding="0" cellspacing="0" bordercolor="B0B0B0">

<tr>
<td colspan="2" width="616" height="25" bgcolor="#FFBB00"></td> 
</tr>                   

<tr bgcolor="#FFFFFF">
<td class="content_header_td_15" align="left"><#CTL_restore#>:</td>
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
<!-- End of table F -->
</td>
</tr>

</table>
</form>
</body>
