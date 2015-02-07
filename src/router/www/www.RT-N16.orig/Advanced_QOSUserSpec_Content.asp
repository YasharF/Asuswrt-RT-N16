<!--[if lte IE 7]><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd"><![endif]-->
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>
<head>
<meta http-equiv="X-UA-Compatible" content="IE=EmulateIE7"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<title>ASUS Wireless Router <#Web_Title#> - <#menu5_3_2#></title>
<link rel="stylesheet" type="text/css" href="/index_style.css"> 
<link rel="stylesheet" type="text/css" href="/form_style.css">

<script type="text/javascript" src="/state.js"></script>
<script type="text/javascript" src="/help.js"></script>
<script type="text/javascript" src="/general.js"></script>
<script type="text/javascript" src="/popup.js"></script>
<script type="text/javascript" src="/slider/slider.js"></script>
<script type="text/javascript" src="/slider/libcrossbrowser.js"></script>
<script type="text/javascript" src="/slider/eventhandler.js"></script>
<script type="text/javascript" src="/detect.js"></script>
<script>
wan_route_x = '<% nvram_get_x("IPConnection", "wan_route_x"); %>';
wan_nat_x = '<% nvram_get_x("IPConnection", "wan_nat_x"); %>';
wan_proto = '<% nvram_get_x("Layer3Forwarding",  "wan_proto"); %>';

<% login_state_hook(); %>
var wireless = [<% wl_auth_list(); %>];	// [[MAC, associated, authorized], ...]

var NAME_WIDTH = 13;
var IP_WIDTH = 16;
var PORT_WIDTH = 12;

var qos_ubw = '<% nvram_get_x("PrinterStatus", "qos_ubw"); %>';
var qos_service_enable_x = '<% nvram_get_x("PrinterStatus", "qos_service_enable"); %>';
var qos_service_ubw_x = '<% nvram_get_x("PrinterStatus", "qos_service_ubw"); %>';
var qos_dfragment_enable_x = '<% nvram_get_x("PrinterStatus", "qos_dfragment_enable"); %>';
var qos_dfragment_size_x = '<% nvram_get_x("PrinterStatus", "qos_dfragment_size"); %>';

var x_USRRuleList = [<% get_nvram_list("PrinterStatus", "x_USRRuleList"); %>];

function initial(){
	show_banner(1);
	show_menu(5,3,2);
	show_footer();
	
	frmload();
	drawSliderBlock();
	
	load_body();
	
	enable_auto_hint(20, 2);
	showBMUserSpec();
}

function Fragment_display()
{
	if(document.form.qos_dfragment_enable_w.checked==true)
	{
		document.form.qos_dfragment_enable.value = 1;
		blocking('Fragment' ,true);
	}
	else
	{
		document.form.qos_dfragment_enable.value = 0;
		blocking('Fragment' ,false);
	}
}

function getObj(name)
{
	if (document.getElementById)
	{
		this.obj = document.getElementById(name);
		this.style = document.getElementById(name).style;
	}
	else if (document.all)
	{
		this.obj = document.all[name];
		this.style = document.all[name].style;
	}
	else if (document.layers)
	{
		this.obj = document.layers[name];
		this.style = document.layers[name];
	}
}

function drawSliderBlock(){
	x = new getObj('sliderDiv_1');
	if ((document.all) || (document.getElementById)){
		x.obj.innerHTML = slider_table(1);
	}
	else{
		x.obj.document.write(slider_table(1));
		x.obj.document.close();
	}
	
	y = new getObj('sliderDiv_2');
	if ((document.all) || (document.getElementById))
		y.obj.innerHTML = slider_table(2);
	else{
		y.obj.document.write(slider_table(2));
		y.obj.document.close();
	}
	initSlider();
}

function slider_table(disk_num)
{
	var result;
	var assignment_box;
	var num_disks_in_pane;
	var disk_array = ["A","B"];
	var space_array = ["100","100"];
	
	syncDisksInPool = true;
	maxSpaceAllowedInSync = 0.0;
	
	var isJbodPool = 1;
	
	result = "<table border=0><tr><td>";
	result = "<div id='sliderDiv" + disk_num + "'></div>\n";
	result += "</td></tr></table>";
	return result;
}

function initSlider(){
	var disk_num;
	sliderArray1 = new Array();
	sliderArray2 = new Array();
	
	Bs_Objects = new Array();
	var colorArray = new Array("red", "red", "orange", "green", "magenta", "yellow", "turquoise");
	
	disk_num = 1;
	sliderArray1[disk_num.toString(10)] = drawSlider(disk_num.toString(10), "sliderDiv"+disk_num, "red", 0, 100, document.form.qos_service_ubw.value, 1, false);
	disk_num = 2;
	sliderArray2[disk_num.toString(10)] = drawSlider(disk_num.toString(10), "sliderDiv"+disk_num, "red", 0, 100, document.form.qos_dfragment_size.value, 1, false);
	
	return;
}

