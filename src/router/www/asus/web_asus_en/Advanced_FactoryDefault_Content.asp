<head>
<title>ZVMODELVZ Web Manager</title>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<link rel="stylesheet" type="text/css" href="style.css" media="screen">
<script language="JavaScript" type="text/javascript" src="overlib.js"></script>
<script language="JavaScript" type="text/javascript" src="general.js"></script>
</head>  
<div id="overDiv" style="position:absolute; visibility:hidden; z-index:1000;"></div>    
<form method="GET" name="form" action="apply.cgi">
<input type="hidden" name="action_mode" value="">
<!-- Table for the conntent page -->	    
<table width="660" border="0" cellpadding="1" cellspacing="0">     	      
    	
<tr>
<td height="96" width="666">		
<table width="666" border="1" cellpadding="0" cellspacing="0" bordercolor="#B0B0B0">
<tr class="content_header_tr">
<td class="content_header_td_title" colspan="2" height="25" width="666"><#t1SYS#> - <#t2Factory#></td><input type="hidden" name="current_page" value="Basic_Operation_Content.asp"><input type="hidden" name="next_page" value="Basic_SaveRestart.asp"><input type="hidden" name="sid_list" value="General;"><input type="hidden" name="group_id" value=""><input type="hidden" name="modified" value="0"><input type="hidden" name="preferred_lang" value="<% nvram_get_x("","preferred_lang"); %>">
</tr>
<tr bgcolor="#FFFFFF">
    <td class="content_desc_td">
    <#DEF_desc#>       
    <p align="center" style="margin-bottom: 16"> <input class=inputSubmit onMouseOut=buttonOut(this) onMouseOver="buttonOver(this)" onClick="onSubmitCtrlOnly(this, 'Restore')" type="submit" value="<#CTL_restore#>" name="action"></p>
  </td></tr>
</table>
</td>
</tr>

<tr>
<td height="29" width="618">
</td>
</tr>

</table>
</form>
