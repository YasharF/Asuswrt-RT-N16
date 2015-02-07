//mssidlist
// [[Index, Selected, wl_ssid, wl_ssid_x_enable, wl_auth_mode, wl_wep_x, wl_key, wl_key1, wl_key2, wl_key3, wl_key4, wl_crypto, wl_wpa_psk, wl_wpa_mode], [...], ...]
var mssidlist=[[0, "1",
                "",
                "1",
                "<% nvram_get_x("WLANConfig11b", "wl_auth_mode"); %>",
                "<% nvram_get_x("WLANConfig11b", "wl_wep_x"); %>",
                <% nvram_get_x("WLANConfig11b", "wl_key"); %>,
                "<% nvram_get_x("WLANConfig11b", "wl_key1"); %>",
                "<% nvram_get_x("WLANConfig11b", "wl_key2"); %>",
                "<% nvram_get_x("WLANConfig11b", "wl_key3"); %>",
                "<% nvram_get_x("WLANConfig11b", "wl_key4"); %>",
                "<% nvram_get_x("WLANConfig11b", "wl_crypto"); %>",
                "<% nvram_get_x("WLANConfig11b", "wl_wpa_psk"); %>",
                "<% nvram_get_x("WLANConfig11b", "wl_wpa_mode"); %>"
               ],
               [1, "0",
                "",
                "<% nvram_get_x("WLANConfig11b", "mbss1_enabled"); %>",
                "<% nvram_get_x("WLANConfig11b", "mbss1_auth_mode"); %>",
                "<% nvram_get_x("WLANConfig11b", "mbss1_wep_x"); %>",
                <% nvram_get_x("WLANConfig11b", "mbss1_key"); %>,
                "<% nvram_get_x("WLANConfig11b", "mbss1_key1"); %>",
                "<% nvram_get_x("WLANConfig11b", "mbss1_key2"); %>",
                "<% nvram_get_x("WLANConfig11b", "mbss1_key3"); %>",
                "<% nvram_get_x("WLANConfig11b", "mbss1_key4"); %>",
                "<% nvram_get_x("WLANConfig11b", "mbss1_crypto"); %>",
                "<% nvram_get_x("WLANConfig11b", "mbss1_wpa_psk"); %>",
                "<% nvram_get_x("WLANConfig11b", "mbss1_wpa_mode"); %>",
                "<% nvram_get_x("WLANConfig11b", "mbss1_nolan"); %>",
                "<% nvram_get_x("WLANConfig11b", "mbss1_nowan"); %>",
                "<% nvram_get_x("WLANConfig11b", "mbss1_priority"); %>",
                "<% nvram_get_x("WLANConfig11b", "mbss1_closed"); %>",
                "8"
               ],
               [2, "0",
                "",
                "<% nvram_get_x("WLANConfig11b", "mbss2_enabled"); %>",
                "<% nvram_get_x("WLANConfig11b", "mbss2_auth_mode"); %>",
                "<% nvram_get_x("WLANConfig11b", "mbss2_wep_x"); %>",
                <% nvram_get_x("WLANConfig11b", "mbss2_key"); %>,
                "<% nvram_get_x("WLANConfig11b", "mbss2_key1"); %>",
                "<% nvram_get_x("WLANConfig11b", "mbss2_key2"); %>",
                "<% nvram_get_x("WLANConfig11b", "mbss2_key3"); %>",
                "<% nvram_get_x("WLANConfig11b", "mbss2_key4"); %>",
                "<% nvram_get_x("WLANConfig11b", "mbss2_crypto"); %>",
                "<% nvram_get_x("WLANConfig11b", "mbss2_wpa_psk"); %>",
                "<% nvram_get_x("WLANConfig11b", "mbss2_wpa_mode"); %>",
                "<% nvram_get_x("WLANConfig11b", "mbss2_nolan"); %>",
                "<% nvram_get_x("WLANConfig11b", "mbss2_nowan"); %>",
                "<% nvram_get_x("WLANConfig11b", "mbss2_priority"); %>",
                "<% nvram_get_x("WLANConfig11b", "mbss2_closed"); %>",
                "12"
               ],
               [3, "0",
                "",
                "<% nvram_get_x("WLANConfig11b", "mbss3_enabled"); %>",
                "<% nvram_get_x("WLANConfig11b", "mbss3_auth_mode"); %>",
                "<% nvram_get_x("WLANConfig11b", "mbss3_wep_x"); %>",
                <% nvram_get_x("WLANConfig11b", "mbss3_key"); %>,
                "<% nvram_get_x("WLANConfig11b", "mbss3_key1"); %>",
                "<% nvram_get_x("WLANConfig11b", "mbss3_key2"); %>",
                "<% nvram_get_x("WLANConfig11b", "mbss3_key3"); %>",
                "<% nvram_get_x("WLANConfig11b", "mbss3_key4"); %>",
                "<% nvram_get_x("WLANConfig11b", "mbss3_crypto"); %>",
                "<% nvram_get_x("WLANConfig11b", "mbss3_wpa_psk"); %>",
                "<% nvram_get_x("WLANConfig11b", "mbss3_wpa_mode"); %>",
                "<% nvram_get_x("WLANConfig11b", "mbss3_nolan"); %>",
                "<% nvram_get_x("WLANConfig11b", "mbss3_nowan"); %>",
                "<% nvram_get_x("WLANConfig11b", "mbss3_priority"); %>",
                "<% nvram_get_x("WLANConfig11b", "mbss3_closed"); %>",
                "24"
               ]               
              ];
              