function drawSlider(sliderName,divName,color,minVal,maxVal,initVal,interval,isDisabled){
	var max_slider_width = 2.92;

	if (arguments[0] != "")
		mySlider = new Bs_Slider(sliderName);
	else
		mySlider = new Bs_Slider();

	if (divName == "sliderDiv1")
		mySlider.attachOnChange(bsSliderChange1);
	else
		mySlider.attachOnChange(bsSliderChange2);

	mySlider.width         =  ( (maxVal / 2) * max_slider_width );
	mySlider.height        = 18;
	mySlider.minVal        = minVal;
	mySlider.maxVal        = maxVal;
	mySlider.valueInterval = interval;
	mySlider.arrowAmount   = interval;
	mySlider.valueDefault  = initVal;
	mySlider.imgDir   = 'slider/';
	mySlider.setBackgroundImage('background.gif', 'repeat');
	mySlider.setBackgroundImageLeft('background_left.gif', 7, 18);
	mySlider.setBackgroundImageRight('background_right.gif', 7, 18);
	mySlider.setSliderIcon('slider.gif', 13, 18);
	mySlider.setArrowIconLeft('arrowLeft.gif', 16, 16);
	mySlider.setArrowIconRight('arrowRight.gif', 16, 16);
	mySlider.useInputField = 0;
	mySlider.colorbar = new Object();
	mySlider.colorbar['color']           = color;
	mySlider.colorbar['height']          = 7;
	mySlider.colorbar['widthDifference'] = -14;
	mySlider.colorbar['offsetLeft']      = -3;
	mySlider.colorbar['offsetTop']       = 9;
	mySlider.setDisabled(isDisabled);
	mySlider.drawInto(divName);
	return mySlider;
}

function bsSliderChange1(sliderObj, val, newPos){
	document.form.qos_service_ubw.value=sliderObj.getValue();
}

function bsSliderChange2(sliderObj, val, newPos){
	document.form.qos_dfragment_size.value=sliderObj.getValue();
}

function frmload()
{
	document.form.qos_service_ubw.value=qos_service_ubw_x;
	document.form.qos_dfragment_size.value=qos_dfragment_size_x;
	if(qos_dfragment_enable_x=='1')
		document.form.qos_dfragment_enable_w.checked = true;
	
	Fragment_display();
}

function applyRule(){
	showLoading();
	
	document.form.action_mode.value = " Restart ";
	document.form.current_page.value = "/as.asp";
	document.form.next_page.value = "";

	document.form.submit();
}

function done_validating(action){
	refreshpage();
}

function validate_duplicate_USRRuleList(ip_value, port_value){	// 2008.01 James.
	for(var i = 0; i < x_USRRuleList.length; ++i){
		if(x_USRRuleList[i][1] == ip_value && x_USRRuleList[i][2] == port_value){
			alert('<#JS_duplicate#>');
			
			return false;
		}
	}
	
	return true;
}

function showBMUserSpec(){
	var code = "";
	
	code +='<table width="100%" border="1" cellspacing="0" cellpadding="3" align="center" class="list_table">';
	if(x_USRRuleList.length == 0)
		code +='<tr><td style="color:#CC0000;"><#IPConnection_VSList_Norule#></td></tr>';
	else{
		for(var i = 0; i < x_USRRuleList.length; i++){
			if(x_USRRuleList[i][3] == "1")
				x_USRRuleList[i][3] = "<#Priority_Level_1#>";
			else if(x_USRRuleList[i][3] == "6")
				x_USRRuleList[i][3] = "<#Priority_Level_3#>";
			else
				x_USRRuleList[i][3] = "<#Priority_Level_2#>";
			
			code +='<tr id="row'+i+'">';
			code +='<td width="96">'+ x_USRRuleList[i][0] +'</td>';		//Service name
			code +='<td width="94">'+ x_USRRuleList[i][1] +'</td>';		//IP
			code +='<td width="76">'+ x_USRRuleList[i][2] +'&nbsp;</td>';	//port
			code +='<td width="34">'+ x_USRRuleList[i][3] +'</td>';		//Priority
			code +='<td width=\"20\"><input type=\"checkbox\" name=\"x_USRRuleList_s\" value='+ i +' onClick="changeBgColor(this,'+i+');" id=\"check'+ i +'\"></td>';
			
			if(i == 0)
				code +="<td width='58' style='background:#C0DAE4;' rowspan=" + x_USRRuleList.length + "><input style=\"padding:2px 2px 0px 2px\" class=\"button\" type=\"submit\" onclick=\"markGroup(this, 'x_USRRuleList', 8, ' Del ')\" name=\"x_USRRuleList\" value=\"<#CTL_del#>\"/></td>";
			
	    code +='</tr>';
		}
	}
  code +='</table>';
	
	$("BMUSerSpec_Block").innerHTML = code;
}

