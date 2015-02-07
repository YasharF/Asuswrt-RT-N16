<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>
<head>
<meta http-equiv="X-UA-Compatible" content="IE=EmulateIE7"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<title>ASUS Wireless Router <#Web_Title#> - <#menu1#></title>
<link rel="stylesheet" type="text/css" href="index_style.css">
<link rel="stylesheet" type="text/css" href="form_style.css">
<link rel="stylesheet" type="text/css" href="NM_style.css">
<link rel="stylesheet" type="text/css" href="other.css">
<style type="text/css">
.style1 {color: #006633}
.style4 {color: #333333}
.style5 {
	color: #CC0000;
	font-weight: bold;
}
</style>
<script type="text/javascript" src="/state.js"></script>
<script type="text/javascript" src="/jquery.js"></script>
<script type="text/javascript" src="/popup.js"></script>
<script type="text/javascript" src="/client_function.js"></script>
<script type="text/javascript" src="/help.js"></script>
<script type="text/javascript" src="/detect.js"></script>
<script type="text/javascript" src="/alttxt.js"></script>
<script>
// for client_function.js
<% login_state_hook(); %>

openHint = null; // disable the openHint().

wan_route_x = '<% nvram_get_x("IPConnection", "wan_route_x"); %>';
wan_nat_x = '<% nvram_get_x("IPConnection", "wan_nat_x"); %>';
wan_proto = '<% nvram_get_x("Layer3Forwarding",  "wan_proto"); %>';

<% wanlink(); %>

var flag = '<% get_parameter("flag"); %>';

var leases = [<% dhcp_leases(); %>];	// [[hostname, MAC, ip, lefttime], ...]
var arps = [<% get_arp_table(); %>];		// [[ip, x, x, MAC, x, type], ...]
var arls = [<% get_arl_table(); %>];		// [[MAC, port, x, x], ...]
var wireless = [<% wl_auth_list(); %>];	// [[MAC, associated, authorized], ...]
var ipmonitor = [<% get_static_client(); %>];	// [[IP, MAC, DeviceName, Type, http, printer, iTune], ...]
var networkmap_fullscan = '<% nvram_match_x("", "networkmap_fullscan", "0", "done"); %>'; //2008.07.24 Add.  1 stands for complete, 0 stands for scanning.;

var clients = getclients(1);

function initial(){
	show_banner(0);
	show_menu(1, 0, 0);
	show_footer();
	
	show_middle_status();
	
	set_default_choice();
	
	if(sw_mode == "2")
		MapUnderRepeatermode();
	else if(sw_mode == "3")
		MapUnderAPmode();
		
	load_alttxt_enviroment();
}

function set_default_choice(){
	var icon_name;
	if(flag && flag.length > 0 && wan_route_x != "IP_Bridged"){
		if(flag == "Internet")
			$("statusframe").src = "/device-map/internet.asp";
		else if(flag == "Client")
			$("statusframe").src = "/device-map/clients.asp";
		else{
			clickEvent($("iconRouter"));
			return;
		}
		
		icon_name = "icon"+flag;
		clickEvent($(icon_name));
	}
	else
		clickEvent($("iconRouter"));
}


function showMapWANStatus(flag){

	if(flag == 1){
		showtext($("internetStatus"), "<#Connected#>");
		$("ifconnect").style.display = "none";
	}
	else{
		showtext($("internetStatus"), "<#Disconnected#>");
		$("ifconnect").style.display = "block";
	}
}

function show_middle_status(){
	if(ssid2.length > 15){
		ssid2 = ssid2.substring(0,14) + "...";
	}
	
	$("SSID").value = ssid2;
	
	var auth_mode = document.form.wl_auth_mode.value;
	var wpa_mode = document.form.wl_wpa_mode.value;
	var wl_wep_x = parseInt(document.form.wl_wep_x.value);
	var security_mode;
	
	if(auth_mode == "open"){
		security_mode = "Open System";
		
		if(wl_wep_x == 0)
			$("iflock").style.background = 'url(images/unlock_icon.gif) no-repeat';
	}
	else if(auth_mode == "shared")
		security_mode = "Shared Key";
	else if(auth_mode == "psk"){
		if(wpa_mode == "1")
			security_mode = "WPA-Personal";
		else if(wpa_mode == "2")
			security_mode = "WPA2-Personal";
		else if(wpa_mode == "0")
			security_mode = "WPA-Auto-Personal";
		else
			alert("System error for showing auth_mode!");
	}
	else if(auth_mode == "wpa"){
		if(wpa_mode == "3")
			security_mode = "WPA-Enterprise";
		else if(wpa_mode == "4")
			security_mode = "WPA-Auto-Enterprise";
		else
			alert("System error for showing auth_mode!");
	}
	else if(auth_mode == "wpa2")
		security_mode = "WPA2-Enterprise";
	else if(auth_mode == "radius")
		security_mode = "Radius with 802.1x";
	else
		alert("System error for showing auth_mode!");
	$("wl_securitylevel_span").innerHTML = security_mode;
	
	$("iflock").style.display = "block";
	
	// clients
	show_client_status();			
}

function show_client_status(){
	var client_str = "";
	var wired_num = 0, wireless_num = 0;
	
	if(sw_mode == "1"){
		client_str += "<#Full_Clients#>: <span>"+clients.length+"</span>";
	}
	else		
		client_str += "<#Noclients#>";
	
	$("clientNumber").innerHTML = client_str;
}

function show_device(){
	var usb_path1 = '<% nvram_get_x("", "usb_path1"); %>';
	//var usb_path1 = '';	
	// show the front usb device
	
	switch(usb_path1){
		case "storage":
			for(var i = 0; i < all_disks.length; ++i){
				if(all_disk_interface[i] == "USB"){
					disk_html(0, i);
					break;
				}
			}
			break;
		case "audio":
			break;
		case "printer":
			printer_html(0, 0);
			break;
		case "HSDPA":
			HSDPA_html(0, 0);
			break;
		case "webcam":
			break;
		default:
			no_device_html(0);
	}
}

var avoidkey;
var lastClicked;
var lastName;

function clickEvent(obj){
	var icon;
	var ContainerWidth;
	var stitle;
	
	if(obj.id == "iflock"){
		obj = $("iconRouter");
	}
	
	if(obj.id.indexOf("Internet") > 0){
		icon = "iconInternet";
		ContainerWidth = "300px";
		stitle = "<#statusTitle_Internet#>";
		$("statusframe").src = "/device-map/internet.asp";
	}
	else if(obj.id.indexOf("Router") > 0){
		icon = "iconRouter";
		ContainerWidth = "340px";
		stitle = "<#statusTitle_System#>";
	}
	else if(obj.id.indexOf("Client") > 0){
		if(sw_mode != "1")
			return;
		
		icon = "iconClient";
		ContainerWidth = "396px";
		stitle = "<#statusTitle_Client#>";
	}
	else if(obj.id.indexOf("USBdisk") > 0){
		icon = "iconUSBdisk";
		ContainerWidth = "300px";
		stitle = "<#statusTitle_USB_Disk#>";
		$("statusframe").src = "/device-map/disk.asp";
	}
	else if(obj.id.indexOf("Printer") > 0){
		icon = "iconPrinter";
		ContainerWidth = "300px";
		stitle = "<#statusTitle_Printer#>";
	}
	else if(obj.id.indexOf("Remote") > 0){
		icon = "iconRemote";
		ContainerWidth = "300px";
		stitle = "<#statusTitle_AP#>";
		$("statusframe").src = "/device-map/remote.asp";
		//alert($("statusframe").src);
		//alert(obj.id);
	}	
	else if(obj.id.indexOf("No") > 0){
		icon = "iconNo";
	}
	else
		alert("mouse over on wrong place!");
	
	$('statusContainer').style.width = ContainerWidth;
	if(lastClicked){
		lastClicked.style.background = 'url(images/map-'+lastName+'.gif) no-repeat';
	}
	
	obj.style.background = 'url(images/map-'+icon+'_d.gif) no-repeat';
	$('statusIcon').src = 'images/iframe-'+icon+'.gif';
	
	$('helpname').innerHTML = stitle;
	
	avoidkey = icon;
	lastClicked = obj;
	lastName = icon;
}

function mouseEvent(obj, key){
	var icon;
	
	if(obj.id.indexOf("Internet") > 0)
		icon = "iconInternet";
	else if(obj.id.indexOf("Router") > 0)
		icon = "iconRouter";
	else if(obj.id.indexOf("Client") > 0){
		if(wan_route_x == "IP_Bridged")
			return;
		
		icon = "iconClient";
	}
	else if(obj.id.indexOf("USBdisk") > 0)
		icon = "iconUSBdisk";
	else if(obj.id.indexOf("Printer") > 0)
		icon = "iconPrinter";
	else if(obj.id.indexOf("No") > 0)
		icon = "iconNo";
	else
		alert("mouse over on wrong place!");
	
	if(avoidkey != icon){
		if(key){ //when mouseover
			obj.style.background = 'url("/images/map-'+icon+'_r.gif") no-repeat';
		}
		else {  //when mouseout
			obj.style.background = 'url("/images/map-'+icon+'.gif") no-repeat';
		}
	}
}//end of mouseEvent

function MapUnderAPmode(){// if under AP mode, disable the Internet icon and show hint when mouseover.
	
		//showtext($("internetStatus"), "<#OP_AP_item#>");
		
		$("iconInternet").style.background = "url(images/map-iconRemote.gif) no-repeat";
		$("iconInternet").style.cursor = "default";
		
		$("iconInternet").onmouseover = function(){
			writetxt('<#underAPmode#>');
		}
		$("iconInternet").onmouseout = function(){
			writetxt(0);
		}
		$("iconInternet").onclick = function(){
			return false;
		}
		$("clientStatusLink").href = "javascript:void(0)";
		$("clientStatusLink").style.cursor = "default";	
		$("iconClient").style.background = "url(images/map-iconClient_0.gif) no-repeat";
		$("iconClient").style.cursor = "default";
}

function MapUnderRepeatermode(){// 2009.01.06 Lock Add: Under Client mode, switch internet icon to AP and show WL radio.
		
		showtext($("internetStatus"), "<#OP_RE_item#>");
		
		$("wan_link_obj").style.background = "";
		
		$("iconInternet").style.display = "none";
		$("iconRemote").style.display = "block";
		$("iconRemote").style.background = "url(images/map-iconRemote.gif) no-repeat";
		
		$("iconRemote").onmouseover = function(){
			return false;
		}
		$("iconRemote").onmouseout = function(){
			return false;
		}		
		$("iconRemote").onclick = function(){ 
			clickEvent($("iconRemote"));
		};
		$("clientStatusLink").href = "javascript:void(0)";
		$("clientStatusLink").style.cursor = "default";	
		$("iconClient").style.background = "url(images/map-iconClient_0.gif) no-repeat";
		$("iconClient").style.cursor = "default";		
}
var $j = jQuery.noConflict();

aplist = "";

function getConnectingStatus(){
	if(aplist){
		for(var i=0; i<aplist.length ;i++){
			if(aplist[i][8] != 0){
				return aplist[i][8]; 
			}
			else{
				continue;
			}
		}
		return 0;
	}
	return -1;
}

function ajaxGetData(e) {
  $j.ajax({
    url: 'aplist.asp',
    dataType: 'script', 
	
    error: function(xhr) {
      ;
    },
    success: function(response) {
    	
	    if(getConnectingStatus()==2){
	    	$j("#internetStatus").html("<#Connected#>");
	    	$("wl_radio_obj").style.visibility = "visible";
	    }
	  	else{
	    	$j("#internetStatus").html("<#CTL_Disconnect#>");
	    	$("wl_radio_obj").style.visibility = "hidden";
	  	}
    }
  });
}

$j(document).ready(function($){
	if(sw_mode == 2){
		setTimeout("ajaxGetData();",3000);
	}
});

</script>
</head>

<body onunload="return unload_body();">
<noscript>
	<div class="popup_bg" style="visibility:visible; z-index:999;">
		<div style="margin:200px auto; width:300px; background-color:#006699; color:#FFFFFF; line-height:150%; border:3px solid #FFF; padding:5px;"><#not_support_script#></p></div>
	</div>
</noscript>

<div id="TopBanner"></div>

<div id="Loading" class="popup_bg"></div>
<div id="hiddenMask" class="popup_bg">
	<table cellpadding="5" cellspacing="0" id="dr_sweet_advise" class="dr_sweet_advise" align="center">
		<tr>
		<td>
			<div class="drword" id="drword"><#Main_alert_proceeding_desc4#> <#Main_alert_proceeding_desc1#>...
				<br>
				<br>
		    </div>
		  <div class="drImg"><img src="images/DrsurfImg.gif"></div>
			<div style="height:70px; "></div>
		</td>
		</tr>
	</table>
<!--[if lte IE 6.5]><iframe class="hackiframe"></iframe><![endif]-->
</div>

<iframe name="hidden_frame" id="hidden_frame" width="0" height="0" frameborder="0" scrolling="no"></iframe>

<form name="form">
<input type="hidden" name="current_page" value="index.asp">
<input type="hidden" name="preferred_lang" id="preferred_lang" value="<% nvram_get_x("LANGUAGE", "preferred_lang"); %>">
<input type="hidden" name="wl_ssid2" value="<% nvram_char_to_ascii("WLANConfig11b", "wl_ssid"); %>">
<input type="hidden" name="firmver" value="<% nvram_get_x("",  "firmver"); %>">

<input type="hidden" name="wl_auth_mode" value="<% nvram_get_x("",  "wl_auth_mode"); %>">
<input type="hidden" name="wl_wpa_mode" value="<% nvram_get_x("",  "wl_wpa_mode"); %>">
<input type="hidden" name="wl_wep_x" value="<% nvram_get_x("",  "wl_wep_x"); %>">
</form>

<table class="content" align="center" cellpadding="0" cellspacing="0">
  <tr>
	<td valign="top" width="23"><div id="Dr_body"></div></td>
		
		<!--=====Beginning of Main Menu=====-->
		<td valign="top" width="202">
			<div id="mainMenu"></div>
			<div id="subMenu"></div>
		</td>
		
		<td align="center" valign="top"  class="bgarrow">
		
		<!--=====Beginning of Network Map=====-->
		<div id="tabMenu"></div><br>
		<table width="350" border="0" cellspacing="0" cellpadding="0">
		  <tr align="left">
            <td colspan="2"><table border="0" cellpadding="0" cellspacing="0" style="margin-left:100px; ">
              <tr>
                <td width="95">
				<a href="/device-map/internet.asp" target="statusframe">
					<div id="iconInternet" onclick="clickEvent(this);"></div>
				</a>
				<a href="/device-map/remote.asp" target="statusframe">
					<div id="iconRemote" onclick="clickEvent(this);" style="display='none'"></div>
				</a>
				<div id="overDiv" style="position:absolute; visibility:hidden; z-index:1000;"></div>
				</td>
                <td class="NMdesp"><span id="internetStatus"></span></td>
              </tr>
            </table>
			</td>
          </tr>
          <tr align="left" valign="middle" >
			<td id="wan_link_obj" height="20" colspan="2" style="background:url(images/map-icon-arrow.gif) repeat-y 98px;">
					<object id="wl_radio_obj" style="margin-left:125px; visibility:hidden;"classid="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000" codebase="http://download.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#version=6,0,29,0" width="30" height="30">
						<param name="movie" value="/images/radio.png" />
						<param name="wmode" value="transparent">
						<param name="quality" value="high" />
						<embed src="/images/radio.png" quality="high" pluginspage="http://www.macromedia.com/go/getflashplayer" type="application/x-shockwave-flash" width="30" height="30" wmode="transparent"></embed>
				  </object>
				<div class="ifconnect" id="ifconnect"></div>
			</td>
          </tr>
          <tr>
            <td colspan="2">
			<table class="NMitem" border="0" cellspacing="0" cellpadding="0" style="margin-left:100px; height:77px;">
              <tr>
                <td width="95" align="left">
					<a href="device-map/router.asp" target="statusframe">						
						<div id="iconRouter" onclick="clickEvent(this);"></div>
					</a>
					<!--a href="#">
						<div id="mode_tag" onmouseover="showMode(0);" >Repeater <span id="arrow_down">&nbsp;</span></div>
					</a-->
					<!--div id="mode_switch" onclick="hideMode();">
					  	<a href="#"><div><input type="radio" name="qmode" checked/>Repeater</div></a>
						<a href="#"><div><input type="radio" name="qmode" />Client</div></a>
					</div-->
				</td>
                <td class="NMdesp">
					<a href="device-map/router.asp" target="statusframe"><div id="iflock" onclick="clickEvent(this);"></div></a>
					<strong><#statusTitle_System#></strong><br/>
					<!--strong>SSID</strong>: <input id="SSID" class="map_ssid" readonly=readonly onmouseover="return overlib(decodeURIComponent(document.form.wl_ssid2.value), RIGHT);" onmouseout="return nd();"><br-->
					<strong>SSID</strong>: <input id="SSID" class="map_ssid" readonly=readonly onmouseover="writetxt(decodeURIComponent(document.form.wl_ssid2.value));" onmouseout="writetxt(0);"><br>
					<strong><#Security_Level#></strong>: <span id="wl_securitylevel_span"></span>
				</td>
              </tr>
            </table>
			</td>
          </tr>
          <tr>
            <td colspan="2" style="background:url(images/map-icon-arrow.gif) no-repeat 98px;">&nbsp;</td>
          </tr>
          <tr>
            <td width="130"  align="left" valign="top">
				<table width="110" border="0" cellpadding="0" cellspacing="0" style="margin-left:90px; ">
					<tr>
						<td align="center">
							<a id="clientStatusLink" href="device-map/clients.asp" target="statusframe">
								<div id="iconClient" onclick="clickEvent(this);"></div>
							</a>
						</td>
					</tr>
					<tr>
						<td align="center" class="clients" id="clientNumber"></td>
					</tr>
				</table>
			</td>
			
			<!--td align="center" class="mapgroup">
				<table width="95%" border="0" cellpadding="0" cellspacing="0" style="margin-left:5px; ">
					<tr id="device_0">
						<td width="88" height="90">
							<div id="deviceIcon_0"></div>
						</td>
						<td height="90" class="NMdesp">
							<div id="deviceDec_0"></div>
						</td>
					</tr>
				</table>
			</td-->
		</tr>
	</table>        
</td>
		<!--=====End of Main Content=====-->
	
	<!--==============Beginning of hint content=============-->
	<td id="statusContainer" width="340" align="left" valign="top" >
	  <table width="95%" border="0" cellpadding="0" cellspacing="0">
		<tr>
		  <td class="statusTitle">
		  	<img id="statusIcon" src="images/iframe-iconRouter.gif" align="absmiddle">
		  	<span id="helpname"><#statusTitle_System#></span></td>
		</tr>
		<tr>
		  <td>
			<iframe id="statusframe" name="statusframe" src="/device-map/router.asp" frameborder="0" width="100%" height="450"></iframe>
		  </td>
		</tr>
	  </table>
	</td>
	<!--==============Ending of hint content=============-->
  </tr>
</table>
<div id="navtxt" class="navtext" style="position:absolute; top:50px; left:-100px; visibility:hidden; font-family:Arial, Verdana"></div>
<div id="footer"></div>

<!-- mask for disabling AiDisk -->
<div id="OverlayMask" class="popup_bg">
	<div align="center">
	<iframe src="" frameborder="0" scrolling="no" id="popupframe" width="400" height="500" allowtransparency="true" style="margin-top:70px;"></iframe>
	</div>
<!--[if lte IE 6.5]><iframe class="hackiframe"></iframe><![endif]-->
</div>
<script>
if(flag == "Internet" || flag == "Client")
	$("statusframe").src = "";
	initial();
</script>
</body>
</html>
