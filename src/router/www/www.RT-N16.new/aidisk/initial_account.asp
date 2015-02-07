<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">

<script>
function initial_account_error(error_msg){
	parent.alert_error_msg(error_msg);
}

function initial_account_success(){
	parent.resultOfInitialAccount();
}
</script>
</head>

<body>

<% initial_account(); %>

</body>
</html>