function changeBgColor(obj, num){
	if(obj.checked)
 		$("row" + num).style.background='#FF9';
	else
 		$("row" + num).style.background='#FFF';
}
</script>
</head>

<body onLoad="initial();" onunLoad="return unload_body();">
<div id="TopBanner"></div>

<div id="Loading" class="popup_bg"></div>

<iframe name="hidden_frame" id="hidden_frame" src="" width="0" height="0" frameborder="0"></iframe>

<form method="post" name="form" id="ruleForm" action="/start_apply.htm" target="hidden_frame">
<input type="hidden" name="current_page" value="Advanced_QOSUserSpec_Content.asp">
<input type="hidden" name="next_page" value="">
<input type="hidden" name="next_host" value="">
<input type="hidden" name="sid_list" value="PrinterStatus;">
<input type="hidden" name="group_id" value="x_USRRuleList">
<input type="hidden" name="modified" value="0">
<input type="hidden" name="action_mode" value="">
<input type="hidden" name="first_time" value="">
<input type="hidden" name="action_script" value="">
<input type="hidden" name="qos_rulenum_x_0" value="<% nvram_get_x("PrinterStatus", "qos_rulenum_x"); %>" readonly="1">
<input type="hidden" name="qos_rule_w" value="">
<input type="hidden" name="qos_dfragment_enable" value="">
<input type="hidden" name="preferred_lang" id="preferred_lang" value="<% nvram_get_x("LANGUAGE", "preferred_lang"); %>">
<input type="hidden" name="ssid_acsii" value="<% nvram_char_to_ascii("WLANConfig11b",  "wl_ssid"); %>">
<input type="hidden" name="firmver" value="<% nvram_get_x("",  "firmver"); %>">

<input type="hidden" name="qos_dfragment_size" value="">

<table class="content" align="center" cellpadding="0" cellspacing="0">
	<tr>
		<td width="23">&nbsp;</td>		
		<td valign="top" width="202">				
			<div id="mainMenu"></div>	
			<div id="subMenu"></div>		
		</td>				
		
		<td valign="top">
			<div id="tabMenu" class="submenuBlock"></div><br />
			<!--===================================Beginning of Main Content===========================================-->
