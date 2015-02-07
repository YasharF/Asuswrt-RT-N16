<head>
<title>ZVMODELVZ Web Manager</title>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<link rel="stylesheet" type="text/css" href="style.css" media="screen"></link>
<input type="hidden" name="preferred_lang" value="<% nvram_get_x("","preferred_lang"); %>">
<script language="JavaScript" type="text/javascript" src="overlib.js"></script>
<script language="JavaScript" type="text/javascript" src="general.js"></script>
<script language=JavaScript>

var MyLinkPage="Advanced_StorageSelectFolder_Content.cgi";


function addNewDir() {
	if(document.forms[0].aname.value=="") {
		alert("<#JS_addnewdir3#>");
		return ;
	} else
		dirname=document.forms[0].aname.value;

	if(top.list.dirpath=="" || top.list.dirpath == "/") {
		alert("<#JS_addnewdir1#>");
		return ;
	}

	if(top.list.pathlevel!=1) {
		alert("<#JS_addnewdir2#>"+top.list.pathlevel + ".");
		return;
	}
	
	if (!validate_share(dirname)) {
//		alert("Folder name is invalid");
		return;
	}

	if (dirname.charAt(0)==" ")
        {
                alert("<#JS_addnewdir5#>");
                return;
        }
	
	for (i=0; i<top.list.dirarray.length; i++) {
		if (dirname == top.list.dirarray[i]) {
			alert(dirname + "<#JS_addnewdir4#>");
			return;
		}
	}
	
	newdir = top.list.dirpath + "/" + dirname;
	j=0;
	for(i=0;i<newdir.length;i++)
	{
		c = newdir.charCodeAt(i);
   		if (c>255)
			j++;	
	}
	j=j*2+newdir.length;
	if (j>32)
	{
		alert("<#JS_addnewdir6#>");
		return;
	}
	
//	alert("Add folder '"+ newdir + "'");
	top.opener.document.forms[0].sh_path_0.value = newdir;
	top.opener.document.forms[0].sh_name_0.value = dirname;
	top.opener.document.forms[0].sh_nameb_0.value = encodeURIComponent(dirname);
	top.opener.document.forms[0].sh_rright_x_0.value = "on";
	top.opener.document.forms[0].sh_wright_x_0.value = "on";
	top.opener.top.Mkdir_Flag = "on";
/* 2005.12.23 Jiahao */
	top.opener.top.Share_Path = newdir;
	top.opener.document.forms[0].Storage_SharedList.click();
	top.window.close();
}

function doSubmit() {
	var f = document.form;
	var str ="";

//	if(f.path.value=="") {
//		str = top.list.dirpath;
//	else if(top.list.dirpath=="/")
//		str = "/" + f.path.value;

	if(f.path.value=="") {
		alert("<#JS_adddira#>");
		return;
	}
        str = top.list.dirpath;
        if(top.list.checkifdirexist(f.path.value)==0) {
                alert(str + "/" + f.path.value + " <#JS_nodir#>");
                return;
        }

	if(top.list.dirpath=="/")
		str = "/" + f.path.value;
	else
		str = top.list.dirpath + "/" + f.path.value;

        if(str=="/") { //can't be root
		alert("<#JS_adddirb#>");
		return;
	}

	if(f.path.value!="") {
		if (top.list.pathlevel!=1) {
			alert("<#JS_adddirc#>" + top.list.pathlevel + ".");
			return;
		}
		
		j=0;
		for(i=0;i<str.length;i++)
		{
			c = str.charCodeAt(i);
   			if (c>255)
				j++;	
		}
		j=j*2+str.length;
		if (j>32)
		{
			alert("<#JS_addnewdir6#>");
			return;
		}
		
		top.opener.document.forms[0].sh_path_0.value = str;
		index=str.lastIndexOf("/")+1;
		top.opener.document.forms[0].sh_name_0.value = str.substr(index);
		top.opener.document.forms[0].sh_nameb_0.value = encodeURIComponent(str.substr(index));
		top.opener.document.forms[0].sh_rright_x_0.value = "on";
		top.opener.document.forms[0].sh_wright_x_0.value = "on";
/* 2005.12.20 Jiahao */
		top.opener.top.Add_Flag = "on";
/* 2005.12.23 Jiahao */
		top.opener.top.Share_Path = str;
		top.opener.document.forms[0].Storage_SharedList.click();
	}
	top.window.close();
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

	if (!validate_string(o)) 
	{
		return false;
	}

	for(i=0;i<o.length;i++)
	{
		c = o.charCodeAt(i);
   	
		if (!validate_sharechar(c)) 
		{
			alert("<#JS_validstr2#> '" + o.charAt(i) +"' !");
			return(false);	
		}
	}
	return(true);  
}

</script>
</head>
<div id="overDiv" style="position:absolute; visibility:hidden; z-index:1000;"></div>
<body>
<form name=form>
<!-- Table for the conntent page -->	    
<table width="450" border="1" cellpadding="0" cellspacing="0" bordercolor="E0E0E0">
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#JS_sharefolder#>', LEFT);" onMouseOut="return nd();" width="45%">
<#ShareNode_FolderManagerForm_ShareFolder_itemname#></td>
<td class="content_input_td">
<input type="text"  class="content_input_fd" maxlength="32" size="32" name="path" value="">
</td>
<td class="content_input_td" width="40">
<input type="button" class="content_input_fd_ro" value="<#CTL_ok#>" name="shareddir" onclick=doSubmit() >
</td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#JS_addfolder#>', LEFT);" onMouseOut="return nd();" width="45%">
<#ShareNode_FolderManagerForm_AddFolder_itemname#></td>
<td class="content_input_td">
<input type="text" class="content_input_fd" value="" maxlength="32" size="32" name="aname" value="">
</td>
<td class="content_input_td" width="40">
<input type="button" class="content_input_fd_ro" value="<#CTL_add#>" name="adddir" onclick=addNewDir()>
</td>
</tr>
</table>
</form>
</body>