//var mssidlist_orig = mssidlist;

function get_free_mssid(){ //return which SSID is not set.
	for(var i=1; i<mssidlist.length; i++){
		if(mssidlist[i][2].indexOf("ASUS_MSSID_") > -1){ // No.i is free for add.
			return i;		
		}
		else{
			continue;
		}
	}
	return 0; // There are no free ssid.
}

function mssid_duplicate(obj){
	for(var i=0; i<mssidlist.length; i++){
		//alert("ssid_order: "+ssid_order +"\ni: "+i);
		if(obj.value == mssidlist[i][2] && ssid_order != i){
			if(confirm("The SSID is duplicate with exist one. \nWe suggest that you use another.\n\n Are you sure you want to continue?")){
				document.form.wl_ssid.focus();
				document.form.wl_ssid.select();
				return false;
			}
			else{
				return true;
			}
		}
	}
	return true;
}

function add_ssid_event(){
	if(free_mssid == 0){
		alert("Your have used 4 set of SSID.\n\n Click red arrow to pull down select menu to modify multiple-SSID settings.");
	}
	else{
		stopFlag=1;
		parent.disableCheckChangedStatus();
		parent.popupWindow('OverlayMask','/device-map/add-mssid.asp')	;
	}
}

function pullMSSIDList(obj){
	//alert("pullMSSIDList" + isMenuopen);
	if(isMenuopen == 0){		
		$("mssid_arrow").style.background = "url(../images/arrow-top.gif) center right no-repeat";
		$("MSSIDList_Block").style.display = 'block';
		isMenuopen = 1;
		document.form.wl_ssid.focus();
		show_mssid(mssidlist);
	}
	else{
		hideMSSID_Block();
		//$("pull_arrow").src = "/images/arrow-down.gif";
		//$('ClientList_Block').style.display='none';
		//isMenuopen = 0;
	}
}
var over_var = 0;
var isMenuopen = 0;
var isCheckboxClicked = 0;
/*document.onmouseup = function hidemenu_when_click_body(){
	$("pull_arrow").src = "/images/arrow-down.gif";
	$('ClientList_Block').style.display='none';
	isMenuopen = 0;
}*/

function hideMSSID_Block(){
	
	//if(over_var == 0){
		$('mssid_arrow').style.background = "url(../images/arrow-down.gif) center right no-repeat";
		$('MSSIDList_Block').style.display= "none";
		isMenuopen = 0;
	//}
}

var ssid_order = 0;  //which ssid is selected;
var am_option = new Array(4);
	for(var i=0; i<am_option.length; i++)
		am_option[i] = document.createElement("option");
			
	am_option[0].value = "wpa";
	am_option[1].value = "wpa2";
	am_option[2].value = "wpa";
	//am_option[3].value = "radius";
	am_option[0].text = "WPA-Enterprise";
	am_option[1].text = "WPA2-Enterprise";
	am_option[2].text = "WPA-Auto-Enterprise";
	//am_option[3].text = "Radius with 802.1x";

