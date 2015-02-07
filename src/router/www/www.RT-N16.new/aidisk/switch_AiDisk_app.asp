<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">

<script>
function set_AiDisk_status_error(error_msg){
	alert(error_msg);
	parent.resultOfSwitchAppStatus(error_msg);
}

function set_AiDisk_status_success(){
	parent.resultOfSwitchAppStatus();
}
</script>
</head>

<body>

<% set_AiDisk_status(); %>

</body>
</html>
