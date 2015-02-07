<head>
<title>WL500g.Deluxe Web Manager</title>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<meta http-equiv="Content-Type" content="text/html">
<link rel="stylesheet" type="text/css" href="style.css" media="screen">
<script language="JavaScript" type="text/javascript" src="overlib.js"></script>
<script language="JavaScript" type="text/javascript" src="general.js"></script>
<script language="JavaScript" type="text/javascript" src="quick.js"></script>
</head>
<body bgcolor="#FFFFFF">
<div id="overDiv" style="position:absolute; visibility:hidden; z-index:1000;"></div>
<form method="post" action="upload.cgi" enctype="multipart/form-data">
<input type="hidden" name="action_mode" value="">
<!-- Table for the conntent page -->
<table width="660" border="0" cellpadding="1" cellspacing="0">
<tr>
<td>
<table width="666" border="2" cellpadding="0" cellspacing="0" bordercolor="E0E0E0">
<tr class="content_header_tr">
<td class="content_header_td_title" colspan="2"><#t1SYS#> - <#t2Setting#></td>
<input type="hidden" name="current_page" value="Basic_Operation_Content.asp">
<input type="hidden" name="next_page" value="Basic_SaveRestart.asp">
<input type="hidden" name="preferred_lang" value="<% nvram_get_x("","preferred_lang"); %>">
</tr>
<tr>
<td class="content_desc_td" colspan="2" width="614">
<p style="margin-top: 7"><#BK_desc1#>
</td>
</tr>
<tr class="content_section_header_tr">
<td id="Mode" class="content_section_header_td" colspan="2" width="614"><#BK_desc2#></td>
</tr>
<tr>
<td class="content_desc_td" colspan="2" width="614">
<p style="margin-top: 7">
<#BK_desc31#> <a href="Settings.CFG">HERE</a>. <#BK_desc32#>
<p style="margin-buttom: 7">
</td>
</tr>
<tr class="content_section_header_tr">
<td id="Mode" class="content_section_header_td" colspan="2" width="614"><#BK_desc4#></td>
</tr>
<tr>
<td class="content_desc_td" colspan="2" width="614">
<p style="margin-top: 7">
<#BK_desc5#>
</td>
</tr>
<tr>
<td class="content_header_td" width="30"><#BK_item1#></td><td class="content_input_td">
<input class="inputSubmit" name="file" size="20" type="file" ></td>
</tr>
<tr>
<td class="content_header_td"></td><td class="content_input_td" width="595">
<p align="left"><input class="inputSubmit" type="submit" onClick="onSubmitCtrlOnly(this, 'Upload')" value="<#CTL_upload#>">
</p>
</td>
</tr>
</table>
</td>
</tr>
</table>
</form>
</body>