function setMSSID(obj){
	
	if(isCheckboxClicked == 1){  //When user click checkbox(enable ssid), return nothing;
		return;
	}
	
	if(!save_temp_ssid_setting(ssid_order)){ //Save current setting to array.
		return false;
	}
	
	mssidlist[ssid_order][1] = "0"; // set last clicked ssid to be unselected.
	
	ssid_order = obj.title;  //This ssid is selected.
	
	$("ssid_check"+ssid_order).id = document.form.ssid_check.id;
	
	document.form.ssid_check.id = "ssid_check"+ssid_order;
	document.form.ssid_check.checked = (mssidlist[ssid_order][3]==1)?true:false;
	document.form.ssid_check.disabled = (mssidlist[ssid_order][0]==0)?"disabled":"";
		
	//$("selected_order").innerHTML = obj.title;  //show order;
	
	document.form.wl_ssid.value = obj.childNodes[0].innerHTML;
	
	// mssid without wpa & radius{		
	if(mssidlist[ssid_order][0] != 0){
		//$("applySecurity").style.visibility = "visible";
		//$("auth_mode").remove(8);
		$("auth_mode").remove(7);
		$("auth_mode").remove(6);
		$("auth_mode").remove(5);
	}
	else{
		//$("applySecurity").style.visibility = "hidden";
		try{
    	$("auth_mode").add(am_option[0],null); // standards compliant
    	$("auth_mode").add(am_option[1],null);
    	$("auth_mode").add(am_option[2],null);
    	//$("auth_mode").add(am_option[3],null);
    }
  	catch(ex){
  		$("auth_mode").add(am_option[0],5);// IE only
  		$("auth_mode").add(am_option[1],6);
  		$("auth_mode").add(am_option[2],7);
  		//$("auth_mode").add(am_option[3],8);
    }
	}
	// }
	
	switch(mssidlist[ssid_order][4])
  {
   case "open":
     document.form.wl_auth_mode[0].selected = true;
     break
   case "shared":
     document.form.wl_auth_mode[1].selected = true;
     break
   case "psk":
			if(mssidlist[ssid_order][13] == "0")
				document.form.wl_auth_mode[2].selected = true;
		 	else if(mssidlist[ssid_order][13] == "1")
				document.form.wl_auth_mode[3].selected = true;
			else if(mssidlist[ssid_order][13] == "2")
				document.form.wl_auth_mode[4].selected = true;
     break
   default:
     document.form.wl_auth_mode[0].selected = true;
	}
		
	change_auth_mode(document.form.wl_auth_mode);
	
	if(mssidlist[ssid_order][4] == "open"){
		
		if(mssidlist[ssid_order][5] == "0")
			document.form.wl_wep_x[0].selected = true;
		else if(mssidlist[ssid_order][5] == "1")
			document.form.wl_wep_x[1].selected = true;
		else if(mssidlist[ssid_order][5] == "2")
			document.form.wl_wep_x[2].selected = true;
		
		change_wlweptype(document.form.wl_wep_x);	

		document.form.wl_wep_x.selectedIndex = parseInt(mssidlist[ssid_order][5]);
		if(mssidlist[ssid_order][5] != "0"){
			document.form.wl_key.selectedIndex = mssidlist[ssid_order][6]-1;
			document.form.wl_asuskey1.value = mssidlist[ssid_order][6+parseInt(mssidlist[ssid_order][6])];
		}
	}
	else if(mssidlist[ssid_order][4] == "shared"){
		//change_wlweptype(document.form.wl_wep_x);

		document.form.wl_wep_x.selectedIndex = mssidlist[ssid_order][5]-1;
		document.form.wl_key.selectedIndex = mssidlist[ssid_order][6]-1;
		document.form.wl_asuskey1.value = mssidlist[ssid_order][6+parseInt(mssidlist[ssid_order][6])];
	}
	else if(mssidlist[ssid_order][4] == "psk" && mssidlist[ssid_order][13] == "1"){
		document.form.wl_auth_mode.selectedIndex = 2;
		change_auth_mode(document.form.wl_auth_mode);
		document.form.wl_wpa_psk.value = mssidlist[ssid_order][12];
	}
	else if(mssidlist[ssid_order][4] == "psk" && mssidlist[ssid_order][13] == "2"){
		document.form.wl_auth_mode.selectedIndex = 3;
		change_auth_mode(document.form.wl_auth_mode);
		document.form.wl_wpa_psk.value = mssidlist[ssid_order][12];
		//document.form.wl_auth_mode[3].selected = true;
	}
	else if(mssidlist[ssid_order][4] == "psk" && mssidlist[ssid_order][13] == "0"){  //WPA-Auto-Personal
		document.form.wl_auth_mode.selectedIndex = 4;
		change_auth_mode(document.form.wl_auth_mode);
		if(mssidlist[ssid_order][11] == "tkip")
			document.form.wl_crypto.selectedIndex = 0;
		else if(mssidlist[ssid_order][11] == "aes")
			document.form.wl_crypto.selectedIndex = 1;
		else
			document.form.wl_crypto.selectedIndex = 2;
			
		document.form.wl_wpa_psk.value = mssidlist[ssid_order][12];
	}
	else if(mssidlist[ssid_order][4] == "wpa"){       
		if(mssidlist[ssid_order][13] == "3"){           //WPA enterprise
			document.form.wl_auth_mode.selectedIndex = 5;
			change_auth_mode(document.form.wl_auth_mode);
		}
		else if(mssidlist[ssid_order][13] == "4"){      //WPA auto enterprise
			document.form.wl_auth_mode.selectedIndex = 7;
			
			change_auth_mode(document.form.wl_auth_mode);
			if(mssidlist[ssid_order][12] == "tkip")
				document.form.wl_crypto.selectedIndex = 0;
			else if(mssidlist[ssid_order][12] == "aes")
				document.form.wl_crypto.selectedIndex = 1;
			else
				document.form.wl_crypto.selectedIndex = 2;
		}
	}
	else if(mssidlist[ssid_order][4] == "wpa2"){
		document.form.wl_auth_mode.selectedIndex = 6;
		change_auth_mode(document.form.wl_auth_mode);
	}
	else if(mssidlist[ssid_order][4] == "radius"){
		document.form.wl_auth_mode.selectedIndex = 8;
		change_auth_mode(document.form.wl_auth_mode);
	}	

	
	$("mssid_qos_tr").style.display = (mssidlist[ssid_order][0] != 0)?"":"none";
	$("mssid_network_tr").style.display = (mssidlist[ssid_order][0] != 0)?"":"none";
	$("mssid_closed_tr").style.display = (mssidlist[ssid_order][0] != 0)?"":"none";
	
	$("wps_pbc_tr").style.display = (mssidlist[ssid_order][0] == 0)?"":"none";
	//$("mssid_time_tr").style.display = (mssidlist[ssid_order][0] != 0)?"":"none";
	
	//load mssid setting in UI.
	if(mssidlist[ssid_order][14] == 0 && mssidlist[ssid_order][15] == 0){
		document.form.mssid_network.selectedIndex = 0;
	}
	else if(mssidlist[ssid_order][14] == 0 && mssidlist[ssid_order][15] == 1)
		document.form.mssid_network.selectedIndex = 1;
	else if(mssidlist[ssid_order][14] == 1 && mssidlist[ssid_order][15] == 0)
		document.form.mssid_network.selectedIndex = 2;
	
	document.form.priority.selectedIndex = (mssidlist[ssid_order][16] == 1)?"0":"1"; //priority

	if(mssidlist[ssid_order][17] == 0) //closed
		document.form.closed[1].checked = true;
	else if(mssidlist[ssid_order][17] == 1)
		document.form.closed[0].checked = true;
	
	//document.form.mssid_time.selectedIndex = parseInt(mssidlist[ssid_order][18])/2-1; //validity time;
	
	/*
	if(mssidlist[ssid_order][3] == "1"){
		document.form.wl_ssid_x_enable[0].checked = true;
	}
	else{
		document.form.wl_ssid_x_enable[1].checked = true;
	}*/
	
	/*for(var i=0; i<mssidlist.length; i++){ //先把全部設為非被選擇
		mssidlist[i][1] = "0";		
	}*/
	mssidlist[ssid_order][1] = 1; //再將被選擇項設好
	
	hideMSSID_Block();
	over_var = 0;
	isMenuopen = 1;
}

