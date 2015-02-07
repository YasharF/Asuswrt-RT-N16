<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<title>Add New Folder</title>
<link rel="stylesheet" href="../form_style.css"  type="text/css">

<script type="text/javascript" src="../state.js"></script>
<script type="text/javascript">
var selectedPool = parent.getSelectedPool();
var folderlist = parent.get_sharedfolder_in_pool(selectedPool);

function initial(){
	showtext($("poolName"), selectedPool);
	
	clickevent();
}

function clickevent(){
	$("Submit").onclick = function(){
			if(validForm()){
				document.createFolderForm.pool.value = selectedPool;
				
				/*alert('action = '+document.createFolderForm.action+'\n'+
					  'pool = '+document.createFolderForm.pool.value+'\n'+
					  'folder = '+document.createFolderForm.folder.value
					  );//*/
				
				parent.showLoading();
				document.createFolderForm.submit();
				parent.hidePop("apply");
			}
		};
}

function validForm(){
	$("folder").value = trim($("folder").value);
	
	// share name
	if($("folder").value.length == 0){
		alert("<#File_content_alert_desc6#>");
		$("folder").focus();
		return false;
	}
	
	var re = new RegExp("[^a-zA-Z0-9 _-]+", "gi");
	if(re.test($("folder").value)){
		alert("<#File_content_alert_desc7#>");
		$("folder").focus();
		return false;
	}
	
	if(parent.checkDuplicateName($("folder").value, folderlist)){
		alert("<#File_content_alert_desc8#>");
		$("folder").focus();
		return false;
	}
	
	if(trim($("folder").value).length > 12)
		if (!(confirm("<#File_content_alert_desc10#>")))
			return false;
	
	return true;
}
</script>
</head>

<body onLoad="initial();">
<form name="createFolderForm" method="post" action="create_sharedfolder.asp" target="hidden_frame">
<input type="hidden" name="pool" id="pool">
	<table width="100%" class="popTable" border="0" align="center" cellpadding="0" cellspacing="0">
	<thead>
    <tr>
      <td colspan="2"><span><#AddFolderTitle#> <#in#> </span><span id="poolName"></span><img src="../images/button-close.gif" onClick="parent.hidePop('OverlayMask');"></td>
    </tr>
	</thead>
	<tbody>
    <tr align="center">
      <td height="50" colspan="2"><#AddFolderAlert#></td>
    </tr>
    <tr>
      <th width="100"><#FolderName#>: </th>
      <td height="50"><input type="text" name="folder" id="folder" style="width:220px;"></td>
    </tr>
	<!--<tr align="center">
      <td height="1" colspan="6" style=" border-bottom:1px dashed #666;"></td>
    </tr>
    <tr bgcolor="#F4F4F4">
      <th>&nbsp;</th>
      <td width="43" align="center">RW</td>
      <td width="43" align="center">R</td>
      <td width="43" align="center">W</td>
      <td width="43" align="center">No</td>
      <td>&nbsp;</td>
    </tr>	
    <tr  bgcolor="#F4F4F4">
      <th>Intranet :</th>
      <td align="center"><input name="newNNFolder" type="radio" checked></td>
      <td align="center"><input name="newNNFolder" type="radio"></td>
      <td align="center"><input name="newNNFolder" type="radio"></td>
      <td align="center"><input name="newNNFolder" type="radio"></td>
      <td>&nbsp;</td>
    </tr>
    <tr  bgcolor="#F4F4F4">
      <th>FTP :</th>
      <td align="center"><input name="newftpFolder" type="radio"></td>
      <td align="center"><input name="newftpFolder" type="radio"></td>
      <td align="center"><input name="newftpFolder" type="radio"></td>
      <td align="center"><input name="newftpFolder" type="radio" checked></td>
      <th>&nbsp;</th>
    </tr>-->
    <tr bgcolor="#E6E6E6">
      <th colspan="2"><input id="Submit" type="button" class="button" value="<#CTL_add#>"></th>
    </tr>
  </tbody>	
  </table>
</form>
</body>
</html>
