<head>
<title><#BM_title2#></title>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<script language="JavaScript" type="text/javascript" src="/javascript/menu.js"></script>
<script language="JavaScript" type="text/javascript" src="/javascript/title.js"></script>
<script language="JavaScript" type="text/javascript" src="/javascript/utils.js"></script>
<script type="text/javascript" src="slider/slider.js"></script>
<script type="text/javascript" src="slider/libcrossbrowser.js"></script>
<script type="text/javascript" src="slider/eventhandler.js"></script>
<link rel="stylesheet" type="text/css" href="style.css" media="screen"></link>
<script language="JavaScript" type="text/javascript" src="overlib.js"></script>
<script language="JavaScript" type="text/javascript" src="general.js"></script>
<input type="hidden" name="preferred_lang" value="<% nvram_get_x("","preferred_lang"); %>">
</head>  
     <script language='javascript'>
<!--
    var qos_service_enable_x="<% nvram_get_x("PrinterStatus", "qos_service_enable"); %>";
    var qos_service_ubw_x="<% nvram_get_x("PrinterStatus", "qos_service_ubw"); %>";
    var qos_dfragment_enable_x="<% nvram_get_x("PrinterStatus", "qos_dfragment_enable"); %>";
    var qos_dfragment_size_x="<% nvram_get_x("PrinterStatus", "qos_dfragment_size"); %>";

var nav;
var keyPressed;
if (navigator.appName == 'Netscape')
        nav = true;
else {
        nav = false;
        //document.onkeydown = true;
}
                                                                                                                                             
function IEKey() {
        window.top.pageChanged = 1;
        return (event.keyCode);
}
                                                                                                                                            
function NSKey() {
        window.top.pageChanged = 1;
        return(0);
}
                                                                                                                                              
function checkName()
{
   var i=0;
   with(document.forms[0]){
      if(qos_name_w.value=='')
	 alert('<#BM_alert_sername#>');
      while(i<qos_service_name.length)
      {
	 if(qos_service_name[i]==qos_name_w.value)
	 {
		alert('<#BM_alert_Name1#> "'+qos_name_w.value+ '" <#BM_alert_Name2#>');
		qos_name_w.value='';
		return false;
	 }	
	 i++;
      }

      s = qos_name_w.value;
      if (!is_string(qos_name_w.value))
      {
	 qos_name_w.value='';
         return false;
      }

      for(i=0;i<s.length;i++)
      {
         c = s.charCodeAt(i);
         if (!validate_namechar(c))
         {
                alert('<%ejGetKW(USERLISTADD_js_desc2)%>');
		qos_name_w.value='';
                return(false);
         }
      }
   }
}

function validate_namechar(ch)
{
        if (ch>=48&&ch<=57) return true;
        if (ch>=97&&ch<=122) return true;
        if (ch>=65&&ch<=90) return true;
        if (ch==45) return true;
        if (ch==95) return true;
        return false;
}
                                                                                                                                              
function is_string(o) {
        if (!nav) keyPressed = IEKey();
        else keyPressed = NSKey();
                                                                                                                                              
        if (keyPressed==0) return true;
        else if (keyPressed>=0&&keyPressed<=126) return true;
                                                                                                                                              
        alert('<%ejGetKW(USERLISTADD_js_desc1)%>');
        return false;
}
                                                                                                                                              
function checkIP()
{
   with ( document.forms[0] ) {
      if ( isValidIpAddress(qos_ip_w.value) == false ) {
         alert('<#BM_alert_IP1#> "' + qos_ip_w.value + '" <#BM_alert_IP2#>');
	 qos_ip_w.value ='';
	 return false;
      }
   }                                                                                                                
}

function checkPort(min, max) {
   with(document.forms[0]){
    	if(!isNaN(qos_port_w.value)) {
            if(qos_port_w.value<min || qos_port_w.value>max) {
                alert('<#BM_alert_port1#> ' + min + ' <#BM_alert_to#> ' + max + '.');
                qos_port_w.value = '';
                return false;
            }
	}
	else {
            alert('"'+qos_port_w.value+'" <#BM_alert_port2#> ' + min + ' <#BM_alert_to#> ' + max + '.');
            qos_port_w.value = '';
            return false;
	}
   }
}