function default_mssid_block(){
	
	for(j=1; j<mssidlist.length; j++){
			$("other_ssid"+j).onmouseover = function(){
				this.style.backgroundColor = "#FFFFCC";
				over_var=1;
			}
			$("other_ssid"+j).onmouseout = function(){
				this.style.backgroundColor = "#cae9f3";
				over_var=0;
			}
			$("other_ssid"+j).onclick = function(){
				setMSSID(this);
			}
	}
	document.form.ssid_check.onclick = function(){
		mssidlist[ssid_order][3] = (this.checked)?"1":"0";
		//$("mbss"+ssid_order+"_enabled").value = (this.checked)?"1":"0";
	}	
}

function show_mssid(id_group){
	var ssid_is_checked, ssid_html;

	for(var i=0, j=1; i<id_group.length; i++){
		var sn = id_group[i][0];

		if(id_group[i][1] == "0"){  // 其他ssid
			
			ssid_is_checked = (id_group[i][3]==1)?"checked":"";
			
			if(id_group[i][2].indexOf("ASUS_MSSID_") != 0){ // 秀出已經新增的SSID
				ssid_html = "<span id='ssid_span"+ sn +"'>"+id_group[i][2]+"</span>";
				ssid_html += "<input type='checkbox' id='ssid_check"+ sn +"' "+ ssid_is_checked + ">";
				$("other_ssid"+j).innerHTML = ssid_html;
				if(sn==0)
					$("ssid_check"+sn).disabled = true;
					
					$("ssid_span"+i).onclick = function(){
						isCheckboxClicked = 0;
				}
				$("ssid_check"+sn).onclick = function(){
					isCheckboxClicked = 1;
					over_var=0;
					id_group[this.parentNode.title][3] = (this.checked)?"1":"0";
					//$("mbss"+this.parentNode.title+"_enabled").value = (this.checked)?"1":"0";
				}
			}
			else{
				$("other_ssid"+j).style.display = "none";
			}
			
			//$("other_ssid"+j).style.color = (id_group[i][3] == 1)?"#008000":"#CC0000";
			$("other_ssid"+j).title = i;
			j++;
		}
	}
	default_mssid_block();
}

