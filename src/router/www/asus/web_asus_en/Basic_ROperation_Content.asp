<head>
<title>ZVMODELVZ Web Manager</title>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<link rel="stylesheet" type="text/css" href="style.css" media="screen"></link>
<script language="JavaScript" type="text/javascript" src="overlib.js"></script>
<script language="JavaScript" type="text/javascript" src="general.js"></script>
<script language="JavaScript" type="text/javascript" src="quick.js"></script>
</head>
<body bgcolor="#FFFFFF" onLoad="loadQuick()">
<div id="overDiv" style="position:absolute; visibility:hidden; z-index:1000;"></div>
<form method="GET" name="form" action="apply.cgi">
<!-- Table for the conntent page -->
<table width="666" border="0" cellpadding="0" cellspacing="0">
<input type="hidden" name="x_Mode" value="0">
<input type="hidden" name="current_page" value="Basic_GOperation_Content.asp">
<input type="hidden" name="next_page" value="Basic_HomeGateway_SaveRestart.asp">
<input type="hidden" name="next_host" value="">
<input type="hidden" name="sid_list" value="Layer3Forwarding;IPConnection;PPPConnection;WLANConfig11a;WLANConfig11b;LANHostConfig;FirewallConfig;">
<input type="hidden" name="action_mode" value="">
<input type="hidden" name="modified" value="0">
<input type="hidden" name="group_id" value="">
<input type="hidden" name="wan_proto" value="<% nvram_get_x("Layer3Forwarding","wan_proto"); %>">
<input type="hidden" name="wan_route_x" value="<% nvram_get_x("IPConnection","wan_route_x"); %>">
<input type="hidden" name="wan_nat_x" value="<% nvram_get_x("IPConnection","wan_nat_x"); %>">
<input type="hidden" name="wl_auth_mode" value="<% nvram_get_x("WLANConfig11b","wl_auth_mode"); %>">
<input type="hidden" name="wl_crypto" value="<% nvram_get_x("WLANConfig11b","wl_crypto"); %>">
<input type="hidden" name="wl_wep_x" value="<% nvram_get_x("WLANConfig11b","wl_wep_x"); %>">
<input type="hidden" name="lan_ipaddr" value="<% nvram_get_x("LANHostConfig","lan_ipaddr"); %>">
<input type="hidden" name="lan_netmask" value="<% nvram_get_x("LANHostConfig","lan_netmask"); %>">
<input type="hidden" name="lan1_ipaddr" value="<% nvram_get_x("LANHostConfig","lan1_ipaddr"); %>">
<input type="hidden" name="lan1_netmask" value="<% nvram_get_x("LANHostConfig","lan1_netmask"); %>">
<input type="hidden" name="preferred_lang" value="<% nvram_get_x("","preferred_lang"); %>">
<input type="hidden" name="wl_ssid2" value="<% nvram_get_x("WLANConfig11b","wl_ssid2"); %>">
<tr>
<td>
<table width="666" border="1" cellpadding="0" cellspacing="0" bordercolor="E0E0E0">
<tr id="Country" class="content_header_tr">
<td class="content_header_td_title" colspan="2"><#BOP_title#></td>
</tr>
<tr class="content_section_header_tr">
<td class="content_section_header_td" colspan="2"><#BOP_time_title#></td>
</tr>
<tr>
<td class="content_desc_td" colspan="2" height="50">
<#BOP_time_desc#></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#BOP_time_zone_desc#>', LEFT);" onMouseOut="return nd();"><#BOP_time_zone_item#></td><td class="content_input_td"><select name="time_zone" class="content_input_fd" onChange="return change_common(this, 'LANHostConfig', 'time_zone')"><option class="content_input_fd" value="UCT12" <% nvram_match_x("LANHostConfig","time_zone", "UCT12","selected"); %>>(GMT-12:00) Eniwetok, Kwajalein</option><option class="content_input_fd" value="UCT11" <% nvram_match_x("LANHostConfig","time_zone", "UCT11","selected"); %>>(GMT-11:00) Midway Island, Samoa</option><option class="content_input_fd" value="UCT10" <% nvram_match_x("LANHostConfig","time_zone", "UCT10","selected"); %>>(GMT-10:00) Hawaii</option><option class="content_input_fd" value="NAST9NADT" <% nvram_match_x("LANHostConfig","time_zone", "NAST9NADT","selected"); %>>(GMT-09:00) Alaska</option><option class="content_input_fd" value="PST8PDT" <% nvram_match_x("LANHostConfig","time_zone", "PST8PDT","selected"); %>>(GMT-08:00) Pacific Time (US, Canada)</option><option class="content_input_fd" value="MST7MDT" <% nvram_match_x("LANHostConfig","time_zone", "MST7MDT","selected"); %>>(GMT-07:00) Mountain Time (US, Canada)</option><option class="content_input_fd" value="MST7" <% nvram_match_x("LANHostConfig","time_zone", "MST7","selected"); %>>(GMT-07:00) Arizona</option><option class="content_input_fd" value="CST6CDT_1" <% nvram_match_x("LANHostConfig","time_zone", "CST6CDT_1","selected"); %>>(GMT-06:00) Central Time (US, Canada)</option><option class="content_input_fd" value="CST6CDT_2" <% nvram_match_x("LANHostConfig","time_zone", "CST6CDT_2","selected"); %>>(GMT-06:00) Saskatchewan</option><option class="content_input_fd" value="CST6CDT_3" <% nvram_match_x("LANHostConfig","time_zone", "CST6CDT_3","selected"); %>>(GMT-06:00) Guadalajara, Mexico City</option><option class="content_input_fd" value="CST6CDT_3_1" <% nvram_match_x("LANHostConfig","time_zone", "CST6CDT_3_1","selected"); %>>(GMT-06:00) Monterrey</option><option class="content_input_fd" value="MST7MDT" <% nvram_match_x("LANHostConfig","time_zone", "MST7MDT","selected"); %>>(GMT-07:00) Chihuahua, La Paz, Mazatlan</option><option class="content_input_fd" value="UCT6" <% nvram_match_x("LANHostConfig","time_zone", "UCT6","selected"); %>>(GMT-06:00) Central America</option><option class="content_input_fd" value="EST5EDT" <% nvram_match_x("LANHostConfig","time_zone", "EST5EDT","selected"); %>>(GMT-05:00) Eastern Time (US, Canada)</option><option class="content_input_fd" value="UCT5_1" <% nvram_match_x("LANHostConfig","time_zone", "UCT5_1","selected"); %>>(GMT-05:00) Indiana (East)</option><option class="content_input_fd" value="UCT5_2" <% nvram_match_x("LANHostConfig","time_zone", "UCT5_2","selected"); %>>(GMT-05:00) Bogota, Lima, Quito</option><option class="content_input_fd" value="AST4ADT" <% nvram_match_x("LANHostConfig","time_zone", "AST4ADT","selected"); %>>(GMT-04:00) Atlantic Time (Canada)</option><option class="content_input_fd" value="UCT4_1" <% nvram_match_x("LANHostConfig","time_zone", "UCT4_1","selected"); %>>(GMT-04:00) Caracas, La Paz</option><option class="content_input_fd" value="UCT4_2" <% nvram_match_x("LANHostConfig","time_zone", "UCT4_2","selected"); %>>(GMT-04:00) Santiago</option><option class="content_input_fd" value="NST3.30NDT" <% nvram_match_x("LANHostConfig","time_zone", "NST3.30NDT","selected"); %>>(GMT-03:30) Newfoundland</option><option class="content_input_fd" value="EBST3EBDT_1" <% nvram_match_x("LANHostConfig","time_zone", "EBST3EBDT_1","selected"); %>>(GMT-03:00) Brasilia</option><option class="content_input_fd" value="UCT3" <% nvram_match_x("LANHostConfig","time_zone", "UCT3","selected"); %>>(GMT-03:00) Buenos Aires, Georgetown</option><option class="content_input_fd" value="EBST3EBDT_2" <% nvram_match_x("LANHostConfig","time_zone", "EBST3EBDT_2","selected"); %>>(GMT-03:00) Greenland</option><option class="content_input_fd" value="NORO2" <% nvram_match_x("LANHostConfig","time_zone", "NORO2","selected"); %>>(GMT-02:00) Mid-Atlantic</option><option class="content_input_fd" value="EUT1EUTDST" <% nvram_match_x("LANHostConfig","time_zone", "EUT1EUTDST","selected"); %>>(GMT-01:00) Azores</option><option class="content_input_fd" value="UCT1" <% nvram_match_x("LANHostConfig","time_zone", "UCT1","selected"); %>>(GMT-01:00) Cape Verde Is.</option><option class="content_input_fd" value="GMT0BST_1" <% nvram_match_x("LANHostConfig","time_zone", "GMT0BST_1","selected"); %>>(GMT) Greenwich Mean Time</option><option class="content_input_fd" value="GMT0BST_2" <% nvram_match_x("LANHostConfig","time_zone", "GMT0BST_2","selected"); %>>(GMT) Casablanca, Monrovia</option><option class="content_input_fd" value="UCT-1_1" <% nvram_match_x("LANHostConfig","time_zone", "UCT-1_1","selected"); %>>(GMT+01:00) Belgrade, Bratislava, Budapest</option><option class="content_input_fd" value="UCT-1_1_1" <% nvram_match_x("LANHostConfig","time_zone", "UCT-1_1_1","selected"); %>>(GMT+01:00) Ljubljana, Prague</option><option class="content_input_fd" value="UCT-1_2" <% nvram_match_x("LANHostConfig","time_zone", "UCT-1_2","selected"); %>>(GMT+01:00) Sarajevo, Skopje, Sofija</option><option class="content_input_fd" value="UCT-1_2_1" <% nvram_match_x("LANHostConfig","time_zone", "UCT-1_2_1","selected"); %>>(GMT+01:00) Vilnius, Warsaw, Zagreb</option><option class="content_input_fd" value="MET-1METDST" <% nvram_match_x("LANHostConfig","time_zone", "MET-1METDST","selected"); %>>(GMT+01:00) Brussels, Copenhagen</option><option class="content_input_fd" value="MET-1METDST_1" <% nvram_match_x("LANHostConfig","time_zone", "MET-1METDST_1","selected"); %>>(GMT+01:00) Madrid, Paris</option><option class="content_input_fd" value="MEZ-1MESZ" <% nvram_match_x("LANHostConfig","time_zone", "MEZ-1MESZ","selected"); %>>(GMT+01:00) Amsterdam, Berlin, Bern</option><option class="content_input_fd" value="MEZ-1MESZ_1" <% nvram_match_x("LANHostConfig","time_zone", "MEZ-1MESZ_1","selected"); %>>(GMT+01:00) Rome, Stockholm, Vienna</option><option class="content_input_fd" value="UCT-1_3" <% nvram_match_x("LANHostConfig","time_zone", "UCT-1_3","selected"); %>>(GMT+01:00) West Central Africa</option><option class="content_input_fd" value="ET-2EETDST" <% nvram_match_x("LANHostConfig","time_zone", "ET-2EETDST","selected"); %>>(GMT+02:00) Bucharest</option><option class="content_input_fd" value="EST-2EDT" <% nvram_match_x("LANHostConfig","time_zone", "EST-2EDT","selected"); %>>(GMT+02:00) Cairo</option><option class="content_input_fd" value="UCT-2_1" <% nvram_match_x("LANHostConfig","time_zone", "UCT-2_1","selected"); %>>(GMT+02:00) Helsinki, Riga, Tallinn</option><option class="content_input_fd" value="UCT-2_2" <% nvram_match_x("LANHostConfig","time_zone", "UCT-2_2","selected"); %>>(GMT+02:00) Athens, Istanbul, Minsk</option><option class="content_input_fd" value="IST-2IDT" <% nvram_match_x("LANHostConfig","time_zone", "IST-2IDT","selected"); %>>(GMT+02:00) Jerusalem</option><option class="content_input_fd" value="SAST-2" <% nvram_match_x("LANHostConfig","time_zone", "SAST-2","selected"); %>>(GMT+02:00) Harare, Pretoria</option><option class="content_input_fd" value="MST-3MDT" <% nvram_match_x("LANHostConfig","time_zone", "MST-3MDT","selected"); %>>(GMT+03:00) Moscow, St. Petersburg</option><option class="content_input_fd" value="MST-3MDT_1" <% nvram_match_x("LANHostConfig","time_zone", "MST-3MDT_1","selected"); %>>(GMT+03:00) Volgograd</option><option class="content_input_fd" value="UCT-3_1" <% nvram_match_x("LANHostConfig","time_zone", "UCT-3_1","selected"); %>>(GMT+03:00) Kuwait, Riyadh</option><option class="content_input_fd" value="UCT-3_2" <% nvram_match_x("LANHostConfig","time_zone", "UCT-3_2","selected"); %>>(GMT+03:00) Nairobi</option><option class="content_input_fd" value="IST-3IDT" <% nvram_match_x("LANHostConfig","time_zone", "IST-3IDT","selected"); %>>(GMT+03:00) Baghdad</option><option class="content_input_fd" value="UCT-3.30" <% nvram_match_x("LANHostConfig","time_zone", "UCT-3.30","selected"); %>>(GMT+03:30) Tehran</option><option class="content_input_fd" value="UCT-4_1" <% nvram_match_x("LANHostConfig","time_zone", "UCT-4_1","selected"); %>>(GMT+04:00) Abu Dhabi, Muscat</option><option class="content_input_fd" value="UCT-4_2" <% nvram_match_x("LANHostConfig","time_zone", "UCT-4_2","selected"); %>>(GMT+04:00) Baku, Tbilisi, Yerevan</option><option class="content_input_fd" value="UCT-4.30" <% nvram_match_x("LANHostConfig","time_zone", "UCT-4.30","selected"); %>>(GMT+04:30) Kabul</option><option class="content_input_fd" value="RFT-5RFTDST" <% nvram_match_x("LANHostConfig","time_zone", "RFT-5RFTDST","selected"); %>>(GMT+05:00) Ekaterinburg</option><option class="content_input_fd" value="UCT-5" <% nvram_match_x("LANHostConfig","time_zone", "UCT-5","selected"); %>>(GMT+05:00) Islamabad, Karachi, Tashkent</option><option class="content_input_fd" value="UCT-5.30" <% nvram_match_x("LANHostConfig","time_zone", "UCT-5.30","selected"); %>>(GMT+05:30) Calcutta, Chennai</option><option class="content_input_fd" value="UCT-5.30_1" <% nvram_match_x("LANHostConfig","time_zone", "UCT-5.30_1","selected"); %>>(GMT+05:30) Mumbai, New Delhi</option><option class="content_input_fd" value="UCT-5.45" <% nvram_match_x("LANHostConfig","time_zone", "UCT-5.45","selected"); %>>(GMT+05:45) Kathmandu</option><option class="content_input_fd" value="UCT-6" <% nvram_match_x("LANHostConfig","time_zone", "UCT-6","selected"); %>>(GMT+06:00) Astana, Dhaka</option><option class="content_input_fd" value="UCT-5.30" <% nvram_match_x("LANHostConfig","time_zone", "UCT-5.30","selected"); %>>(GMT+06:00) Sri Jayawardenepura</option><option class="content_input_fd" value="RFT-6RFTDST" <% nvram_match_x("LANHostConfig","time_zone", "RFT-6RFTDST","selected"); %>>(GMT+06:00) Almaty, Novosibirsk</option><option class="content_input_fd" value="UCT-6.30" <% nvram_match_x("LANHostConfig","time_zone", "UCT-6.30","selected"); %>>(GMT+06:30) Rangoon</option><option class="content_input_fd" value="UCT-7" <% nvram_match_x("LANHostConfig","time_zone", "UCT-7","selected"); %>>(GMT+07:00) Bangkok, Hanoi, Jakarta</option><option class="content_input_fd" value="RFT-7RFTDST" <% nvram_match_x("LANHostConfig","time_zone", "RFT-7RFTDST","selected"); %>>(GMT+07:00) Krasnoyarsk</option><option class="content_input_fd" value="CST-8" <% nvram_match_x("LANHostConfig","time_zone", "CST-8","selected"); %>>(GMT+08:00) Beijing, Hong Kong</option><option class="content_input_fd" value="CST-8_1" <% nvram_match_x("LANHostConfig","time_zone", "CST-8_1","selected"); %>>(GMT+08:00) Chongqing, Urumqi</option><option class="content_input_fd" value="SST-8" <% nvram_match_x("LANHostConfig","time_zone", "SST-8","selected"); %>>(GMT+08:00) Kuala Lumpur, Singapore</option><option class="content_input_fd" value="CCT-8" <% nvram_match_x("LANHostConfig","time_zone", "CCT-8","selected"); %>>(GMT+08:00) Taipei</option><option class="content_input_fd" value="WAS-8WAD" <% nvram_match_x("LANHostConfig","time_zone", "WAS-8WAD","selected"); %>>(GMT+08:00) Perth</option><option class="content_input_fd" value="UCT_8" <% nvram_match_x("LANHostConfig","time_zone", "UCT_8","selected"); %>>(GMT+08:00) Irkutsk, Ulaan Bataar</option><option class="content_input_fd" value="UCT-9_1" <% nvram_match_x("LANHostConfig","time_zone", "UCT-9_1","selected"); %>>(GMT+09:00) Seoul</option><option class="content_input_fd" value="JST" <% nvram_match_x("LANHostConfig","time_zone", "JST","selected"); %>>(GMT+09:00) Osaka, Sapporo, Tokyo</option><option class="content_input_fd" value="UCT-9_2" <% nvram_match_x("LANHostConfig","time_zone", "UCT-9_2","selected"); %>>(GMT+09:00) Yakutsk</option><option class="content_input_fd" value="CST-9.30CDT" <% nvram_match_x("LANHostConfig","time_zone", "CST-9.30CDT","selected"); %>>(GMT+09:30) Darwin</option><option class="content_input_fd" value="UCT-9.30" <% nvram_match_x("LANHostConfig","time_zone", "UCT-9.30","selected"); %>>(GMT+09:30) Adelaide</option><option class="content_input_fd" value="UCT-10_1" <% nvram_match_x("LANHostConfig","time_zone", "UCT-10_1","selected"); %>>(GMT+10:00) Canberra, Melbourne, Sydney</option><option class="content_input_fd" value="UCT-10_2" <% nvram_match_x("LANHostConfig","time_zone", "UCT-10_2","selected"); %>>(GMT+10:00) Brisbane</option><option class="content_input_fd" value="TST-10TDT" <% nvram_match_x("LANHostConfig","time_zone", "TST-10TDT","selected"); %>>(GMT+10:00) Hobart</option><option class="content_input_fd" value="RFT-10RFTDST" <% nvram_match_x("LANHostConfig","time_zone", "RFT-10RFTDST","selected"); %>>(GMT+10:00) Vladivostok</option><option class="content_input_fd" value="UCT-10_5" <% nvram_match_x("LANHostConfig","time_zone", "UCT-10_5","selected"); %>>(GMT+10:00) Guam, Port Moresby</option><option class="content_input_fd" value="UCT-10_6" <% nvram_match_x("LANHostConfig","time_zone", "UCT-10_6","selected"); %>>(GMT+10:00) Canberra, Melbourne, Sydney</option><option class="content_input_fd" value="UCT-11" <% nvram_match_x("LANHostConfig","time_zone", "UCT-11","selected"); %>>(GMT+11:00) Magadan, Solomon Is.</option><option class="content_input_fd" value="UCT-11_1" <% nvram_match_x("LANHostConfig","time_zone", "UCT-11_1","selected"); %>>(GMT+11:00) New Caledonia</option><option class="content_input_fd" value="UCT-12" <% nvram_match_x("LANHostConfig","time_zone", "UCT-12","selected"); %>>(GMT+12:00) Fiji, Kamchatka, Marshall Is.</option><option class="content_input_fd" value="NZST-12NZDT" <% nvram_match_x("LANHostConfig","time_zone", "NZST-12NZDT","selected"); %>>(GMT+12:00) Auckland, Wellington</option><option class="content_input_fd" value="UCT-13" <% nvram_match_x("LANHostConfig","time_zone", "UCT-13","selected"); %>>(GMT+13:00) Nuku'alofa</option></select></td>
</tr>
<tr>
<td class="content_input_td" colspan="2">
<table>
<tr>
<td width="500" height="100"></td><td>
<div align="center">
<font face="Arial"><input class=inputSubmit onMouseOut="buttonOut(this)" onMouseOver="buttonOver(this)" onClick="toPrevTag('Country')" type="hidden" value="<#CTL_prev#>" name="action1"></font>&nbsp;&nbsp;
<font face="Arial"> <input class=inputSubmit onMouseOut="buttonOut(this)" onMouseOver="buttonOver(this)" onClick="toTag('Country')" type="button" value="<#CTL_next#>" name="action"></font>
</div>
</td>
</tr>
<tr><td colspan="2" height="360"></td></tr>
</table>
</td>
</tr>
<tr id="Broadband" class="content_header_tr">
<td class="content_header_td_title" colspan="2"><#BOP_title#></td>
</tr>
<tr class="content_section_header_tr">
<td class="content_section_header_td" colspan="2"><#BOP_ctype_title#></td>
</tr>
<tr>
<td class="content_desc_td" colspan="2" height="50">
<#BOP_ctype_desc#></td>
</tr>
<tr> 
          <td class="content_header_td_less" onMouseOut="return nd();" colspan="2">          
          <p></p>
          <p><input type="radio" checked name="x_WANType" value="1" class="content_input_fd" onClick="changeWANType()"><#BOP_ctype_item1#></p>          
          <p><input type="radio" checked name="x_WANType" value="2" class="content_input_fd" onClick="changeWANType()"><#BOP_ctype_item2#></p>          
          <p><input type="radio" checked name="x_WANType" value="3" class="content_input_fd" onClick="changeWANType()"><#BOP_ctype_item3#></p>          
          <p><input type="radio" checked name="x_WANType" value="4" class="content_input_fd" onClick="changeWANType()"><#BOP_ctype_item4#></p>
          <p><input type="radio" checked name="x_WANType" value="5" class="content_input_fd" onClick="changeWANType()"><#BOP_ctype_item5#></p>
          <p></p>
          <p></p>
          </td>
