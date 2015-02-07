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
    	
<input type="hidden" name="current_page" value="Advanced_StorageUserList_Content.asp">
<input type="hidden" name="next_page" value="SaveRestart.asp">
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
<table width="666" border="2" cellpadding="0" cellspacing="0" bordercolor="E0E0E0">

<tr class="content_list_header_tr">
<td class="content_list_header_td" width="60%" id="UserList"><#t1USB#> - <#t2UL#>
	<input type="hidden" name="acc_num_0" value="<% nvram_get_x("Storage", "acc_num"); %>" readonly="1">
</td>
<td width="10%">
<div align="center">
	<input class="inputSubmit" type="submit" onMouseOut="buttonOut(this)" onMouseOver="buttonOver(this)" onClick="return markGroup(this, 'Storage_UserList', 16, ' Add ');" name="Storage_UserList" value="<#CTL_add#>" size="12">
</div>
</td>
<td width="10%">
<div align="center">
	<input class="inputSubmit" type="submit" onMouseOut="buttonOut(this)" onMouseOver="buttonOver(this)" onClick="return markGroup(this, 'Storage_UserList', 16, ' Del ');" name="Storage_UserList" value="<#CTL_del#>" size="12">
</div>
</td>
<td width="5%">
</td>
</tr>

<!-- Start of table B -->
<table class="content_list_table" width="640" border="0" cellspacing="0" cellpadding="0">

<tr>
<td colspan="3">
<div align="center">
<!-- Start of table C -->
<table class="content_list_value_table" border="1" cellspacing="0" cellpadding="0" width="665">

<tr>
<td class="content_list_field_header_td" colspan="" width="331">
<#UserList_x_username_itemname#></td>
<td class="content_list_field_header_td" width="332">
<#UserList_x_password_itemname#></td>
</tr>

<tr>
<td class="content_list_input_td" colspan="" width="331">
	<input type="text" maxlength="28" size="28" name="acc_username_0" class="content_input_list_fd" onKeyPress="return is_string(this)">
</td>
<td class="content_list_input_td" colspan="" width="332">
	<input type="text" maxlength="24" size="24" name="acc_password_0" class="content_input_list_fd" onKeyPress="return is_string(this)">
</td>
</tr>

<tr>
<td colspan="2" width="661">
	<select size="8" name="Storage_UserList_s" multiple="true" style="font-family: 'fixedsys'; font-size: '8pt'">
	<% nvram_get_table_x("Storage","Storage_UserList"); %>
	</select>
</td>
</tr>

</table>
<!-- End of table C -->
</div>
</td>
</tr>

<tr>
<td colspan="3" height="3"></td>
</tr>

</table>
<!-- End of table B -->

</table>
<!-- End of table A -->
</td>
</tr>

<tr>
<td>		
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
