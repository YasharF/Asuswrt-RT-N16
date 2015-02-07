// JavaScript Document 2007_12_20 Add by lock

function change_profile(obj){

	//var profile = obj.selectedIndex;
	var profile = obj.value;
	switch(profile){
		case "1":
			document.form.wl_ssid.value = "guest";
			document.form.wl_auth_mode.selectedIndex = 1;
			change_auth_mode(document.form.wl_auth_mode);
			document.form.mssid_network.selectedIndex = 2;
			document.form.priority.selectedIndex = 1;
			break;		
		case "2":
			document.form.wl_ssid.value = "VIP";
			document.form.wl_auth_mode.selectedIndex = 3;
			change_auth_mode(document.form.wl_auth_mode);
			document.form.mssid_network.selectedIndex = 0;
			document.form.priority.selectedIndex = 0;
			break;
		default:
			document.form.wl_ssid.value = "";
			document.form.wl_auth_mode.selectedIndex = 0;
			change_auth_mode(document.form.wl_auth_mode);
			document.form.mssid_network.selectedIndex = 0;
			document.form.priority.selectedIndex = 0;
			document.form.wl_ssid.focus();
	}
}

function domore_link(obj){
	parent.showLoading();
	parent.location.href = obj.value;
}
function wl_auth_mode_change(isload){
	var mode = document.form.wl_auth_mode.value;
	var opts = document.form.wl_auth_mode.options;
	var new_array;
	var cur_crypto;
	var cur_key_index, cur_key_obj;
	
	//if(mode == "open" || mode == "shared" || mode == "radius"){ //2009.03 magic
	if(mode == "open" || mode == "shared"){ //2009.03 magic
		blocking("all_related_wep", 1);
		change_wep_type(mode);
	}
	else{
		blocking("all_related_wep", 0);
	}
	
	/* enable/disable crypto algorithm */
	if(mode == "wpa" || mode == "wpa2" || mode == "psk")
		blocking("wl_crypto", 1);
	else
		blocking("wl_crypto", 0);
	
	/* enable/disable psk passphrase */
	if(mode == "psk")
		blocking("wl_wpa_psk", 1);
	else
		blocking("wl_wpa_psk", 0);
	
	/* update wl_crypto */
	for(var i = 0; i < document.form.wl_crypto.length; ++i)
		if(document.form.wl_crypto[i].selected){
			cur_crypto = document.form.wl_crypto[i].value;
			break;
		}
	
	/* Reconstruct algorithm array from new crypto algorithms */
	if(mode == "psk"){
		/* Save current crypto algorithm */
		if(isModel() == "SnapAP" || isBand() == 'b')
			new_array = new Array("TKIP");
		else{
			if(opts[opts.selectedIndex].text == "WPA-Personal")
				new_array = new Array("TKIP");
			else if(opts[opts.selectedIndex].text == "WPA2-Personal")
				new_array = new Array("AES");
			else
				new_array = new Array("TKIP", "AES", "TKIP+AES");
		}
		
		free_options(document.form.wl_crypto);
		for(var i in new_array){
			document.form.wl_crypto[i] = new Option(new_array[i], new_array[i].toLowerCase());
			document.form.wl_crypto[i].value = new_array[i].toLowerCase();
			if(new_array[i].toLowerCase() == cur_crypto)
				document.form.wl_crypto[i].selected = true;
		}
	}
	else if(mode == "wpa"){
		if(opts[opts.selectedIndex].text == "WPA-Enterprise")
			new_array = new Array("TKIP");
		else
			new_array = new Array("TKIP", "AES", "TKIP+AES");
		
		free_options(document.form.wl_crypto);
		for(var i in new_array){
			document.form.wl_crypto[i] = new Option(new_array[i], new_array[i].toLowerCase());
			document.form.wl_crypto[i].value = new_array[i].toLowerCase();
			if(new_array[i].toLowerCase() == cur_crypto)
				document.form.wl_crypto[i].selected = true;
		}
	}
	else if(mode == "wpa2"){
		new_array = new Array("AES");
		
		free_options(document.form.wl_crypto);
		for(var i in new_array){
			document.form.wl_crypto[i] = new Option(new_array[i], new_array[i].toLowerCase());
			document.form.wl_crypto[i].value = new_array[i].toLowerCase();
			if(new_array[i].toLowerCase() == cur_crypto)
				document.form.wl_crypto[i].selected = true;
		}
	}
	
	/* Save current network key index */
	for(var i = 0; i < document.form.wl_key.length; ++i)
		if(document.form.wl_key[i].selected){
			cur_key_index = document.form.wl_key[i].value;
			break;
		}
	
	/* Define new network key indices */
	//if(mode == "psk" || mode == "wpa" || mode == "wpa2" || mode == "radius")
	if(mode == "psk" || mode == "wpa" || mode == "wpa2")
		new_array = new Array("2", "3");
	else{
		new_array = new Array("1", "2", "3", "4");
		
		if(!isload)
			cur_key_index = "1";
	}
	
	/* Reconstruct network key indices array from new network key indices */
	free_options(document.form.wl_key);
	for(var i in new_array){
		document.form.wl_key[i] = new Option(new_array[i], new_array[i]);
		document.form.wl_key[i].value = new_array[i];
		if(new_array[i] == cur_key_index)
			document.form.wl_key[i].selected = true;
	}
	
	wl_wep_change();
}

