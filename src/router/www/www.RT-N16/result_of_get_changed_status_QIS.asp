<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<script>
<% wanlink(); %>

var flag = '<% get_parameter("flag"); %>';

var manually_stop_wan = '<% nvram_get_x("", "manually_disconnect_wan"); %>';
var ifWANConnect = <% detect_if_wan(); %>;
var detect_dhcp_pppoe = "<% detect_dhcp_pppoe(); %>";
var wan_status_log = "<% get_wan_status_log(); %>";

function initial(){
	parent.set_changed_status(manually_stop_wan,
														ifWANConnect,
														wanlink_statusstr(),
														detect_dhcp_pppoe,
														wan_status_log
														);
	
	parent.check_changed_status(flag);
}
</script>
</head>

<body onload="initial();">
</body>
</html>
