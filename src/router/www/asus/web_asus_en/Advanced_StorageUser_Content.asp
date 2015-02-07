<head>
<title>ZVMODELVZ Web Manager</title>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<link rel="stylesheet" type="text/css" href="style.css" media="screen"></link>
<script language="JavaScript" type="text/javascript" src="overlib.js"></script>
<script language="JavaScript" type="text/javascript" src="general.js"></script>
<script language="JavaScript">
	function Acc_Access_Right() {
		selindex = document.form.Storage_UserRight_List_s.selectedIndex;
		seltext = document.form.Storage_UserRight_List_s.options[selindex].text;
		acc = seltext.substring(0, 28);
		for (i=0; i<document.form.sh_acc_user_0.options.length; i++) {
			len = document.form.sh_acc_user_0.options[i].text.length;
			if (document.form.sh_acc_user_0.options[i].text == acc.substring(0, len)) {
				document.form.sh_acc_user_0.selectedIndex = i;
				if (seltext.indexOf("Read/Write") == -1)
					document.form.sh_acc_right_0[0].checked = true;
				else
					document.form.sh_acc_right_0[1].checked = true;
				break;
			}
		}
	}
	function show_hidden() {
		acc_array = document.form.sh_acc_user_x_0.value.split(";");
		acc_array.pop();
		for (i = 0; i < acc_array.length; i++) {
			if (acc_array[i] == document.form.sh_acc_user_0.value) {
				alert("<#JS_adduser#>");
				return false;
			}
		}
		return markGroup(this, 'Storage_UserRight_List', 12, ' Add ');
	}
        function blanktest(o) {
                if (o.value=="")
                {
                        o.value=decodeURIComponent(document.form.shared_name.value);
                        alert("<#JS_Shareblanktest#>");
                }
        }
        function xfr()
        {
        	document.form.sh_name_x.value = decodeURIComponent(document.form.shared_name.value);
        	document.form.sh_nameb_x.value = encodeURIComponent(document.form.sh_name_x.value);
        	if (document.form.comment_str.value!="")
        	{
        	document.form.sh_comment_x.value = decodeURIComponent(document.form.comment_str.value);
        	document.form.sh_commentb_x.value = encodeURIComponent(document.form.sh_comment_x.value);
        	}
        	else
        	{
        		document.form.sh_comment_x.value="";
        		document.form.sh_commentb_x.value="";
        	}
	}
	function copytob()
	{
		document.form.sh_nameb_x.value = encodeURIComponent(document.form.sh_name_x.value);
	}
	function copytob2()
	{
		document.form.sh_commentb_x.value = encodeURIComponent(document.form.sh_comment_x.value);
	}
	
function validate_string(o) 
{

	if (o.charAt(0)=='.')	
	{
		alert("<#JS_validstr1#> '\"'");
		o="";
		return false;
	}
	
	if (o.charAt(o.length-1)=='.')	
	{
		alert("<#JS_validstr3#> '\"'");
		o="";
		return false;
	}
	
	if (o.charAt(0)=='"')	
	{
		alert("<#JS_validstr1#> '\"'");
		o="";
		return false;
	}
	else
	{  	
		inv="";
		for(i=0; i<o.length; i++)
		{
			if ( o.charAt(i)<' '|| ((o.charAt(i)>'~') && (o.charAt(i)<256)) )
			{  			  			
				inv = inv + o.charAt(i);  			
			}
		}
		if (inv!="")
		{
			alert("<#JS_validstr2#> " + inv +"!");
			o="";
			return false;
		}
	}  
	return true;  
}

function validate_sharechar(ch)
{
/*		
	if (ch>=48&&ch<=57) return true;
	if (ch>=97&&ch<=122) return true;
	if (ch>=65&&ch<=90) return true;	
	if (ch==45) return true; // '-'
	if (ch==95) return true; // '_'
	if (ch==32) return true; // ' '	
	return false;
*/
	if (ch==33) return false; // '!'
	if (ch==34) return false; // '"'
	if (ch>=37&&ch<=42) return false; // % & ' ( ) *
	if (ch==47) return false; // '/'
	if (ch==58) return false; // ':'
	if (ch==59) return false; // ';'
	if (ch==60) return false; // '<'
	if (ch==62) return false; // '>'
	if (ch==63) return false; // '?'
	if (ch==91) return false; // '['
	if (ch==92) return false; // '\'
	if (ch==93) return false; // ']'
	if (ch==96) return false; // '`'
	if (ch==123) return false; // '{'
	if (ch==124) return false; // '|'
	if (ch==125) return false; // '}'
	return true;
}

function validate_share(o)
{

	if (!validate_string(o.value)) 
	{
		o.value=decodeURIComponent(document.form.shared_name.value);
	}

	for(i=0;i<o.value.length;i++)
	{
		c = o.value.charCodeAt(i);
   	
		if (!validate_sharechar(c)) 
		{
			alert("<#JS_validstr2#> '" + o.value.charAt(i) +"' !");
			o.value=decodeURIComponent(document.form.shared_name.value);
		}
	}
}
	
</script>
</head>  
<div id="overDiv" style="position:absolute; visibility:hidden; z-index:1000;"></div>    
<body onLoad="load_body();xfr()" onunLoad="return unload_body();">
<form method="GET" name="form" action="apply.cgi">
		<input type=hidden name=sh_acc_user_x_0 value="<% nvram_get_x("Storage", "sh_acc_user_x"); %>" readonly="1">
		<input type=hidden name=sh_acc_onlist_num_x_0 value="<% nvram_get_x("Storage", "sh_acc_onlist_num_x"); %>" readonly="1">
<!-- Table for the conntent page -->	    
<table width="666" border="0" cellpadding="0" cellspacing="0">     	      
    	
