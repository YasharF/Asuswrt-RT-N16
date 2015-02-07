#include "shared.h"
#include "shutils.h"

int switch_bridge_mode(int port)
{
	if(port < 1 || port > 4) {
		eval("et", "robowr", "0x34", "0x12", "0x02");
		eval("et", "robowr", "0x34", "0x14", "0x02");
		eval("et", "robowr", "0x34", "0x16", "0x02");
		eval("et", "robowr", "0x34", "0x18", "0x02");
		eval("et", "robowr", "0x05", "0x83", "0x3E3F");
	}
	else {
		switch(port) {
			case 1:
				eval("et", "robowr", "0x34", "0x12", "0x02");
				eval("et", "robowr", "0x05", "0x83", "0x0623");
				break;
			case 2:
				eval("et", "robowr", "0x34", "0x14", "0x02");
				eval("et", "robowr", "0x05", "0x83", "0x0A25");
				break;
			case 3:
				eval("et", "robowr", "0x34", "0x16", "0x02");
				eval("et", "robowr", "0x05", "0x83", "0x1229");
				break;
			case 4:
				eval("et", "robowr", "0x34", "0x18", "0x02");
				eval("et", "robowr", "0x05", "0x83", "0x2231");
				break;
		}
	}
	eval("et", "robowr", "0x05", "0x81", "0x0002");
	eval("et", "robowr", "0x05", "0x80", "0x0000");
	eval("et", "robowr", "0x05", "0x80", "0x0080");
	return 0;
}

int
switch_init(void)
{
	return 0;
}

void
switch_fini(void)
{

}
