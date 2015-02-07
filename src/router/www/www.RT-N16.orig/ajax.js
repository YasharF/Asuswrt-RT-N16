// Use AJAX to get nvram
//var http_request = false;

function makeRequest(url) {
	//http_request = new XMLHttpRequest();
	var http_request = new XMLHttpRequest();
	
	if (http_request && http_request.overrideMimeType)
		http_request.overrideMimeType('text/xml');
	else
		return false;

	//http_request.onreadystatechange = alertContents;
	http_request.onreadystatechange = function(){
				alertContents(this);
			};
	http_request.open('GET', url, true);
	http_request.send(null);
}

var xmlDoc_ie;

function makeRequest_ie(file)
{
	xmlDoc_ie = new ActiveXObject("Microsoft.XMLDOM");
	xmlDoc_ie.async = false;
	if (xmlDoc_ie.readyState==4)
	{
		xmlDoc_ie.load(file);
		refresh_wpsinfo(xmlDoc_ie);
	}
}

//function alertContents()
function alertContents(request_obj)
{
	//if (http_request != null && http_request.readyState != null && http_request.readyState == 4)
	if (request_obj != null && request_obj.readyState != null && request_obj.readyState == 4)
	{
		//if (http_request.status != null && http_request.status == 200)
		if (request_obj.status != null && request_obj.status == 200)
		{
			//var xmldoc_mz = http_request.responseXML;
			var xmldoc_mz = request_obj.responseXML;
			refresh_wpsinfo(xmldoc_mz);
		}
	}
}
