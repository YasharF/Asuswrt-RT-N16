<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<title>Untitled Document</title>
<link rel="stylesheet" type="text/css" href="../NM_style.css">
<link rel="stylesheet" type="text/css" href="../form_style.css">

<script type="text/javascript" src="formcontrol.js"></script>
<script type="text/javascript" src="/state.js"></script>
<script type="text/javascript" src="/detectWAN.js"></script>
<script>
<% wanlink(); %>

function initial(){
	flash_button();
	//detectWANstatus();
	show_connect_status();
	
	showtext($("WANIP"), wanlink_ipaddr());
	
	var dnsArray = wanlink_dns().split(" ");
	if(dnsArray[0])
		showtext($("DNS1"), dnsArray[0]);
	if(dnsArray[1])
		showtext($("DNS2"), dnsArray[1]);
	
	showtext($("connectionType"), wanlink_type());
	
	showtext($("gateway"), wanlink_gateway());
}

function show_connect_status(){
	if(getWANStatus() == 1){
		$("connectstatus").innerHTML="<#Connected#>";
		$("connectbutton").value="<#Disconnect#>";
		
		$("connectbutton").style.display = "";
	}
	else if(getWANStatus() == 0){
		$("connectstatus").innerHTML="<#Disconnected#>";
		$("connectbutton").value="<#Connect#>";
		
		$("connectbutton").style.display = "";
	}
	else
		$("connectbutton").style.display = "none";
	
	setTimeout("show_connect_status();", 1);
}

function getWANStatus(){
	return parent.get_connect_status();
}

function submitWANAction(){
	var status = getWANStatus();
	
	switch(status){
		case 0:
			parent.showLoading();
			setTimeout('location.href = "/device-map/wan_action.asp?wanaction=Connect";', 1);
			break;
		case 1:
			parent.showLoading();
			setTimeout('location.href = "/device-map/wan_action.asp?wanaction=Disconnect";', 1);
			break;
		default:
			alert("No change!");
	}
}

function goQIS(){
	parent.showLoading();
	parent.location.href = '/QIS_wizard.htm';
}

function sbtnOver(o){
	o.style.color = "#FFFFFF";		
	o.style.background = "url(/images/sbtn.gif) #FFCC66";
	o.style.cursor = "pointer";
}

function sbtnOut(o){
	o.style.color = "#000000";
	o.style.background = "url(/images/sbtn0.gif) #FFCC66";
}
</script>
</head>

<body class="statusbody" onload="initial();">
<table width="95%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="table1px">
  <tr>
    <th width="120"><#ConnectionStatus#></th>
    <td width="150">
	  <span id="connectstatus"></span>
    <input type="button" id="connectbutton" class="button" value="" onclick="submitWANAction();">
	</td>
  </tr>
  <tr>
    <th><#WAN_IP#></th>
    <td><span id="WANIP"></span></td>
  </tr>
  <tr>
    <th>DNS</th>
    <td><span id="DNS1"></span><br><span id="DNS2"></span></td>
  </tr>
  <tr>
    <th><#Connectiontype#></th>
    <td><span id="connectionType"></span></td>
  </tr>
  <tr>
    <th><#Gateway#></th>
    <td><span id="gateway"></span></td>
  </tr>
  <tr>
    <th><#QIS#></th>
    <td><input type="button" class="sbtn" value="<#btn_go#>" onclick="javascript:goQIS();" onmouseover="sbtnOver(this);" onmouseout="sbtnOut(this);"></td>
	</tr>
</table>
<select class="domore" onchange="domore_link(this);">
  <option selected="selected"><#MoreConfig#>...</option>
  <option value="../Advanced_WAN_Content.asp"><#menu5_3_1#></option>
  <option value="../Advanced_PortTrigger_Content.asp"><#menu5_3_3#></option>
  <option value="../Advanced_VirtualServer_Content.asp"><#menu5_3_4#></option>
  <option value="../Advanced_Exposed_Content.asp"><#menu5_3_5#></option>  
  <option value="../Advanced_ASUSDDNS_Content.asp"><#menu5_3_6#></option>
  <option value="../Main_IPTStatus_Content.asp"><#menu5_7_5#></option>
</select>
</body>
</html>
