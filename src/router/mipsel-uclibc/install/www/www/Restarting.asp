<html>
<head>
<title>WL500g.Deluxe Web Manager</title>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<meta http-equiv="Content-Type" content="text/html">
<meta http-equiv="Pragma" content="no-cache">
<script language="javascript">
function redirect() {
setTimeout("redirect1()", 25000)
}
function redirect1() {
top.location="index.asp"
}
function restart_translate_lang(o,v)
{if(v=='TW')
{if(o=='SAVE_restart_title')
translated_string ="如果您選擇連接至一台ADSL數據機，您便不能將此兩個欄位保留空白。";
else if(o=='SAVE_restart_desc')
translated_string="您應該為系統指定一個靜態IP位址!";
}
else
{if(o=='SAVE_restart_title')
translated_string ="System Restarting!";
else if(o=='SAVE_restart_desc')
translated_string="System is restarting! Please wait until home page of WL500gP setting is shown up again.";
else if (o == 'SAVE_note')
translate_string="Note: It takes about 20 seconds.";
}
return translated_string;
}
</script>
<style type="text/css">
<!--
.wounderline {  font-family: "Arial", "Helvetica", "sans-serif"; text-decoration: none; color: #FFFFFF; font-size: 9pt}
-->
</style>
</head>
<body bgcolor="#FFFFFF" onLoad="redirect()">
<table width="666" border="0" cellpadding="2" cellspacing="0" vspace="30" height="1">
<tr bgcolor="#CCCCCC">
<td width="666" height="125">
<table width="660" height="125" cellpadding="2" cellspacing="0" border="0">
<tbody>
<tr>
<td bgcolor="#FF0000" height="50" width="652"><b><font size="5" face="Arial" color="#FFFFFF"><#SAVE_restart_title#></font></b></td>
</tr>
<tr>
<td width="652" height="25" bgcolor="#FFFFFF" colspan="2"><#SAVE_restart_desc#></td>
</tr>
<tr>
<td width="652" height="25" bgcolor="#FFFFFF" colspan="2"></td>
</tr>
<tr>
<td width="652" height="25" bgcolor="#FFFFFF" colspan="2"><font face="Arial">
<font color="#FF0000"><b><#SAVE_note#></b></font></font></td>
</tr>
</tbody>
</table>
</td>
</tr>
</table>
</body>
</html>
