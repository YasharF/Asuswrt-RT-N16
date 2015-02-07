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
<script type="text/javascript" src="/popup.js"></script>
<script type="text/javascript" src="/disk_functions.js"></script>
<script type="text/javascript" src="/client_function.js"></script>
<script type="text/javascript" src="/help.js"></script>
<script type="text/javascript" src="/js/ex_overlib.js"></script>
<script type="text/javascript" src="/detect.js"></script>
<script>
wan_route_x = '<% nvram_get_x("IPConnection", "wan_route_x"); %>';
wan_nat_x = '<% nvram_get_x("IPConnection", "wan_nat_x"); %>';
wan_proto = '<% nvram_get_x("Layer3Forwarding",  "wan_proto"); %>';

openHint = null; // disable the openHint().

<% disk_pool_mapping_info(); %>
<% available_disk_names_and_sizes(); %>

<% wanlink(); %>
<% get_printer_info(); %>

var all_disks = foreign_disks().concat(blank_disks());

var flag = '<% get_parameter("flag"); %>';
var disk_number = foreign_disks().length+blank_disks().length;
var device_number = Math.min(2, disk_number+printer_models().length);

// for client_function.js
<% login_state_hook(); %>

var leases = [<% dhcp_leases(); %>];	// [[hostname, MAC, ip, lefttime], ...]
var arps = [<% get_arp_table(); %>];		// [[ip, x, x, MAC, x, type], ...]
var arls = [<% get_arl_table(); %>];		// [[MAC, port, x, x], ...]
var wireless = [<% wl_auth_list(); %>];	// [[MAC, associated, authorized], ...]
var ipmonitor = [<% get_static_client(); %>];	// [[IP, MAC, DeviceName, Type, http, printer, iTune], ...]
var networkmap_fullscan = '<% nvram_match_x("", "networkmap_fullscan", "0", "done"); %>'; //2008.07.24 Add.  1 stands for complete, 0 stands for scanning.;

var clients = getclients(1);

function initial(){
	show_banner(0);
	show_menu(1, -1, 0);
	show_footer();
	
	show_usb_device();
	
	show_middle_status();
	
	set_default_choice();
	
	MapUnderAPmode();	
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
	if(flag == 2){
		showtext($("internetStatus"), "AP mode");
	}
	else if(flag == 1){
		showtext($("internetStatus"), "<#Connected#>");
		$("ifconnect").style.display = "none";
	}
	else{
		showtext($("internetStatus"), "<#Disconnected#>");
		$("ifconnect").style.display = "block";
	}
}

function show_middle_status(){
	var ssid_value = decodeURIComponent(document.form.ssid_acsii.value);
	
	if(ssid_value.length > 15){
		ssid_value = ssid_value.substring(0,14) + "...";
	}
	
	$("SSID").value = ssid_value;
	
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
	
	if(wan_route_x != "IP_Bridged"){
		/*for(var i = 0; i < clients.length; ++i){
			if(clients[i][3] == 10)
				++wireless_num;
			else
				++wired_num;
		}
		
		client_str += "<#Wired_Clients#>: <span>"+wired_num+"</span>";
		client_str += "<br>";
		client_str += "<#Wireless_Clients#>: <span>"+wireless_num+"</span>";//*/
		client_str += "<#Full_Clients#>: <span>"+clients.length+"</span>";
	}
	else		
		client_str += "<#Noclients#>";
	
	$("clientNumber").innerHTML = client_str;
}

function show_usb_device(){
	var usb_path1 = '<% nvram_get_x("", "usb_path1"); %>';
	var usb_path2 = '<% nvram_get_x("", "usb_path2"); %>';
	
	// show the lower usb device
	switch(usb_path1){
		case "storage":
			for(var i = 0; i < all_disks.length; ++i)
				if(foreign_disk_interface_names()[i] == "1"){
					disk_html(1, i);
					break;
				}
			break;
		case "printer":
			printer_html(1, 0);
			break;
		case "audio":
		case "webcam":
		default:
			no_device_html(1);
	}
	
	// show the upper usb device
	switch(usb_path2){
		case "storage":
			for(var i = 0; i < all_disks.length; ++i)
				if(foreign_disk_interface_names()[i] == "2"){
					disk_html(0, i);
					break;
				}
			break;
		case "printer":
			printer_html(0, 0);
			break;
		case "audio":
		case "webcam":
		default:
			no_device_html(0);
	}
}

