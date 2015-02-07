<head>
<title>WL500g.Deluxe Web Manager</title>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<link rel="stylesheet" type="text/css" href="style.css" media="screen">
<script language="JavaScript" type="text/javascript" src="overlib.js"></script>
<script language="JavaScript" type="text/javascript" src="general.js"></script>
</head>
<body bgcolor="#FFFFFF"  onLoad="load_body()">
<div id="overDiv" style="position:absolute; visibility:hidden; z-index:1000;"></div>
<form method="GET" name="form" action="apply.cgi">
<input type="hidden" name="action_mode" value="">
<input type="hidden" name="preferred_lang" value="<% nvram_get_x("","preferred_lang"); %>">
<!-- Table for the conntent page -->
<table width="660" border="0" cellpadding="1" cellspacing="0">
<tr>
<td>
<table width="666" border="2" cellpadding="0" cellspacing="0" bordercolor="E0E0E0">
<tr class="content_header_tr">
<td class="content_header_td_title" colspan="2"><#t1SYS#> - <#t2Pass#></td>
<input type="hidden" name="current_page" value="Advanced_Password_Content.asp">
<input type="hidden" name="next_page" value="SaveRestart.asp">
<input type="hidden" name="next_host" value="">
<input type="hidden" name="sid_list" value="General;">
<input type="hidden" name="group_id" value="">
<input type="hidden" name="modified" value="0">
</tr>
<tr>
<td class="content_desc_td" colspan="2" width="614">
<p style="margin-top: 7">
<#PASS_desc#>
<p style="margin-buttom: 7">
</td>
</tr>
<tr class="content_section_header_tr">
<td id="Mode" class="content_section_header_td" colspan="2" width="614"><#PASS_changepasswd#></td>
</tr>
<tr>
<td class="content_header_td" width="34"><#PASS_new#></td><td class="content_input_td" width="310"><input type="password" maxlength="16" size="16" name="http_passwd"></td>
</tr>
<tr>
<td class="content_header_td" width="34"><#PASS_retype#></td><td class="content_input_td" width="310"><input type="password" maxlength="16" size="16" name="v_password"></td>
</tr>
<tr>
<td class="content_input_td_less" height="60" colspan="2">
<p align="right">
<input class="inputSubmit" onMouseOut="buttonOut(this)" onMouseOver="buttonOver(this)" type="submit" value="<#CTL_onlysave#>" name="action" onClick="return checkPass(this, document.form.http_passwd, document.form.v_password);">&nbsp;<input class="inputSubmit" onMouseOut="buttonOut(this)" onMouseOver="buttonOver(this)" type="reset" value="<#CTL_clear#>" name="reset">
</p>
</td>
</tr>
</table>
</td>
</tr>
</table>
</form>
</body>