function clickAdd() {
   var i=0;
   var j=0;
   var k=0;
   with(document.forms[0]) {
	//AddBtn.disabled=true;
	qos_rule_w.value = 1;
	if(qos_service_name.length==8) {
		qos_rule_w.value = 0;
		alert("<#BM_alert_Add1#>");
		//AddBtn.disabled=false;
		return false;
	}
        if(qos_name_w.value=='') {
                qos_rule_w.value = 0;
        	alert('<#BM_alert_sername#>');
		//AddBtn.disabled=false;
		return false;
	}
	while(i<qos_service_name.length)
	{
        	 if(qos_service_name[i]==qos_name_w.value)
	         {
	               qos_rule_w.value = 0;
        	        alert('<#BM_alert_Name1#> "'+qos_name_w.value+ '" <#BM_alert_Name2#>');
                	qos_name_w.value='';
			//AddBtn.disabled=false;
	                return false;
        	 }
	         i++;
	}
	if ( isValidIpAddress(qos_ip_w.value) == false ) {
                qos_rule_w.value = 0;
        	alert('<#BM_alert_IP1#> "' + qos_ip_w.value + '"  <#BM_alert_IP2#>');
	        qos_ip_w.value = '';
		//AddBtn.disabled=false;
        	return false;
      	}
	if(qos_port_w.value<1 || qos_port_w.value>65535) {
                qos_rule_w.value = 0;
               	alert('<#BM_alert_port3#>');
               	qos_port_w.value = '';
		//AddBtn.disabled=false;
               	return false;
        }
        while(j<qos_ip.length)
        {
                if(qos_ip[j]==qos_ip_w.value && qos_port[j]==qos_port_w.value)
                {
			qos_rule_w.value = 0;
			qos_ip_w.value = '';
			qos_port_w.value = '';
			alert('<#BM_alert_length#>');
			//AddBtn.disabled=false;
			return false;
                }
                j++;
        }
        while(k<qos_prio.length)
        {
                 if( qos_prio[k] == qos_prio_w.value )
                 {
                        qos_rule_w.value = 0;
                        alert('<#BM_alert_prio1#> "'+qos_prio_w.value+ '" <#BM_alert_port2#>');
			//AddBtn.disabled=false;
                        return false;
                 }
                 k++;
        }
	Submit.value='Add';
	submit();
	AddBtn.disabled=true;
   }   
}

function clickRemove()
{
   with(document.forms[0]) {
	Submit.value='Remove';
        submit();
        RemoveBtn.disabled=true;
   }
}

function clickSave()
{
   with(document.forms[0]) {
        Submit.value='Save';
        submit();
        SaveBtn.disabled=true;
   }
}
                                                                                
function blocking(nr,show)
{
        if (document.getElementById)
        {
                var vista = (show) ? 'block' : 'none';
                document.getElementById(nr).style.display = vista;
        }
        else if (document.layers)
        {
                var current = (show) ? 'block' : 'none';
                document.layers[nr].display = current;
        }
        else if (document.all)
        {
                var current = (show) ? 'block' : 'none';
                document.all[nr].style.display = current;
        }
}
                                                                                
