	struct variable variables_Storage_SharedList[] = {
			{"sh_path", "45", validate_string, ARGV("32"), FALSE, 0x0},
			{"sh_name", "34", validate_string, ARGV("16"), FALSE, 0x0},
			{"sh_nameb", "", validate_string, ARGV("16"), FALSE, 0x0},
			{"sh_comment", "", validate_string, ARGV(""), FALSE, FALSE},
			{"sh_commentb", "", validate_string, ARGV(""), FALSE, FALSE},
			{"sh_acc_onlist_num", "", validate_string, ARGV("0","12"), FALSE, FALSE},
			{0,0,0,0,0,0}
		};
	
	struct variable variables_Storage_UserList[] = {
			{"acc_username", "41", validate_string, ARGV("28"), FALSE, FALSE},
			{"acc_password", "38", validate_string, ARGV("24"), FALSE, FALSE},
			{0,0,0,0,0,0}
		};
	
      //2005.12.16 Yau add
      struct variable variables_Storage_UserRights_List[] = {
                {"sh_acc_user", "34", validate_string, ARGV(""), FALSE, FALSE},
                {"sh_rright", "28", validate_string, ARGV(""), FALSE, FALSE},
                {"sh_wright", "28", validate_string, ARGV(""), FALSE, FALSE},
                {0,0,0,0,0,0}
      };

	struct variable variables_Language[] = {
			{"preferred_lang", "", validate_choice, ARGV(
					"EN:English",
					"TW:Traditional Chinese",
					"CN:Simple Chinese",
					"KR:Korean",
				0), FALSE, FALSE}
		};
	
	struct variable variables_Storage[] = {
			{"st_samba_mode", "", validate_choice, ARGV(
					"0:Disable",
					"1:Share all partitions in disk",
					"2:Apply rules in shared node list",
				0), FALSE, RESTART_FTPSAMBA},	// 2007.10 James
			{"computer_name", "", validate_string, ARGV("32"), FALSE, RESTART_REBOOT},	// 2007.10 James
			{"computer_nameb", "", validate_string, ARGV("32"), FALSE, RESTART_REBOOT},	// 2007.10 James
			{"st_samba_workgroup", "", validate_string, ARGV("32"), FALSE, RESTART_FTPSAMBA},	// 2007.10 James
			{"st_samba_workgroupb", "", validate_string, ARGV("32"), FALSE, RESTART_FTPSAMBA},	// 2007.10 James
			{"st_ftp_mode", "", validate_choice, ARGV(
					"0:Disable",
					"1:Login to first partition",
					"2:Login to first matched shared node",
				0), FALSE, RESTART_FTPSAMBA},	// 2007.10 James
			{"ftp_lang", "", validate_choice, ARGV("EN", "TW", "CN", "KR"), FALSE, RESTART_FTPSAMBA},	// 2007.10 James
			{"st_max_user", "", validate_range, ARGV("1", "5", ""), FALSE, RESTART_FTPSAMBA},	// 2007.10 James
			{"usb_index", "", validate_choice, ARGV(
					"0:Auto Copy",
					"1:Sharing USB Disk",
				0), FALSE, FALSE},
			{"run_prog", "", validate_string, ARGV("32"), FALSE, FALSE},
			{"sh_num", "", validate_range, ARGV("0","65535"), FALSE, FALSE},
			{"acc_num", "", validate_range, ARGV("0","65535"), FALSE, RESTART_FTPSAMBA},	// 2007.10 James
			
		//2005.12.16 Yau add
			{"sh_acc_num", "", validate_range, ARGV("0","65535"), FALSE, FALSE},
			
			{"sh_name_x", "", validate_string, ARGV("16"), FALSE, FALSE},
			{"sh_nameb_x", "", validate_string, ARGV("16"), FALSE, FALSE},
			{"sh_comment_x", "", validate_string, ARGV(""), FALSE, FALSE},
			{"sh_commentb_x", "", validate_string, ARGV(""), FALSE, FALSE},
			{"sh_acc_user_x", "", validate_string, ARGV(""), FALSE, FALSE},
			{"sh_acc_onlist_num_x", "", validate_string, ARGV("0","12"), FALSE, FALSE},
              	{"x_PartType", "", validate_choice, ARGV("0:IDE","1:USB",0), FALSE, FALSE},
                
      		{"x_PartAll", "Status", NULL, ARGV("Storage","x_PartAll"), FALSE, FALSE},

                {"x_Part1", "", validate_string, ARGV(""), FALSE, FALSE}, 
            
                {"x_Part2", "", validate_string, ARGV(""), FALSE, FALSE}, 
            
                {"x_Part3", "", validate_string, ARGV(""), FALSE, FALSE}, 
            
                {"x_Part4", "", validate_string, ARGV(""), FALSE, FALSE}, 
                 
      		{"x_StorageModel", "Status", NULL, ARGV("Storage","StorageModel"), FALSE, FALSE},
     
      		{"x_StoragePart1", "Status", NULL, ARGV("Storage","StoragePart1"), FALSE, FALSE},
     
      		{"x_StoragePart2", "Status", NULL, ARGV("Storage","StoragePart2"), FALSE, FALSE},
     
      		{"x_StoragePart3", "Status", NULL, ARGV("Storage","StoragePart3"), FALSE, FALSE},
     
      		{"x_StoragePart4", "Status", NULL, ARGV("Storage","StoragePart4"), FALSE, FALSE},
     
      		{"x_StorageStatus", "Status", NULL, ARGV("Storage","StorageStatus"), FALSE, FALSE},
                                        
                {"x_StorageAction", "", validate_string, ARGV(""), FALSE, FALSE},                                                                                                                   

      		{"x_USBModel", "Status", NULL, ARGV("Storage","StorageModel"), FALSE, FALSE},
/* 2005.12.22 Jiahao - start */     
      		{"st_usbpart1_t", "Status", NULL, ARGV("Storage","USBPart1"), FALSE, FALSE},
     
      		{"st_usbpart2_t", "Status", NULL, ARGV("Storage","USBPart2"), FALSE, FALSE},
     
      		{"st_usbpart3_t", "Status", NULL, ARGV("Storage","USBPart3"), FALSE, FALSE},
     
      		{"st_usbpart4_t", "Status", NULL, ARGV("Storage","USBPart4"), FALSE, FALSE},
     
      		{"st_usb_status_t", "Status", NULL, ARGV("Storage","USBStatus"), FALSE, FALSE},
/* 2005.12.22 Jiahao - end */
                {"x_USBAction", "", validate_string, ARGV(""), FALSE, FALSE},

#ifdef DLM
                {"apps_dl", "", validate_range, ARGV("0","1"), FALSE, RESTART_APPS},
                
                {"apps_dl_share", "", validate_range, ARGV("0","1"), FALSE, RESTART_APPS},

                {"apps_seed", "", validate_range, ARGV("0","1"), FALSE, RESTART_APPS},

                {"apps_upload_max", "", validate_string, ARGV(""), FALSE, RESTART_APPS},

                {"apps_dl_share_port_from", "", validate_range, ARGV("1024","60000"), FALSE, RESTART_APPS},
                
                {"apps_dl_share_port_to", "", validate_range, ARGV("1024","60000"), FALSE, RESTART_APPS},
                
                {"apps_dl_seed", "", validate_range, ARGV("0","168"), FALSE, RESTART_APPS},
#endif
                
                {"apps_dms", "", validate_range, ARGV("0","1"), FALSE, RESTART_UPNP},	// 2007.10 James
                
                {"apps_dms_usb_port", "", validate_range, ARGV("1","2"), FALSE, FALSE},
                
      		{"Storage_SharedList", "Group", validate_group, ARGV(variables_Storage_SharedList, "32", "79", "sh_num"), FALSE, RESTART_FTPSAMBA},	// 2007.10 James
       
      		{"Storage_UserList", "Group", validate_group, ARGV(variables_Storage_UserList, "16", "79", "acc_num"), FALSE, RESTART_FTPSAMBA},	// 2007.10 James

		//2005.12.16 Yau
                {"Storage_UserRight_List", "Group", validate_group, ARGV(variables_Storage_UserRights_List, "", "79", "sh_acc_num"),
		  FALSE, FALSE},
                        
      		{0,0,0,0,0,0}
      };

#ifndef NOUSB  
      struct variable variables_IPConnection_PortMappingList[] = {          
      	              
              {"", "7", validate_choice, ARGV(                
              
                   "TCP",
              
                   "UDP",
              
              0), FALSE, 0x0},
           
                {"", "0", validate_ipaddr, NULL, FALSE, 0x0},
                                     
                       {"", "0", validate_range, ARGV("0","65535"), FALSE, 0x0},                                                                                          
                    
                {"", "16", validate_ipaddr, NULL, FALSE, 0x0},
                                     
                       {"", "6", validate_range, ARGV("0","65535"), FALSE, 0x0},                                                                                          
                                                            
                    {"", "25", validate_string, ARGV("24"), FALSE, 0x0},                                                                                                                     
                                        
                       {"", "6", validate_range, ARGV("0","4294927695"), FALSE, 0x0},                
                                    
      {0,0,0,0,0,0}
      };      
#endif
        
      struct variable variables_IPConnection_ExposedIPList[] = {          
     
#ifndef NOUSB 
                {"", "16", validate_ipaddr, NULL, FALSE, 0x0},
                {"", "16", validate_ipaddr, NULL, FALSE, 0x0},
                {"", "20", validate_string, ARGV("20"), FALSE, 0x0},        
#endif    
      {0,0,0,0,0,0}
      };      
	
	struct variable variables_IPConnection_VSList[] = {
			{"vts_port_x", "12", validate_portrange, NULL, FALSE, FALSE},
      {"vts_ipaddr_x", "16", validate_ipaddr, NULL, FALSE, FALSE},
			{"vts_lport_x", "7", validate_range, ARGV("0","4294927695"), FALSE, FALSE},
			{"vts_proto_x", "8", validate_choice, ARGV(
					"TCP",
					"UDP",
					"BOTH",
					"OTHER",
					0), FALSE, FALSE},
#ifndef NOUSB
			{"vts_protono_x", "12", validate_range, ARGV("0", "255", ""), FALSE , FALSE},
#endif
			{"vts_desc_x", "20", validate_string, ARGV("20"), FALSE, FALSE},
			{0,0,0,0,0,0}
		};
	
	struct variable variables_IPConnection_TriggerList[] = {          
			{"autofw_outport_x", "12", validate_portrange, NULL, FALSE, FALSE},
			{"autofw_outproto_x", "7", validate_choice, ARGV(                
					"TCP",
					"UDP",
					0), FALSE, FALSE},
			{"autofw_inport_x", "12", validate_portrange, NULL, FALSE, FALSE},
			{"autofw_inproto_x", "7", validate_choice, ARGV(                
					"TCP",
					"UDP",
					0), FALSE, FALSE},
			{"autofw_desc_x", "18", validate_string, ARGV("18"), FALSE, FALSE},
			{0,0,0,0,0,0}
		};      
        
      struct variable variables_PPPConnection_PPPoERouteList[] = {         
#ifndef NOUSB 
      	              
              {"", "7", validate_choice, ARGV(                
              
                   "0",
              
                   "1",
              
                   "2",
              
              0), FALSE, 0x0},
           	              
              {"", "16", validate_choice, ARGV(                
              
                   "Source",
              
                   "Destination",
              
              0), FALSE, 0x0},
           
                {"", "18", validate_ipaddr, NULL, FALSE, 0x0},
                         
             {"", "11", validate_range, ARGV("0", "32", "1"), FALSE , 0x0},                                          	                                        
#endif                        
      {0,0,0,0,0,0}
      };      
        
      struct variable variables_FirewallConfig_WLFilterList[] = {          
      
                {"filter_wl_srcip_x", "16", validate_ipaddr, NULL, FALSE, FALSE},
            
                {"filter_wl_srcport_x", "12", validate_portrange, NULL, FALSE, FALSE},
            
                {"filter_wl_dstip_x", "16", validate_ipaddr, NULL, FALSE, FALSE},
            
                {"filter_wl_dstport_x", "12", validate_portrange, NULL, FALSE, FALSE},
            	              
              {"filter_wl_proto_x", "8", validate_choice, ARGV(                
              
                   "TCP",
              
                   "TCP ALL",
              
                   "TCP SYN",
              
                   "TCP ACK",
              
                   "TCP FIN",
              
                   "TCP RST",
              
                   "TCP URG",
              
                   "TCP PSH",
              
                   "UDP",
              
              0), FALSE, FALSE},
                           
      {0,0,0,0,0,0}
      };      
        
	struct variable variables_FirewallConfig_LWFilterList[] = {          
			{"filter_lw_srcip_x", "16", validate_ipaddr, NULL, FALSE, FALSE},
#ifndef NOUSB            
			{"filter_lw_srcport_x", "12", validate_portrange, NULL, FALSE, FALSE},
			{"filter_lw_dstip_x", "16", validate_ipaddr, NULL, FALSE, FALSE},
#endif
			{"filter_lw_dstport_x", "12", validate_portrange, NULL, FALSE, FALSE},
			{"filter_lw_proto_x", "8", validate_choice, ARGV(                
					"TCP",
					"TCP ALL",
					"TCP SYN",
					"TCP ACK",
					"TCP FIN",
					"TCP RST",
					"TCP URG",
					"TCP PSH",
					"UDP",
					0), FALSE, FALSE},
#ifdef NOUSB       
			{"filter_lw_desc_x", "20", validate_string, ARGV("20"), FALSE, FALSE},
#endif
			{0,0,0,0,0,0}
		};      
        
	struct variable variables_FirewallConfig_UrlList[] = {          
			{"url_keyword_x", "36", validate_string, ARGV("32"), FALSE, FALSE},                                                                                                                     
			{0,0,0,0,0,0}
		};      

#ifndef NOUSB        
      struct variable variables_FirewallConfig_WLocalFilterList[] = {          
      
                {"", "16", validate_ipaddr, NULL, FALSE, 0x0},
            
                {"", "12", validate_portrange, NULL, FALSE, 0x0},
            	              
              {"", "8", validate_choice, ARGV(                
              
                   "TCP",
              
                   "TCP ALL",
              
                   "TCP SYN",
              
                   "TCP ACK",
              
                   "TCP FIN",
              
                   "TCP RST",
              
                   "TCP URG",
              
                   "TCP PSH",
              
                   "UDP",
              
              0), FALSE, 0x0},
                           
      {0,0,0,0,0,0}
      };      
        
      struct variable variables_FirewallConfig_DWFilterList[] = {          
      
                {"", "16", validate_ipaddr, NULL, FALSE, 0x0},
            
                {"", "12", validate_portrange, NULL, FALSE, 0x0},
            
                {"", "16", validate_ipaddr, NULL, FALSE, 0x0},
            
                {"", "12", validate_portrange, NULL, FALSE, 0x0},
            	              
              {"", "8", validate_choice, ARGV(                
              
                   "TCP",
              
                   "TCP ALL",
              
                   "TCP SYN",
              
                   "TCP ACK",
              
                   "TCP FIN",
              
                   "TCP RST",
              
                   "TCP URG",
              
                   "TCP PSH",
              
                   "UDP",
              
              0), FALSE, 0x0},
                           
      {0,0,0,0,0,0}
      };      
        
      struct variable variables_FirewallConfig_WDFilterList[] = {          
      
                {"", "16", validate_ipaddr, NULL, FALSE, 0x0},
            
                {"", "12", validate_portrange, NULL, FALSE, 0x0},
            
                {"", "16", validate_ipaddr, NULL, FALSE, 0x0},
            
                {"", "12", validate_portrange, NULL, FALSE, 0x0},
            	              
              {"", "8", validate_choice, ARGV(                
              
                   "TCP",
              
                   "TCP ALL",
              
                   "TCP SYN",
              
                   "TCP ACK",
              
                   "TCP FIN",
              
                   "TCP RST",
              
                   "TCP URG",
              
                   "TCP PSH",
              
                   "UDP",
              
              0), FALSE, 0x0},
                           
      {0,0,0,0,0,0}
      };      
        
      struct variable variables_FirewallConfig_DLFilterList[] = {          
      
                {"", "16", validate_ipaddr, NULL, FALSE, 0x0},
            
                {"", "12", validate_portrange, NULL, FALSE, 0x0},
            
                {"", "16", validate_ipaddr, NULL, FALSE, 0x0},
            
                {"", "12", validate_portrange, NULL, FALSE, 0x0},
            	              
              {"", "8", validate_choice, ARGV(                
              
                   "TCP",
              
                   "TCP ALL",
              
                   "TCP SYN",
              
                   "TCP ACK",
              
                   "TCP FIN",
              
                   "TCP RST",
              
                   "TCP URG",
              
                   "TCP PSH",
              
                   "UDP",
              
              0), FALSE, 0x0},
                           
      {0,0,0,0,0,0}
      };      
        
      struct variable variables_FirewallConfig_LDFilterList[] = {          
      
                {"", "16", validate_ipaddr, NULL, FALSE, 0x0},
            
                {"", "12", validate_portrange, NULL, FALSE, 0x0},
            
                {"", "16", validate_ipaddr, NULL, FALSE, 0x0},
            
                {"", "12", validate_portrange, NULL, FALSE, 0x0},
            	              
              {"", "8", validate_choice, ARGV(                
              
                   "TCP",
              
                   "TCP ALL",
              
                   "TCP SYN",
              
                   "TCP ACK",
              
                   "TCP FIN",
              
                   "TCP RST",
              
                   "TCP URG",
              
                   "TCP PSH",
              
                   "UDP",
              
              0), FALSE, 0x0},
                           
      {0,0,0,0,0,0}
      };      