function save_temp_ssid_setting(LastSSID){

	var auth_mode = document.form.wl_auth_mode.value;
		
	if(!validate_string_ssid(document.form.wl_ssid))
		return false;
		
	if(!mssid_duplicate(document.form.wl_ssid))
		return false;
		
	if(auth_mode == "psk" || auth_mode == "wpa" || auth_mode == "wpa2"){
		if(!validate_psk(document.form.wl_wpa_psk))
			return false;
	}
	else{
		if(auth_mode!="radius" && !validate_wlkey(document.form.wl_asuskey1))
			return false;
	}
	
	
	mssidlist[LastSSID][2] = document.form.wl_ssid.value;
	mssidlist[LastSSID][4] = document.form.wl_auth_mode.value;
	
	if(auth_mode == "open" || auth_mode == "shared"){
		mssidlist[LastSSID][5] = document.form.wl_wep_x.value;
		if(document.form.wl_wep_x.value != "0"){
			mssidlist[LastSSID][6] = document.form.wl_key.value;
			$("key"+document.form.wl_key.value).value = document.form.wl_asuskey1.value;
			mssidlist[LastSSID][7] = document.form.wl_key1.value;
			mssidlist[LastSSID][8] = document.form.wl_key2.value;
			mssidlist[LastSSID][9] = document.form.wl_key3.value;
			mssidlist[LastSSID][10] = document.form.wl_key4.value;
		}
	}
	
	if(auth_mode == "psk" || auth_mode == "wpa" || auth_mode == "wpa2"){
		mssidlist[LastSSID][11] = document.form.wl_crypto.value;
		mssidlist[LastSSID][12] = document.form.wl_wpa_psk.value;
			
		if(document.form.wl_auth_mode.selectedIndex == 2)
			document.form.wl_wpa_mode.value = "1";
		else if(document.form.wl_auth_mode.selectedIndex == 3)
			document.form.wl_wpa_mode.value="2";
		else if(document.form.wl_auth_mode.selectedIndex == 4)
			document.form.wl_wpa_mode.value="0";
		else if(document.form.wl_auth_mode.selectedIndex == 5)
			document.form.wl_wpa_mode.value="3";
		else if(document.form.wl_auth_mode.selectedIndex == 7)
			document.form.wl_wpa_mode.value="4";
		else
			document.form.wl_wpa_mode.value="";
		
		mssidlist[LastSSID][13] = document.form.wl_wpa_mode.value;
	}
	
	if(document.form.mssid_network.selectedIndex == 0){
		mssidlist[LastSSID][14] = "0";
		mssidlist[LastSSID][15] = "0";
	}
	else if(document.form.mssid_network.selectedIndex == 1){
		mssidlist[LastSSID][14] = "0";
		mssidlist[LastSSID][15] = "1";
	}
	else if(document.form.mssid_network.selectedIndex == 2){
		mssidlist[LastSSID][14] = "1";
		mssidlist[LastSSID][15] = "0";
	}
	mssidlist[LastSSID][16] = document.form.priority.value;
	
	mssidlist[LastSSID][17] = (document.form.closed[0].checked)?"1":"0";
	
	//mssidlist[LastSSID][18] = document.form.mssid_time.value;
	//alert(mssidlist[LastSSID]);
	return true;
}