function Fragment_display()
{
    with( document.forms[0])
    {
        if(qos_dfragment_enable_w.checked==true)
        {
		qos_dfragment_enable.value = 1;
	        blocking('Fragment' ,true);
	}
        else
	{
		qos_dfragment_enable.value = 0;
                blocking('Fragment' ,false);
	}
    }
}
 
 function FTPServer_display()
{
    with( document.forms[0])
    {
        if(qos_service_enable_x==1)
                blocking('FTPServer' ,true);
        else
                blocking('FTPServer' ,false);
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
        if ((document.all) || (document.getElementById)){
                y.obj.innerHTML = slider_table(2);
        }
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
	//var disk_num=1;
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
	var colorArray = new Array("red", "red","orange","green","magenta","yellow","turquoise");

	disk_num=1;
        sliderArray1[disk_num.toString(10)] = drawSlider(disk_num.toString(10),"sliderDiv" + disk_num, "red", 0, 100, document.forms[0].qos_service_ubw.value, 1, false);
        disk_num=2;
        sliderArray2[disk_num.toString(10)] = drawSlider(disk_num.toString(10),"sliderDiv" + disk_num, "red", 0, 100, document.forms[0].qos_dfragment_size.value, 1, false);

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
	document.forms[0].qos_service_ubw.value=sliderObj.getValue();
}

function bsSliderChange2(sliderObj, val, newPos){
        document.forms[0].qos_dfragment_size.value=sliderObj.getValue();
}

function frmload()
{
    with(document.forms[0]){
	qos_service_ubw.value=qos_service_ubw_x;
	qos_dfragment_size.value=qos_dfragment_size_x;
	if(qos_dfragment_enable_x=='1')
		qos_dfragment_enable_w.checked = true;
	FTPServer_display();
	Fragment_display();
    }
}
-->
</script>

<div id="overDiv" style="position:absolute; visibility:hidden; z-index:1000;"></div>    
<body onLoad="frmload();drawSliderBlock();load_body();" onunLoad="return unload_body();">
<form method="GET" name="form" action="apply.cgi">
<!-- Table for the conntent page -->	    
<table width="666" border="0" cellpadding="0" cellspacing="0">     	      
    	
<input type="hidden" name="current_page" value="Advanced_QOS_Content.asp"><input type="hidden" name="next_page" value="SaveRestart.asp"><input type="hidden" name="next_host" value=""><input type="hidden" name="sid_list" value="PrinterStatus;"><input type="hidden" name="group_id" value=""><input type="hidden" name="modified" value="0"><input type="hidden" name="action_mode" value=""><input type="hidden" name="first_time" value=""><input type="hidden" name="action_script" value=""><input type="hidden" name="preferred_lang" value="<% nvram_get_x("","preferred_lang"); %>">
<input type="hidden" name="qos_rulenum_x_0" value="<% nvram_get_x("PrinterStatus", "qos_rulenum_x"); %>" readonly="1">
<input type='hidden' name='qos_rule_w'>
<input type='hidden' name='qos_dfragment_enable'>
<tr>
<td>
<table width="666" border="1" cellpadding="0" cellspacing="0" bordercolor="E0E0E0">
<tr class="content_header_tr">
<td class="content_header_td_title" colspan="2"><#BM_title_User#></td>
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
<table width="666" border="2" cellpadding="0" cellspacing="0" bordercolor="E0E0E0"></table>
</td>
</tr>
<tr>
<td>
<table width="666" border="2" cellpadding="0" cellspacing="0" bordercolor="E0E0E0">
<tr class="content_list_header_tr">
<td class="content_list_header_td" width="60%" id="x_QRuleList"><#BM_UserList_title#>
         <input type="hidden" name="qos_rulenum_x_0" value="<% nvram_get_x("PrinterStatus", "qos_rulenum_x"); %>" readonly="1"></td><td width="10%">
<div align="center">
<input class="inputSubmit" type="submit" onClick="return markGroup(this, 'x_USRRuleList', 8, ' Add ', document.form.x_USRRuleList_s);" name="x_USRRuleList" value="<#CTL_add#>" size="12">
</div>
</td><td width="10%">
<div align="center">
<input class="inputSubmit" type="submit" onClick="return markGroup(this, 'x_USRRuleList', 8, ' Del ', document.form.x_USRRuleList_s);" name="x_USRRuleList" value="<#CTL_del#>" size="12">
</div>
</td><td width="5%"></td>
</tr>
<table class="content_list_table" width="640" border="0" cellspacing="0" cellpadding="0">
<tr>
<td colspan="3" height="10"></td>
</tr>
<tr>
<td height="60" width="640" align="center"><#BM_user_desc#>    </td>
</tr>

<tr>
<td colspan="3">
<div align="center">
	    <table border="0" cellspacing="0" cellpadding="0">
            	<tr>
	    	<td colspan="3">
		    <div align="center">
		    <table border="1" cellspacing="0" cellpadding="0">
		    	<tr>
			<td></td>
		    	<td class="groupitem" colspan=""><#BM_UserList1#></td>
		    	<td class="groupitem" colspan=""><#BM_UserList2#></td>
		    	<td class="groupitem" colspan=""><#BM_UserList3#>	</td>
		    	<td class="groupitem" colspan=""><#BM_UserList4#>	</td>
		    	<td></td>
		    	</tr>
		    	<tr>
			<td></td>
			<td colspan=""><input type="text" maxlength="15" class="content_input_list_fd" size="14" name="qos_service_name_x_0"></td>
			<td colspan=""><input type="text" maxlength="15" class="content_input_list_fd" size="14" name="qos_ip_x_0" onKeyPress="return is_iprange(this)" onKeyUp="change_iprange(this)"></td>
			<td colspan=""><input type="text" maxlength="10" size="10" class="content_input_list_fd" name="qos_port_x_0" onKeyPress="return is_number(this)"></td>
		        <td align="center">
                		<select name='qos_prio_x_0'>
 		                    <option value='0'> &nbsp;1 </option>
                		    <option value='1'> &nbsp;2 </option>
		                    <option value='2'> &nbsp;3 </option>
                		    <option value='3'> &nbsp;4 </option>
		                    <option value='4'> &nbsp;5 </option>
                		    <option value='5'> &nbsp;6 </option>
		                    <option value='6'> &nbsp;7 </option>
                		    <option value='7'> &nbsp;8 </option>
		                </select>
            		</td>
                    	</tr>
		    	<tr>
			<td></td><td colspan="10"><select size="8" name="x_USRRuleList_s" multiple="true" style="font-family: 'fixedsys'; font-size: '8pt'">
			<% nvram_get_table_x("PrinterStatus","x_USRRuleList"); %>
			</select></td>
		    	</tr>
		    </table>
		    </div>
	    	</td>
	    	</tr>
	    	<tr>
	    	<td colspan="3" height="10"></td>
	    	</tr>
	    </table>
</div>
</td>
</tr>
<tr>
<td colspan="3" height="10"></td>
</tr>
</table>
</table>
</td>
</tr>

<table  width="640" border="0" cellspacing="0" cellpadding="0">
<tr>
<td colspan="3" height="30"></td>
</tr>
<tr>
<td colspan="3">
	<div id='<#BM_FTP_title#>' style='display:none'>
	    <#BM_FTP_desc#>
            <table>
            	<tr>
            	<td width='240'></td>
            	<td width='50'><b><#BM_Low#></b></td>
            	<td width='120'></td>
            	<td width='55'><b><#BM_High#></b></td>
            	</td>
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
    	<br>
	<input type="checkbox" name="qos_dfragment_enable_w" onclick='Fragment_display();'><#BM_pkt_flg#><br>
	<div id='Fragment' style='display:none'>
            <table>
            	<tr>
            	<td width='240'></td>
            	<td width='50'><b><#BM_small#></b></td>
            	<td width='120'></td>
            	<td width='55'><b><#BM_large#></b></td>
            	</td>
            	</tr>
            </table>
            <table border='0'>
            <tr>
		<td width='50'></td>
                <td width='185'><#BM_pkt_size#><input type="hidden" name="qos_dfragment_size"></td>
		<td width='220' valign='top'>
                <div id="sliderDiv_2">
                    <script language="javascript">
                        <!-- document.writeln(slider_table(2)); -->
                    </script>
                </div>
		</td>
	    </tr>
            </table>
            <table>
            <tr>
                <td width='50'></td>
		<td valign='top'>
		    <font size='2'><#BM_note#> </font>
        	</td>
        	<td><font size='2'>
        	    <#BM_note1#><br>
		    <#BM_note2#><br>
                    <#BM_note3#>
        	</font>
		</td>
	    </tr>
	    </table>
	</div>
</td>
</tr>
<tr>
<td colspan="3" height="30"></td>
</tr>
</table>

<tr>
<td>		
<table width="666" border="1" cellpadding="0" cellspacing="0" bordercolor="B0B0B0">
<tr bgcolor="#CCCCCC"><td colspan="3"><font face="arial" size="2"><b>&nbsp</b></font></td></tr>
<tr bgcolor="#FFFFFF">  
   <td id ="Confirm" height="25" width="34%">  
   <div align="center"><font face="Arial"> <input class=inputSubmit onMouseOut=buttonOut(this) onMouseOver="buttonOver(this)" onClick="return onSubmitCtrl(this, ' Restore ');" type="submit" value=" <#CTL_restore#> " name="action"></font></div> 
   </td>  
   <td height="25" width="33%">  
   <div align="center"><font face="Arial"> <input class=inputSubmit onMouseOut=buttonOut(this) onMouseOver="buttonOver(this)" onClick="return onSubmitCtrl(this, ' Finish ');" type="submit" value=" <#CTL_finish#> " name="action"></font></div> 
   </td>
   <td height="25" width="33%">  
   <div align="center"><font face="Arial"> <input class=inputSubmit onMouseOut=buttonOut(this) onMouseOver="buttonOver(this)" onClick="return onSubmitCtrl(this, ' Apply ');" type="submit" value=" <#CTL_apply#> " name="action"></font></div> 
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
