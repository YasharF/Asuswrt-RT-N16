<head>
<title>ZVMODELVZ Web Manager</title>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<link rel="stylesheet" type="text/css" href="style.css" media="screen"></link>
<input type="hidden" name="preferred_lang" value="<% nvram_get_x("","preferred_lang"); %>">
<script language="JavaScript" type="text/javascript" src="overlib.js"></script>
<script language="JavaScript" type="text/javascript" src="general.js"></script>
<script language=JavaScript>

// dirstring="/currentdir,.,..,A,B,C,D,E";
dirstring="<% select_folder("Storage"); %>";
dirarray=dirstring.split(",");
dirpath=dirarray.shift();
patharray = dirpath.split("/");
pathlevel = patharray.length-1;

str="";
current="-1";

var MyLinkPage="Advanced_StorageSelectFolder_Content.cgi";

function gfileclick(f, d) { // Double-click! Enter directory or select file
	if (d) {
		if (current == f) {// Double-click
			if (dirpath=='/')
				newdirpath='/'+f;
			else
				newdirpath=dirpath+'/'+f;
			top.location.href=MyLinkPage+"?NewDirPath="+encodeURIComponent(newdirpath);
		} else {
			current=f; // One-click
			top.frames[1].document.form.path.value=f;
		}
	} else {
		if(!isNaN(f)) { // f is a number
			patharray = dirpath.split("/");
			// ex: /tmp/harddisk/dira/dirb	index of "/"    is 4
			//				index of "/tmp" is 3
			//				index of "/tmp/harddisk" is 2
			//				index of "/tmp/hraddisk/dira" is 1
			//				index of "/tmp/harddisk/dira/dirb" is 0
			//				patharray.length = 5
			pathindex = parseInt(f, 10);
			newindex = patharray.length - pathindex - 1;
			// ex: /tmp/harddisk/dira/dirb	new index of "/"    is 0
			//				new index of "/tmp" is 1
			//				new index of "/tmp/harddisk" is 2
			//				new index of "/tmp/hraddisk/dira" is 3
			//				new index of "/tmp/harddisk/dira/dirb" is 4
			str3 = "";
			for (i=0; i<=newindex; i++) {
				str3+=patharray[i];
				str3+="/";
			}
//			alert(str3);
//			if (str3.length!=1) {				// str3 != "/"
				str3 = str3.substring(0,str3.length-1);	// erase the "/" tail of path
				top.location.href=MyLinkPage+"?NewDirPath="+encodeURIComponent(str3);
//			} else {
//				alert("can not change path to '/'");
//			}
		}
	}
}

function genpath() {
	patharray = dirpath.split("/");
	len = patharray.length;
	str2 = "/";
	for(j=1; j<len; j++) {
		str2+="<a href=\"javascript:gfileclick('";
		str2+=len-j-1;
		str2+="', 0);\">";
		str2+=patharray[j];
		str2+="</a>/"
	}
	if (str2 != "/")
		str2 = str2.substring(0,str2.length-1);
	document.writeln(str2);
}

function writedirlist() {
	for(i=0; i<dirarray.length; i++) {
		if(dirarray[i]=="." ||dirarray[i]=="")
			continue;
        	str+="<tr><td><a href=\"javascript:gfileclick('";
		if(dirarray[i]=="..")
			str+="1', 0);\">";
		else
			str+=dirarray[i]+"',1);\">";
		str+="<img border=0 src=./graph/dir.gif>";
		str+="</a>&nbsp;"
		str+="<a href=\"javascript:gfileclick('";
		if(dirarray[i]=="..")
			str+="1', 0);\">";
		else
			str+=dirarray[i]+"',1);\">";
		str+=dirarray[i];
		str+="</a></td></tr>"
	}
	document.writeln(str);
}

function checkifdirexist(dirname) {
        for(i=0; i<dirarray.length; i++) {
                if(dirarray[i]=="." || dirarray[i]=="..")
                        continue;
                if(dirarray[i]==dirname)
                        return 1;
        }
        return 0;
}


</script>
</head>

<body>
<form>
<!-- Table for the conntent page -->
<table width="450" border="0" cellpadding="0" cellspacing="0">

<tr>
<td>
<table width="450" border="1" cellpadding="0" cellspacing="0" bordercolor="E0E0E0">
<tr class="content_header_tr">
<td class="content_header_td_title"><#t2FM#></td>
</tr>
</table>
</td>
</tr>

<tr>
<td>
<table cellspacing=0 cellpadding=0 width="100%" border=0>
  <tbody>
  <tr>
    <td></td></tr>
  <tr>
    <td>
      <table cellpadding=0 width="90%" align=center border=0>
        <tbody>
        <tr>
          <td colspan=2><font face=arial size=3>
            <script>genpath();</script>
            </font></td></tr>
        <tr>
          <td><font face=arial size=4>
            <script>writedirlist();</script>
            </font></td></tr>
        </tbody>
      </table>
    </td>
  </tr>
  </tbody>
</table>
</td>
</tr>


</table>
</form>
</body>