function disk_html(device_order, all_disk_order){
	var device_icon = $("deviceIcon_"+device_order);
	var device_dec = $("deviceDec_"+device_order);
	var icon_html_code = '';
	var dec_html_code = '';
	
	var disk_model_name = "";
	var TotalSize;
	var mount_num = getDiskMountedNum(all_disk_order);
	var all_accessable_size;
	var percentbar = 0;
	
	if(all_disk_order < foreign_disks().length)
		disk_model_name = foreign_disk_model_info()[all_disk_order];
	else
		disk_model_name = blank_disks()[all_disk_order-foreign_disks().length];
	
	icon_html_code += '<a href="device-map/disk.asp" target="statusframe">\n';
	icon_html_code += '    <div id="iconHarddisk_'+all_disk_order+'" class="iconHarddisk" onclick="setSelectedDiskOrder(this.id);clickEvent(this);"></div>\n';
	icon_html_code += '</a>\n';
	
	dec_html_code += disk_model_name+'<br>\n';
	
	if(mount_num > 0){
		if(all_disk_order < foreign_disks().length)
			TotalSize = simpleNum(foreign_disk_total_size()[all_disk_order]);
		else
			TotalSize = simpleNum(blank_disk_total_size()[all_disk_order-foreign_disks().length]);
		
		all_accessable_size = simpleNum2(computeallpools(all_disk_order, "size")-computeallpools(all_disk_order, "size_in_use"));
		
		percentbar = simpleNum2((all_accessable_size)/TotalSize*100);
		percentbar = Math.round(100-percentbar);		
		dec_html_code += '<div id="diskquota">\n';
		dec_html_code += '<img src="images/quotabar.gif" width="'+percentbar+'" height="13">';
		dec_html_code += '</div>\n';
		dec_html_code += '<strong><#Totaldisk#></strong>: '+TotalSize+' GB<br>\n';		
		dec_html_code += '<span class="style1"><strong><#Availdisk#></strong>: '+(all_accessable_size)+' GB</span>\n';
	}
	else{
		dec_html_code += '<span class="style1"><strong><#DISK_UNMOUNTED#></strong></span>\n';
	}
	
	device_icon.innerHTML = icon_html_code;
	device_dec.innerHTML = dec_html_code;
}

function printer_html(device_order, printer_order){
	var printer_name = printer_manufacturers()[printer_order]+" "+printer_models()[printer_order];
	var printer_status = "";
	var device_icon = $("deviceIcon_"+device_order);
	var device_dec = $("deviceDec_"+device_order);
	var icon_html_code = '';
	var dec_html_code = '';
	
	if(printer_pool()[printer_order] != "")
		printer_status = '<#CTL_Enabled#>';
	else
		printer_status = '<#CTL_Disabled#>';
	
	icon_html_code += '<a href="device-map/printer.asp" target="statusframe">\n';
	icon_html_code += '    <div id="iconPrinter_'+printer_order+'" class="iconPrinter" onclick="clickEvent(this);"></div>\n';
	icon_html_code += '</a>\n';
	
	dec_html_code += printer_name+'<br>\n';
	dec_html_code += '<span class="style5">'+printer_status+'</span>\n';
	
	device_icon.innerHTML = icon_html_code;
	device_dec.innerHTML = dec_html_code;
}

function no_device_html(device_order){
	var device_icon = $("deviceIcon_"+device_order);
	var device_dec = $("deviceDec_"+device_order);
	var icon_html_code = '';
	var dec_html_code = '';
	
	icon_html_code += '    <div class="iconNo"></div>';
	
	dec_html_code += '<span class="account style4"><#NoDevice#></span>\n';
	
	device_icon.innerHTML = icon_html_code;
	device_dec.innerHTML = dec_html_code;
}

