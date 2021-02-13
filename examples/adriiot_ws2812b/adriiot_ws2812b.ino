
#include <ADRIIOT_RGBneo.h>

RGBneoManagment relay_managment;
int 			relayId;
int 			relay2Id;

void setup()
{

	Serial.begin(115200);

	relay_managment.create 		(relayId, 10, D5);

	delay(3000);
	relay_managment.module(relayId)->leds_rgb(0, 255, 0);
	delay(3000);
	relay_managment.module(relayId)->leds_black();
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