</tr>
<tr>
<td class="content_input_td" colspan="2">
<table>
<tr>
<td width="444" height="100"></td><td>
<div align="center">
<font face="Arial"><input class=inputSubmit onMouseOut="buttonOut(this)" onMouseOver="buttonOver(this)" onClick="toPrevTag('Broadband')" type="button" value="<#CTL_prev#>" name="action"></font>&nbsp;&nbsp;
<font face="Arial"> <input class=inputSubmit onMouseOut="buttonOut(this)" onMouseOver="buttonOver(this)" onClick="toTag('Broadband')" type="button" value="<#CTL_next#>" name="action"></font>
</div>
</td>
</tr>
<tr><td colspan="2" height="360"></td></tr>
</table>
</td>
</tr>
<tr id="PPPoE" class="content_header_tr">
<td class="content_header_td_title" colspan="2"><#BOP_title#></td>
</tr>
<tr class="content_section_header_tr">
<td  class="content_section_header_td" colspan="2"><#BOP_account_title#></td>
</tr>
<tr>
<td class="content_desc_td" colspan="2" height="50"><#BOP_account_desc#></td>
</tr>
<tr>
<td class="content_header_td_less"><#BOP_account_user_item#></td><td class="content_input_td"><input type="text" maxlength="64" size="32" name="wan_pppoe_username" class="content_input_fd" value="<% nvram_get_x("PPPConnection","wan_pppoe_username"); %>"></td>
</tr>
<tr>
<td class="content_header_td_less"><#BOP_account_pass_item#></td><td class="content_input_td"><input type="password" maxlength="64" size="32" name="wan_pppoe_passwd" class="content_input_fd" value="<% nvram_get_x("PPPConnection","wan_pppoe_passwd"); %>"></td>
</tr>
<tr>
<td class="content_input_td" colspan="2">
<table>
<tr>
<td width="444" height="100"></td>
<td>
<div align="center">
<font face="Arial"><input class=inputSubmit onMouseOut="buttonOut(this)" onMouseOver="buttonOver(this)" onClick="toPrevTag('PPPoE')" type="button" value="<#CTL_prev#>" name="action"></font>&nbsp;&nbsp;
<font face="Arial"><input class=inputSubmit onMouseOut="buttonOut(this)" onMouseOver="buttonOver(this)" onClick="toTag('PPPoE')" type="button" value="<#CTL_next#>" name="action"></font>
</div>
</td>
</tr>
<tr><td colspan="2" height="360"></td></tr>
</table>
</td>
</tr>
<tr id="MacHost" class="content_header_tr">
<td class="content_header_td_title" colspan="2"><#BOP_title#></td>
</tr>
<tr class="content_section_header_tr">
<td  class="content_section_header_td" colspan="2"><#BOP_isp_title#></td>
</tr>
<tr>
<td class="content_desc_td" colspan="2" height="50"><#BOP_isp_desc#></td>
</tr>
<tr>
<td class="content_header_td_less" onMouseOver="return overlib('<#BOP_isp_host_desc#>', LEFT);" onMouseOut="return nd();"><#BOP_isp_host_item#></td><td class="content_input_td"><input type="text" maxlength="32" size="32" name="wan_hostname" class="content_input_fd" value="<% nvram_get_x("PPPConnection","wan_hostname"); %>"></td>
</tr>
<tr>
<td class="content_header_td_less" onMouseOver="return overlib('<#BOP_isp_mac_desc#>', LEFT);" onMouseOut="return nd();"><#BOP_isp_mac_item#></td><td class="content_input_td"><input type="text" maxlength="12" size="12" name="wan_hwaddr_x" class="content_input_fd" value="<% nvram_get_x("PPPConnection","wan_hwaddr_x"); %>" onBlur="return validate_hwaddr(this)" onKeyPress="return is_hwaddr()"></td>
</tr>
<tr>
<td class="content_header_td_less" onMouseOver="return overlib('<#BOP_isp_heart_desc#>', LEFT);" onMouseOut="return nd();"><#BOP_isp_heart_item#></td><td class="content_input_td"><input type="text" maxlength="32" size="32" name="wan_heartbeat_x" class="content_input_fd" value="<% nvram_get_x("PPPConnection","wan_heartbeat_x"); %>"></td>
</tr>
<tr>
<td class="content_input_td_less" colspan="2">
<table>
<tr>
<td width="444" height="100"></td>
<td>
<div align="center">
<font face="Arial"><input class=inputSubmit onMouseOut="buttonOut(this)" onMouseOver="buttonOver(this)" onClick="toPrevTag('MacHost')" type="button" value="<#CTL_prev#>" name="action"></font>&nbsp;&nbsp;
<font face="Arial"><input class=inputSubmit onMouseOut="buttonOut(this)" onMouseOver="buttonOver(this)" onClick="toTag('MacHost')" type="button" value="<#CTL_next#>" name="action"></font>
</div>
</td>
</tr>
<tr><td colspan="2" height="360"></td></tr>
</table>
</td>
</tr>
<tr id="WANSetting" class="content_header_tr">
<td class="content_header_td_title" colspan="2"><#BOP_title#></td>
</tr>
<tr class="content_section_header_tr">
<td class="content_section_header_td" colspan="2"><#BOP_wan_title#></td>
</tr>
<tr>
<td class="content_desc_td" colspan="2" height="50"><#BOP_wan_desc#></td>
</tr>
<tr>
<td class="content_header_td_less"><#BOP_wan_dhcp_item#></td><td class="content_input_td"><input type="radio" value="1" name="x_DHCPClient" class="content_input_fd" onClick="changeDHCPClient()">Yes</input><input type="radio" value="0" name="x_DHCPClient" class="content_input_fd" onClick="changeDHCPClient()">No</input></td>
</tr>
<tr>
<td class="content_header_td_less" onMouseOver="return overlib('<#BOP_wan_ip_desc#>', LEFT);" onMouseOut="return nd();">IP Address:</td><td class="content_input_td"><input type="text" maxlength="15" size="15" name="wan_ipaddr" class="content_input_fd" value="<% nvram_get_x("IPConnection","wan_ipaddr"); %>" onBlur="return validate_ipaddr(this, 'wan_ipaddr')" onKeyPress="return is_ipaddr(this)"></td>
</tr>
<tr>
<td class="content_header_td_less"><#BOP_wan_sb_item#></td><td class="content_input_td"><input type="text" maxlength="15" size="15" name="wan_netmask" class="content_input_fd" value="<% nvram_get_x("IPConnection","wan_netmask"); %>" onBlur="return validate_ipaddr(this, 'wan_netmask')" onKeyPress="return is_ipaddr(this)"></td>
</tr>
<tr>
<td class="content_header_td_less"><#BOP_wan_gw_item#></td><td class="content_input_td"><input type="text" maxlength="15" size="15" name="wan_gateway" class="content_input_fd" value="<% nvram_get_x("IPConnection","wan_gateway"); %>" onBlur="return validate_ipaddr(this, 'wan_gateway')" onKeyPress="return is_ipaddr(this)"></td>
</tr>
<tr>
<td class="content_header_td_less"><#BOP_wan_dns_item#></td><td class="content_input_td"><input type="radio" value="1" name="wan_dnsenable_x" class="content_input_fd" onClick="changeDNSServer()" <% nvram_match_x("IPConnection","wan_dnsenable_x", "1", "checked"); %>>Yes</input><input type="radio" value="0" name="wan_dnsenable_x" class="content_input_fd" onClick="changeDNSServer()" <% nvram_match_x("IPConnection","wan_dnsenable_x", "0", "checked"); %>>No</input></td>
</tr>
<tr>
<td class="content_header_td_less"><#BOP_wan_dns1_item#></td><td class="content_input_td"><input type="text" maxlength="15" size="15" name="wan_dns1_x" class="content_input_fd" value="<% nvram_get_x("IPConnection","wan_dns1_x"); %>" onBlur="return validate_ipaddr(this)" onKeyPress="return is_ipaddr(this)"></td>
</tr>
<tr>
<td class="content_header_td_less"><#BOP_wan_dns2_item#></td><td class="content_input_td"><input type="text" maxlength="15" size="15" name="wan_dns2_x" class="content_input_fd" value="<% nvram_get_x("IPConnection","wan_dns2_x"); %>" onBlur="return validate_ipaddr(this)" onKeyPress="return is_ipaddr(this)"></td>
</tr>
<tr>
<td class="content_input_td" colspan="2">
<table>
<tr>
<td width="444" height="100"></td><td>
<div align="center">
<font face="Arial"><input class=inputSubmit onMouseOut="buttonOut(this)" onMouseOver="buttonOver(this)" onClick="toPrevTag('WANSetting')" type="button" value="<#CTL_prev#>" name="action"></font>&nbsp;&nbsp;
<font face="Arial"><input class=inputSubmit onMouseOut="buttonOut(this)" onMouseOver="buttonOver(this)" onClick="toTag('WANSetting')" type="button" value="<#CTL_next#>" name="action"></font>
</div>
</td>
</tr>
<tr><td colspan="2" height="360"></td></tr>
</table>
</td>
</tr>

