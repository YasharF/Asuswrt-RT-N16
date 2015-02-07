<html>
<head>
<title>ASUS Wireless Router Web Manager</title>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link href="other.css"  rel="stylesheet" type="text/css">
<style type="text/css">
body {
	background-image: url(images/bg.gif);
	margin:50px auto;
}
</style>
<script language="javascript">
function closeWindow(){
	window.focus();
	window.open('','_parent',''); //for IE 7
	window.opener=null;
	window.close();
}
</script>
</head>

<body>
<form name="formname" method="POST">
<table width="500" border="0" align="center" cellpadding="10" cellspacing="0" class="erTable">
<thead>
  <tr>
    <td height="52" background="images/er_top.gif"></td>
  </tr>
</thead> 
  <tr>
    <th align="left" valign="top" background="images/er_bg.gif">
		<div class="drword">
			<#logoutmessage#>
			<div style="float:right;">
				<a href="/"><#Not_authpage_re_login#></a>
			</div>
			<br/><br/>
		</div>
		<div class="drImg"><img src="images/DrsurfImg.gif"></div>		
		<div style="height:70px; "></div>
	  	</th>
  </tr>
  <tr>
    <td height="22" background="images/er_bottom.gif"><span></span></td>
  </tr>			
</table>
</form>
</body>
</html>
