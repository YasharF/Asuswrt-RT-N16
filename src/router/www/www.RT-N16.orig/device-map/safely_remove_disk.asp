<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">

<script type="text/javascript">
function safely_remove_disk_error(error_msg){
	parent.alert_error_msg(error_msg);
}

function safely_remove_disk_success(){
	parent.showLoading(5);
	parent.refreshpage(5);
}
</script>
</head>

<body>

<% safely_remove_disk(); %>

</body>
</html>