<tr  id="Wireless"  class="content_header_tr">
<td class="content_header_td_title" colspan="2"><#BOP_title#></td>
</tr>
<tr class="content_section_header_tr">
<td class="content_section_header_td" colspan="2"><#BOP_wlan_title#></td>
</tr>
<tr>
<td class="content_desc_td" colspan="2" height="50"><#BOP_wlan_desc#></td>
</tr>
<tr>
<td class="content_header_td_less" onMouseOver="return overlib('<#BOP_wlan_ssid_desc#>', LEFT);" onMouseOut="return nd();"><#BOP_wlan_ssid_item#></td><td class="content_input_td"><input type="text" maxlength="32" size="32" name="wl_ssid" class="content_input_fd" value="<% nvram_get_x("WLANConfig11b","wl_ssid"); %>"  onBlur="validate_string(this)"></td>
</tr>
<tr>
<td class="content_header_td_less" onMouseOver="return overlib('<#BOP_wlan_sec_desc#>', LEFT);" onMouseOut="return nd();"><#BOP_wlan_sec_item#></td>
<td class="content_input_td">
 <select name="SecurityLevel" class="content_input_fd" onChange="return change_security(this, 'WLANConfig11b', 0)">
     <option value="0">Low(None)</option>
     <option value="1">Medium(WEP-64bits)</option>
     <option value="2">Medium(WEP-128bits)</option>
     <option value="3">High(WPA-PSK)</option>
 </select>    
