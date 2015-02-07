<head>
<title>ZVMODELVZ Web Manager</title>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<link rel="stylesheet" type="text/css" href="style.css" media="screen"></link>
<script language="JavaScript" type="text/javascript" src="overlib.js"></script>
<script language="JavaScript" type="text/javascript" src="general.js"></script>
<script language="JavaScript">
function swapoff_fail_check()
{
        ret_st="<%nvram_get_x("","preferred_lang");%>";
	if (document.form.swapoff_failed.value=="1")
	{
        	if(ret_st=='EN')
			alert("Some problems happen when ejecting the USB storage device.\nWe recommend you to reboot the router.");
        	else if(ret_st =='TW')
			alert("移除USB儲存裝置時發生了問題。\n建議您重新啟動系統！");
        	else if(ret_st =='CN')
			alert("移除USB磁盘時发生了问题。\n建议您重新启动系统！");
		else
			alert("Some problems happen when ejecting the USB storage device.\nWe recommend you to reboot the router.");
		document.form.action2.click();
	}
}
</script>
</head>  
<div id="overDiv" style="position:absolute; visibility:hidden; z-index:1000;"></div>    
<body onLoad="load_body();swapoff_fail_check()" onunLoad="return unload_body();">
<form method="GET" name="form" action="apply.cgi">
<!-- Table for the conntent page -->	    
<table width="666" border="0" cellpadding="0" cellspacing="0">     	      
    	
<input type="hidden" name="current_page" value="Main_UStatus_Content.asp">
<input type="hidden" name="next_page" value="Main_WStatus_Content.asp">
<input type="hidden" name="next_host" value="">
<input type="hidden" name="sid_list" value="default value">
<input type="hidden" name="group_id" value="">
<input type="hidden" name="modified" value="0">
<input type="hidden" name="action_mode" value="">
<input type="hidden" name="first_time" value="">
<input type="hidden" name="action_script" value="">
<input type="hidden" name="preferred_lang" value="<% nvram_get_x("","preferred_lang"); %>">
<input type="hidden" name="swapoff_failed" value="<% nvram_get_x("","swapoff_failed"); %>">

<tr>
<td>
<table width="666" border="1" cellpadding="0" cellspacing="0" bordercolor="E0E0E0">
<tr class="content_header_tr">
<td class="content_header_td_title" colspan="2"><#t1Status#> - <#t2UTStatus#></td>
</tr>
</table>
</td>
</tr>

<tr>
<td>
<table width="666" border="1" cellpadding="0" cellspacing="0" bordercolor="E0E0E0">

<tr class="content_header_tr">
<td colspan="2">
	<textarea class="content_log_td" cols="63" rows="12" wrap="off" readonly="1">
	<% nvram_dump("usstatus",""); %>
	</textarea>
</td>
</tr>

<tr>
<td class="content_header_td_less"><#USBStatus_x_Action_itemname#></td>
<td class="content_input_td">
	<input type="hidden" name="usb_disc0_index" value="<% nvram_get_x("", "usb_disc0_index"); %>" readonly="1">
	<input type="hidden" name="usb_disc1_index" value="<% nvram_get_x("", "usb_disc1_index"); %>" readonly="1">
	<input type="hidden" name="usb_disc0_dev" value="<% nvram_get_x("", "usb_disc0_dev"); %>" readonly="1">
	<input type="hidden" name="usb_disc1_dev" value="<% nvram_get_x("", "usb_disc1_dev"); %>" readonly="1">
	<input type="hidden" name="usb_disc0_port" value="<% nvram_get_x("", "usb_disc0_port"); %>" readonly="1">
	<input type="hidden" name="usb_disc1_port" value="<% nvram_get_x("", "usb_disc1_port"); %>" readonly="1">
<!--	<input type="submit" maxlength="15" class="content_input_fd_ro" onClick="return onSubmitApply('usbfs_check')" size="12" name="USBStatus_x_Action_button1" value="<#USBStatus_x_Action_button1name#>">
-->
	<input type="submit" maxlength="15" class="content_input_fd_ro" onClick="return onSubmitApply('usbfs_eject2')" size="12" name="USBStatus_x_Action_button3" value="<#USBStatus_x_Action_button3name#>">
	<input type="submit" maxlength="15" class="content_input_fd_ro" onClick="return onSubmitApply('usbfs_eject')" size="12" name="USBStatus_x_Action_button2" value="<#USBStatus_x_Action_button2name#>">
</td>
</tr>

</table>
</td>
</tr>

<tr>
<td>
<table>
</table>
</td>
</tr>

<td>
<table width="666" border="1" cellpadding="0" cellspacing="0" bordercolor="B0B0B0">
<tr bgcolor="#CCCCCC"><td colspan="3"><font face="arial" size="2"><b>&nbsp;</b></font></td></tr>
<tr bgcolor="#FFFFFF">  
<td height="25" width="34%">  
</td>
<td height="25" width="33%">  
</td>
<td height="25" width="33%">  
<div align="center">
	<font face="Arial">
	<input class=inputSubmit onMouseOut=buttonOut(this) onMouseOver="buttonOver(this)" onClick="onSubmitCtrl(this, ' Refresh ')" type="submit" value="<#CTL_refresh#>" name="action">
	</font>
        <div id="hidden" style="display:none">
	<input class=inputSubmit onMouseOut=buttonOut(this) onMouseOver="buttonOver(this)" onClick="onSubmitCtrl(this, ' Restart ')" type="submit" value=" Finish " name="action2">
	</div>
</div> 
</td>
</tr>
</table>
</td>
</tr>

</table>
</form>
</body>