function set_mssid_network(obj){
	if(obj.selectedIndex == 0){
		$("nolan").value = "0";
		$("nowan").value = "0";
	}
	else if(obj.selectedIndex == 1){
		$("nolan").value = "0";
		$("nowan").value = "1";
	}
	else if(obj.selectedIndex == 2){
		$("nolan").value = "1";
		$("nowan").value = "0";		
	}
	else{
		return;	
	}
}

function applyForm2(){
	
	idx = ssid_order;	
	
	var auth_mode = document.form.wl_auth_mode.value;
		
	if(!validate_string_ssid(document.form.wl_ssid))
		return false;
		
	if(!mssid_duplicate(document.form.wl_ssid))
		return false;
		
	if(auth_mode == "psk" || auth_mode == "wpa" || auth_mode == "wpa2"){
		if(!validate_psk(document.form.wl_wpa_psk))
			return false;
	}
	else{
		if(auth_mode!="radius" && !validate_wlkey(document.form.wl_asuskey1))
			return false;
	}
/*
	$("ssid").name = "mbss"+idx+"_ssid";
	$("auth_mode").name = "mbss"+idx+"_auth_mode";
	$("wep_x").name = "mbss"+idx+"_wep_x";
	
	$("key1").name = "mbss"+idx+"_key1";
	$("key2").name = "mbss"+idx+"_key2";
	$("key3").name = "mbss"+idx+"_key3";
	$("key4").name = "mbss"+idx+"_key4";
	
	//var mbss_wep = eval('document.form.mbss_'+idx+'_key'+ document.form.wl_key.value);
	//mbss_wep.value = document.form.wl_asuskey1.value;
	
	//$j("#key"+document.form.wl_key.value).val(document.form.wl_asuskey1.value);
	$("key"+document.form.wl_key.value).value = document.form.wl_asuskey1.value;
	$("key").name = "mbss"+idx+"_key";
	
	$("crypto").name = "mbss"+idx+"_crypto";
	$("wpa_psk").name = "mbss"+idx+"_wpa_psk";
	
	//$("mbss_enable0").name = "mbss"+idx+"_enable";
	//$("mbss_enable1").name = "mbss"+idx+"_enable";
	
	$("nolan").name = "mbss"+idx+"_nolan";
	$("nowan").name = "mbss"+idx+"_nowan";
	
	$("priority").name = "mbss"+idx+"_priority";

	$("mbss_closed0").name = "mbss"+idx+"_closed";
	$("mbss_closed1").name = "mbss"+idx+"_closed";
	
	//alert(eval('document.form.mbss'+idx+'_ssid').value);
*/
	
	
	
	stopFlag = 1;
	parent.showLoading();
	
	document.form.current_page.value = "";
	document.form.next_page.value = "/";
	document.form.flag.value = "mssid";
	document.form.action_mode.value = " Update ";
	document.form.submit();
	
	return true;
}