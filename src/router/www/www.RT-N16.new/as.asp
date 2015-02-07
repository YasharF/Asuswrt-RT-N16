<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>
<head>
<meta http-equiv="X-UA-Compatible" content="IE=EmulateIE7"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<title>ASUS Wireless Router <#Web_Title#> - <#menu5_title#></title>
<link rel="stylesheet" type="text/css" href="index_style.css"> 
<link rel="stylesheet" type="text/css" href="form_style.css">

<script type="text/javascript" src="state.js"></script>
<script type="text/javascript" src="popup.js"></script>
<script type="text/javascript" src="help.js"></script>
<script>
wan_route_x = '<% nvram_get_x("IPConnection", "wan_route_x"); %>';
wan_nat_x = '<% nvram_get_x("IPConnection", "wan_nat_x"); %>';
wan_proto = '<% nvram_get_x("Layer3Forwarding",  "wan_proto"); %>';

var map_code = "";

function initial(){
	show_banner(0);
	show_menu(5,0,0);
	show_footer();
	$("menu_body").style.visibility = "visible";
	show_map();
}

function show_map(){
	for(var j = 0; j < tabtitle.length; ++j){
		if(tabtitle[j].length <= 0)
				continue;
		
		map_code = '<ul>\n';
		
		for(var i = 1; i < tabtitle[j].length; ++i){
			if(tablink[j][i] == "")
				continue;
			
			map_code += '    <li>\n';
			map_code += '        <a href="'+tablink[j][i]+'">'
			map_code += tabtitle[j][i];
			map_code += '</a>\n    </li>\n';
		}
		
		map_code += '</ul>\n';
		$("map"+j).innerHTML = map_code;
	}
	
	if(wan_route_x == "IP_Bridged")
		hide_no_use_block();
	
	$("menu_body").style.visibility = "visible";
}

function hide_no_use_block(){
	$("AP_hide3_1").style.display = "none";
	$("AP_hide3_2").style.display = "none";
	$("AP_hide3_3").style.display = "none";
	$("AP_hide_full1_1").style.display = "";
	$("AP_hide_full1_2").style.display = "";
	$("AP_hide_full1_3").style.display = "";
	
	$("AP_hide5_1").style.display = "none";
	$("AP_hide5_2").style.display = "none";
	$("AP_hide5_3").style.display = "none";
	$("AP_hide_full2_1").style.display = "";
	$("AP_hide_full2_2").style.display = "";
	$("AP_hide_full2_3").style.display = "";
}
</script>
</head>

<body onload="initial();" onunload="return unload_body();">
<div id="TopBanner"></div>

<div id="Loading" class="popup_bg"></div>

<iframe name="hidden_frame" id="hidden_frame" width="0" height="0" frameborder="0" scrolling="no"></iframe>

<form name="form">
<input type="hidden" name="preferred_lang" id="preferred_lang" value="<% nvram_get_x("LANGUAGE", "preferred_lang"); %>">
<input type="hidden" name="ssid_acsii" value="<% nvram_char_to_ascii("WLANConfig11b",  "wl_ssid"); %>">
<input type="hidden" name="firmver" value="<% nvram_get_x("",  "firmver"); %>">
</form>

<table width="983" border="0" align="center" cellpadding="0" cellspacing="0" background="images/body-bg1.gif">
	<tr>
		<td width="23">&nbsp;</td>
		
		<td valign="top" width="202">
			<div id="mainMenu"></div>	
			<div id="subMenu"></div>
		</td>				
		<div id="tabMenu"></div>
		<!--=====Beginning of Main Content=====-->
		<td height="400" valign="top">
			<table id="menu_body" style="visibility:hidden;" width="98%" border="0" align="center" cellpadding="3" cellspacing="1" bgcolor="#CCCCCC" class="sitemap">
				<tr class="head">
					<td width="25%"><#menu5_1#></td>
					<td width="25%"><#menu5_2#></td>
					<td id="AP_hide3_1" width="25%"><#menu5_3#></td>
					<td width="25%"><#menu5_4#></td>
					<td id="AP_hide_full1_1" width="25%" bgcolor="#E6EDF0" style="display:none;">&nbsp;</td>
				</tr>
				
				<tr>
					<td class="desp"><#menu5_1_desc#></td>
					<td class="desp"><#menu5_2_desc#></td>
					<td id="AP_hide3_2" class="desp"><#menu5_3_desc#></td>
					<td class="desp"><#menu5_4_desc#></td>
					<td id="AP_hide_full1_2" width="25%" style="display:none;">&nbsp;</td>
				</tr>
				
				<tr valign="top">
					<td height="120">
						<div id="map0"></div></td>
					<td height="120">
						<div id="map1"></div></td>
					<td id="AP_hide3_3" height="120">
						<div id="map2"></div></td>
					<td height="120">
						<div id="map3"></div></td>
					<td id="AP_hide_full1_3" height="120" style="display:none;">&nbsp;</td>
				</tr>
				
				<tr class="head">
					<td id="AP_hide5_1"><#menu5_5#></td>
					<td><#menu5_6#></td>
					<td><#menu5_7#></td>
					<td bgcolor="#E6EDF0">&nbsp;</td>
					<td id="AP_hide_full2_1" bgcolor="#E6EDF0" style="display:none;">&nbsp;</td>
				</tr>
				
				<tr>
					<td id="AP_hide5_2" width="25%" class="desp"><#menu5_5_desc#></td>
					<td width="25%" class="desp"><#menu5_6_desc#></td>
					<td width="25%" class="desp"><#menu5_7_desc#></td>
					<td width="25%">&nbsp;</td>
					<td id="AP_hide_full2_2" width="25%" bgcolor="#E6EDF0" style="display:none;">&nbsp;</td>
				</tr>
				
				<tr valign="top">
					<td id="AP_hide5_3" height="120">
						<div id="map4"></div></td>
					<td height="120">
						<div id="map5"></div></td>
					<td height="120">
						<div id="map6"></div></td>
					<td height="120">&nbsp;</td>
					<td id="AP_hide_full2_3" height="120" style="display:none;">&nbsp;</td>
				</tr>
			</table>
		</td>
		<td id="help_td" style="width:15px;" align="center" valign="top">
		<form name="hint_form"></form>
		<div id="helpicon"></div>
		<div id="hintofPM" style="display:none;">
			<table width="100%" cellpadding="0" cellspacing="1" class="Help" bgcolor="#999999">
				<thead>
				<tr>
					<td>
						<div id="helpname" class="AiHintTitle"></div>
					</td>
				</tr>
				</thead>				
				<tr>
					<td valign="top">
						<div class="hint_body2" id="hint_body"></div>
						<iframe id="statusframe" name="statusframe" class="statusframe" src="" frameborder="0"></iframe>						
					</td>
				</tr>
			</table>
		</div>		
		</td>
		<td width="20" align="center">&nbsp</td>
	</tr>
</table>

<div id="footer"></div>

</body>
</html>