function MapUnderAPmode(){// if under AP mode, disable the Internet icon and show hint when mouseover.
	if(wan_route_x == "IP_Bridged"){
		showMapWANStatus(2);
		
		$("iconInternet").style.display = "none";
		$("iconAPmode").style.display = "block";
		
		$("clientStatusLink").href = "javascript:void(0)";
		$("clientStatusLink").style.cursor = "default";
		$("iconClient").style.background = "url(images/map-iconClient_0.gif) no-repeat";
		$("iconClient").style.cursor = "default";
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
	}
	else if(obj.id.indexOf("Router") > 0){
		icon = "iconRouter";
		ContainerWidth = "300px";
		stitle = "<#statusTitle_System#>";
	}
	else if(obj.id.indexOf("Client") > 0){
		if(wan_route_x == "IP_Bridged")
			return;
		
		icon = "iconClient";
		ContainerWidth = "396px";
		stitle = "<#statusTitle_Client#>";
	}
	else if(obj.id.indexOf("Harddisk") > 0){
		icon = "iconHarddisk";
		ContainerWidth = "300px";
		stitle = "<#statusTitle_USB_Disk#>";
	}
	else if(obj.id.indexOf("Printer") > 0){
		icon = "iconPrinter";
		ContainerWidth = "300px";
		stitle = "<#statusTitle_Printer#>";
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
	//$('statusIcon').src = 'images/iframe-'+icon+'.gif';
	$('statusIcon').style.background = "url(images/iframe-"+ icon +".gif) no-repeat";
	
	$('helpname').innerHTML = stitle;
	
	avoidkey = icon;
	lastClicked = obj;
	lastName = icon;
}

function apmode_hint(){
	return overlib("<#underAPmode#>", RIGHT);
}
</script>
</head>

<body onload="initial();">
<noscript>
	<div class="popup_bg" style="display:block">
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
<input type="hidden" name="current_page" value="/">
<input type="hidden" name="next_page" value="/">
<input type="hidden" name="preferred_lang" id="preferred_lang" value="<% nvram_get_x("LANGUAGE", "preferred_lang"); %>">
<input type="hidden" name="ssid_acsii" value="<% nvram_char_to_ascii("WLANConfig11b",  "wl_ssid"); %>">
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
				 <a href="device-map/internet.asp" target="statusframe">
					<div id="iconInternet" onclick="clickEvent(this);"></div>
				 </a>
 				<div id="iconAPmode" onmouseout="return nd();" onmouseover="return apmode_hint();"></div>
				<div id="overDiv" style="position:absolute; visibility:hidden; z-index:1000;"></div>
				</td>
                <td class="NMdesp"><span id="internetStatus"></span></td>
              </tr>
            </table></td>
          </tr>
          <tr align="left" valign="middle" >
            <td height="20" colspan="2" style="background:url(images/map-icon-arror.gif) repeat-y 98px;"><div class="ifconnect" id="ifconnect"></div></td>
          </tr>
          <tr>
            <td colspan="2">
			<table class="NMitem" border="0" cellspacing="0" cellpadding="0" style="margin-left:100px; height:77px;">
              <tr>
                <td width="95">
					<a href="device-map/router.asp" target="statusframe">						
						<div id="iconRouter" onclick="clickEvent(this);"></div>
					</a>
				</td>
                <td class="NMdesp">
					<a href="device-map/router.asp" target="statusframe"><div id="iflock" onclick="clickEvent(this);"></div></a>
					<div><strong><#statusTitle_System#></strong></div>
					<strong>SSID</strong>: <input id="SSID" class="map_ssid" readonly=readonly onmouseover="return overlib(decodeURIComponent(document.form.ssid_acsii.value), RIGHT);" onmouseout="return nd();"><br>
					<strong><#Security_Level#></strong>: <span id="wl_securitylevel_span"></span>
				</td>
              </tr>
            </table>
			</td>
          </tr>
          <tr>
            <td colspan="2" style="background:url(images/map-icon-arror1.gif) no-repeat 80px;">&nbsp;</td>
          </tr>
          <tr>
            <td width="130" align="right" valign="top">
				<table width="120" border="0" cellpadding="0" cellspacing="0">
					<tr>
						<td align="center">
							<a id="clientStatusLink" href="device-map/clients.asp" target="statusframe"><!--lock 1226-->
								<div id="iconClient" onclick="clickEvent(this);"></div></a></td>
					</tr>
					<tr>
						<td align="center" class="clients" id="clientNumber"></td><!--lock 1226-->
					</tr>
              </table>
			</td>
            <td align="center" class="mapgroup">
              <table width="95%" border="0" cellpadding="0" cellspacing="0" style="margin-left:5px; ">
                <tr id="device_0">
                  <td width="88" height="90">
                  	<div id="deviceIcon_0"></div>
				  </td>
                  <td height="90" class="NMdesp">
                  	<div id="deviceDec_0"></div>
                  </td>
                </tr>
                <tr id="device_1">
                  <td width="88" height="90">
                  	<div id="deviceIcon_1"></div>
				  </td>
                  <td height="90" class="NMdesp">
                  	<div id="deviceDec_1"></div>
                  </td>
                </tr>
              </table>
            </td>
          </tr>
        </table>        
        </td>
		<!--=====End of Main Content=====-->
	
	<!--==============Beginning of hint content=============-->
	<td id="statusContainer" width="300" align="left" valign="top" >
		<div id="statusIcon"></div>
	  <table width="95%" border="0" cellpadding="0" cellspacing="0">
		<tr>
		  <td class="statusTitle" valign="center">
		  	<!--img id="statusIcon" src="images/iframe-iconRouter.gif"/-->
		  	<!--span id="helpname"><#statusTitle_System#></span-->
		  	<div id="helpname"></div>
		  </td>
		</tr>
		<tr>
		  <td>
			<iframe id="statusframe" name="statusframe" src="/device-map/router.asp" frameborder="0" width="100%" height="400"></iframe>
<script>
if(flag == "Internet" || flag == "Client")
	$("statusframe").src = "";
</script>
		  </td>
		</tr>
	  </table>
	</td>
	<!--==============Ending of hint content=============-->
  </tr>
</table>

<div id="footer"></div>
</body>
</html>
