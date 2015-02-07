<head>
<title>ZVMODELVZ Web Manager</title>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<meta http-equiv="Content-Type" content="text/html">
<link rel="stylesheet" type="text/css" href="style.css" media="screen">
<script language="JavaScript" type="text/javascript" src="overlib.js"></script>
<script language="JavaScript" type="text/javascript" src="general.js"></script>
<script language="JavaScript" type="text/javascript" src="quick.js"></script>
</head>  
<body bgcolor="#FFFFFF" onload="load_upload()">
<div id="overDiv" style="position:absolute; visibility:hidden; z-index:1000;"></div>    
<form method="post" action="upgrade.cgi" name="form" enctype="multipart/form-data">
<!-- Table for the conntent page -->	    
<table width="660" border="0" cellpadding="1" cellspacing="0">     	      
    	
<tr>
<td>
<table width="666" border="2" cellpadding="0" cellspacing="0" bordercolor="E0E0E0">
<tr class="content_header_tr">
<td class="content_header_td_title" colspan="2"><#t1SYS#> - <#t2Upgrade#></td>
<input type="hidden" name="current_page" value="Basic_Operation_Content.asp">
<input type="hidden" name="next_page" value="Basic_SaveRestart.asp">
<input type="hidden" name="action_mode" value="">
<input type="hidden" name="preferred_lang" value="<% nvram_get_x("","preferred_lang"); %>">
</tr>
<tr class="content_section_header_tr">
<td id="Mode" class="content_section_header_td" colspan="2" width="614"> <#FW_desc1#></td>
</tr>

<tr>
<td class="content_desc_td" colspan="2" width="614">
              	<ol>
              		<li>
              <p style="margin-top: 7">
                    <#FW_desc2#>
              </li>
              		<li>
              <p style="margin-top: 7">
                    <#FW_desc3#>               
              </li>
              		<li>
              <p style="margin-top: 7">
                    <#FW_desc4#>
              </li>
              		<li>
              <p style="margin-top: 7">
                    <#FW_desc5#>
              </li>
              <li>
              <p style="margin-top: 7">
                    <#FW_desc6#>
 	      </li>
              </ol>
</td>
</tr>
<tr>
		<td class="content_header_td" width="30"><#FW_item1#></td><td class="content_input_td" width="595"><input type="text" class="content_input_fd" value="<% nvram_get_f("general.log","productid"); %>" readonly="1"></td>
</tr>
<tr>
		<td class="content_header_td" width="30"><#FW_item2#></td><td class="content_input_td" width="595"><input type="text" class="content_input_fd" value="<% nvram_get_f("general.log","firmver"); %>" readonly="1"></td>
</tr>
<tr>
<td class="content_header_td" width="30"><#FW_item5#></td><td class="content_input_td">  
                  <input class="content_input_fd" style="cursor:pointer;" name="file" size="20" type="file" ></td>
</tr>
<tr>
<td class="content_header_td"></td><td class="content_input_td" width="595">
  <p align="left"><input class="inputSubmit" style="cursor:pointer;" name="button" onClick="onSubmitCtrlOnly(this, 'Upload1')" type="button" value="<#CTL_upload#>"> 
  </p>
</td>
</tr>
<tr>
<td class="content_desc_td" colspan="2" width="614">
	             <b><#FW_note#></b>
                     <ol>
                     	<li><#FW_n1#></li>
                     	<li><#FW_n2#></li>
                     </ol>
</td>
</tr>
</table>
</td>
</tr>
</table>
</form>
</body>