<table width="98%" border="0" align="center" cellpadding="0" cellspacing="0">
	<tr>
		<td valign="top" >
			<table width="500" border="0" align="center" cellpadding="4" cellspacing="0" class="FormTitle" table>
				<thead>
				<tr>
					<td><#BM_title_User#></td>
				</tr>
				</thead>
				
				<tbody>
				<tr>
					<td bgcolor="#FFFFFF"><#BM_user_desc#></td>
				</tr>
				</tbody>
				
				<tr>
					<td bgcolor="#FFFFFF">
						<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
							<thead>
							<tr>
								<td colspan="2"><#BM_status#></td>
							</tr>
							</thead>
							
							<tr>
								<th width="40%"><a class="hintstyle" href="javascript:void(0);" onClick="openHint(20, 1);"><#BM_measured_uplink_speed#></a></th>
								<td width="60%"><% nvram_get_x("PrinterStatus", "qos_ubw"); %> Kb/s</td>
							</tr>
							
							<tr>
								<th width="40%"><a class="hintstyle" href="javascript:void(0);" onClick="openHint(20, 2);"><#BM_manual_uplink_speed#></a></th>
								<td width="60%"><input type="text" name="qos_manual_ubw" onKeyPress="return is_number_sp(event, this);" class="input" size="8" value="<% nvram_get_x("PrinterStatus", "qos_manual_ubw"); %>"> Kb/s</td>
							</tr>
						</table>
					</td>
				</tr>
				
				<tr>
					<td bgcolor="#FFFFFF">		
						<table width="100%"  border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
							<thead>
							<tr>
								<td colspan="5" id="TriggerList"><#BM_UserList_title#></td>
							</tr>
							</thead>
							
							<tr>
								<th style="text-align:center;"><#BM_UserList1#></th>
								<th style="text-align:center;"><a class="hintstyle" href="javascript:void(0);" onClick="openHint(18,3);"><#BM_UserList2#></a></th>
								<th style="text-align:center;"><a class="hintstyle" href="javascript:void(0);" onClick="openHint(18,4);"><#BM_UserList3#></a></th>
								<th style="text-align:center;"><#BM_UserList4#></th>
								<th></th>
							</tr>
							
							<tr>
								<td><input type="text" maxlength="15" size="14" name="qos_service_name_x_0" class="input" onKeyPress="return is_string(this)"></td>
								<td><input type="text" maxlength="15" size="14" name="qos_ip_x_0" class="input" onKeyPress="return is_iprange(this)" onKeyUp="change_iprange(this)"></td>
								<td><input type="text" maxlength="11" size="10" name="qos_port_x_0" class="input" onKeyPress="return is_portrange(this)"></td>
								<td width="70">
									<select name='qos_prio_x_0' class="input">
										<option value='1'><#Priority_Level_1#></option>
										<option value='4' selected="selected"><#Priority_Level_2#></option>
										<option value='6'><#Priority_Level_3#></option>
									</select>
								</td>
								<td valign="bottom" bgcolor="#FFFFFF" width="60">
									<input type="submit" name="x_USRRuleList2" class="button" onClick="return markGroup(this, 'x_USRRuleList', 8, ' Add ');" value="<#CTL_add#>" size="12" />
								</td>
							</tr>
							</table>
							  <div id="BMUSerSpec_Block"></div>
							<table width="100%"  border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">							
							<tr bgcolor="#FFFFFF">
								<td colspan="5"><input type="checkbox" name="qos_dfragment_enable_w" onclick='Fragment_display();' style="vertical-align:middle; "><#BM_pkt_flg#></td>
							</tr>
							<tr bgcolor="#FFFFFF">
								<td colspan="5">
									<div id='FTPServer' style='display:none'>
										<#BM_FTP_desc#>
										<table>
											<tr>
												<td width='240'></td>
												<td width='50'><b><#BM_Low#></b></td>
												<td width='120'></td>
												<td width='55'><b><#BM_High#></b></td>
											</tr>
										</table>
										
										<table border='0'>
											<tr>
												<td width='240'><#BM_rbw#><input type="text" name="qos_service_ubw" size="1" length="1" align="right">%</td>
												<td width='220' valign='top'>
													<div id="sliderDiv_1">
														<script language="javascript"> 
															<!-- document.writeln(slider_table(1)); -->
														</script>
													</div>
												</td>
											</tr>
										</table>
									</div>
									
									<div id='Fragment' style='display:block'>
										<table width="100%" border="0" cellpadding="2" cellspacing="2" style="font-size:12px; ">
											<tr>
												<td width="20%" valign='top'></td>
												<td width="175"><span style="font-size:10px; font-weight:bold;"><#BM_small#></span></td>
												<td width="200"><span style="font-size:10px; font-weight:bold;"><#BM_large#></span></td>
											</tr>
											
											<tr>
												<td width="20%" align="right" valign="bottom">			  
													<#BM_pkt_size#>
												</td>
												<td colspan="2" valign="middle">
													<div id="sliderDiv_2" style=" margin-bottom:14px;">
														<script language="javascript">
															<!-- document.writeln(slider_table(2)); -->
														</script>
													</div>
												</td>
											</tr>
											
											<tr>
												<td width="20%" align="right" valign="top">
													<#BM_note#>
												</td>
												<td colspan="2">
													<ol>
														<li><#BM_note1#></li>
														<li><#BM_note2#></li>
														<li><#BM_note3#></li>
													</ol>
												</td>
											</tr>
										</table>
									</div>
								</td>
							</tr>		  
							
							<tr>
								<td align="right"  colspan="5">
									<input name="button" type="button" class="button" onClick="applyRule()" value="<#CTL_apply#>"/>
								</td>
							</tr>
						</table>
					</td>
				</tr>
			</table>
		</td>
</form>

		<td id="help_td" style="width:15px;" valign="top">
			  <div id="helpicon" onClick="openHint(0,0);" title="<#Help_button_default_hint#>"><img src="images/help.gif" /></div>
	  <div id="hintofPM" style="display:none;">
<form name="hint_form"></form>
	    <table width="100%" cellpadding="0" cellspacing="1" class="Help" bgcolor="#999999">
		  <thead>
		  <tr>
			<td>
			  <div id="helpname" class="AiHintTitle"></div>
			  <a href="javascript:;" onclick="closeHint()" ><img src="images/button-close.gif" class="closebutton" /></a>
			</td>
		  </tr>
		  </thead>
		  
		  <tr>				
			<td valign="top" >
			  <div class="hint_body2" id="hint_body"></div>
			  <iframe id="statusframe" name="statusframe" class="statusframe" src="" frameborder="0"></iframe>
			</td>
		  </tr>
		</table>
	  </div><!--End of hintofPM--> 
		 </td>
      </tr>
      </table>				
		<!--===================================Ending of Main Content===========================================-->		
	</td>
		
    <td width="10" align="center" valign="top">&nbsp;</td>
	</tr>
</table>

<div id="footer"></div>

</body>
</html>