function change_wep_type(mode){
	var cur_wep = document.form.wl_wep_x.value;
	var wep_type_array;
	var value_array;
	
	free_options(document.form.wl_wep_x);
	
	//if(mode == "shared" || mode == "radius"){ //2009.03 magic
	if(mode == "shared"){ //2009.03 magic
		wep_type_array = new Array("WEP-64bits", "WEP-128bits");
		value_array = new Array("1", "2");
	}
	else{
		wep_type_array = new Array("None", "WEP-64bits", "WEP-128bits");
		value_array = new Array("0", "1", "2");
	}
	
	add_options_x2(document.form.wl_wep_x, wep_type_array, value_array, cur_wep);

	if(mode == "open"){ //Lock Add 20091211;
		document.form.wl_wep_x.selectedIndex = 0;
	}	

	if(mode == "psk" || mode == "wpa" || mode == "wpa2") //2009.03 magic
	//if(mode == "psk" || mode == "wpa" || mode == "wpa2" || mode == "radius") //2009.03 magic
		document.form.wl_wep_x.value = "0";
	
	change_wlweptype(document.form.wl_wep_x);
}

function change_wlweptype(wep_type_obj){
	var mode = document.form.wl_auth_mode.value; //2009.03 magic
	
	//if(wep_type_obj.value == "0" || mode == "radius") //2009.03 magic
	if(wep_type_obj.value == "0")  //2009.03 magic
		blocking("all_wep_key", 0);
	else {
		if(document.form.wl_gmode.value == 2 && document.form.wl_wep_x.value != 0){
                        nmode_limitation();
                }
		blocking("all_wep_key", 1);
	}
	
	wl_wep_change();
}

function wl_wep_change(){
	var mode = document.form.wl_auth_mode.value;
	var wep = document.form.wl_wep_x.value;
	
	if(mode == "psk" || mode == "wpa" || mode == "wpa2"){
		if(mode == "psk"){
			blocking("wl_crypto", 1);
			blocking("wl_wpa_psk", 1);
		}
		
		blocking("all_related_wep", 0);
	}
	else{
		blocking("wl_crypto", 0);
		blocking("wl_wpa_psk", 0);
		
		//if(mode == "radius") //2009.03 magic
		//	blocking("all_related_wep", 0); //2009.03 magic
		//else //2009.03 magic
		//	blocking("all_related_wep", 1);
		
		if(wep == "0" || mode == "radius")
			blocking("all_wep_key", 0);
		else{
			blocking("all_wep_key", 1);
			
			show_key();
		}
	}
	
	change_key_des();
}

function change_key_des(){
	var objs = getElementsByName_iefix("span", "key_des");
	var wep_type = document.form.wl_wep_x.value;
	var str = "";
	
	if(wep_type == "1")
		str = " (<#WLANConfig11b_WEPKey_itemtype1#>)";
	else if(wep_type == "2")
		str = " (<#WLANConfig11b_WEPKey_itemtype2#>)";
	
	str += ":";
	
	for(var i = 0; i < objs.length; ++i)
		showtext(objs[i], str);
}

function change_auth_mode(auth_mode_obj){
	wl_auth_mode_change(0);
	
	if(auth_mode_obj.value == "psk" || auth_mode_obj.value == "wpa"){
		var opts = document.form.wl_auth_mode.options;
		
		if(opts[opts.selectedIndex].text == "WPA-Personal")
			document.form.wl_wpa_mode.value = "1";
		else if(opts[opts.selectedIndex].text == "WPA2-Personal")
			document.form.wl_wpa_mode.value="2";
		else if(opts[opts.selectedIndex].text == "WPA-Auto-Personal")
			document.form.wl_wpa_mode.value="0";
		else if(opts[opts.selectedIndex].text == "WPA-Enterprise")
			document.form.wl_wpa_mode.value="3";
		else if(opts[opts.selectedIndex].text == "WPA-Auto-Enterprise")
			document.form.wl_wpa_mode.value = "4";
		
		if(auth_mode_obj.value == "psk"){
			document.form.wl_wpa_psk.focus();
			document.form.wl_wpa_psk.select();
		}
	}
	else if(auth_mode_obj.value == "shared")
		show_key();	
        else{
                document.form.wl_wep_x.selectedIndex = 0;
                show_key();
                wl_wep_change();
        }
        nmode_limitation();

	//else if(auth_mode_obj.value == "shared" || auth_mode_obj.value == "radius")
}

function show_key(){
	var wep_type = document.form.wl_wep_x.value;
	var keyindex = document.form.wl_key.value;
	var cur_key_obj = eval("document.form.wl_key"+keyindex);
	var cur_key_length = cur_key_obj.value.length;
	
	if(wep_type == 1){
		if(cur_key_length == 5 || cur_key_length == 10)
			document.form.wl_asuskey1.value = cur_key_obj.value;
		else
			document.form.wl_asuskey1.value = "0000000000";
	}
	else if(wep_type == 2){
		if(cur_key_length == 13 || cur_key_length == 26)
			document.form.wl_asuskey1.value = cur_key_obj.value;
		else
			document.form.wl_asuskey1.value = "00000000000000000000000000";
	}
	else
		document.form.wl_asuskey1.value = "";
	
	document.form.wl_asuskey1.focus();
	document.form.wl_asuskey1.select();
}
