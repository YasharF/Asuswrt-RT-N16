<html>
<head>
<title>IGD Device Description</title>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<meta http-equiv="Expires" content="Fri, Jun 12 1981 00:00:00 GMT">
<meta http-equiv="Cache-control" content="no-cache,must-revalidate">
<meta http-equiv="Pragma" content="no-cache">
<input type="hidden" name="preferred_lang" value="<% nvram_get_x("","preferred_lang"); %>">
<link rel="stylesheet" type="text/css" href="style.css" media="screen">
<script language="JavaScript" type="text/javascript" src="overlib.js"></script>
<script language="JavaScript" type="text/javascript">
function change_lang(f)
{if ((f.first_time.value != '1') || (f.preferred_lang_menu.value != 'fake' && f.preferred_lang.value != f.preferred_lang_menu.value))
{f.submit();
return true;
}
}
</script>
</head>
<div id="overDiv" style="position:absolute; visibility:hidden; z-index:1000;"></div>
<body>
<form method="GET" name="form" action="change_lang.cgi">
<input type="hidden" name="current_page" value="Title.asp">
<input type="hidden" name="next_host" value="">
<input type="hidden" name="wan_route_x" value="<% nvram_get_x("IPConnection","wan_route_x"); %>">
<input type="hidden" name="wan_nat_x" value="<% nvram_get_x("IPConnection","wan_nat_x"); %>">
<input type="hidden" name="productid" value="<% nvram_get_f("general.log","productid"); %>">
<input type="hidden" name="support_cdma" value="<% nvram_get_x("IPConnection","support_cdma"); %>">
<input type="hidden" name="first_time" value="<% nvram_get_x("General","x_Setting"); %>">
<input type="hidden" name="preferred_lang" value="<% nvram_get_x("","preferred_lang"); %>">
<table background="./graph/iBox_title_all.jpg" style="width: 663; height: 81;" border="0" cellspacing="0" cellpadding="0">
<tr>
<td style="text-align: right; vertical-align: bottom;">
<select name="preferred_lang_menu" value="fake" style="background-color: rgb(255, 255, 255);" onChange="change_lang(this.form);">
<option class="content_input_fd" value="fake" selected > <#LANG_select#> </option>
<option class="content_input_fd" value="EN" > <#LANG_EN#> </option>
<option class="content_input_fd" value="TW" > <#LANG_TW#> </option>
<option class="content_input_fd" value="CN" > <#LANG_CN#> </option>
<option class="content_input_fd" value="KR" > <#LANG_KR#> </option>
</select>
</table>
</form>
</body>
</html>
