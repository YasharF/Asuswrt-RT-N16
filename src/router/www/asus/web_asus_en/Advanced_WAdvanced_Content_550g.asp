<head>
<title>ZVMODELVZ Web Manager</title>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<link rel="stylesheet" type="text/css" href="style.css" media="screen"></link>
<input type="hidden" name="preferred_lang" value="<% nvram_get_x("","preferred_lang"); %>">
<script language="JavaScript" type="text/javascript" src="overlib.js"></script>
<script language="JavaScript" type="text/javascript" src="general.js"></script>
<script type="text/javascript" src="slider/slider.js"></script>
<script type="text/javascript" src="slider/libcrossbrowser.js"></script>
<script type="text/javascript" src="slider/eventhandler.js"></script>
<script language="JavaScript" type="text/javascript">
<!--

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
	
	x = new getObj('sliderDiv');
	if ((document.all) || (document.getElementById)){
		x.obj.innerHTML = slider_table();
	}
	else{
		x.obj.document.write(slider_table());
		x.obj.document.close();
	}

	initSlider();
}

function loadSlider()
{	
	changePower(0);
	drawSliderBlock();
}


function slider_table()
{
	var result;
	var assignment_box;
	var disk_num=1;
	var num_disks_in_pane;
	var disk_array = ["A","B"];
	var space_array = ["100","100"];
	syncDisksInPool = true;
	maxSpaceAllowedInSync = 0.0;
	var isJbodPool = 1;
	
	result = "<table border=0 cellspacing=0 cellpadding=0><tr><td>";
	
	result += "<div id='sliderDiv" + disk_num + "'></div>\n";
		// the slider movement and gigabytes textbox value are kept in sync only if there is one disk per pane and it is a mirrored or striped pool

	//alert(stripe_count());
	//alert(mirror_count());
	result += "</td></tr></table>";
	return result;

}


 var max_slider_width = 18; // change it if needed

 function initSlider(){
 
 	var disk_num;
 	
 	sliderArray = new Array();
 	Bs_Objects = new Array(); // Have to initialize the internal array used by slider class as well
 	
 	var colorArray = new Array("red", "red","orange","green","magenta","yellow","turquoise");


	disk_num=1;
	
	if (document.forms[0].regulation_domain.value == "30DE")
 		sliderArray[disk_num.toString(10)] = drawSlider(disk_num.toString(10),"sliderDiv" + disk_num, "red",1, 9, document.forms[0].wl_radio_power_x.value, 1, false); 
 	else sliderArray[disk_num.toString(10)] = drawSlider(disk_num.toString(10),"sliderDiv" + disk_num, "red",1, 12, document.forms[0].wl_radio_power_x.value, 1, false); 
	return;
 
 }
 
 function drawSlider(sliderName,divName,color,minVal,maxVal,initVal,interval,isDisabled){
   
   if (arguments[0] != "")
   	mySlider = new Bs_Slider(sliderName);
   else
   	mySlider = new Bs_Slider();
   	
   mySlider.attachOnChange(bsSliderChange);
   mySlider.width         =  ( (maxVal / 2) * max_slider_width );
   //mySlider.width       = 121;
   mySlider.height        = 18;
   mySlider.minVal        = minVal;
   mySlider.maxVal        = maxVal;
   mySlider.valueInterval = interval;
   mySlider.arrowAmount   = interval;
   mySlider.valueDefault  = initVal;
   mySlider.imgDir   = 'slider/';
   //mySlider.setBackgroundImage('background.gif', 'no-repeat');
   mySlider.setBackgroundImage('background.gif', 'repeat');
   mySlider.setBackgroundImageLeft('background_left.gif', 7, 18);
   mySlider.setBackgroundImageRight('background_right.gif', 7, 18);
   mySlider.setSliderIcon('slider.gif', 13, 18);
   mySlider.setArrowIconLeft('arrowLeft.gif', 16, 16);
   mySlider.setArrowIconRight('arrowRight.gif', 16, 16);
   mySlider.useInputField = 0;
   //mySlider.styleValueFieldClass = 'sliderInput';
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
 
 function changePower(flag)
 {
 	var bpower, gpower, power_index;
 	
 	power_index=document.forms[0].wl_radio_power_x.value;
      	
      	if (power_index==12)
      	{	
      		bpower=24;
   		gpower=20;      	
      	}
      	else if(power_index==11)
      	{	
      		bpower=22;
   		gpower=20;
      	}
      	else
      	{
   		bpower=eval(power_index)+10;
   		gpower=eval(power_index)+10;
   	}	
   		 
   	document.forms[0].power_index_1.value = "B: " + bpower + " dbm / G: " + gpower + " dbm";
   	if (flag)
   	{	
   		window.top.pageChanged = 1;	
   	}	
 }
 
function bsSliderChange(sliderObj, val, newPos){       
   // if mirrored or striped and one disk per pane, make sure all disk sizes are made the same
   // update slider value and hidden field value here for the current field alone
   document.forms[0].wl_radio_power_x.value=sliderObj.getValue();
   changePower(1);   
}


//-->
</script>
</head>  
<div id="overDiv" style="position:absolute; visibility:hidden; z-index:1000;"></div>    
<body onLoad="load_body();loadSlider();" onunLoad="return unload_body();">
<form method="GET" name="form" action="apply.cgi">
<!-- Table for the conntent page -->	    
<table width="666" border="0" cellpadding="0" cellspacing="0">     	      
    	
<input type="hidden" name="current_page" value="Advanced_WAdvanced_Content.asp"><input type="hidden" name="next_page" value="SaveRestart.asp"><input type="hidden" name="next_host" value=""><input type="hidden" name="sid_list" value="WLANAuthentication11a;WLANConfig11b;"><input type="hidden" name="group_id" value=""><input type="hidden" name="modified" value="0"><input type="hidden" name="action_mode" value=""><input type="hidden" name="first_time" value=""><input type="hidden" name="action_script" value="">
<tr>
<td>
<table width="666" border="1" cellpadding="0" cellspacing="0" bordercolor="E0E0E0">
<tr class="content_header_tr">
<td class="content_header_td_title" colspan="2"><#t1Wireless#> - <#t2Advanced#></td>
</tr>
<input type="hidden" name="wl_gmode" value="<% nvram_get_x("WLANConfig11b","wl_gmode"); %>"><input type="hidden" name="wl_gmode_protection_x" value="<% nvram_get_x("WLANConfig11b","wl_gmode_protection_x"); %>">
<tr>
<td class="content_desc_td" colspan="2"><#WLANConfig11b_display5_sectiondesc#>
         </td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_x_RadioPowerEx_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_x_RadioPower_itemname#>
</td>


<td class="content_input_td">        
    <div id="sliderDiv" >

			<!--	<script>document.write(slider_table());</script> -->
    </div>
    <div id="sliderVal" align="right"><input type="text"  size="20" style="font-size:12px;" name="power_index_1" readonly/></div>
    <input type="hidden" maxlength="5" size="5" name="wl_radio_power_x" class="content_input_fd" value="<% nvram_get_x("WLANConfig11b", "wl_radio_power_x"); %>"></td>
    <input type="hidden" maxlength="5" size="5" name="regulation_domain" class="content_input_fd" value="<% nvram_get_x("WLANConfig11b", "regulation_domain"); %>"></td>
</td>    	  
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_x_AfterBurner_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_x_AfterBurner_itemname#>
           </td><td class="content_input_td"><select name="wl_afterburner" class="content_input_fd" onChange="return change_common(this, 'WLANConfig11b', 'wl_afterburner')"><option class="content_input_fd" value="off" <% nvram_match_x("WLANConfig11b","wl_afterburner", "off","selected"); %>>Disabled</option><option class="content_input_fd" value="auto" <% nvram_match_x("WLANConfig11b","wl_afterburner", "auto","selected"); %>>Enabled</option></select></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_x_BlockBCSSID_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_x_BlockBCSSID_itemname#>
           </td><td class="content_input_td"><input type="radio" value="1" name="wl_closed" class="content_input_fd" onClick="return change_common_radio(this, 'WLANConfig11b', 'wl_closed', '1')" <% nvram_match_x("WLANConfig11b","wl_closed", "1", "checked"); %>>Yes</input><input type="radio" value="0" name="wl_closed" class="content_input_fd" onClick="return change_common_radio(this, 'WLANConfig11b', 'wl_closed', '0')" <% nvram_match_x("WLANConfig11b","wl_closed", "0", "checked"); %>>No</input></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_x_IsolateAP_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_x_IsolateAP_itemname#>
           </td><td class="content_input_td"><input type="radio" value="1" name="wl_ap_isolate" class="content_input_fd" onClick="return change_common_radio(this, 'WLANConfig11b', 'wl_ap_isolate', '1')" <% nvram_match_x("WLANConfig11b","wl_ap_isolate", "1", "checked"); %>>Yes</input><input type="radio" value="0" name="wl_ap_isolate" class="content_input_fd" onClick="return change_common_radio(this, 'WLANConfig11b', 'wl_ap_isolate', '0')" <% nvram_match_x("WLANConfig11b","wl_ap_isolate", "0", "checked"); %>>No</input></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_DataRateAll_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_DataRateAll_itemname#>
           </td><td class="content_input_td"><select name="wl_rate" class="content_input_fd" onChange="return change_common(this, 'WLANConfig11b', 'wl_rate')"><option class="content_input_fd" value="0" <% nvram_match_x("WLANConfig11b","wl_rate", "0","selected"); %>>Auto</option><option class="content_input_fd" value="1000000" <% nvram_match_x("WLANConfig11b","wl_rate", "1000000","selected"); %>>1</option><option class="content_input_fd" value="2000000" <% nvram_match_x("WLANConfig11b","wl_rate", "2000000","selected"); %>>2</option><option class="content_input_fd" value="5500000" <% nvram_match_x("WLANConfig11b","wl_rate", "5500000","selected"); %>>5.5</option><option class="content_input_fd" value="6000000" <% nvram_match_x("WLANConfig11b","wl_rate", "6000000","selected"); %>>6</option><option class="content_input_fd" value="9000000" <% nvram_match_x("WLANConfig11b","wl_rate", "9000000","selected"); %>>9</option><option class="content_input_fd" value="11000000" <% nvram_match_x("WLANConfig11b","wl_rate", "11000000","selected"); %>>11</option><option class="content_input_fd" value="12000000" <% nvram_match_x("WLANConfig11b","wl_rate", "12000000","selected"); %>>12</option><option class="content_input_fd" value="18000000" <% nvram_match_x("WLANConfig11b","wl_rate", "18000000","selected"); %>>18</option><option class="content_input_fd" value="24000000" <% nvram_match_x("WLANConfig11b","wl_rate", "24000000","selected"); %>>24</option><option class="content_input_fd" value="36000000" <% nvram_match_x("WLANConfig11b","wl_rate", "36000000","selected"); %>>36</option><option class="content_input_fd" value="48000000" <% nvram_match_x("WLANConfig11b","wl_rate", "48000000","selected"); %>>48</option><option class="content_input_fd" value="54000000" <% nvram_match_x("WLANConfig11b","wl_rate", "54000000","selected"); %>>54</option></select></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_DataRate_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_DataRate_itemname#>
           </td><td class="content_input_td"><select name="wl_rateset" class="content_input_fd" onChange="return change_common(this, 'WLANConfig11b', 'wl_rateset')"><option class="content_input_fd" value="default" <% nvram_match_x("WLANConfig11b","wl_rateset", "default","selected"); %>>Default</option><option class="content_input_fd" value="all" <% nvram_match_x("WLANConfig11b","wl_rateset", "all","selected"); %>>All</option><option class="content_input_fd" value="12" <% nvram_match_x("WLANConfig11b","wl_rateset", "12","selected"); %>>1, 2 Mbps</option></select></td>
</tr>
<tr>
<td class="content_header_td"><#WLANConfig11b_x_Frag_itemname#>
           </td><td class="content_input_td"><input type="text" maxlength="5" size="5" name="wl_frag" class="content_input_fd" value="<% nvram_get_x("WLANConfig11b", "wl_frag"); %>" onBlur="validate_range(this, 256, 2346)" onKeyPress="return is_number(this)"></td>
</tr>
<tr>
<td class="content_header_td"><#WLANConfig11b_x_RTS_itemname#>
           </td><td class="content_input_td"><input type="text" maxlength="5" size="5" name="wl_rts" class="content_input_fd" value="<% nvram_get_x("WLANConfig11b", "wl_rts"); %>" onBlur="validate_range(this, 0, 2347)" onKeyPress="return is_number(this)"></td>
</tr>
<tr>
<td class="content_header_td"><#WLANConfig11b_x_DTIM_itemname#>
           </td><td class="content_input_td"><input type="text" maxlength="5" size="5" name="wl_dtim" class="content_input_fd" value="<% nvram_get_x("WLANConfig11b", "wl_dtim"); %>" onBlur="validate_range(this, 1, 255)" onKeyPress="return is_number(this)"></td>
</tr>
<tr>
<td class="content_header_td"><#WLANConfig11b_x_Beacon_itemname#>
           </td><td class="content_input_td"><input type="text" maxlength="5" size="5" name="wl_bcn" class="content_input_fd" value="<% nvram_get_x("WLANConfig11b", "wl_bcn"); %>" onBlur="validate_range(this, 1, 65535)" onKeyPress="return is_number(this)"></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_x_FrameBurst_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_x_FrameBurst_itemname#>
           </td><td class="content_input_td"><select name="wl_frameburst" class="content_input_fd" onChange="return change_common(this, 'WLANConfig11b', 'wl_frameburst')"><option class="content_input_fd" value="off" <% nvram_match_x("WLANConfig11b","wl_frameburst", "off","selected"); %>>Disabled</option><option class="content_input_fd" value="on" <% nvram_match_x("WLANConfig11b","wl_frameburst", "on","selected"); %>>Enabled</option></select></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_x_RadioEnable_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_x_RadioEnable_itemname#>
           </td><td class="content_input_td"><input type="radio" value="1" name="wl_radio_x" class="content_input_fd" onClick="return change_common_radio(this, 'WLANConfig11b', 'wl_radio_x', '1')" <% nvram_match_x("WLANConfig11b","wl_radio_x", "1", "checked"); %>>Yes</input><input type="radio" value="0" name="wl_radio_x" class="content_input_fd" onClick="return change_common_radio(this, 'WLANConfig11b', 'wl_radio_x', '0')" <% nvram_match_x("WLANConfig11b","wl_radio_x", "0", "checked"); %>>No</input></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_x_WMM_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_x_WMM_itemname#>
           </td><td class="content_input_td"><select name="wl_wme" class="content_input_fd" onChange="return change_common(this, 'WLANConfig11b', 'wl_wme')"><option class="content_input_fd" value="off" <% nvram_match_x("WLANConfig11b","wl_wme", "off","selected"); %>>Disabled</option><option class="content_input_fd" value="on" <% nvram_match_x("WLANConfig11b","wl_wme", "on","selected"); %>>Enabled</option></select></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_x_NOACK_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_x_NOACK_itemname#>
           </td><td class="content_input_td"><select name="wl_wme_no_ack" class="content_input_fd" onChange="return change_common(this, 'WLANConfig11b', 'wl_wme_no_ack')"><option class="content_input_fd" value="off" <% nvram_match_x("WLANConfig11b","wl_wme_no_ack", "off","selected"); %>>Disabled</option><option class="content_input_fd" value="on" <% nvram_match_x("WLANConfig11b","wl_wme_no_ack", "on","selected"); %>>Enabled</option></select></td>
</tr>
</table>
</td>
</tr>
<tr>
<td>
<table width="666" border="2" cellpadding="0" cellspacing="0" bordercolor="E0E0E0"></table>
</td>
</tr>
<tr>
<td>		
<table width="666" border="1" cellpadding="0" cellspacing="0" bordercolor="B0B0B0">
<tr bgcolor="#CCCCCC"><td colspan="3"><font face="arial" size="2"><b>&nbsp</b></font></td></tr>
<tr bgcolor="#FFFFFF">  
   <td id ="Confirm" height="25" width="34%">  
   <div align="center"><font face="Arial"> <input class=inputSubmit onMouseOut=buttonOut(this) onMouseOver="buttonOver(this)" onClick="onSubmitCtrl(this, ' Restore ')" type="submit" value=" <#CTL_restore#> " name="action"></font></div> 
   </td>  
   <td height="25" width="33%">  
   <div align="center"><font face="Arial"> <input class=inputSubmit onMouseOut=buttonOut(this) onMouseOver="buttonOver(this)" onClick="onSubmitCtrl(this, ' Finish ')" type="submit" value=" <#CTL_finish#> " name="action"></font></div> 
   </td>
   <td height="25" width="33%">  
   <div align="center"><font face="Arial"> <input class=inputSubmit onMouseOut=buttonOut(this) onMouseOver="buttonOver(this)" onClick="onSubmitCtrl(this, ' Apply ')" type="submit" value=" <#CTL_apply#> " name="action"></font></div> 
   </td>    
</tr>
</table>
</td>
</tr>

<tr>
<td>
<table width="666" border="1" cellpadding="0" cellspacing="0" bordercolor="B0B0B0">
<tr>
    <td colspan="2" width="616" height="25" bgcolor="#FFBB00"></td> 
</tr>                   
<tr bgcolor="#FFFFFF">
    <td class="content_header_td_15" align="left"><#CTL_restore#>: </td>
    <td class="content_input_td_padding" align="left"><#CTL_desc_restore#></td>
</tr>
<tr bgcolor="#FFFFFF">
    <td class="content_header_td_15" align="left"><#CTL_finish#>: </td>
    <td class="content_input_td_padding" align="left"><#CTL_desc_finish#></td>
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
