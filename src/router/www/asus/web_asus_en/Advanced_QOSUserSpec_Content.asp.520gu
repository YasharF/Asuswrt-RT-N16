<head>
<title>ZVMODELVZ Web Manager</title>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<link rel="stylesheet" type="text/css" href="style.css" media="screen"></link>
<script language="JavaScript" type="text/javascript" src="overlib.js"></script>
<script language="JavaScript" type="text/javascript" src="general.js"></script>
</head>  
<div id="overDiv" style="position:absolute; visibility:hidden; z-index:1000;"></div>    
<body onLoad="load_body()" onunLoad="return unload_body();">

<form method="GET" name="form" action="apply.cgi">
<input type="hidden" name="current_page" value="Advanced_QOSUserSpec_Content.asp">
<input type="hidden" name="next_page" value="SaveRestart.asp">
<input type="hidden" name="next_host" value="">
<input type="hidden" name="sid_list" value="PrinterStatus;">
<input type="hidden" name="group_id" value="">
<input type="hidden" name="modified" value="0">
<input type="hidden" name="action_mode" value="">
<input type="hidden" name="first_time" value="">
<input type="hidden" name="action_script" value="">
<input type="hidden" name="qos_rulenum_x_0" value="<% nvram_get_x("PrinterStatus", "qos_rulenum_x"); %>" readonly="1">
<input type="hidden" name="qos_rule_w" value="">
<input type="hidden" name="preferred_lang" value="<% nvram_get_x("","preferred_lang"); %>">
<input type="hidden" name="qos_service_ubw" value="0">

<!-- Table for the conntent page -->	    
<table width="666" border="0" cellpadding="0" cellspacing="0">     	      

<tr>
<td>
<table width="666" border="1" cellpadding="0" cellspacing="0" bordercolor="E0E0E0">

<tr class="content_header_tr">
	<td class="content_header_td_title" colspan="2"><#BM_title_User#></td>
</tr>
<tr>
	<td class="content_header_td" onMouseOver="return overlib('<#BM_note1#><#BM_note2#><#BM_note3#>', LEFT);" onMouseOut="return nd();"><#BM_pkt_flg#></td>
	<td class="content_input_td">
		<input type="radio" value="1" name="qos_dfragment_enable" class="content_input_fd" onClick="return change_common_radio(this, 'PrinterStatus', 'qos_dfragment_enable', '1')" <% nvram_match_x("PrinterStatus","qos_dfragment_enable", "1", "checked"); %>>Yes</input>
		<input type="radio" value="0" name="qos_dfragment_enable" class="content_input_fd" onClick="return change_common_radio(this, 'PrinterStatus', 'qos_dfragment_enable', '0')" <% nvram_match_x("PrinterStatus","qos_dfragment_enable", "0", "checked"); %>>No</input>
	</td>
</tr>
<tr>
	<td class="content_header_td"><#BM_pkt_size#></td>
	<td class="content_input_td">
		<input type="text" maxlength="4" size="4" name="qos_dfragment_size" class="content_input_fd" value="<% nvram_get_x("PrinterStatus", "qos_dfragment_size"); %>" onBlur="validate_range(this, 536, 1500)" onKeyPress="return is_number(this)">
	</td>
</tr>

</table>
</td>
</tr>

<!--
<input type="text" name="qos_dfragment_size">
-->

<tr>
<td>
<table width="666" border="2" cellpadding="0" cellspacing="0" bordercolor="E0E0E0">
<tr class="content_list_header_tr">
<td class="content_list_header_td" width="60%" id="x_QRuleList"><#BM_UserList_title#></td>
<td width="10%">
<div align="center">
<input class="inputSubmit" type="submit" onClick="return markGroup(this, 'x_USRRuleList', 8, ' Add ', document.form.x_USRRuleList_s);" name="x_USRRuleList" value="<#CTL_add#>" size="12">
</div>
</td><td width="10%">
<div align="center">
<input class="inputSubmit" type="submit" onClick="return markGroup(this, 'x_USRRuleList', 8, ' Del ');" name="x_USRRuleList" value="<#CTL_del#>" size="12">
</div>
</td><td width="5%"></td>
</tr>
<tr>
<td class="content_desc_td" colspan="6"><#BM_user_desc#></td>
</tr>

<table class="content_list_table" width="640" border="0" cellspacing="0" cellpadding="0">
<tr>
<td colspan="3" height="10"></td>
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
		    	<td class="content_list_field_header_td" colspan=""><#BM_UserList1#></td>
		    	<td class="content_list_field_header_td" colspan=""><#BM_UserList2#></td>
		    	<td class="content_list_field_header_td" colspan=""><#BM_UserList3#>	</td>
		    	<td class="content_list_field_header_td" colspan=""><#BM_UserList4#>	</td>
		    	<td></td>
		    	</tr>
		    	<tr>
			<td></td>
			<td class="content_list_input_td" colspan=""><input type="text" maxlength="15" class="content_input_list_fd" size="14" name="qos_service_name_x_0" onKeyPress="return is_string(this)"></td>
			<td class="content_list_input_td" colspan=""><input type="text" maxlength="15" class="content_input_list_fd" size="14" name="qos_ip_x_0" onKeyPress="return is_iprange(this)" onKeyUp="change_iprange(this)"></td>
			<td class="content_list_input_td" colspan=""><input type="text" maxlength="10" size="10" class="content_input_list_fd" name="qos_port_x_0" onKeyPress="return is_portrange(this)"></td>
		        <td align="center">
                		<select name='qos_prio_x_0' style="font-family: 'fixedsys'; font-size: '8pt'">
 		                    <option value='1'> &nbsp;1 </option>
                		    <option value='2'> &nbsp;2 </option>
		                    <option value='3'> &nbsp;3 </option>
                		    <option value='4'> &nbsp;4 </option>
		                    <option value='5'> &nbsp;5 </option>
                		    <option value='6'> &nbsp;6 </option>
		                    <option value='7'> &nbsp;7 </option>
                		    <option value='8'> &nbsp;8 </option>
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
