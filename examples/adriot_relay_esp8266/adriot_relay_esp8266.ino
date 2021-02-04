
#include <ADRIIOT_RGBneo.h>
#include <adri_tools_v2.h>

RGBneoManagment relay_managment;
int 			relayId;
int 			relay2Id;

void setup()
{
	new adri_toolsV2();
	new adriTools_logger();

	Serial.begin(115200);

	relay_managment.create 		(relayId, 10, D5);

	ADRI_LOG(-1, 0, 2, "", "");
	delay(3000);
	relay_managment.module(relayId)->leds_rgb(0, 255, 0);
	ADRI_LOG(-1, 0, 2, "", "");
	delay(3000);
	relay_managment.module(relayId)->leds_black();
	ADRI_LOG(-1, 0, 2, "", "");
	delay(3000);
	relay_managment.module(relayId)->leds_mod();

	// byte R,G,B;

	// R = 255;
	// G = 0;
	// B = 255;

	// char hex[7] = {0};
	// sprintf(hex,"%02X%02X%02X",R,G,B); //convert to an hexadecimal string. Lookup sprintf for what %02X means.
	// Serial.println(hex); //Print the string.

}

void loop()
{

}