</td>
</tr>
<tr>
<td class="content_header_td_less" onMouseOver="return overlib('<#BOP_wlan_pass_desc#>', LEFT);" onMouseOut="return nd();"><#BOP_wlan_pass_item#></td><td class="content_input_td"><script language="JavaScript" type="text/javascript" src="phrase.js"></script><script language="JavaScript" type="text/javascript" src="md5.js"></script><input type="password" maxlength="64" size="32" name="wl_wpa_psk" class="content_input_fd"  value="<% nvram_get_x("WLANConfig11b","wl_wpa_psk"); %>" onKeyUp="return is_wlphrase_q('WLANConfig11b', this)" onBlur="return validate_wlphrase_q('WLANConfig11b', this)"></td>
</tr>
<tr>
<td class="content_header_td_less"><#BOP_wlan_wep1_item#></td><td class="content_input_td"><input type="pssword" maxlength="32" size="32" name="wl_key1" class="content_input_fd" value="<% nvram_get_x("WLANConfig11b","wl_key1"); %>" onBlur="return validate_wlkey(this, 'WLANConfig11b')" onKeyPress="return is_wlkey(this, 'WLANConfig11b')" onKeyUp="return change_wlkey(this, 'WLANConfig11b')"></td>
</tr>				   
<tr>
<td class="content_header_td_less"><#BOP_wlan_wep2_item#></td><td class="content_input_td"><input type="pssword" maxlength="32" size="32" name="wl_key2" class="content_input_fd" value="<% nvram_get_x("WLANConfig11b","wl_key2"); %>" onBlur="return validate_wlkey(this, 'WLANConfig11b')" onKeyPress="return is_wlkey(this, 'WLANConfig11b')" onKeyUp="return change_wlkey(this, 'WLANConfig11b')"></td>
</tr>
<tr>
<td class="content_header_td_less"><#BOP_wlan_wep3_item#></td><td class="content_input_td"><input type="pssword" maxlength="32" size="32" name="wl_key3" class="content_input_fd" value="<% nvram_get_x("WLANConfig11b","wl_key3"); %>" onBlur="return validate_wlkey(this, 'WLANConfig11b')" onKeyPress="return is_wlkey(this, 'WLANConfig11b')" onKeyUp="return change_wlkey(this, 'WLANConfig11b')"></td>
</tr>
<tr>
<td class="content_header_td_less"><#BOP_wlan_wep4_item#></td><td class="content_input_td"><input type="pssword" maxlength="32" size="32" name="wl_key4" class="content_input_fd" value="<% nvram_get_x("WLANConfig11b","wl_key4"); %>" onBlur="return validate_wlkey(this, 'WLANConfig11b')" onKeyPress="return is_wlkey(this, 'WLANConfig11b')" onKeyUp="return change_wlkey(this, 'WLANConfig11b')"></td>
</tr>
<tr>
<td class="content_header_td_less"><#BOP_wlan_defkey_item#></td><td class="content_input_td"><select name="wl_key" class="content_input_fd" onChange="return change_common(this, 'WLANConfig11b', 'wl_key')"><option value="1" <% nvram_match_x("WLANConfig11b","wl_key", "1","selected"); %>>1</option><option value="2" <% nvram_match_x("WLANConfig11b","wl_key", "2","selected"); %>>2</option><option value="3" <% nvram_match_x("WLANConfig11b","wl_key", "3","selected"); %>>3</option><option value="4" <% nvram_match_x("WLANConfig11b","wl_key", "4","selected"); %>>4</option></select></td>
</tr>
</table>
</td>
</tr>