#endif
        
	struct variable variables_FirewallConfig_MFList[] = {          
			{"macfilter_list_x", "32", validate_hwaddr, NULL, FALSE, FALSE},
			{0,0,0,0,0,0}
		};      
        
	struct variable variables_RouterConfig_GWStatic[] = {          
			{"sr_ipaddr_x", "17", validate_ipaddr, NULL, FALSE, FALSE},
			{"sr_netmask_x", "17", validate_ipaddr, NULL, FALSE, FALSE},
			{"sr_gateway_x", "17", validate_ipaddr, NULL, FALSE, FALSE},
			{"sr_matric_x", "5", validate_ipaddr, NULL, FALSE, FALSE},
			{"sr_if_x", "3", validate_choice, ARGV(                
					"LAN",
					"WAN",
					0), FALSE, FALSE},
			{0,0,0,0,0,0}
		};      
      
#ifndef NOUSB  
	struct variable variables_RouterConfig_StaticRoute[] = {          
      
                {"dr_staticipaddr_x", "20", validate_ipaddr, NULL, FALSE, RESTART_REBOOT},	// 2007.10 James
                         
             {"dr_staticnetmask_x", "11", validate_range, ARGV("0", "32", "1"), FALSE , RESTART_REBOOT},	// 2007.10 James
        
                {"dr_staticgateway_x", "18", validate_ipaddr, NULL, FALSE, RESTART_REBOOT},	// 2007.10 James
                            
      {0,0,0,0,0,0}
      }; 
     
      struct variable variables_RouterConfig_RipSRoute[] = {          
      
                {"", "17", validate_ipaddr, NULL, FALSE, 0x0},
                         
             {"", "7", validate_range, ARGV("0", "32", "1"), FALSE , 0x0},                                          	                                                                      
                        
      {0,0,0,0,0,0}
      };      
        
      struct variable variables_RouterConfig_RouteFilter[] = {          
      	              
              {"", "9", validate_choice, ARGV(                
              
                   "0",
              
                   "1",
              
                   "2",
              
                   "3",
              
              0), FALSE, 0x0},
           
                {"", "17", validate_ipaddr, NULL, FALSE, 0x0},
                         
             {"", "8", validate_range, ARGV("0", "32", "1"), FALSE , 0x0},                                          	                                                                      
                        
      {0,0,0,0,0,0}
      };      
        
      struct variable variables_RouterConfig_RipOffset[] = {          
      	              
              {"", "7", validate_choice, ARGV(                
              
                   "IN",
              
                   "OUT",
              
              0), FALSE, 0x0},
           
                {"", "17", validate_ipaddr, NULL, FALSE, 0x0},
                         
             {"", "6", validate_range, ARGV("0", "32", "1"), FALSE , 0x0},                                          	                                                                      
                        
      {0,0,0,0,0,0}
      };      
#endif
        
	struct variable variables_LANHostConfig_ManualDHCPList[] = {          
			{"dhcp_staticmac_x", "14", validate_hwaddr, NULL, FALSE, FALSE},
			{"dhcp_staticip_x", "15", validate_ipaddr, NULL, FALSE, FALSE},
			{0,0,0,0,0,0}
		};      
       
#ifndef NOUSB 
      struct variable variables_LANHostConfig_DmzManualDHCPList[] = {          
      
                {"", "14", validate_hwaddr, NULL, FALSE, 0x0},
            
                {"", "15", validate_ipaddr, NULL, FALSE, 0x0},
                            
      {0,0,0,0,0,0}
      };      
        
      struct variable variables_LANHostConfig_ReservedAddressList[] = {          
      
                {"", "24", validate_ipaddr, NULL, FALSE, 0x0},
                            
      {0,0,0,0,0,0}
      };      
#endif
	
	struct variable variables_DeviceSecurity11a_ACLList[] = {
			{"wl_maclist_x", "32", validate_hwaddr, NULL, FALSE, 0x0},
			{0,0,0,0,0,0}
		};
	
#ifndef NOUSB
      struct variable variables_DeviceSecurity11a_AESList[] = {          
      
                {"", "16", validate_hwaddr, NULL, FALSE, 0x0},
            	              
              {"", "10", validate_choice, ARGV(                
              
                   "64bits",
              
                   "128bits",
              
                   "152bits",
              
              0), FALSE, 0x0},
           
                {"", "32", validate_wlkey, NULL, FALSE, 0x0},
                            
      {0,0,0,0,0,0}
      };      
        
      struct variable variables_WLANAuthentication11a_LocalAuthDB[] = {          
                                              
                    {"", "18", validate_string, ARGV("16"), FALSE, 0x0},                                                                                                                     
                                                         
                    {"", "18", validate_string, ARGV("16"), FALSE, 0x0},                                                                                                                     
                                 
      {0,0,0,0,0,0}
      };      
        
      struct variable variables_WLANAuthentication11a_LocalCertDB[] = {          
      	              
              {"", "8", validate_choice, ARGV(                
              
                   "Server",
              
                   "Client",
              
              0), FALSE, 0x0},
           
                {"", "", validate_string, ARGV("32"), FALSE, 0x0}, 
                            
      {0,0,0,0,0,0}
      };      
#endif
	
	struct variable variables_WLANConfig11b_RBRList[] = {
			{"wl_wdslist_x", "32", validate_hwaddr, NULL, FALSE, FALSE},
			{0,0,0,0,0,0}
		};
	
	struct variable variables_DeviceSecurity11b_ACLList[] = {
			{"wl_maclist_x", "32", validate_hwaddr, NULL, FALSE, FALSE},
			{0,0,0,0,0,0}
		};
	
#ifndef NOUSB
      struct variable variables_DeviceSecurity11b_AESList[] = {          
      
                {"", "16", validate_hwaddr, NULL, FALSE, 0x0},
            	              
              {"", "10", validate_choice, ARGV(                
              
                   "64bits",
              
                   "128bits",
              
                   "152bits",
              
              0), FALSE, 0x0},
           
                {"", "32", validate_wlkey, NULL, FALSE, 0x0},
                            
      {0,0,0,0,0,0}
      };      
        
      struct variable variables_WLANAuthentication11b_LocalAuthDB[] = {          
                                              
                    {"", "18", validate_string, ARGV("16"), FALSE, 0x0},                                                                                                                     
                                                         
                    {"", "18", validate_string, ARGV("16"), FALSE, 0x0},                                                                                                                     
                                 
      {0,0,0,0,0,0}
      };      
        
      struct variable variables_WLANAuthentication11b_LocalCertDB[] = {          
      	              
              {"", "8", validate_choice, ARGV(                
              
                   "Server",
              
                   "Client",
              
              0), FALSE, 0x0},
           
                {"", "", validate_string, ARGV("32"), FALSE, 0x0}, 
                            
      {0,0,0,0,0,0}
      };      
#endif

#ifndef NOUSB        
      struct variable variables_PrinterStatus_x_FUserList[] = {          
                                              
                    {"usb_ftpusername_x", "18", validate_string, ARGV("16"), FALSE, 0x0},                                                                                                                     
                                                         
                    {"usb_ftppasswd_x", "18", validate_string, ARGV("16"), FALSE, 0x0},                                                                                                                     
                              
             {"usb_ftpmaxuser_x", "7", validate_range, ARGV("0", "12", ""), FALSE , 0x0},                                          	                                                                      
        	            
              {"usb_ftprights_x", "16", validate_choice, ARGV(                
              
                   "Read/Write/Erase",
              
                   "Read/Write",
              
                   "Read Only",
              
                   "View Only",
              
                   "Private",
              
                   "Private(WO)",
              
              0), FALSE, 0x0},
                           
      {0,0,0,0,0,0}
      };      
        
      struct variable variables_PrinterStatus_x_FBanIPList[] = {          
      
                {"usb_ftpbanip_x", "24", validate_ipaddr, NULL, FALSE, 0x0},
                            
      {0,0,0,0,0,0}
      };      
#endif

#ifndef NOQOS 
/*
      struct variable variables_PrinterStatus_x_QRuleList[] = {          
      
                {"qos_ipaddr_x", "16", validate_ipaddr, NULL, FALSE, 0x0},
                                   
                       {"qos_port_x", "12", validate_range, ARGV("0","4294927695"), FALSE, 0x0},                
                                           
                       {"qos_maxbw_x", "9", validate_range, ARGV("0","4294927695"), FALSE, 0x0},                
                                           
                       {"qos_minbw_x", "7", validate_range, ARGV("0","4294927695"), FALSE, 0x0},                
                                    
      {0,0,0,0,0,0}
      };      

      struct variable variables_PrinterStatus_x_UQRuleList[] = {          
                             
                       {"qos_uport_x", "12", validate_range, ARGV("0","4294927695"), FALSE, 0x0},                
                                           
                       {"qos_umaxbw_x", "9", validate_range, ARGV("0","4294927695"), FALSE, 0x0},                
                                           
                       {"qos_uminbw_x", "7", validate_range, ARGV("0","4294927695"), FALSE, 0x0},                
                                    
      {0,0,0,0,0,0}
      };      
*/
	struct variable variables_PrinterStatus_x_USRRuleList[] = {
			{"qos_service_name_x", "16", validate_string, ARGV("32"), FALSE, FALSE},
			{"qos_ip_x", "16", validate_ipaddr, NULL, FALSE, FALSE},
			{"qos_port_x", "12", validate_range, ARGV("0","65535"), FALSE, FALSE},
			{"qos_prio_x", "3", validate_choice, ARGV(
					"1",
					"2",
					"3",
					"4",
					"5",
					"6",
					"7",
					"8",
					0), FALSE, FALSE},
			{0,0,0,0,0,0}
			};
#endif
      
/*
 * actions_$ServiceId[] = { $ActionName, $InArgu, $OutArgu, Callback}
 * Used in GetVarRequest
 *  	1. Find serviceId from svcLinks
 *	2. Get value from varaibles$ServiceId by means of nvram_get_x
 * 
 * Used in ActionRequest
 *      1. Find serviceId from svcLinks
 *	2. If $callback is not null, excute it
 * 	   else goto 3
 *	3. Check validate from $Validate_Func of variables_$ServiceId, if not validate return error
 *      4. Set value by means of nvram_set_x or nvram_add_list_x or nvram_del_list_x
 *      5. return
 *
 * variables_$ServiceId[] = { $Name, $Validate_Func, $Argu, $NullOk, $Event}
 * For writable variable
 *     $Validate_Func : check if the variable is ok
 *     $Argu : arguments list pass into $Validate_Func
 *     $NullOk: if variable is allowed as NULL
 *     $Event: if event is sent once variable is changed
 * For read only variable
 *     $Validate_Func == NULL
 *     $Argu : file and field to get value
 *     $NullOk: if variable is allowed as NULL
 *     $Event: if event is sent once variable is changed
 */