<input type="hidden" name="current_page" value="Advanced_StorageUser_Content.asp">
<input type="hidden" name="next_page" value="Advanced_StorageRight_Content.asp">
<input type="hidden" name="next_host" value="">
<input type="hidden" name="sid_list" value="Storage;">
<input type="hidden" name="group_id" value="Storage_UserRight_List">
<input type="hidden" name="modified" value="0">
<input type="hidden" name="action_mode" value=" Add ">
<input type="hidden" name="first_time" value="">
<input type="hidden" name="action_script" value="">
<input type="hidden" name="preferred_lang" value="<% nvram_get_x("","preferred_lang"); %>">
<input type="hidden" name="shared_name" value="<% nvram_get_x("Storage","sh_nameb_x"); %>">
<input type="hidden" name="comment_str" value="<% nvram_get_x("Storage","sh_commentb_x"); %>">

<tr>
<td>
<table width="666" border="1" cellpadding="0" cellspacing="0" bordercolor="E0E0E0">
<tr class="content_header_tr">
<td class="content_header_td_title" colspan="2"><#t1USB#> - <#t2AR#></td>
</tr>

<tr>
<td class="content_header_td" width="63%">
<#USB_AccessRight_SharedPathList_itemname#></td>
<td class="content_input_td">
	<input type="text" maxlength="16" class="content_input_fd" size="16" name="sh_path_x" value="<% select_list("Storage_x_SharedPath"); %>" onKeyPress="return is_string(this)" onBlur="validate_string(this)" disabled>
</td>
</tr>

<tr>
<td class="content_header_td" width="63%"><#USB_AccessRight_SharedName_itemname#></td>
<td class="content_input_td"><input type="text" maxlength="16" class="content_input_fd" size="16" name="sh_name_x" value="" onKeyPress="return is_string(this)" onBlur="blanktest(this);validate_share(this);copytob()">
<input type="hidden" name="sh_nameb_x" value="">
</td>
</tr>

<tr>
<td class="content_header_td" width="63%"><#USB_AccessRight_SharedComment_itemname#></td>
<td class="content_input_td"><input type="text" maxlength="16" class="content_input_fd" size="16" name="sh_comment_x" value="" onKeyPress="return is_string(this)" onBlur="copytob2()">
<input type="hidden" name="sh_commentb_x" value="">
</td>
</tr>

</table>
</td>
</tr>

<tr>
<td height="3"></td>
</tr>

<tr>
<td>
<table width="666" border="1" cellpadding="0" cellspacing="0" bordercolor="E0E0E0">
<tr class="content_list_header_tr">
	<td class="content_list_header_td" width="65%" id="UserRight_List">
	<#USB_AccessRight_UserList_groupitemdesc#></td>
	<td width="10%">
	<div align="center">
		<input class="inputSubmit" type="submit" onMouseOut="buttonOut(this)" onMouseOver="buttonOver(this)" onClick="return show_hidden();" name="Storage_UserRight_List" value="<#CTL_add#>" size="12">
	</div>
	</td>
	<td width="10%">
	<div align="center">
		<input class="inputSubmit" type="submit" onMouseOut="buttonOut(this)" onMouseOver="buttonOver(this)" onClick="return markGroup(this, 'Storage_UserRight_List', 12, ' Del ');" name="Storage_UserRight_List" value="<#CTL_del#>" size="12">
	</div>
	</td>
	<td width="10%">
	<div align="center">
		<input class="inputSubmit" type="submit" onMouseOut="buttonOut(this)" onMouseOver="buttonOver(this)" onClick="return markGroup(this, 'Storage_UserRight_List', 12, ' Modify ');" name="Storage_UserRight_List" value="<#CTL_modify#>" size="12">
	</div>
	</td>
	<td width="5%">
	</td>
</tr>

<table class="content_list_value_table" border="1" cellspacing="0" cellpadding="0" width="665">

<tr>
<td class="content_header_td" width="63%" onMouseOver="return overlib('<#JS_accessright#>', LEFT);" onMouseOut="return nd();">
<#USB_AccessRight_UserList_UserName_itemname#></td>
<td class="content_header_td"><#USB_AccessRight_UserList_AccessRight_itemname#></td>
</tr>

<tr>
<td class="content_header_td" width="63%">
	<select name="sh_acc_user_0" class="content_input_fd" onChange="return change_common(this, 'Storage', 'sh_acc_user_0')">
	<% select_list("Storage_AllUser_List"); %>
	</select>
</td>
<td class="content_header_td">
<input type="radio" value="1" name="sh_acc_right_0" class="content_input_fd" onClick="return change_common_radio(this, 'Storage', 'sh_acc_right', '1')" checked>Read 
Only
<input type="radio" value="0" name="sh_acc_right_0" class="content_input_fd" onClick="return change_common_radio(this, 'Storage', 'sh_acc_right', '0')" >Read/Write
</td>
</tr>

<tr>
<td colspan="4">
	<select size="8" name="Storage_UserRight_List_s" multiple="true" style="font-family: 'fixedsys'" ondblclick="Acc_Access_Right()">
	<% nvram_get_table_x("Storage","Storage_UserRight_List"); %>
	</select>
</td>
</tr>

</table>


<tr>
<td height="3"></td>
</tr>

<tr>
<td>		
<table width="666" border="1" cellpadding="0" cellspacing="0" bordercolor="B0B0B0">

<tr bgcolor="#CCCCCC">
<td colspan="3"><font face="arial" size="2"><b>&nbsp;</b></font>
</td>
</tr>

<tr bgcolor="#FFFFFF">  
<td id ="Confirm" height="25" width="34%">     
</td>  
<td height="25" width="33%">     
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
<td colspan="2" width="616" height="25" bgcolor="#FFBB00">
</td>
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