<tr>
<td>		
<table width="666" border="1" cellpadding="0" cellspacing="0" bordercolor="B0B0B0">
<tr bgcolor="#CCCCCC"><td colspan="3"><font face="arial" size="2"><b>&nbsp</b></font></td></tr>
<tr bgcolor="#FFFFFF">  
   <td height="25" width="34%">  
   </td>
   <td height="25" width="33%">  
   </td>
   <td height="25" width="33%">  
   <div align="center">
   <font face="Arial"><input class=inputSubmit onMouseOut="buttonOut(this)" onMouseOver="buttonOver(this)" onClick="toPrevTag('Wireless')" type="button" value="<#CTL_prev#>" name="action"></font>&nbsp;&nbsp;&nbsp;&nbsp;
   <font face="Arial"><input class=inputSubmit onMouseOut=buttonOut(this) onMouseOver="buttonOver(this)" type="submit" value="<#CTL_finish#>" name="action"  onClick="saveQuick(this)"></font></div>
   </td>
</tr>
</table>
</td>
</tr>

<tr>
<td>
<table width="666" border="1" cellpadding="0" cellspacing="0" bordercolor="B0B0B0">
<tr>
    <td colspan="2" width="666" height="25" bgcolor="#FFBB00"></td> 
</tr>                   
</table>
</td>
</tr>
<tr><td colspan="2" height="240"></td></tr>
</table>
</form>