/* 
 * Variables are set in order (put dependent variables later). Set
 * nullok to TRUE to ignore zero-length values of the variable itself.
 * For more complicated validation that cannot be done in one pass or
 * depends on additional form components or can throw an error in a
 * unique painful way, write your own validation routine and assign it
 * to a hidden variable (e.g. filter_ip).
 *
 * This data structure is generated automatically by script, maybe not 
 * easy to read. But it really work!!!
*/
	
	struct variable variables_General[] = {
//2007.10 James {		
		{"x_Setting", "", validate_range, ARGV("0","1"), FALSE, FALSE},
		
		{"machine_name", "", validate_string, ARGV("32"), FALSE, RESTART_REBOOT},
//2007.10 James }
		
		{"http_username", "", validate_string, ARGV("32"), FALSE, FALSE},
		                                                         
		{"http_passwd", "", validate_string, ARGV("32"), FALSE, FALSE},
		                 
		{"x_SystemUpTime", "Status", NULL, ARGV("general.log","UpTime"), FALSE, FALSE},
		
		{"x_ProductID", "Status", NULL, ARGV("general.log","ProductID"), FALSE, FALSE},
		
		{"x_FirmwareVersion", "Status", NULL, ARGV("general.log","FirmwareVer"), FALSE, FALSE},
		
		{"x_HardwareVersion", "Status", NULL, ARGV("general.log","HardwareVer"), FALSE, FALSE},
		
		{"x_BootloaderVersion", "Status", NULL, ARGV("general.log","BootloaderVer"), FALSE, FALSE},
		
		{0,0,0,0,0,0}
	};
	
      struct variable variables_Layer3Forwarding[] = {
                                              
							{"", "", validate_string, ARGV(""), FALSE, FALSE},                                                                                                                   
							
							{"x_DHCPClient", "", validate_choice, ARGV("0", "1"), FALSE, RESTART_REBOOT},	// 2008.03 James
							
							{"wan_pptp_options_x", "", validate_string, ARGV("255"), FALSE, RESTART_REBOOT},	// 2008.03 James
							
              {"wan_proto", "", validate_choice, ARGV(              
              
                   "dhcp:Automatic IP",
              
                   "static:Static IP",
              
                   "pppoe:PPPoE",
              
                   "pptp:PPTP",
                   
                   "l2tp:L2TP",
                   
                   //"bigpond:BigPond",
              
              0), FALSE, RESTART_REBOOT},	// 2007.10 James
           	
              {"wan_mode_x", "", validate_choice, ARGV(              
              
                   "0:Disabled",
              
                   "1:Enabled",
              
                   "2:Auto",
              
              0), FALSE, RESTART_REBOOT},	// 2007.10 James
           	
              {"wan_etherspeed_x", "", validate_choice, ARGV(              
              
                   "auto:Auto negotiation",
              
                   "10half:10Mbps half-duplex",
              
                   "10full:10Mbps full-duplex",
              
                   "100half:100Mpbs half-duplex",
              
                   "100full:100Mpbs full-duplex",
              
              0), FALSE, RESTART_REBOOT},	// 2007.10 James

		{"wan_stb_x", "", validate_range, ARGV("0", "4"), FALSE, RESTART_REBOOT},	// 2008.03 James
                             
      {0,0,0,0,0,0}
      };
   
      struct variable variables_WANCommonInterface[] = {
#ifndef NOUSB
      	
              {"", "", validate_choice, ARGV(              
              
                   "Ethernet",
              
              0), FALSE, FALSE},
                                  
                       {"", "", validate_range, ARGV("0","4294927695"), FALSE, FALSE},                                                             
                                           
                       {"", "", validate_range, ARGV("0","4294927695"), FALSE, FALSE},                                                             
                    	
              {"", "", validate_choice, ARGV(              
              
                   "Up",
              
                   "Down",
              
              0), FALSE, FALSE},
                                                   
                    {"", "", validate_string, ARGV(""), FALSE, FALSE},                                                                                                                   
                              
             {"", "", validate_range, ARGV("1", "", "1"), FALSE, FALSE},
                                 
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                                                            
                    {"", "", validate_string, ARGV(""), FALSE, FALSE},                                                                                                                   
                                                         
                    {"", "", validate_string, ARGV(""), FALSE, FALSE},                                                                                                                   
                                        
                       {"", "", validate_range, ARGV("0","4294927695"), FALSE, FALSE},                                                             
                                           
                       {"", "", validate_range, ARGV("0","4294927695"), FALSE, FALSE},                                                             
                                           
                       {"", "", validate_range, ARGV("0","4294927695"), FALSE, FALSE},                                                             
                                           
                       {"", "", validate_range, ARGV("0","4294927695"), FALSE, FALSE},                                                             
                                     
                 {"", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
#endif
                                
      {0,0,0,0,0,0}
      };
   
      struct variable variables_IPConnection[] = {
      	
              {"wan_route_x", "", validate_choice, ARGV(              
              
                   "Unconfigured",
              
                   "IP_Routed",
              
                   "IP_Bridged",
              
              0), FALSE, RESTART_REBOOT},	// 2007.10 James
           	
              {"", "", validate_choice, ARGV(              
              
                   "Unconfigured",
              
                   "IP_Routed",
              
                   "IP_Bridged",
              
              0), FALSE, FALSE},
           
      {"ConnectionStatus", "Status", NULL, ARGV("wan.log","IPLink"), FALSE, FALSE},

      {"Uptime", "Status", NULL, ARGV("wan.log","Uptime"), FALSE, FALSE},
	
              {"", "", validate_choice, ARGV(              
              
                   "ERROR_NONE",
              
              0), FALSE, FALSE},
                            
                 {"wan_nat_x", "", validate_range, ARGV("0","1"), FALSE, RESTART_REBOOT},	// 2007.10 James
                               
                 {"", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
              
                {"wan_ipaddr", "", validate_ipaddr, NULL, FALSE, RESTART_REBOOT},	// 2007.10 James
            
                {"wan_netmask", "", validate_ipaddr, NULL, FALSE, RESTART_REBOOT},	// 2007.10 James
            
                {"wan_gateway", "", validate_ipaddr, NULL, FALSE, RESTART_REBOOT},	// 2007.10 James
                             
                 {"wan_dnsenable_x", "", validate_range, ARGV("0","1"), FALSE, RESTART_REBOOT},	// 2007.10 James
              
                {"wan_dns1_x", "", validate_ipaddr, NULL, FALSE, RESTART_REBOOT},	// 2007.10 James
            
                {"wan_dns2_x", "", validate_ipaddr, NULL, FALSE, RESTART_REBOOT},	// 2007.10 James
            
      {"IPTablesInfo", "Status", NULL, ARGV("iptable.log",""), FALSE, FALSE},
                         
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                                     
                 {"", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       

                {"dmz_ip", "", validate_ipaddr, NULL, FALSE, RESTART_FIREWALL},	// 2007.10 James
                             
                 {"sp_battle_ips", "", validate_range, ARGV("0","1"), FALSE, RESTART_FIREWALL},	// 2007.10 James
                                       
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                                     
                 {"vts_enable_x", "", validate_range, ARGV("0","1"), FALSE, RESTART_FIREWALL},	// 2007.10 James
                                       
                       {"vts_num_x", "", validate_range, ARGV("0","65535"), FALSE, RESTART_FIREWALL},	// 2007.10 James
                                     
                 {"autofw_enable_x", "", validate_range, ARGV("0","1"), FALSE, RESTART_FIREWALL},	// 2007.10 James
                                       
                       {"autofw_num_x", "", validate_range, ARGV("0","65535"), FALSE, RESTART_FIREWALL},	// 2007.10 James
#ifndef NOUSB                    
                {"", "", validate_ipaddr, NULL, FALSE, FALSE},
            
                {"", "", validate_portrange, NULL, FALSE, FALSE},
                                                    
                    {"", "", validate_string, ARGV("24"), FALSE, FALSE},                                                                                                                   
                                                         
                    {"", "", validate_string, ARGV(""), FALSE, FALSE},                                                                                                                   
                 
                {"", "", validate_ipaddr, NULL, FALSE, FALSE},
            
                {"", "", validate_portrange, NULL, FALSE, FALSE},
                                                    
                    {"", "", validate_string, ARGV("24"), FALSE, FALSE},                                                                                                                   
                                                         
                    {"", "", validate_string, ARGV(""), FALSE, FALSE},                                                                                                                   
                 
                {"", "", validate_ipaddr, NULL, FALSE, FALSE},
            
                {"", "", validate_portrange, NULL, FALSE, FALSE},
                                                    
                    {"", "", validate_string, ARGV("24"), FALSE, FALSE},                                                                                                                   
                                                         
                    {"", "", validate_string, ARGV(""), FALSE, FALSE},                                                                                                                   
                 
                {"", "", validate_ipaddr, NULL, FALSE, FALSE},
            
                {"", "", validate_portrange, NULL, FALSE, FALSE},
                                                    
                    {"", "", validate_string, ARGV("24"), FALSE, FALSE},                                                                                                                   
                                                         
                    {"", "", validate_string, ARGV(""), FALSE, FALSE},                                                                                                                   
                 
                {"", "", validate_ipaddr, NULL, FALSE, FALSE},
            
                {"", "", validate_portrange, NULL, FALSE, FALSE},
                                                    
                    {"", "", validate_string, ARGV("24"), FALSE, FALSE},                                                                                                                   
                                                         
                    {"", "", validate_string, ARGV(""), FALSE, FALSE},                                                                                                                   
                 
                {"", "", validate_ipaddr, NULL, FALSE, FALSE},
            
                {"", "", validate_portrange, NULL, FALSE, FALSE},
                                                    
                    {"", "", validate_string, ARGV("24"), FALSE, FALSE},                                                                                                                   
                                                         
                    {"", "", validate_string, ARGV(""), FALSE, FALSE},                                                                                                                   
                  
      {"PortMappingList", "Group", validate_group, ARGV(variables_IPConnection_PortMappingList, "16", "58", "PortMappingNumberOfEntries"), FALSE, FALSE},     
#endif
       
      {"ExposedIPList", "Group", validate_group, ARGV(variables_IPConnection_ExposedIPList, "32", "52", "ExposedIPCount"), FALSE, FALSE},     
       
      {"VSList", "Group", validate_group, ARGV(variables_IPConnection_VSList, "24", "75", "vts_num_x"), FALSE, RESTART_FIREWALL},	// 2008.01 James.
       
      {"TriggerList", "Group", validate_group, ARGV(variables_IPConnection_TriggerList, "10", "56", "autofw_num_x"), FALSE, RESTART_FIREWALL},	// 2008.01 James.
                        
      {0,0,0,0,0,0}
      };
   
      struct variable variables_PPPConnection[] = {
      
#ifndef NOUSB	
              {"", "", validate_choice, ARGV(              
              
                   "Unconfigured",
              
                   "IP_Routed",
              
              0), FALSE, FALSE},
           	
              {"", "", validate_choice, ARGV(              
              
                   "IP_Routed",
              
              0), FALSE, FALSE},
#endif
                                                   
                    {"wan_pppoe_username", "", validate_string, ARGV("64"), FALSE, RESTART_REBOOT},	// 2007.10 James
                 
                {"wan_pppoe_passwd", "", validate_string, ARGV("64"), FALSE, RESTART_REBOOT},	// 2007.10 James
            
      {"ConnectionStatus", "Status", NULL, ARGV("wan.log","WANLink"), FALSE, FALSE},

#ifndef NOUSB
                       
                       {"", "", validate_range, ARGV("0","4294927695"), FALSE, FALSE},                                                             
                                           
                       {"", "", validate_range, ARGV("0","4294927695"), FALSE, FALSE},                                                             
                                           
                       {"", "", validate_range, ARGV("0","4294927695"), FALSE, FALSE},                                                             
                    	
              {"", "", validate_choice, ARGV(              
              
                   "ERROR_NONE",
              
              0), FALSE, FALSE},
                                  
                       {"", "", validate_range, ARGV("0","4294927695"), FALSE, FALSE},                                                             
                                           
#endif
                       {"wan_pppoe_idletime", "", validate_range, ARGV("0","4294927695"), FALSE, RESTART_REBOOT},	// 2007.10 James
                    
                {"wan_pppoe_txonly_x", "", validate_string, ARGV(""), FALSE, RESTART_REBOOT},	// 2007.10 James
		
		{"wan_pppoe_options_x", "", validate_string, ARGV("255"), FALSE, RESTART_REBOOT},	// 2008.03 James
                         
             {"wan_pppoe_mtu", "", validate_range, ARGV("576", "1492", ""), FALSE, RESTART_REBOOT},	// 2007.10 James
                     
             {"wan_pppoe_mru", "", validate_range, ARGV("576", "1492", ""), FALSE, RESTART_REBOOT},	// 2007.10 James
                               
                       {"", "", validate_range, ARGV("0","4294927695"), FALSE, FALSE},                                                             
                                                            
                    {"wan_pppoe_service", "", validate_string, ARGV("32"), FALSE, RESTART_REBOOT},	// 2007.10 James
                                                         
                    {"wan_pppoe_ac", "", validate_string, ARGV("32"), FALSE, RESTART_REBOOT},	// 2007.10 James
                                  
                 {"wan_pppoe_relay_x", "", validate_range, ARGV("0","1"), FALSE, RESTART_REBOOT},	// 2007.10 James
                                                      
                    {"wan_hostname", "", validate_string, ARGV("32"), FALSE, RESTART_REBOOT},	// 2007.10 James
                 
                {"wan_hwaddr_x", "", validate_hwaddr, NULL, FALSE, RESTART_REBOOT},	// 2007.10 James
                                                    
                    {"wan_heartbeat_x", "", validate_string, ARGV(""), FALSE, RESTART_REBOOT},	// 2007.10 James
#ifndef NOUSB
                 
                {"", "", validate_ipaddr, NULL, FALSE, FALSE},
            
                {"", "", validate_ipaddr, NULL, FALSE, FALSE},
            
                {"", "", validate_ipaddr, NULL, FALSE, FALSE},
#endif
            
      {"x_WANType", "Status", NULL, ARGV("wan.log", "wan_proto_t"), FALSE, FALSE},

      {"x_WANIPAddress", "Status", NULL, ARGV("wan.log","wan_ipaddr_t"), FALSE, FALSE},

      {"x_WANSubnetMask", "Status", NULL, ARGV("wan.log","wan_netmask_t"), FALSE, FALSE},

      {"x_WANGateway", "Status", NULL, ARGV("wan.log","wan_gateway_t"), FALSE, FALSE},

      {"x_WANDNSServer", "Status", NULL, ARGV("wan.log","wan_dns_t"), FALSE, FALSE},

      {"x_WANLink", "Status", NULL, ARGV("wan.log","wan_status_t"), FALSE, FALSE},
                                        
                    {"", "", validate_string, ARGV(""), FALSE, FALSE},                                                                                                                   
                 
      {"x_DDNSStatus", "Status", NULL, ARGV("ddns.log","DDNSStatus"), FALSE, FALSE},

#ifndef NOUSB
                         
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                    	
              {"", "", validate_choice, ARGV(              
              
                   "0:PPPoE Session 0",
              
                   "1:PPPoE Session 1",
              
                   "2:PPPoE Session 2",
              
              0), FALSE, FALSE},
                      
                 {"", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
                                                      
                    {"", "", validate_string, ARGV("64"), FALSE, FALSE},                                                                                                                   
                 
                {"", "", validate_string, ARGV("64"), FALSE, FALSE}, 
                                   
                       {"", "", validate_range, ARGV("0","4294927695"), FALSE, FALSE},                                                             
                    
                {"", "", validate_string, ARGV(""), FALSE, FALSE}, 
                         
             {"", "", validate_range, ARGV("576", "1492", ""), FALSE, FALSE},
                     
             {"", "", validate_range, ARGV("576", "1492", ""), FALSE, FALSE},
                                                
                    {"", "", validate_string, ARGV("32"), FALSE, FALSE},                                                                                                                   
                                                         
                    {"", "", validate_string, ARGV("32"), FALSE, FALSE},                                                                                                                   
                                  
                 {"", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
                                                      
                    {"", "", validate_string, ARGV("64"), FALSE, FALSE},                                                                                                                   
                 
                {"", "", validate_string, ARGV("64"), FALSE, FALSE}, 
                                   
                       {"", "", validate_range, ARGV("0","4294927695"), FALSE, FALSE},                                                             
                    
                {"", "", validate_string, ARGV(""), FALSE, FALSE}, 
                         
             {"", "", validate_range, ARGV("576", "1492", ""), FALSE, FALSE},
                     
             {"", "", validate_range, ARGV("576", "1492", ""), FALSE, FALSE},
                                                
                    {"", "", validate_string, ARGV("32"), FALSE, FALSE},                                                                                                                   
                                                         
                    {"", "", validate_string, ARGV("32"), FALSE, FALSE},                                                                                                                   
                                          
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                     
      {"PPPoERouteList", "Group", validate_group, ARGV(variables_PPPConnection_PPPoERouteList, "4", "50", "PPPoERouteCount"), FALSE, FALSE},     
#endif
                        
      {0,0,0,0,0,0}
      };
   
      struct variable variables_EthernetLink[] = {
#ifndef NOUSB      	
              {"", "", validate_choice, ARGV(              
              
                   "Up",
              
                   "Down",
              
              0), FALSE, FALSE},
#endif
                             
      {0,0,0,0,0,0}
      };
   
      struct variable variables_FirewallConfig[] = {
#ifndef NOUSB
                               
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                                             
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                                             
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
             
#endif                        
                 {"fw_enable_x", "", validate_range, ARGV("0","1"), FALSE, RESTART_REBOOT},	// 2007.10 James

		 {"fw_dos_x", "", validate_range, ARGV("0","1"), FALSE, RESTART_FIREWALL},	// 2008.03 James
              	
              {"fw_log_x", "", validate_choice, ARGV(              
              
                   "none:None",
              
                   "drop:Dropped",
              
                   "accept:Accepted",
              
                   "both:Both",
              
              0), FALSE, RESTART_REBOOT},	// 2007.10 James
                            
                 {"misc_natlog_x", "", validate_range, ARGV("0","1"), FALSE, RESTART_REBOOT},	// 2007.10 James
                               
                 {"misc_http_x", "", validate_range, ARGV("0","1"), FALSE, RESTART_FIREWALL},	// 2007.10 James
                           
             {"misc_httpport_x", "", validate_range, ARGV("1024", "65535", ""), FALSE, RESTART_FIREWALL},	// 2007.10 James
                         
                 {"misc_lpr_x", "", validate_range, ARGV("0","1"), FALSE, RESTART_FIREWALL},	// 2007.10 James
                               
                 {"misc_ping_x", "", validate_range, ARGV("0","1"), FALSE, RESTART_FIREWALL},	// 2007.10 James
                               
                 {"fw_wl_enable_x", "", validate_range, ARGV("0","1"), FALSE, RESTART_REBOOT},	// 2007.10 James
              
                {"filter_wl_date_x", "", validate_portrange, NULL, FALSE, RESTART_REBOOT},	// 2007.10 James
            
                {"filter_wl_time_x", "", validate_portrange, NULL, FALSE, RESTART_REBOOT},	// 2007.10 James
            	
              {"filter_wl_default_x", "", validate_choice, ARGV(              
              
                   "DROP",
              
                   "ACCEPT",
              
              0), FALSE, RESTART_FIREWALL},	// 2007.10 James
           
                {"filter_wl_icmp_x", "", validate_portrange, NULL, FALSE, RESTART_FIREWALL},	// 2007.10 James
            
                {"", "", validate_portrange, NULL, FALSE, FALSE},
                             
                 {"fw_lw_enable_x", "", validate_range, ARGV("0","1"), FALSE, RESTART_REBOOT},	// 2007.10 James
              
                {"filter_lw_date_x", "", validate_portrange, NULL, FALSE, RESTART_REBOOT},	// 2007.10 James
            
                {"filter_lw_time_x", "", validate_portrange, NULL, FALSE, RESTART_REBOOT},	// 2007.10 James
            	
              {"filter_lw_default_x", "", validate_choice, ARGV(              
              
                   "DROP",
              
                   "ACCEPT",
              
              0), FALSE, RESTART_REBOOT},	// 2007.10 James
           
                {"filter_lw_icmp_x", "", validate_portrange, NULL, FALSE, RESTART_REBOOT},	// 2007.10 James
            
      {"FirewallLog", "Status", NULL, ARGV("firewall.log",""), FALSE, FALSE},

      {"SystemLog", "Status", NULL, ARGV("syslog.log",""), FALSE, FALSE},

      {"SystemCmd", "Status", NULL, ARGV("syscmd.log",""), FALSE, FALSE},
                 
                 {"url_enable_x", "", validate_range, ARGV("0","1"), FALSE, RESTART_REBOOT},	// 2007.10 James
                 {"url_enable_x_1", "", validate_range, ARGV("0","1"), FALSE, RESTART_REBOOT},	// 2007.10 James
              
                {"url_date_x", "", validate_portrange, NULL, FALSE, RESTART_REBOOT},	// 2007.10 James
            
                {"url_time_x", "", validate_portrange, NULL, FALSE, RESTART_REBOOT},	// 2007.10 James
                {"url_time_x_1", "", validate_portrange, NULL, FALSE, RESTART_REBOOT},	// 2007.10 James
            	
              {"", "", validate_choice, ARGV(              
              
                   "DROP",
              
                   "ACCEPT",
              
              0), FALSE, FALSE},
                                    
                       {"url_num_x", "", validate_range, ARGV("0","65535"), FALSE, RESTART_REBOOT},	// 2007.10 James
                                             
                       {"filter_wl_num_x", "", validate_range, ARGV("0","65535"), FALSE, RESTART_FIREWALL},	// 2007.10 James
                                             
                       {"filter_lw_num_x", "", validate_range, ARGV("0","65535"), FALSE, RESTART_REBOOT},	// 2007.10 James
#ifndef NOUSB
                                     
                 {"", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
              
                {"", "", validate_portrange, NULL, FALSE, FALSE},
            
                {"", "", validate_portrange, NULL, FALSE, FALSE},
            	
              {"", "", validate_choice, ARGV(              
              
                   "DROP",
              
                   "ACCEPT",
              
              0), FALSE, FALSE},
           
                {"", "", validate_portrange, NULL, FALSE, FALSE},
            	
              {"", "", validate_choice, ARGV(              
              
                   "None",
              
                   "Dropped",
              
                   "Accepted",
              
                   "Both",
              
              0), FALSE, FALSE},
                                    
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                                             
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                                     
                 {"", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
              	
              {"", "", validate_choice, ARGV(              
              
                   "Both",
              
                   "802.11a only",
              
                   "802.11g only",
              
              0), FALSE, FALSE},
           
                {"", "", validate_ipaddr, NULL, FALSE, FALSE},
            
                {"", "", validate_ipaddr, NULL, FALSE, FALSE},
                             
                 {"", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
              	
              {"", "", validate_choice, ARGV(              
              
                   "DROP",
              
                   "ACCEPT",
              
              0), FALSE, FALSE},
           
                {"", "", validate_portrange, NULL, FALSE, FALSE},
            	
              {"", "", validate_choice, ARGV(              
              
                   "DROP",
              
                   "ACCEPT",
              
              0), FALSE, FALSE},
           
                {"", "", validate_portrange, NULL, FALSE, FALSE},
            	
              {"", "", validate_choice, ARGV(              
              
                   "None",
              
                   "Dropped",
              
                   "Accepted",
              
                   "Both",
              
              0), FALSE, FALSE},
                                    
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                                             
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                                     
                 {"", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
              	
              {"", "", validate_choice, ARGV(              
              
                   "DROP",
              
                   "ACCEPT",
              
              0), FALSE, FALSE},
           
                {"", "", validate_portrange, NULL, FALSE, FALSE},
            	
              {"", "", validate_choice, ARGV(              
              
                   "DROP",
              
                   "ACCEPT",
              
              0), FALSE, FALSE},
           
                {"", "", validate_portrange, NULL, FALSE, FALSE},
            	
              {"", "", validate_choice, ARGV(              
              
                   "None",
              
                   "Dropped",
              
                   "Accepted",
              
                   "Both",
              
              0), FALSE, FALSE},
                                    
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                                             
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                                             
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
#endif
                    	
              {"macfilter_enable_x", "", validate_choice, ARGV(              
              
                   "0:Disable",
              
                   "1:Accept",
              
                   "2:Reject",
              
              0), FALSE, RESTART_FIREWALL},	// 2007.10 James
                                    
                       {"macfilter_num_x", "", validate_range, ARGV("0","65535"), FALSE, RESTART_FIREWALL},	// 2007.10 James
                     
      {"WLFilterList", "Group", validate_group, ARGV(variables_FirewallConfig_WLFilterList, "32", "63", "filter_wl_num_x"), FALSE, RESTART_FIREWALL},	// 2007.10 James
       
      {"LWFilterList", "Group", validate_group, ARGV(variables_FirewallConfig_LWFilterList, "32", "63", "filter_lw_num_x"), FALSE, RESTART_REBOOT},	// 2007.11 James
       
      {"UrlList", "Group", validate_group, ARGV(variables_FirewallConfig_UrlList, "128", "36", "url_num_x"), FALSE, RESTART_REBOOT},	// 2007.11 James

#ifndef NOUSB
       
      {"WLocalFilterList", "Group", validate_group, ARGV(variables_FirewallConfig_WLocalFilterList, "32", "36", "WanLocalRuleCount"), FALSE, FALSE},     
       
      {"DWFilterList", "Group", validate_group, ARGV(variables_FirewallConfig_DWFilterList, "32", "63", "DmzWanRuleCount"), FALSE, FALSE},     
       
      {"WDFilterList", "Group", validate_group, ARGV(variables_FirewallConfig_WDFilterList, "32", "63", "WanDmzRuleCount"), FALSE, FALSE},     
       
      {"DLFilterList", "Group", validate_group, ARGV(variables_FirewallConfig_DLFilterList, "32", "63", "DmzLanRuleCount"), FALSE, FALSE},     
       
      {"LDFilterList", "Group", validate_group, ARGV(variables_FirewallConfig_LDFilterList, "32", "63", "LanDmzRuleCount"), FALSE, FALSE},     
#endif
       
      {"MFList", "Group", validate_group, ARGV(variables_FirewallConfig_MFList, "16", "32", "macfilter_num_x"), FALSE, RESTART_FIREWALL},	// 2007.11 James
                        
      {0,0,0,0,0,0}
      };
   
      struct variable variables_RouterConfig[] = {
                       
                 {"sr_enable_x", "", validate_range, ARGV("0","1"), FALSE, RESTART_REBOOT},	// 2007.10 James
		
		{"dr_enable_x", "", validate_range, ARGV("0","1"), FALSE, RESTART_REBOOT},	// 2008.03 James
		
		{"mr_enable_x", "", validate_range, ARGV("0","1"), FALSE, RESTART_REBOOT},	// 2008.03 James
              	
              {"sr_rip_x", "", validate_choice, ARGV(              
              
                   "0:Disabled",
              
                   "1:LAN",
              
                   "2:WAN",
              
                   "3:BOTH",
              
              0), FALSE, RESTART_REBOOT},	// 2007.10 James
                                    
                       {"sr_num_x", "", validate_range, ARGV("0","65535"), FALSE, RESTART_REBOOT},	// 2007.10 James
                                     
                 {"dr_static_rip_x", "", validate_range, ARGV("0","1"), FALSE, RESTART_REBOOT},	// 2007.10 James
                           
             {"dr_static_matric_x", "", validate_range, ARGV("1", "16", "1"), FALSE, RESTART_REBOOT},	// 2007.10 James
        	
              {"dr_default_x", "", validate_choice, ARGV(              
              
                   "0:be redistributed",
              
                   "1:not be redistributed",
              
              0), FALSE, RESTART_REBOOT},	// 2007.10 James
           
      {"RouteInfo", "Status", NULL, ARGV("route.log",""), FALSE, FALSE},
                         
                       {"dr_static_rip_x", "", validate_range, ARGV("0","65535"), FALSE, RESTART_REBOOT},	// 2007.10 James
                                             
                       {"dr_staticnum_x", "", validate_range, ARGV("0","65535"), FALSE, RESTART_REBOOT},	// 2007.10 James
#ifndef NOUSB
                                             
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                                     
                 {"", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
              	
              {"", "", validate_choice, ARGV(              
              
                   "RIP1",
              
                   "RIP2",
              
                   "Both",
              
              0), FALSE, FALSE},
                            
                 {"", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
              	
              {"", "", validate_choice, ARGV(              
              
                   "Disable",
              
                   "Text",
              
                   "MD5",
              
              0), FALSE, FALSE},
                                                   
                    {"", "", validate_string, ARGV("16"), FALSE, FALSE},                                                                                                                   
                                  
                 {"", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
              	
              {"", "", validate_choice, ARGV(              
              
                   "RIP1",
              
                   "RIP2",
              
                   "Both",
              
              0), FALSE, FALSE},
                            
                 {"", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
              	
              {"", "", validate_choice, ARGV(              
              
                   "Disable",
              
                   "Text",
              
                   "MD5",
              
              0), FALSE, FALSE},
                                                   
                    {"", "", validate_string, ARGV("16"), FALSE, FALSE},                                                                                                                   
                                          
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                                             
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                                             
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                    	
              {"", "", validate_choice, ARGV(              
              
                   "1:processed",
              
                   "0:dropped",
              
              0), FALSE, FALSE},
           	
              {"", "", validate_choice, ARGV(              
              
                   "1:processed",
              
                   "0:dropped",
              
              0), FALSE, FALSE},
           	
              {"", "", validate_choice, ARGV(              
              
                   "1:processed",
              
                   "0:dropped",
              
              0), FALSE, FALSE},
           	
              {"", "", validate_choice, ARGV(              
              
                   "1:processed",
              
                   "0:dropped",
              
              0), FALSE, FALSE},
                        
             {"", "", validate_range, ARGV("1", "16", "1"), FALSE, FALSE},
        	
              {"", "", validate_choice, ARGV(              
              
                   "0:none",
              
                   "1:routes specified below",
              
                   "2:routes not specified below",
              
              0), FALSE, FALSE},
           	
              {"", "", validate_choice, ARGV(              
              
                   "0:none",
              
                   "1:routes specified below",
              
                   "2:routes not specified below",
              
              0), FALSE, FALSE},
                                    
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                                             
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                               
#endif             
                     
      {"GWStatic", "Group", validate_group, ARGV(variables_RouterConfig_GWStatic, "6", "59", "sr_num_x"), FALSE, RESTART_REBOOT},	// 2008.01 James.
      
#ifndef NOUSB 
      {"StaticRoute", "Group", validate_group, ARGV(variables_RouterConfig_StaticRoute, "16", "46", "dr_staticnum_x"), FALSE, FALSE},     
#endif

#ifndef NOUSB       
      {"RipSRoute", "Group", validate_group, ARGV(variables_RouterConfig_RipSRoute, "16", "24", "RipSRouteCount"), FALSE, FALSE},     
       
      {"RouteFilter", "Group", validate_group, ARGV(variables_RouterConfig_RouteFilter, "16", "33", "RipDRouteCount"), FALSE, FALSE},     
       
      {"RipOffset", "Group", validate_group, ARGV(variables_RouterConfig_RipOffset, "16", "30", "RipOffsetCount"), FALSE, FALSE},     
#endif
                        
      {0,0,0,0,0,0}
      };
   
      struct variable variables_LANHostConfig[] = {
		{"wcn_enable", "", validate_range, ARGV("0","1"), FALSE, RESTART_REBOOT},	// 2007.10 James
                               
                {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                                                                 
                {"jumbo_frame_enable", "", validate_range, ARGV("0","1"), FALSE, RESTART_REBOOT},	

                {"jumbo_frame_size", "", validate_range, ARGV("0","9720"), FALSE, RESTART_REBOOT},

                {"udpxy_enable_x", "", validate_range, ARGV("0","65535"), FALSE, RESTART_REBOOT},

                {"lan_proto_x", "", validate_range, ARGV("0","1"), FALSE, RESTART_REBOOT},	// 2007.10 James
              
                {"lan_ipaddr", "", validate_ipaddr, NULL, FALSE, RESTART_REBOOT},	// 2007.10 James
            
                {"lan_netmask", "", validate_ipaddr, NULL, FALSE, RESTART_REBOOT},	// 2007.10 James
            
      {"x_LANIPAddress", "Status", NULL, ARGV("lan.log","lan_ipaddr_t"), FALSE, FALSE},

      {"x_LANSubnetMask", "Status", NULL, ARGV("lan.log","lan_netmask_t"), FALSE, FALSE},

      {"x_LANGateway", "Status", NULL, ARGV("lan.log","lan_gateway_t"), FALSE, FALSE},
                                        
                    {"lan_hostname", "", validate_string, ARGV("32"), FALSE, RESTART_REBOOT},	// 2007.10 James
                 
                {"lan_gateway", "", validate_ipaddr, NULL, FALSE, RESTART_REBOOT},	// 2007.10 James
                             
                 {"dhcp_enable_x", "", validate_range, ARGV("0","1"), FALSE, RESTART_REBOOT},	// 2007.10 James
#ifndef NOUSB                               
                 {"", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
              
                {"", "", validate_ipaddr, NULL, FALSE, FALSE},
#endif
                                                    
                    {"lan_domain", "", validate_string, ARGV("32"), FALSE, RESTART_REBOOT},	// 2007.10 James
                 
                {"dhcp_start", "", validate_ipaddr, NULL, FALSE, RESTART_DHCPD},	// 2007.10 James
            
                {"dhcp_end", "", validate_ipaddr, NULL, FALSE, RESTART_DHCPD},	// 2007.10 James
                         
             {"dhcp_lease", "", validate_range, ARGV("1", "86400", ""), FALSE, RESTART_DHCPD},	// 2007.10 James
        
                {"dhcp_gateway_x", "", validate_ipaddr, NULL, FALSE, RESTART_REBOOT},	// 2007.10 James
            
                {"dhcp_dns1_x", "", validate_ipaddr, NULL, FALSE, RESTART_REBOOT},	// 2007.10 James
            
      {"x_LDNSServer2", "Status", NULL, ARGV("LANHostConfig","lan_ipaddr"), FALSE, RESTART_REBOOT},	// 2007.10 James

                {"dhcp_wins_x", "", validate_ipaddr, NULL, FALSE, RESTART_DHCPD},	// 2007.10 James
                             
                 {"dhcp_static_x", "", validate_range, ARGV("0","1"), FALSE, RESTART_REBOOT},	// 2007.10 James
                                       
                       {"dhcp_staticnum_x", "", validate_range, ARGV("0","65535"), FALSE, RESTART_REBOOT},	// 2007.10 James
                                             
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                    
      {"DHCPLog", "Status", NULL, ARGV("leases.log",""), FALSE, FALSE},

#ifndef NOUSB                 
                 {"", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
                                                      
                    {"", "", validate_string, ARGV("32"), FALSE, FALSE},                                                                                                                   
                 
                {"", "", validate_ipaddr, NULL, FALSE, FALSE},
            
                {"", "", validate_ipaddr, NULL, FALSE, FALSE},
                                   
                       {"", "", validate_range, ARGV("0","4294927695"), FALSE, FALSE},                                                             
                    
                {"", "", validate_ipaddr, NULL, FALSE, FALSE},
            
      {"x_DmzLDNSServer2", "Status", NULL, ARGV("FirewallConfig","DmzIP"), FALSE, FALSE},

                {"", "", validate_ipaddr, NULL, FALSE, FALSE},
                             
                 {"", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
                                                              {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},     
                                   #endif
                    
      {"DmzDHCPLog", "Status", NULL, ARGV("dleases.log",""), FALSE, FALSE},
                 
                 {"upnp_enable", "", validate_range, ARGV("0","1"), FALSE, RESTART_UPNP},	// 2007.10 James

                 {"telnet_enable", "", validate_range, ARGV("0","1"), FALSE, RESTART_REBOOT},
              
                {"log_ipaddr", "", validate_ipaddr, NULL, FALSE, RESTART_SYSLOG},	// 2007.10 James

              {"time_zone", "", validate_choice, 
#ifndef NOUSB             	
							ARGV(
									"UCT12:(GMT-12:00) Eniwetok, Kwajalein",
									"UCT11:(GMT-11:00) Midway Island, Samoa",
									"UCT10:(GMT-10:00) Hawaii",
									"NAST9NADT:(GMT-09:00) Alaska",
									"PST8PDT:(GMT-08:00) Pacific Time (US, Canada)",
									"MST7MDT:(GMT-07:00) Mountain Time (US, Canada)",
									"MST7:(GMT-07:00) Arizona",
									"MST7MDT:(GMT-07:00) Chihuahua, La Paz, Mazatlan",
									"CST6CDT_1:(GMT-06:00) Central Time (US, Canada)",
									"CST6CDT_2:(GMT-06:00) Saskatchewan",
									"CST6CDT_3:(GMT-06:00) Guadalajara, Mexico City",
									"CST6CDT_3_1:(GMT-06:00) Monterrey",
									"UCT6:(GMT-06:00) Central America",
									"EST5EDT:(GMT-05:00) Eastern Time (US, Canada)",
									"UCT5_1:(GMT-05:00) Indiana (East)",
									"UCT5_2:(GMT-05:00) Bogota, Lima, Quito",
									"AST4ADT:(GMT-04:00) Atlantic Time (Canada)",
									"UCT4_1:(GMT-04:00) Caracas, La Paz",
									"UCT4_2:(GMT-04:00) Santiago",
									"NST3.30:(GMT-03:00) Newfoundland",
									"EBST3EBDT_1:(GMT-03:00) Brasilia",
									"UCT3:(GMT-03:00) Buenos Aires, Georgetown",
									"EBST3EBDT_2:(GMT-03:00) Greenland",
									"NORO2:(GMT-02:00) Mid-Atlantic",
									"EUT1EUTDST:(GMT-01:00) Azores",
									"UCT1:(GMT+01:00) Cape Verde Is.",
									"GMT0BST_1:(GMT) Greenwich Mean Time",
									"GMT0BST_2:(GMT) Casablanca, Monrovia",
									"UCT-1_1:(GMT+01:00) Belgrade, Bratislava, Budapest",
									"UCT-1_1_1:(GMT+01:00) Ljubljana, Prague",
									"UCT-1_2:(GMT+01:00) Sarajevo, Skopje, Sofija",
									"UCT-1_2_1:(GMT+01:00) Vilnius, Warsaw, Zagreb",
									"MET-1METDST:(GMT+01:00) Brussels, Copenhagen",
									"MET-1METDST_1:(GMT+01:00) Madrid, Paris",
									"MEZ-1MESZ:(GMT+01:00) Amsterdam, Berlin, Bern",
									"MEZ-1MESZ_1:(GMT+01:00) Rome, Stockholm, Vienna",
									"UCT-1_3:(GMT+01:00) West Central Africa",
									"ET-2EETDST:(GMT+02:00) Bucharest",
									"EST-2EDT:(GMT+02:00) Cairo",
									"UCT-2_1:(GMT+02:00) Helsinki, Riga, Tallinn",
									"UCT-2_2:(GMT+02:00) Athens, Istanbul, Minsk",
									"IST-2IDT:(GMT+02:00) Jerusalem",
									"SAST-2:(GMT+02:00) Harare, Pretoria",
									"MST-3MDT:(GMT+03:00) Moscow, St. Petersburg",
									"MST-3MDT_1:(GMT+03:00) Volgograd",
									"UCT-3_1:(GMT+03:00) Kuwait, Riyadh",
									"UCT-3_2:(GMT+03:00) Nairobi",
									"IST-3IDT:(GMT+03:00) Baghdad",
									"UCT-3.30:(GMT+03:00) Tehran",
									"UCT-4_1:(GMT+04:00) Abu Dhabi, Muscat",
									"UCT-4_2:(GMT+04:00) Baku, Tbilisi, Yerevan",
									"UCT-4.30:(GMT+04:30) Kabul",
									"RFT-5RFTDST:(GMT+05:00) Ekaterinburg",
									"UCT-5:(GMT+05:00) Islamabad, Karachi, Tashkent",
									"UCT-5.30:(GMT+05:30) Calcutta, Chennai",
									"UCT-5.30_1:(GMT+05:30) Mumbai, New Delhi",
									"UCT-5.45:(GMT+05:45) Kathmandu",
									"UCT-6:(GMT+06:00) Astana, Dhaka",
									"UCT-5.30:(GMT+06:00) Sri Jayawardenepura",
									"UCT-6RFTDST:(GMT+06:00) Almaty, Novosibirsk",
									"UCT-6.30:(GMT+06:30) Rangoon",
									"UCT-7:(GMT+07:00) Bangkok, Hanoi, Jakarta",
									"UCT-7RFTDST:(GMT+07:00) Krasnoyarsk",
									"CST-8:(GMT+08:00) Beijing, Hong Kong",
									"CST-8_1:(GMT+08:00) Chongqing, Urumqi",
									"SST-8:(GMT+08:00) Kuala Lumpur, Singapore",
									"CCT-8:(GMT+08:00) Taipei",
									"WAS-8WAD:(GMT+08:00) Perth",
									"UCT-8:(GMT+08:00) Irkutsk, Ulaan Bataar",
									"UCT-9_1:(GMT+09:00) Seoul",
									"JST:(GMT+09:00) Osaka, Sapporo, Tokyo",
									"UCT-9_2:(GMT+09:00) Yakutsk",
									"CST-9.30CDT:(GMT+09:30) Darwin",
									"UCT-9.30:(GMT+09:30) Adelaide",
									"UCT-10_1:(GMT+10:00) Canberra, Melbourne, Sydney",
									"UCT-10_2:(GMT+10:00) Brisbane",
									"TST-10TDT:(GMT+10:00) Hobart",
									"RFT-10RFTDST:(GMT+10:00) Vladivostok",
									"UCT-10_5:(GMT+10:00) Guam, Port Moresby",
									"UCT-11:(GMT+11:00) Magadan, Solomon Is.",
									"UCT-11_1:(GMT+11:00) New Caledonia",
									"UCT-12:(GMT+12:00) Fiji, Kamchatka, Marshall Is.",
									"NZST-12NZDT:(GMT+12:00) Auckland, Wellington",
									"UCT-13:(GMT+13:00) Nuku'alofa",
              0)
#else
							NULL
#endif
							, FALSE, RESTART_TIME},	// 2007.10 James
                                                   
                    {"time_interval", "", validate_string, ARGV(""), FALSE, RESTART_REBOOT},	// 2007.10 James
                                                         
                    {"ntp_server0", "", validate_string, ARGV(""), FALSE, RESTART_NTPC},	// 2007.10 James
                                                         
                    {"ntp_server1", "", validate_string, ARGV(""), FALSE, RESTART_NTPC},	// 2007.10 James
#ifndef NOUSB
                                  
                 {"", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
              
                {"", "", validate_ipaddr, NULL, FALSE, FALSE},
#endif
                             
                 {"ddns_enable_x", "", validate_range, ARGV("0","1"), FALSE, RESTART_DDNS},	// 2007.10 James
                 
                 {"asusddns_tos_agreement", "", validate_range, ARGV("0","1"), FALSE, FALSE},	// 2007.12 James added.
              	
              {"ddns_server_x", "", validate_choice, ARGV(              
              
		   "WWW.ASUS.COM",//2007.03.20 Yau add for asus ddns

                   "WWW.DYNDNS.ORG",
              
                   "WWW.DYNDNS.ORG(CUSTOM)",
              
                   "WWW.DYNDNS.ORG(STATIC)",
              
                   "WWW.TZO.COM",
              
                   "WWW.ZONEEDIT.COM",
              
              0), FALSE, RESTART_DDNS},	// 2007.10 James
                                                   
                    {"ddns_username_x", "", validate_string, ARGV("32"), FALSE, RESTART_DDNS},	// 2007.10 James
                 
                {"ddns_passwd_x", "", validate_string, ARGV("32"), FALSE, RESTART_DDNS},	// 2007.10 James
                                                    
                    {"ddns_hostname_x", "", validate_string, ARGV("32"), FALSE, RESTART_DDNS},	// 2007.10 James
                                  
                 {"ddns_wildcard_x", "", validate_range, ARGV("0","1"), FALSE, RESTART_DDNS},	// 2007.10 James
              
		//2007.03.20 Yau add for asus ddns
		{"ddns_timeout", "", validate_string, ARGV("32"), FALSE, RESTART_DDNS},	// 2007.10 James

		{"ddns_return_code", "", validate_string, ARGV("32"), FALSE, RESTART_DDNS},	// 2007.10 James

		{"ddns_old_name", "", validate_string, ARGV("32"), FALSE, RESTART_DDNS},	// 2007.10 James

		{"ddns_suggest_name", "", validate_string, ARGV("32"), FALSE, RESTART_DDNS},	// 2007.10 James
		//End of Yau add

      {"x_DDNSStatus", "Status", NULL, ARGV("ddns.log","DDNSStatus"), FALSE, FALSE},
 
      {"ManualDHCPList", "Group", validate_group, ARGV(variables_LANHostConfig_ManualDHCPList, "8", "29", "dhcp_staticnum_x"), FALSE, RESTART_DHCPD},	// 2007.11 James
      
#ifndef NOUSB 
      {"DmzManualDHCPList", "Group", validate_group, ARGV(variables_LANHostConfig_DmzManualDHCPList, "8", "29", "DmzManualDHCPCount"), FALSE, FALSE},     
       
      {"ReservedAddressList", "Group", validate_group, ARGV(variables_LANHostConfig_ReservedAddressList, "6", "24", "ReservedAddressListCount"), FALSE, FALSE}, 
#endif
                        
      {0,0,0,0,0,0}
      };
   
      struct variable variables_WLANConfig11a[] = {

#ifndef NOUSB
      
                {"", "", validate_string, NULL, FALSE, FALSE},
                                     
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                                                            
                    {"", "", validate_string, ARGV("32"), FALSE, FALSE},                                                                                                                   
                 
                {"", "", validate_wlchannel, NULL, FALSE, FALSE},
            
                {"", "", validate_string, ARGV(""), FALSE, FALSE}, 
            	
              {"", "", validate_choice, ARGV(              
              
                   "0:Best",
              
                   "1:6",
              
                   "2:9",
              
                   "3:12",
              
                   "4:18",
              
                   "5:24",
              
                   "6:36",
              
                   "7:48",
              
                   "8:54",
              
              0), FALSE, FALSE},
           
                {"", "", validate_string, ARGV(""), FALSE, FALSE}, 
            	
              {"", "", validate_choice, ARGV(              
              
                   "Open System",
              
                   "Shared Key",
              
              0), FALSE, FALSE},
           
                {"", "", validate_wlwep, NULL, FALSE, FALSE},
            
                {"", "", validate_string, ARGV("32"), FALSE, FALSE}, 
            
                {"", "", validate_wlkey, NULL, FALSE, FALSE},
            
                {"", "", validate_wlkey, NULL, FALSE, FALSE},
            
                {"", "", validate_wlkey, NULL, FALSE, FALSE},
            
                {"", "", validate_wlkey, NULL, FALSE, FALSE},
            	
              {"", "", validate_choice, ARGV(              
              
                   "Key1",
              
                   "Key2",
              
                   "Key3",
              
                   "Key4",
              
              0), FALSE, FALSE},
                            
                 {"", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
                               
                 {"", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
#endif
              
      {"WirelessLog", "Status", NULL, ARGV("wlan11a.log",""), FALSE, FALSE},
// 11N
{"wl_nbw", "20", 0, 0, 0, 0},
{"wl_nbw_cap", "1", validate_range, ARGV("0","1"), 0, 0},
{"wl_nctrlsb", "none", 0, 0, 0, 0},
{"wl_nband", "2", 0, 0, 0, 0},
{"wl_nmcsidx", "-1", 0, 0, 0, 0},
{"wl_nmode", "-1", 0, 0, 0, 0},
{"wl_leddc", "0x640000", 0, 0, 0, 0},
{"wl_wme_apsd", "on", 0, 0, 0, 0},
{"wl_sta_retry_time", "5", 0, 0, 0, 0},
{"wl_phytype", "b", 0,0,0,0},

                 
      {0,0,0,0,0,0}
      };
   
      struct variable variables_DeviceSecurity11a[] = {
                               
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                    	
              {"wl_macmode", "", validate_choice, ARGV(              
              
                   "disabled:Disable",
              
                   "allow:Accept",
              
                   "deny:Reject",
              
              0), FALSE, RESTART_REBOOT},	// 2007.10 James
           	
              {"wl_macapply_x", "", validate_choice, ARGV(              
              
                   "Both",
              
                   "802.11a only",
              
                   "802.11g only",
              
              0), FALSE, RESTART_REBOOT},	// 2007.10 James
                                    
                       {"wl_macnum_x", "", validate_range, ARGV("0","65535"), FALSE, RESTART_REBOOT},	// 2007.10 James
                                             
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                    
                {"", "", validate_string, ARGV(""), FALSE, FALSE}, 
             
      {"ACLList", "Group", validate_group, ARGV(variables_DeviceSecurity11a_ACLList, "64", "32", "wl_macnum_x"), FALSE, RESTART_REBOOT},	// 2008.04 James.
#ifndef NOUSB
       
      {"AESList", "Group", validate_group, ARGV(variables_DeviceSecurity11a_AESList, "64", "58", "AESListCount"), FALSE, FALSE},     
#endif
                        
      {0,0,0,0,0,0}
      };
   
      struct variable variables_WLANAuthentication11a[] = {
                               
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                    
                {"wl_radius_ipaddr", "", validate_ipaddr, NULL, FALSE, RESTART_REBOOT},	// 2007.10 James
                                     
                       {"wl_radius_port", "", validate_range, ARGV("0","65535"), FALSE, RESTART_REBOOT},	// 2007.10 James
                    
                {"wl_radius_key", "", validate_string, ARGV("64"), FALSE, RESTART_REBOOT},	// 2007.10 James
#ifndef NOUSB
            	
              {"", "", validate_choice, ARGV(              
              
                   "0:Disabled",
              
                   "1:LEAP",
              
                   "3:EAP/TLS",
              
                   "2:PEAP/MSCHAPv2",
              
                   "4:PEAP/TLS",
              
              0), FALSE, FALSE},
                                                   
                    {"", "", validate_string, ARGV(""), FALSE, FALSE},                                                                                                                   
                                                         
                    {"", "", validate_string, ARGV(""), FALSE, FALSE},                                                                                                                   
                  
      {"LocalAuthDB", "Group", validate_group, ARGV(variables_WLANAuthentication11a_LocalAuthDB, "32", "36", "AuthDBNumberOfEntries"), FALSE, FALSE},     
       
      {"LocalCertDB", "Group", validate_group, ARGV(variables_WLANAuthentication11a_LocalCertDB, "32", "40", "CertDBNumberOfEntries"), FALSE, FALSE},     
#endif
                        
      {0,0,0,0,0,0}
      };
   
      struct variable variables_WLANConfig11b[] = {
                               
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                                             
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
			
#ifdef WSC
			{"wl_wps_mode", "", validate_string, ARGV("enabled","disabled"), FALSE, RESTART_REBOOT},	// 2008.01 James.
			{"wps_mode", "", validate_string, ARGV("enabled","disabled"), FALSE, RESTART_REBOOT},	// 2008.01 James.
			{"wps_config_state", "", validate_range, ARGV("0","1"), FALSE, RESTART_REBOOT},	// 2008.01 James.
			{"wps_proc_status", "", validate_range, ARGV("0","4"), FALSE, RESTART_WPS},	// 2008.01 James.
			{"wps_method", "", validate_range, ARGV("0","2"), FALSE, RESTART_WPS},	// 2008.01 James.
			{"wps_config_command", "", validate_range, ARGV("0","2"), FALSE, RESTART_WPS},	// 2008.01 James.
			{"wps_pbc_force", "", validate_range, ARGV("0","2"), FALSE, RESTART_WPS},	// 2008.01 James.
			{"wps_sta_pin", "", validate_string, ARGV("8"), FALSE, RESTART_WPS},	// 2008.01 James.
			{"pbc_overlap", "", validate_range, ARGV("0", "1"), FALSE, RESTART_WPS},	// 2008.02 James.
			{"wps_client_role", "", validate_string, ARGV("registrar", "enrollee"), FALSE, FALSE},	// 2008.06 James.
#endif
			
			{"wl_country_code", "", validate_string, NULL, FALSE, RESTART_REBOOT},	// 2007.10 James
                                                    
			{"wl_bss_enabled", "", validate_choice, ARGV("0","1"), FALSE, RESTART_REBOOT},    // 2008.06 James
			{"wl_ssid", "", validate_string, ARGV("32"), FALSE, RESTART_REBOOT},	// 2007.10 James
			//{"wl_ssid2", "", validate_string, ARGV("32"), FALSE, RESTART_REBOOT},	// 2007.10 James
			{"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                    	
              {"wl_mode_x", "", validate_choice, ARGV(              
              
                   "0:AP Only",
              
                   "1:WDS Only",
              
                   "2:Hybrid",
              
              0), FALSE, RESTART_REBOOT},	// 2007.10 James
           
                {"wl_channel", "", validate_wlchannel, NULL, FALSE, RESTART_REBOOT},	// 2007.10 James
                             
                 {"wl_wdsapply_x", "", validate_range, ARGV("0","1"), FALSE, RESTART_REBOOT},	// 2007.10 James
                               
                 {"wl_lazywds", "", validate_range, ARGV("0","1"), FALSE, RESTART_REBOOT},	// 2007.10 James
                                       
                 {"wl_wdsnum_x", "", validate_range, ARGV("0","65535"), FALSE, RESTART_REBOOT},	// 2007.10 James
                    	
                 {"wl_maxassoc", "", validate_range, ARGV("1","128"), FALSE, RESTART_REBOOT},	// 2008.06 James

                 {"wl_bss_maxassoc", "", validate_range, ARGV("1","128"), FALSE, RESTART_REBOOT},	// 2008.06 James
				 
              {"wl_gmode", "", validate_choice, ARGV(              
              
                   "1:Auto",
              
                   "4:54G Only",
              
                   "0:802.11B Only",
              
              0), FALSE, RESTART_REBOOT},	// 2007.10 James
           
                {"wl_gmode_protection_x", "", validate_string, ARGV(""), FALSE, RESTART_REBOOT},	// 2007.10 James
            	
              {"wl_auth_mode", "", validate_choice, ARGV(              
              
                   "open:Open System or Shared Key",
              
                   "shared:Shared Key",
              
                   "psk:WPA-PSK",
              
                   "wpa:WPA",
              
                   "radius:Radius with 802.1x",
              
              0), FALSE, RESTART_REBOOT},	// 2007.10 James

              {"wl_wpa_mode", "", validate_choice, ARGV(              
              
                   "0:WPA-Auto-Personal",
              
                   "1:WPA-Personal",
              
                   "2:WPA2-Personal",
		
		   "3:WPA-Enterprise",
	
		   "4:WPA-Auto-Enterprise",

              0), FALSE, RESTART_REBOOT},	// 2007.10 James

              {"wl_crypto", "", validate_choice, ARGV(              
              
                   "tkip:TKIP",
              
                   "aes:AES",
              
                   "tkip+aes:TKIP+AES",
              
              0), FALSE, RESTART_REBOOT},	// 2007.10 James
           
                {"wl_wpa_psk", "", validate_string, ARGV("64"), FALSE, RESTART_REBOOT},	// 2007.10 James
            	
              {"wl_wep_x", "", validate_choice, ARGV(              
              
                   "0:None",
              
                   "1:WEP-64bits",
              
                   "2:WEP-128bits",
              
              0), FALSE, RESTART_REBOOT},	// 2007.10 James
				
				{"wep_key_type", "", validate_choice, ARGV(
						"0:ASCII digits",
						"1:Hex digits",
						0), FALSE, RESTART_REBOOT},	// 2008.01 James
				
                {"wl_phrase_x", "", validate_string, ARGV("64"), FALSE, RESTART_REBOOT},	// 2007.10 James
            
                {"wl_key1", "", validate_wlkey, NULL, FALSE, RESTART_REBOOT},	// 2007.10 James
            
                {"wl_key2", "", validate_wlkey, NULL, FALSE, RESTART_REBOOT},	// 2007.10 James
            
                {"wl_key3", "", validate_wlkey, NULL, FALSE, RESTART_REBOOT},	// 2007.10 James
            
                {"wl_key4", "", validate_wlkey, NULL, FALSE, RESTART_REBOOT},	// 2007.10 James
            	
              {"wl_key", "", validate_choice, ARGV(              
              
                   "1:Key1",
              
                   "2:Key2",
              
                   "3:Key3",
              
                   "4:Key4",
              
              0), FALSE, RESTART_REBOOT},	// 2007.10 James
                        
             {"wl_wpa_gtk_rekey", "", validate_range, ARGV("0", "86400", ""), FALSE, RESTART_REBOOT},	// 2007.10 James
                                 
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                    	
              {"wl_afterburner", "", validate_choice, ARGV(              
              
                   "off:Disabled",
              
                   "auto:Enabled",
              
              0), FALSE, RESTART_REBOOT},	// 2007.10 James

			  {"wl_nbw_cap", "", validate_choice, ARGV("0", "1", "2"), FALSE, RESTART_REBOOT},   // 2008.06 James
			  
			  {"wl_txstreams", "", validate_range, ARGV("0", "1", "2"), FALSE, RESTART_REBOOT},   // 2008.06 James
			  
			  {"wl_rxstreams", "", validate_range, ARGV("0", "1", "2"), FALSE, RESTART_REBOOT},   // 2008.06 James
                            
                 {"wl_closed", "", validate_range, ARGV("0","1"), FALSE, RESTART_REBOOT},	// 2007.10 James
                               
                 {"wl_ap_isolate", "", validate_range, ARGV("0","1"), FALSE, RESTART_REBOOT},	// 2007.10 James
				 
                 {"wl_radarthrs", "", validate_string, ARGV(""), FALSE, RESTART_REBOOT},	// 2008.06 James
              	
              {"wl_rate", "", validate_choice, ARGV(              
              
                   "0:Auto",
              
                   "1000000:1",
              
                   "2000000:2",
              
                   "5500000:5.5",
              
                   "6000000:6",
              
                   "9000000:9",
              
                   "11000000:11",
              
                   "12000000:12",
              
                   "18000000:18",
              
                   "24000000:24",
              
                   "36000000:36",
              
                   "48000000:48",
              
                   "54000000:54",
              
              0), FALSE, RESTART_REBOOT},	// 2007.10 James

              {"wl_mrate", "", validate_choice, ARGV(              
              
									"0:Auto",
									
									"1000000:1",
									
             	    "2000000:2",
									
									"5500000:5.5",
									
									"6000000:6",
									
									"9000000:9",
									
									"11000000:11",
									
									"12000000:12",
									
									"18000000:18",
									
									"24000000:24",
									
									"36000000:36",
									
									"48000000:48",
									
									"54000000:54",
									
									0), FALSE, RESTART_REBOOT},	// 2008.03 James
           	
              {"wl_rateset", "", validate_choice, ARGV(              
              
                   "default:Default",
              
                   "all:All",
              
                   "12:1, 2 Mbps",
              
              0), FALSE, RESTART_REBOOT},	// 2007.10 James
                        
             {"wl_frag", "", validate_range, ARGV("256", "2346", ""), FALSE, RESTART_REBOOT},	// 2007.10 James
                     
             {"wl_rts", "", validate_range, ARGV("0", "2347", ""), FALSE, RESTART_REBOOT},	// 2007.10 James
                     
             {"wl_dtim", "", validate_range, ARGV("1", "255", ""), FALSE, RESTART_REBOOT},	// 2007.10 James
                     
             {"wl_bcn", "", validate_range, ARGV("1", "65535", ""), FALSE, RESTART_REBOOT},	// 2007.10 James
        	
              {"wl_frameburst", "", validate_choice, ARGV(              
              
                   "off:Disabled",
              
                   "on:Enabled",
              
              0), FALSE, RESTART_REBOOT},	// 2007.10 James
           	
              {"wl_mode_ex", "", validate_choice, ARGV(              
              
                   "ap:AP or WDS",
              
                   "sta:Station",
              
                   "wet:Ethernet Bridge",
              
              0), FALSE, RESTART_REBOOT},	// 2007.10 James
                            
                 {"wl_radio_x", "", validate_range, ARGV("0","1"), FALSE, RESTART_REBOOT},	// 2007.10 James
              
                {"wl_radio_date_x", "", validate_portrange, NULL, FALSE, RESTART_REBOOT},	// 2007.10 James
            
                {"wl_radio_time_x", "", validate_portrange, NULL, FALSE, RESTART_REBOOT},	// 2007.10 James
                         
             {"wl_radio_power_x", "", validate_range, ARGV("1", "84", ""), FALSE, RESTART_REBOOT},	// 2007.10 James
        
      {"WirelessLog", "Status", NULL, ARGV("wlan11b.log",""), FALSE, FALSE},
                                        
                    {"", "", validate_string, ARGV(""), FALSE, FALSE},                                                                                                                   
                 	
              {"wl_wme", "", validate_choice, ARGV(              
              
                   "off:Disabled",
              
                   "on:Enabled",
              
              0), FALSE, RESTART_REBOOT},	// 2007.10 James
           	
              {"wl_wme_bss_disable", "", validate_choice, ARGV("0", "1"), FALSE, RESTART_REBOOT}, // 2008.06 James
			  
              {"wl_wme_no_ack", "", validate_choice, ARGV(              
              
                   "off:Disabled",
              
                   "on:Enabled",
              
              0), FALSE, RESTART_REBOOT},	// 2007.10 James

              {"wl_mimo_preamble", "", validate_choice, ARGV(

                   "gf:Green Field",

                   "mm:Mixed Mode",
              
              0), FALSE, RESTART_REBOOT},	// 2008.12 Yen

		{"wl_plcphdr", "", validate_choice, ARGV(

                   "long:Long",

                   "short:Short",
              
              0), FALSE, RESTART_REBOOT	},

              {"wl_wme_apsd", "", validate_choice, ARGV(              
              
                   "off:Disabled",
              
                   "on:Enabled",
              
              0), FALSE, RESTART_REBOOT},	// 2008.12 Yen

                                                   
                    {"wl_wme_ap_bk", "", validate_string, ARGV(""), FALSE, RESTART_REBOOT},	// 2007.10 James
                                                         
                    {"wl_wme_ap_be", "", validate_string, ARGV(""), FALSE, RESTART_REBOOT},	// 2007.10 James
                                                         
                    {"wl_wme_ap_vi", "", validate_string, ARGV(""), FALSE, RESTART_REBOOT},	// 2007.10 James
                                                         
                    {"wl_wme_ap_vo", "", validate_string, ARGV(""), FALSE, RESTART_REBOOT},	// 2007.10 James
                                                         
                    {"wl_wme_sta_bk", "", validate_string, ARGV(""), FALSE, RESTART_REBOOT},	// 2007.10 James
                                                         
                    {"wl_wme_sta_be", "", validate_string, ARGV(""), FALSE, RESTART_REBOOT},	// 2007.10 James
                                                         
                    {"wl_wme_sta_vi", "", validate_string, ARGV(""), FALSE, RESTART_REBOOT},	// 2007.10 James
                                                         
                {"wl_wme_sta_vo", "", validate_string, ARGV(""), FALSE, RESTART_REBOOT},	// 2007.10 James

		{"wl_wme_txp_be", "", validate_string, ARGV(""), FALSE, RESTART_REBOOT},    // 2008.06 James

		{"wl_wme_txp_bk", "", validate_string, ARGV(""), FALSE, RESTART_REBOOT},    // 2008.06 James

		{"wl_wme_txp_vi", "", validate_string, ARGV(""), FALSE, RESTART_REBOOT},    // 2008.06 James

		{"wl_wme_txp_vo", "", validate_string, ARGV(""), FALSE, RESTART_REBOOT},    // 2008.06 James

                {"wl_preauth", "", validate_range, ARGV("0","1"), FALSE, RESTART_REBOOT},	// 2007.10 James
                                                      
                {"wl_net_reauth", "", validate_string, ARGV(""), FALSE, RESTART_REBOOT},	// 2007.10 James

                {"sw_mode", "", validate_string, ARGV(""), FALSE, RESTART_REBOOT},	
                {"ui_triggered", "", validate_string, ARGV(""), FALSE, RESTART_REBOOT},	

{"wl_guest_enable", "0", 0, 0, 0, RESTART_REBOOT},	// 2007.10 James
{"wl_guest_ssid_1", "guest", 0, 0, 0, RESTART_REBOOT},	// 2007.10 James
{"wl_guest_auth_mode_1", "open", 0, 0, 0, RESTART_REBOOT},	// 2007.10 James
{"wl_guest_crypto_1", "0", 0, 0, 0, RESTART_REBOOT},	// 2007.10 James
{"wl_guest_wpa_psk_1", "", 0, 0, 0, RESTART_REBOOT},	// 2007.10 James
{"wl_guest_wep_x_1", "0", 0, 0, 0, RESTART_REBOOT},	// 2007.10 James
{"wl_guest_phrase_x_1", "", 0, 0, 0, RESTART_REBOOT},	// 2007.10 James
{"wl_guest_key1_1", "", 0, 0, 0, RESTART_REBOOT},	// 2007.10 James
{"wl_guest_key2_1", "", 0, 0, 0, RESTART_REBOOT},	// 2007.10 James
{"wl_guest_key3_1", "", 0, 0, 0, RESTART_REBOOT},	// 2007.10 James
{"wl_guest_key4_1", "", 0, 0, 0, RESTART_REBOOT},	// 2007.10 James
{"wl_guest_key_1", "1", 0, 0, 0, RESTART_REBOOT},	// 2007.10 James
{"wl_guest_wpa_gtk_rekey_1", "0", 0, 0, 0, RESTART_REBOOT},	// 2007.10 James
{"lan1_ipaddr", "192.168.2.1", 0, 0, 0, RESTART_REBOOT},	// 2007.10 James
{"lan1_netmask", "255.255.255.0", 0, 0, 0, RESTART_REBOOT},	// 2007.10 James
{"dhcp1_enable_x", "1", 0, 0, 0, RESTART_REBOOT},	// 2007.10 James
{"dhcp1_start", "192.168.2.2", 0, 0, 0, RESTART_DHCPD},	// 2007.10 James
{"dhcp1_end", "192.168.2.254", 0, 0, 0, RESTART_DHCPD},	// 2007.10 James
{"lan1_lease", "86400", 0, 0, 0, RESTART_DHCPD},	// 2007.10 James

{ "sta_ssid", "", 0, 0, 0, RESTART_REBOOT},
{ "sta_auth_mode", "", 0, 0, 0, RESTART_REBOOT},
{ "sta_wep_x", "", 0, 0, 0, RESTART_REBOOT},
{ "sta_wpa_mode", "", 0, 0, 0, RESTART_REBOOT},
{ "sta_crypto", "", 0, 0, 0, RESTART_REBOOT},
{ "sta_wpa_psk", "", 0, 0, 0, RESTART_REBOOT},
{ "sta_key", "", 0, 0, 0, RESTART_REBOOT},
{ "sta_key_type", "", 0, 0, 0, RESTART_REBOOT},
{ "sta_key1", "", 0, 0, 0, RESTART_REBOOT},
{ "sta_key2", "", 0, 0, 0, RESTART_REBOOT},
{ "sta_key3", "", 0, 0, 0, RESTART_REBOOT},
{ "sta_key4", "", 0, 0, 0, RESTART_REBOOT},

#if 0
{ "sta_ssid", "", 0, 0, 0, RESTART_URE},
{ "sta_auth_mode", "", 0, 0, 0, RESTART_URE},
{ "sta_wep_x", "", 0, 0, 0, RESTART_URE},
{ "sta_wpa_mode", "", 0, 0, 0, RESTART_URE},
{ "sta_crypto", "", 0, 0, 0, RESTART_URE},
{ "sta_wpa_psk", "", 0, 0, 0, RESTART_URE},
{ "sta_key", "", 0, 0, 0, RESTART_URE},
{ "sta_key_type", "", 0, 0, 0, RESTART_URE},
{ "sta_key1", "", 0, 0, 0, RESTART_URE},
{ "sta_key2", "", 0, 0, 0, RESTART_URE},
{ "sta_key3", "", 0, 0, 0, RESTART_URE},
{ "sta_key4", "", 0, 0, 0, RESTART_URE},
#endif

/* Multiple SSID */
{"mbss1_enabled", "0", 0, 0, 0, RESTART_REBOOT},
{"mbss1_akm", "", 0, 0, 0, RESTART_REBOOT},
{"mbss1_wpa_mode", "0", 0, 0, 0, RESTART_REBOOT},
{"mbss1_ap_isolate", "0", 0, 0, 0, RESTART_REBOOT},
{"mbss1_auth", "0", 0, 0, 0, RESTART_REBOOT},
{"mbss1_auth_mode", "open", 0, 0, 0, RESTART_REBOOT},
{"mbss1_closed", "0", 0, 0, 0, RESTART_REBOOT},
{"mbss1_crypto", "tkip", 0, 0, 0, RESTART_REBOOT},
{"mbss1_key1", "", 0, 0, 0, RESTART_REBOOT},
{"mbss1_key2", "", 0, 0, 0, RESTART_REBOOT},
{"mbss1_key3", "", 0, 0, 0, RESTART_REBOOT},
{"mbss1_key4", "", 0, 0, 0, RESTART_REBOOT},
{"mbss1_key", "1", 0, 0, 0, RESTART_REBOOT},
{"mbss1_ssid", "ASUS_MSSID_1", 0, 0, 0, RESTART_REBOOT},
{"mbss1_wep_x", "0", 0, 0, 0, RESTART_REBOOT},
{"mbss1_wpa_gtk_rekey", "0", 0, 0, 0, RESTART_REBOOT},
{"mbss1_wpa_psk", "", 0, 0, 0, RESTART_REBOOT},
{"mbss1_nolan", "0", 0, 0, 0, RESTART_REBOOT},
{"mbss1_nowan", "0", 0, 0, 0, RESTART_REBOOT},
{"mbss1_priority", "1", 0, 0, 0, RESTART_REBOOT},

{"mbss2_enabled", "0", 0, 0, 0, RESTART_REBOOT},
{"mbss2_akm", "", 0, 0, 0, RESTART_REBOOT},
{"mbss2_wpa_mode", "0", 0, 0, 0, RESTART_REBOOT},
{"mbss2_ap_isolate", "0", 0, 0, 0, RESTART_REBOOT},
{"mbss2_auth", "0", 0, 0, 0, RESTART_REBOOT},
{"mbss2_auth_mode", "open", 0, 0, 0, RESTART_REBOOT},
{"mbss2_closed", "0", 0, 0, 0, RESTART_REBOOT},
{"mbss2_crypto", "tkip", 0, 0, 0, RESTART_REBOOT},
{"mbss2_key1", "", 0, 0, 0, RESTART_REBOOT},
{"mbss2_key2", "", 0, 0, 0, RESTART_REBOOT},
{"mbss2_key3", "", 0, 0, 0, RESTART_REBOOT},
{"mbss2_key4", "", 0, 0, 0, RESTART_REBOOT},
{"mbss2_key", "1", 0, 0, 0, RESTART_REBOOT},
{"mbss2_ssid", "ASUS_MSSID_2", 0, 0, 0, RESTART_REBOOT},
{"mbss2_wep_x", "0", 0, 0, 0, RESTART_REBOOT},
{"mbss2_wpa_gtk_rekey", "0", 0, 0, 0, RESTART_REBOOT},
{"mbss2_wpa_psk", "", 0, 0, 0, RESTART_REBOOT},
{"mbss2_nolan", "0", 0, 0, 0, RESTART_REBOOT},
{"mbss2_nowan", "0", 0, 0, 0, RESTART_REBOOT},
{"mbss2_priority", "1", 0, 0, 0, RESTART_REBOOT},

{"mbss3_enabled", "0", 0, 0, 0, RESTART_REBOOT},
{"mbss3_akm", "", 0, 0, 0, RESTART_REBOOT},
{"mbss3_wpa_mode", "0", 0, 0, 0, RESTART_REBOOT},
{"mbss3_ap_isolate", "0", 0, 0, 0, RESTART_REBOOT},
{"mbss3_auth", "0", 0, 0, 0, RESTART_REBOOT},
{"mbss3_auth_mode", "open", 0, 0, 0, RESTART_REBOOT},
{"mbss3_closed", "0", 0, 0, 0, RESTART_REBOOT},
{"mbss3_crypto", "tkip", 0, 0, 0, RESTART_REBOOT},
{"mbss3_key1", "", 0, 0, 0, RESTART_REBOOT},
{"mbss3_key2", "", 0, 0, 0, RESTART_REBOOT},
{"mbss3_key3", "", 0, 0, 0, RESTART_REBOOT},
{"mbss3_key4", "", 0, 0, 0, RESTART_REBOOT},
{"mbss3_key", "1", 0, 0, 0, RESTART_REBOOT},
{"mbss3_ssid", "ASUS_MSSID_3", 0, 0, 0, RESTART_REBOOT},
{"mbss3_wep_x", "0", 0, 0, 0, RESTART_REBOOT},
{"mbss3_wpa_gtk_rekey", "0", 0, 0, 0, RESTART_REBOOT},
{"mbss3_wpa_psk", "", 0, 0, 0, RESTART_REBOOT},
{"mbss3_nolan", "0", 0, 0, 0, RESTART_REBOOT},
{"mbss3_nowan", "0", 0, 0, 0, RESTART_REBOOT},
{"mbss3_priority", "1", 0, 0, 0, RESTART_REBOOT},

      {"RBRList", "Group", validate_group, ARGV(variables_WLANConfig11b_RBRList, "16", "32", "wl_wdsnum_x"), FALSE, RESTART_REBOOT},	// 2008.01 James.
                        
      {0,0,0,0,0,0}
      };
   
      struct variable variables_DeviceSecurity11b[] = {
                               
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                    	
              {"wl_macmode", "", validate_choice, ARGV(              
              
                   "disabled:Disable",
              
                   "allow:Accept",
              
                   "deny:Reject",
              
              0), FALSE, RESTART_REBOOT},	// 2007.10 James
           	
              {"wl_macapply_x", "", validate_choice, ARGV(              
              
                   "Both",
              
                   "802.11a only",
              
                   "802.11g only",
              
              0), FALSE, RESTART_REBOOT},	// 2007.10 James
                                    
                       {"wl_macnum_x", "", validate_range, ARGV("0","65535"), FALSE, RESTART_REBOOT},	// 2007.10 James
                                             
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                    
                {"", "", validate_string, ARGV(""), FALSE, FALSE}, 
             
      {"ACLList", "Group", validate_group, ARGV(variables_DeviceSecurity11b_ACLList, "64", "32", "wl_macnum_x"), FALSE, RESTART_REBOOT},	// 2008.01 James.
      

#ifndef NOUSB
       
      {"AESList", "Group", validate_group, ARGV(variables_DeviceSecurity11b_AESList, "64", "58", "AESListCount"), FALSE, FALSE},     
#endif
                        
      {0,0,0,0,0,0}
      };
   
      struct variable variables_WLANAuthentication11b[] = {
                               
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                    
                {"wl_radius_ipaddr", "", validate_ipaddr, NULL, FALSE, RESTART_REBOOT},	// 2007.10 James
                                     
                       {"wl_radius_port", "", validate_range, ARGV("0","65535"), FALSE, RESTART_REBOOT},	// 2007.10 James
                    
                {"wl_radius_key", "", validate_string, ARGV("64"), FALSE, RESTART_REBOOT},	// 2007.10 James
            	
#ifndef NOUSB
              {"", "", validate_choice, ARGV(              
              
                   "0:Disabled",
              
                   "1:LEAP",
              
                   "3:EAP/TLS",
              
                   "2:PEAP/MSCHAPv2",
              
                   "4:PEAP/TLS",
              
              0), FALSE, FALSE},
                                                   
                    {"", "", validate_string, ARGV(""), FALSE, FALSE},                                                                                                                   
                                                         
                    {"", "", validate_string, ARGV(""), FALSE, FALSE},                                                                                                                   
                  
      {"LocalAuthDB", "Group", validate_group, ARGV(variables_WLANAuthentication11b_LocalAuthDB, "32", "36", "AuthDBNumberOfEntries"), FALSE, FALSE},     
       
      {"LocalCertDB", "Group", validate_group, ARGV(variables_WLANAuthentication11b_LocalCertDB, "32", "40", "CertDBNumberOfEntries"), FALSE, FALSE},     
#endif
                        
      {0,0,0,0,0,0}
      };
   
      struct variable variables_PrinterStatus[] = {
      
      {"x_PrinterModel", "Status", NULL, ARGV("printer_status.log","printer_model_t"), FALSE, FALSE},

      {"x_PrinterStatus", "Status", NULL, ARGV("printer_status.log","printer_status_t"), FALSE, FALSE},

      {"x_PrinterUser", "Status", NULL, ARGV("printer_status.log","printer_user_t"), FALSE, FALSE},
                                        
                    {"", "", validate_string, ARGV(""), FALSE, FALSE},                                                                                                                   
#ifndef NOUSB                 	
              {"usb_webenable_x", "", validate_choice, ARGV(              
              
                   "0:Disabled",
              
                   "1:LAN Only",
              
                   "2:LAN and WAN",
              
              0), FALSE, FALSE},
           	
              {"usb_webmode_x", "", validate_choice, ARGV(              
              
                   "0:ActiveX Only",
              
                   "1:ActiveX and Refresh",
              
                   "2:Refresh Only",
              
              0), FALSE, FALSE},
           	
              {"usb_webdriver_x", "", validate_choice, ARGV(              
              
                   "0:PWC 8.8",
              
                   "1:OV511 2.10",
              
              0), FALSE, FALSE},
           	
              {"usb_webimage_x", "", validate_choice, ARGV(              
              
                   "0:640 X 480",
              
                   "1:320 X 240",
              
                   "2:160 X 120",
              
              0), FALSE, FALSE},
           	
              {"usb_websense_x", "", validate_choice, ARGV(              
              
                   "0:Low",
              
                   "1:Medium",
              
                   "2:High",
              
              0), FALSE, FALSE},
                                    
                       {"usb_webrectime_x", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                                 
             {"usb_webfresh_x", "", validate_range, ARGV("1", "65535", ""), FALSE, FALSE},
                                                
                    {"usb_webcaption_x", "", validate_string, ARGV(""), FALSE, FALSE},                                                                                                                   
                              
             {"usb_webhttpport_x", "", validate_range, ARGV("1024", "65535", ""), FALSE, FALSE},
        
                {"usb_webhttpcheck_x", "", validate_string, ARGV(""), FALSE, FALSE}, 
                         
             {"usb_webactivex_x", "", validate_range, ARGV("1024", "65535", ""), FALSE, FALSE},
                         
                 {"usb_websecurity_x", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
              
                {"usb_websecurity_date_x", "", validate_portrange, NULL, FALSE, FALSE},
            
                {"usb_websecurity_time_x", "", validate_portrange, NULL, FALSE, FALSE},
                                                    
                    {"usb_websendto_x", "", validate_string, ARGV(""), FALSE, FALSE},                                                                                                                   
                                                         
                    {"usb_webmserver_x", "", validate_string, ARGV(""), FALSE, FALSE},                                                                                                                   
                                                         
                    {"usb_websubject_x", "", validate_string, ARGV(""), FALSE, FALSE},                                                                                                                   
                                  
                 {"usb_webattach_x", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
              	
              {"", "", validate_choice, ARGV(              
              
                   "0:None",
              
                   "1:PIN 1",
              
                   "2:PIN 2",
              
              0), FALSE, FALSE},
           	
              {"usb_webremote_x", "", validate_choice, ARGV(              
              
                   "0:LAN Only",
              
              0), FALSE, FALSE},
           
                {"usb_webremote1_x", "", validate_ipaddr, NULL, FALSE, FALSE},
            
                {"usb_webremote2_x", "", validate_ipaddr, NULL, FALSE, FALSE},
            
                {"usb_webremote3_x", "", validate_ipaddr, NULL, FALSE, FALSE},
            
                {"usb_webremote4_x", "", validate_ipaddr, NULL, FALSE, FALSE},
            
                {"usb_webremote5_x", "", validate_ipaddr, NULL, FALSE, FALSE},
            
                {"usb_webremote6_x", "", validate_ipaddr, NULL, FALSE, FALSE},
            
      {"x_FEject", "Status", NULL, ARGV("ddns.log","DDNSStatus"), FALSE, FALSE},
                 
                 {"usb_ftpenable_x", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
                               
                 {"usb_ftpanonymous_x", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
                               
                 {"usb_ftpsuper_x", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
                           
             {"usb_ftpport_x", "", validate_range, ARGV("1", "65535", ""), FALSE, FALSE},
                     
             {"usb_ftpmax_x", "", validate_range, ARGV("1", "12", ""), FALSE, FALSE},
                                 
                       {"usb_ftptimeout_x", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                                             
                       {"usb_ftpstaytimeout_x", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                                                            
                    {"usb_ftpscript_x", "", validate_string, ARGV(""), FALSE, FALSE},                                                                                                                   
                 	
              {"", "", validate_choice, ARGV(              
              
                   "0:English",
              
                   "1:Traditional Chinese",
              
                   "2:Simplified Chinese",
              
                   "3:Korean",
              
              0), FALSE, FALSE},
                                    
                       {"usb_ftpnum_x", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                                             
                       {"usb_bannum_x", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                                             
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                             
      {"x_FUserList", "Group", validate_group, ARGV(variables_PrinterStatus_x_FUserList, "32", "59", "usb_ftpnum_x"), FALSE, FALSE},     
       
      {"x_FBanIPList", "Group", validate_group, ARGV(variables_PrinterStatus_x_FBanIPList, "16", "24", "usb_bannum_x"), FALSE, FALSE},   
#endif

#ifndef NOQOS 
//      {"qos_urulenum_x", "", validate_range, ARGV("0","65535"), FALSE, RESTART_QOS},	// 2007.10 James

      {"qos_rulenum_x", "8", validate_range, ARGV("0","8"), FALSE, RESTART_QOS},	// 2007.10 James

      {"qos_enable", "", validate_range, ARGV("0","1"), FALSE, RESTART_QOS},	// 2007.10 James

      {"qos_global_enable", "", validate_range, ARGV("0","1"), FALSE, RESTART_QOS},	// 2007.10 James

      {"qos_service_enable", "", validate_range, ARGV("0","1"), FALSE, RESTART_QOS},	// 2007.10 James

      {"qos_tos_prio", "", validate_range, ARGV("0","1"), FALSE, RESTART_QOS},	// 2007.10 James

      {"qos_pshack_prio", "", validate_range, ARGV("0","1"), FALSE, RESTART_QOS},	// 2007.10 James

      {"qos_shortpkt_prio", "", validate_range, ARGV("0","1"), FALSE, RESTART_QOS},	// 2007.10 James

      {"qos_dfragment_enable", "", validate_range, ARGV("0","1"), FALSE, RESTART_QOS},	// 2007.10 James

      {"qos_dfragment_size", "", validate_range, ARGV("0","100"), FALSE, RESTART_QOS},	// 2007.10 James

      {"qos_service_ubw", "", validate_range, ARGV("0","100"), FALSE, RESTART_QOS},	// 2007.10 James

      {"qos_manual_ubw", "", validate_range, ARGV("0","1073741824"), FALSE, RESTART_QOS},	// 2007.10 James

      {"x_USRRuleList", "Group", validate_group, ARGV(variables_PrinterStatus_x_USRRuleList, "8", "47", "qos_rulenum_x"), FALSE, RESTART_QOS},	// 2008.01 James.

//      {"x_QRuleList", "Group", validate_group, ARGV(variables_PrinterStatus_x_QRuleList, "8", "44", "qos_rulenum_x"), FALSE, FALSE},

//      {"x_UQRuleList", "Group", validate_group, ARGV(variables_PrinterStatus_x_UQRuleList, "8", "28", "qos_urulenum_x"), FALSE, FALSE},
#endif                        
      {0,0,0,0,0,0}
      };
     
#ifndef NOUSB 
      struct action actions_General[] = {
      
          { "x_GetSystemInfo",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewSystemUpTime",
             
             "NewProductID",
             
             "NewFirmwareVersion",
             
             "NewHardwareVersion",
             
             "NewBootloaderVersion",
             
          0), 
          ARGV(
          
             "x_SystemUpTime", 
             
             "x_ProductID", 
             
             "x_FirmwareVersion", 
             
             "x_HardwareVersion", 
             
             "x_BootloaderVersion", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
                          
      { 0, 0, 0, 0}
      };
      
      struct action actions_Layer3Forwarding[] = {
      
          { "SetDefaultConnectionService",
          ARGV(
          
             "NewDefaultConnectionService",
             
          0),
          ARGV(
          
             "DefaultConnectionService",
             
          0),
          ARGV(
          
          0), 
          ARGV(
          
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetDefaultConnectionService",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewDefaultConnectionService",
             
          0), 
          ARGV(
          
             "DefaultConnectionService", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetConnectionType",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewConnectionType",
             
          0), 
          ARGV(
          
             "x_ConnectionType", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "SetConnectionType",
          ARGV(
          
             "NewConnectionType",
             
          0),
          ARGV(
          
             "x_ConnectionType",
             
          0),
          ARGV(
          
          0), 
          ARGV(
          
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
                          
      { 0, 0, 0, 0}
      };
      
      struct action actions_WANCommonInterface[] = {
      
          { "SetEnabledForInternet",
          ARGV(
          
             "NewEnabledForInternet",
             
          0),
          ARGV(
          
             "EnabledForInternet",
             
          0),
          ARGV(
          
          0), 
          ARGV(
          
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetEnabledForInternet",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewEnabledForInternet",
             
          0), 
          ARGV(
          
             "EnabledForInternet", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetCommonLinkProperties",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewWANAccessType",
             
             "NewLayer1UpstreamMaxBitRate",
             
             "NewLayer1DownstreamMaxBitRate",
             
             "NewPhysicalLinkStatus",
             
          0), 
          ARGV(
          
             "WANAccessType", 
             
             "Layer1UpstreamMaxBitRate", 
             
             "Layer1DownstreamMaxBitRate", 
             
             "PhysicalLinkStatus", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetWANAccessProvider",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewWANAccessProvider",
             
          0), 
          ARGV(
          
             "WANAccessProvider", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetMaximumActiveConnections",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewMaximumActiveConnections",
             
          0), 
          ARGV(
          
             "MaximumActiveConnections", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetTotalBytesSent",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewTotalBytesSent",
             
          0), 
          ARGV(
          
             "TotalBytesSent", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetTotalBytesReceived",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewTotalBytesReceived",
             
          0), 
          ARGV(
          
             "TotalBytesReceived", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetTotalPacketsSent",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewTotalPacketsSent",
             
          0), 
          ARGV(
          
             "TotalPacketsSent", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetTotalPacketsReceived",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewTotalPacketsReceived",
             
          0), 
          ARGV(
          
             "TotalPacketsReceived", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetActiveConnection",
          ARGV(
          
             "NewActiveConnectionIndex",
             
          0),
          ARGV(
          
             "NumberOfActiveConnections",
             
          0),
          ARGV(
          
             "NewActiveConnDeviceContainer",
             
             "NewActiveConnectionServiceID",
             
          0), 
          ARGV(
          
             "ActiveConnectionDeviceContainer", 
             
             "ActiveConnectionServiceID", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
                          
      { 0, 0, 0, 0}
      };
      
      struct action actions_IPConnection[] = {
      
          { "SetConnectionType",
          ARGV(
          
             "NewConnectionType",
             
          0),
          ARGV(
          
             "ConnectionType",
             
          0),
          ARGV(
          
          0), 
          ARGV(
          
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetConnectionTypeInfo",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewConnectionType",
             
             "NewPossibleConnectionTypes",
             
          0), 
          ARGV(
          
             "ConnectionType", 
             
             "PossibleConnectionTypes", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetStatusInfo",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewConnectionStatus",
             
             "NewLastConnectionError",
             
             "NewUptime",
             
          0), 
          ARGV(
          
             "ConnectionStatus", 
             
             "LastConnectionError", 
             
             "Uptime", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetNATRSIPStatus",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewRSIPAvailable",
             
             "NewNATEnabled",
             
          0), 
          ARGV(
          
             "RSIPAvailable", 
             
             "NATEnabled", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetGenericPortMappingEntry",
          ARGV(
          
             "NewPortMappingIndex",
             
          0),
          ARGV(
          
             "PortMappingNumberOfEntries",
             
          0),
          ARGV(
          
             "NewRemoteHost",
             
             "NewExternalPort",
             
             "NewProtocol",
             
             "NewInternalPort",
             
             "NewInternalClient",
             
             "NewEnabled",
             
             "NewPortMappingDescription",
             
             "NewLeaseDuration",
             
          0), 
          ARGV(
          
             "RemoteHost", 
             
             "ExternalPort", 
             
             "PortMappingProtocol", 
             
             "InternalPort", 
             
             "InternalClient", 
             
             "PortMappingEnabled", 
             
             "PortMappingDescription", 
             
             "PortMappingLeaseDuration", 
             
          0), 
          ARGV(       
          
             "PortMappingList", 
             "External", 
                      
          0),
          NULL                    
          },
      
          { "GetSpecificPortMappingEntry ",
          ARGV(
          
             "NewRemoteHost",
             
             "NewExternalPort",
             
             "NewProtocol",
             
          0),
          ARGV(
          
             "RemoteHost",
             
             "ExternalPort",
             
             "PortMappingProtocol",
             
          0),
          ARGV(
          
             "NewInternalPort",
             
             "NewInternalClient",
             
             "NewEnabled",
             
             "NewPortMappingDescription",
             
             "NewLeaseDuration",
             
          0), 
          ARGV(
          
             "InternalPort", 
             
             "InternalClient", 
             
             "PortMappingEnabled", 
             
             "PortMappingDescription", 
             
             "PortMappingLeaseDuration", 
             
          0), 
          ARGV(       
          
             "PortMappingList", 
             "External", 
                      
          0),
          NULL                    
          },
      
          { "AddPortMapping ",
          ARGV(
          
             "NewRemoteHost",
             
             "NewExternalPort",
             
             "NewProtocol",
             
             "NewInternalPort",
             
             "NewInternalClient",
             
             "NewEnabled",
             
             "NewPortMappingDescription",
             
             "NewLeaseDuration",
             
          0),
          ARGV(
          
             "RemoteHost",
             
             "ExternalPort",
             
             "PortMappingProtocol",
             
             "InternalPort",
             
             "InternalClient",
             
             "PortMappingEnabled",
             
             "PortMappingDescription",
             
             "PortMappingLeaseDuration",
             
          0),
          ARGV(
          
          0), 
          ARGV(
          
          0), 
          ARGV(       
          
             "PortMappingList", 
             "External", 
                      
          0),
          NULL                    
          },
      
          { "DeletePortMapping",
          ARGV(
          
             "NewRemoteHost",
             
             "NewExternalPort",
             
             "NewProtocol",
             
          0),
          ARGV(
          
             "RemoteHost",
             
             "ExternalPort",
             
             "PortMappingProtocol",
             
          0),
          ARGV(
          
          0), 
          ARGV(
          
          0), 
          ARGV(       
          
             "PortMappingList", 
             "External", 
                      
          0),
          NULL                    
          },
      
          { "GetExternalIPAddress",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewExternalIPAddress",
             
          0), 
          ARGV(
          
             "ExternalIPAddress", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
                          
      { 0, 0, 0, 0}
      };
      
      struct action actions_PPPConnection[] = {
      
          { "SetConnectionType",
          ARGV(
          
             "NewConnectionType",
             
          0),
          ARGV(
          
             "ConnectionType",
             
          0),
          ARGV(
          
          0), 
          ARGV(
          
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetConnectionTypeInfo",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewConnectionType",
             
             "NewPossibleConnectionTypes",
             
          0), 
          ARGV(
          
             "ConnectionType", 
             
             "PossibleConnectionTypes", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "ConfigureConnection",
          ARGV(
          
             "NewUserName",
             
             "NewPassword",
             
          0),
          ARGV(
          
             "UserName",
             
             "Password",
             
          0),
          ARGV(
          
          0), 
          ARGV(
          
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "RequestConnection",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
          0), 
          ARGV(
          
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "RequestTermination",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
          0), 
          ARGV(
          
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "ForceTermination",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
          0), 
          ARGV(
          
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "SetAutoDisconnectTime",
          ARGV(
          
             "NewAutoDisconnectTime",
             
          0),
          ARGV(
          
             "AutoDisconnectTime",
             
          0),
          ARGV(
          
          0), 
          ARGV(
          
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "SetIdleDisconnectTime",
          ARGV(
          
             "NewIdleDisconnectTime",
             
          0),
          ARGV(
          
             "IdleDisconnectTime",
             
          0),
          ARGV(
          
          0), 
          ARGV(
          
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "SetWarnDisconnectDelay",
          ARGV(
          
             "NewWarnDisconnectDelay",
             
          0),
          ARGV(
          
             "WarnDisconnectDelay",
             
          0),
          ARGV(
          
          0), 
          ARGV(
          
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetStatusInfo",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewConnectionStatus",
             
             "NewLastConnectionError",
             
             "NewUptime",
             
          0), 
          ARGV(
          
             "ConnectionStatus", 
             
             "LastConnectionError", 
             
             "Uptime", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetLinkLayerMaxBitRates",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewUpstreamMaxBitRate",
             
             "NewDownstreamMaxBitRate",
             
          0), 
          ARGV(
          
             "UpstreamMaxBitRate", 
             
             "DownstreamMaxBitRate", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetUserName",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewUserName",
             
          0), 
          ARGV(
          
             "UserName", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetPassword",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewPassword",
             
          0), 
          ARGV(
          
             "Password", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetAutoDisconnectTime",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewAutoDisconnectTime",
             
          0), 
          ARGV(
          
             "AutoDisconnectTime", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetIdleDisconnectTime",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewIdleDisconnectTime",
             
          0), 
          ARGV(
          
             "IdleDisconnectTime", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetWarnDisconnectDelay",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewWarnDisconnectDelay",
             
          0), 
          ARGV(
          
             "WarnDisconnectDelay", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetNATRSIPStatus",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewRSIPAvailable",
             
             "NewNATEnabled",
             
          0), 
          ARGV(
          
             "RSIPAvailable", 
             
             "NATEnabled", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetGenericPortMappingEntry",
          ARGV(
          
             "NewPortMappingIndex",
             
          0),
          ARGV(
          
             "PortMappingNumberOfEntries",
             
          0),
          ARGV(
          
             "NewRemoteHost",
             
             "NewExternalPort",
             
             "NewProtocol",
             
             "NewInternalPort",
             
             "NewInternalClient",
             
             "NewEnabled",
             
             "NewPortMappingDescription",
             
             "NewLeaseDuration",
             
          0), 
          ARGV(
          
             "RemoteHost", 
             
             "ExternalPort", 
             
             "PortMappingProtocol", 
             
             "InternalPort", 
             
             "InternalClient", 
             
             "PortMappingEnabled", 
             
             "PortMappingDescription", 
             
             "PortMappingLeaseDuration", 
             
          0), 
          ARGV(       
          
             "PortMappingList", 
             "External", 
                      
          0),
          NULL                    
          },
      
          { "GetSpecificPortMappingEntry ",
          ARGV(
          
             "NewRemoteHost",
             
             "NewExternalPort",
             
             "NewProtocol",
             
          0),
          ARGV(
          
             "RemoteHost",
             
             "ExternalPort",
             
             "PortMappingProtocol",
             
          0),
          ARGV(
          
             "NewInternalPort",
             
             "NewInternalClient",
             
             "NewEnabled",
             
             "NewPortMappingDescription",
             
             "NewLeaseDuration",
             
          0), 
          ARGV(
          
             "InternalPort", 
             
             "InternalClient", 
             
             "PortMappingEnabled", 
             
             "PortMappingDescription", 
             
             "PortMappingLeaseDuration", 
             
          0), 
          ARGV(       
          
             "PortMappingList", 
             "External", 
                      
          0),
          NULL                    
          },
      
          { "AddPortMapping ",
          ARGV(
          
             "NewRemoteHost",
             
             "NewExternalPort",
             
             "NewProtocol",
             
             "NewInternalPort",
             
             "NewInternalClient",
             
             "NewEnabled",
             
             "NewPortMappingDescription",
             
             "NewLeaseDuration",
             
          0),
          ARGV(
          
             "RemoteHost",
             
             "ExternalPort",
             
             "PortMappingProtocol",
             
             "InternalPort",
             
             "InternalClient",
             
             "PortMappingEnabled",
             
             "PortMappingDescription",
             
             "PortMappingLeaseDuration",
             
          0),
          ARGV(
          
          0), 
          ARGV(
          
          0), 
          ARGV(       
          
             "PortMappingList", 
             "External", 
                      
          0),
          NULL                    
          },
      
          { "DeletePortMapping",
          ARGV(
          
             "NewRemoteHost",
             
             "NewExternalPort",
             
             "NewProtocol",
             
          0),
          ARGV(
          
             "RemoteHost",
             
             "ExternalPort",
             
             "PortMappingProtocol",
             
          0),
          ARGV(
          
          0), 
          ARGV(
          
          0), 
          ARGV(       
          
             "PortMappingList", 
             "External", 
                      
          0),
          NULL                    
          },
      
          { "GetExternalIPAddress",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewExternalIPAddress",
             
          0), 
          ARGV(
          
             "ExternalIPAddress", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
                          
      { 0, 0, 0, 0}
      };
      
      struct action actions_EthernetLink[] = {
      
          { "GetEthernetLinkStatus",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewEthernetLinkStatus",
             
          0), 
          ARGV(
          
             "EthernetLinkStatus", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
                          
      { 0, 0, 0, 0}
      };
      
      struct action actions_LANHostConfig[] = {
      
          { "SetDHCPServerConfigurable",
          ARGV(
          
             "NewDHCPServerConfigurable",
             
          0),
          ARGV(
          
             "DHCPServerConfigurable",
             
          0),
          ARGV(
          
          0), 
          ARGV(
          
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetDHCPServerConfigurable",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewDHCPServerConfigurable",
             
          0), 
          ARGV(
          
             "DHCPServerConfigurable", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "SetDHCPRelay",
          ARGV(
          
             "NewDHCPRelay",
             
          0),
          ARGV(
          
             "DHCPRelay",
             
          0),
          ARGV(
          
          0), 
          ARGV(
          
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetDHCPRelay",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewDHCPRelay",
             
          0), 
          ARGV(
          
             "DHCPRelay", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "SetSubnetMask",
          ARGV(
          
             "NewSubnetMask",
             
          0),
          ARGV(
          
             "SubnetMask",
             
          0),
          ARGV(
          
          0), 
          ARGV(
          
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetSubnetMask",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewSubnetMask",
             
          0), 
          ARGV(
          
             "SubnetMask", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "SetIPRouter",
          ARGV(
          
             "NewIPRouters",
             
          0),
          ARGV(
          
             "IPRouters",
             
          0),
          ARGV(
          
          0), 
          ARGV(
          
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "DeleteIPRouter",
          ARGV(
          
             "NewIPRouters",
             
          0),
          ARGV(
          
             "IPRouters",
             
          0),
          ARGV(
          
          0), 
          ARGV(
          
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetIPRoutersList",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewIPRouters",
             
          0), 
          ARGV(
          
             "IPRouters", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "SetDomainName",
          ARGV(
          
             "NewDomainName",
             
          0),
          ARGV(
          
             "DomainName",
             
          0),
          ARGV(
          
          0), 
          ARGV(
          
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetDomainName",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewDomainName",
             
          0), 
          ARGV(
          
             "DomainName", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "SetAddressRange",
          ARGV(
          
             "NewMinAddress",
             
             "NewMaxAddress",
             
          0),
          ARGV(
          
             "MinAddress",
             
             "MaxAddress",
             
          0),
          ARGV(
          
          0), 
          ARGV(
          
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetAddressRange",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewMinAddress",
             
             "NewMaxAddress",
             
          0), 
          ARGV(
          
             "MinAddress", 
             
             "MaxAddress", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "SetReservedAddress",
          ARGV(
          
             "NewReservedAddresses",
             
          0),
          ARGV(
          
             "ReservedAddresses",
             
          0),
          ARGV(
          
          0), 
          ARGV(
          
          0), 
          ARGV(       
          
             "ReservedAddressList", 
             "Add", 
                      
          0),
          NULL                    
          },
      
          { "DeleteReservedAddress",
          ARGV(
          
             "NewReservedAddresses",
             
          0),
          ARGV(
          
             "ReservedAddresses",
             
          0),
          ARGV(
          
          0), 
          ARGV(
          
          0), 
          ARGV(       
          
             "ReservedAddressList", 
             "Del", 
                      
          0),
          NULL                    
          },
      
          { "GetReservedAddresses",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewReservedAddresses",
             
          0), 
          ARGV(
          
             "ReservedAddresses", 
             
          0), 
          ARGV(       
          
             "ReservedAddressList", 
             "Get", 
                      
          0),
          NULL                    
          },
      
          { "SetDNSServer",
          ARGV(
          
             "NewDNSServers",
             
          0),
          ARGV(
          
             "DNSServers",
             
          0),
          ARGV(
          
          0), 
          ARGV(
          
          0), 
          ARGV(       
          
             "DNSServerList", 
             "Add", 
                      
          0),
          NULL                    
          },
      
          { "DeleteDNSServer",
          ARGV(
          
             "NewDNSServers",
             
          0),
          ARGV(
          
             "DNSServers",
             
          0),
          ARGV(
          
          0), 
          ARGV(
          
          0), 
          ARGV(       
          
             "DNSServerList", 
             "Del", 
                      
          0),
          NULL                    
          },
      
          { "GetDNSServers",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewDNSServers",
             
          0), 
          ARGV(
          
             "DNSServers", 
             
          0), 
          ARGV(       
          
             "DNSServerList", 
             "Get", 
                      
          0),
          NULL                    
          },
      
          { "GetHostName",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewHostName",
             
          0), 
          ARGV(
          
             "x_HostName", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "SetHostName",
          ARGV(
          
             "NewHostName",
             
          0),
          ARGV(
          
             "x_HostName",
             
          0),
          ARGV(
          
          0), 
          ARGV(
          
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
                          
      { 0, 0, 0, 0}
      };
#else
struct action actions_Layer3Forwarding[];
struct action actions_LANHostConfig[];
struct action actions_EthernetLink[];
struct action actions_IPConnection[];
struct action actions_PPPConnection[];
struct action actions_WANCommonInterface[];
#endif
  
struct action actions_General[];
struct action actions_FirewallConfig[];
struct action actions_RouterConfig[];
struct action actions_WLANConfig[];
struct action actions_DeviceSecurity[];
struct action actions_WLANAuthentication[];
struct action actions_PrinterStatus[];
struct variable variables_WLANAuthentication11b[];
struct action actions_Storage[];
struct action actions_Language[];


struct svcLink svcLinks[] = {            
           {"General", 	"urn:schemas-upnp-org:service:General:1", variables_General, actions_General},
           {"LANHostConfig", "urn:schemas-upnp-org:service:Layer3Forwarding:1", variables_LANHostConfig, actions_LANHostConfig},
           {"Layer3Forwarding", "urn:schemas-upnp-org:service:LANHostConfigManagement:0.8", variables_Layer3Forwarding, actions_Layer3Forwarding},
           {"WANCommonInterface", "urn:schemas-upnp-org:service:WANCommonInterfaceConfig:1", variables_WANCommonInterface, actions_WANCommonInterface},           
           {"IPConnection", "urn:schemas-upnp-org:service:WANIPConnection:1", variables_IPConnection, actions_IPConnection},
           {"PPPConnection", "urn:schemas-upnp-org:service:WANPPPConnection:1", variables_PPPConnection, actions_PPPConnection},
           {"EthernetLink", "urn:schemas-upnp-org:service:WANEthernetLinkConfig:1", variables_EthernetLink, actions_EthernetLink},
           {"FirewallConfig", "urn:schemas-upnp-org:service:FirewallConfig:1", variables_FirewallConfig, actions_FirewallConfig},
           {"RouterConfig", "urn:schemas-upnp-org:service:RouterConfig:1", variables_RouterConfig, actions_RouterConfig},           
           {"WLANConfig11a", "urn:schemas-upnp-org:service:WLANConfiguration:1", variables_WLANConfig11a, actions_WLANConfig},
           {"DeviceSecurity11a", "urn:schemas-upnp-org:service:DeviceSecurity:1", variables_DeviceSecurity11a, actions_DeviceSecurity},
           {"WLANAuthentication11a", "urn:schemas-upnp-org:service:WLANAuthentication:1", variables_WLANAuthentication11a, actions_WLANAuthentication},
           {"WLANConfig11b", "urn:schemas-upnp-org:service:WLANConfiguration:1", variables_WLANConfig11b, actions_WLANConfig},
           {"DeviceSecurity11b", "urn:schemas-upnp-org:service:DeviceSecurity:1", variables_DeviceSecurity11b, actions_DeviceSecurity},
           {"WLANAuthentication11b", "urn:schemas-upnp-org:service:WLANAuthentication:1", variables_WLANAuthentication11b, actions_WLANAuthentication},         
           {"PrinterStatus", "urn:schemas-upnp-org:service:PrinterStatus:1", variables_PrinterStatus, actions_PrinterStatus},
           {"Storage", "urn:schemas-upnp-org:service:Storage:1", variables_Storage, actions_Storage},
	   {"LANGUAGE", "urn:schemas-upnp-org:service:LANGUAGE:1", variables_Language, actions_Language},
           {0, 0, 0}
      };
	
