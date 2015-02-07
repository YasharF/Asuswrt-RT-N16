var aplist = <% nvram_dump("apscan",""); %>

var profile_ssid = '<% nvram_get_x("",  "sta_ssid"); %>';