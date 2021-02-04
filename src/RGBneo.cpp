#include "RGBneo.h"
#include <adri_tools_v2.h>

extern boolean aic_relay;

#define MAX_RELAY 10

#define HSV_SECTION_3 (86)

HsvColor rgb2hsv(unsigned char r, unsigned char  g, unsigned char  b) ;

RGBneoClass * RGBneoClassArray[MAX_RELAY];

RGBneoManagment::RGBneoManagment(){

}

void RGBneoManagment::create( int & result, int n, int p, int t){
	neoPixelType type;
	switch (t) {
	    case 0:
	    	type = NEO_GRB + NEO_KHZ800;
	    break;
	    case 1:
	    	type = NEO_GRBW + NEO_KHZ800;
	    break;
	    default:
	    	type = NEO_GRB + NEO_KHZ800;
	    break;
	}
	ADRI_LOG(-1, 2, 2, "RGBneoManagment create","");
	RGBneoClassArray[_pos] = new RGBneoClass(n, p, type);
	result = _pos;
	_pos++;
}

RGBneoClass * RGBneoManagment::module(int pos){
	return RGBneoClassArray[pos];
}


RGBneoClass::RGBneoClass(int n, int p, neoPixelType t){

    _num_leds 	= n;
    _pin 		= p;
	ADRI_LOGV(-1, 2, 2, _num_leds, "","");
	ADRI_LOGV(-1, 2, 2, _pin, "","");

    _leds = new Adafruit_NeoPixel(n, p, NEO_GRB + NEO_KHZ800);

	#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
	  clock_prescale_set(clock_div_1);
	#endif  
	
	_leds->begin();  
	_leds->setBrightness(_bri);	

	delay(500);

	_leds->clear();
	_leds->show();

}

void RGBneoClass::leds_black(){
	_leds->clear();
	_leds->show();
	_off = true;
}
void RGBneoClass::leds_white(uint8_t w){
	_off = false;
	_white = w;
	_leds->fill(_leds->Color(0, 0, 0,  _leds->gamma8(_white))); 
	_mod = RGBneoMod_tw;
	_leds->show();
}
void RGBneoClass::leds_bri(uint8_t bri){
	if (_off) {
		leds_mod();
	}
	_bri = bri;
	_leds->setBrightness(_bri);	
	_leds->show();
	_off = false;
}
void RGBneoClass::leds_rgb(uint8_t r, uint8_t g, uint8_t b){
	_off = false;
	_color = {r, g, b};
	_leds->fill(_leds->Color(r, g, b));  
	_mod = RGBneoMod_rgb;
	_leds->show();
}
void RGBneoClass::leds_mod(){
	if (_mod == RGBneoMod_rgb) {
		leds_rgb(_color.R, _color.G, _color.B);
	}
	if (_mod == RGBneoMod_tw) {
		leds_white(_white);
	}	
}
void RGBneoClass::leds_mod(RGBneoMod mod){
	_mod = RGBneoMod_rgb;
	if (_mod == RGBneoMod_rgb) {
		leds_rgb(_color.R, _color.G, _color.B);
	}
	if (_mod == RGBneoMod_tw) {
		leds_white(_white);
	}	
}
void RGBneoClass::leds_rgb(uint8_t r, uint8_t g, uint8_t b, uint8_t w){
	_off = false;
	_color = {r, g, b};
	_leds->fill(_leds->Color(r, g, b));  
	_bri = w;
	_leds->setBrightness(_bri);		
	_mod = RGBneoMod_rgb;
	_leds->show();
}
void RGBneoClass::getStatus(boolean & ret){
	ret = _off;
}

void RGBneoClass::json(JsonObject & root){

}
void RGBneoClass::domoticzJson(JsonObject & root){
	if (_off) 	root[F("nvalue")] = 0;
	else		root[F("nvalue")] = 1;
	root[F("svalue")] = String(_bri);
}






RgbColor hsv2rgb (uint8_t h, uint8_t s, uint8_t v, int pix_nbr) {
	//long start=micros();

	// fsprintf("\nHSV_SECTION_3: %d\n", HSV_SECTION_3);
	RgbColor rgb;

	uint8_t value = v;
	uint8_t sat = s;

	// The brightness floor is minimum number that all of R, G, and B will be set to.
	uint8_t brightness_floor=(value*(255-sat))/256;

	// The color amplitude is the maximum amount of R, G, and B that will be added on top of the brightness_floor
	uint8_t color_amplitude = value-brightness_floor;

	// Figure out which section of the hue wheel we're in, and how far offset we are within that section
	uint8_t section = h / HSV_SECTION_3;              // 0..2
	uint8_t offset  = h % HSV_SECTION_3;              // 0..63

	uint8_t rampup = offset;                    // 0..63
	uint8_t rampdown = (HSV_SECTION_3 - 1) - offset;  // 63..0

	// compute color amplitude scaled down versions of rampup and rampdown
	uint8_t rampup_amp_adj   = (rampup   * color_amplitude) / (86);
	uint8_t rampdown_amp_adj = (rampdown * color_amplitude) / (86);

	// add brightness_floor offset to everything
	uint8_t rampup_adj_with_floor   = rampup_amp_adj   + brightness_floor;
	uint8_t rampdown_adj_with_floor = rampdown_amp_adj + brightness_floor;
	// fsprintf("\nsection: %d\n", section);

	switch (section) {
		case 0:
			rgb.R = rampdown_adj_with_floor;
			rgb.G = rampup_adj_with_floor;
			rgb.B = brightness_floor;
		break;
		case 1:
			rgb.R = brightness_floor;
			rgb.G = rampdown_adj_with_floor;
			rgb.B = rampup_adj_with_floor;
		break;
		case 2:
			rgb.R = rampup_adj_with_floor;
			rgb.G = brightness_floor;
			rgb.B = rampdown_adj_with_floor;
		break;
	}

		return rgb;
}
HsvColor rgb2hsv(unsigned char r, unsigned char  g, unsigned char  b) {
	HsvColor hsv;
	unsigned char rgb_min, rgb_max;

	rgb_min = r<g ? (r<b?r:b) : (g<b?g:b);
	rgb_max = r>g ? (r>b?r:b) : (g>b?g:b);

	hsv.v = rgb_max;
	if (hsv.v==0)  { hsv.h=0; hsv.s=0; return hsv; }
	hsv.s = 255 * long(rgb_max - rgb_min) / hsv.v;

	if (hsv.s==0)   { hsv.h=0;  return hsv; }

	if (rgb_max==r) 		hsv.h =   	0+43*(g-b)/(rgb_max-rgb_min);
	else if (rgb_max==g)   	hsv.h =  	85+43*(b-r)/(rgb_max-rgb_min);
	else               		hsv.h = 	171+43*(r-g)/(rgb_max-rgb_min);

	return hsv;
}

int* splitColor(String Val) {
    String      aVal            = Val;
    byte        firstIndex      = aVal.indexOf('.');
    byte        secondIndex     = aVal.indexOf('.', firstIndex + 1);
    String      red             = aVal.substring(0, firstIndex);
    String      green           = aVal.substring(firstIndex + 1, secondIndex);
    String      blue            = aVal.substring(secondIndex + 1);
    int         r               = red.toInt();
    int         g               = green.toInt();
    int         b               = blue.toInt();

    int     *array          = new int[3];
            array[0]        = r;
            array[1]        = g;
            array[2]        = b;
    return array;    
}

uint32_t hsv2_32(uint8_t h, uint8_t s, uint8_t v) {
	RgbColor c1 = hsv2rgb (h, s, v, 0);
	 return ( ((uint32_t)c1.R << 16) | ((uint16_t)c1.G << 8 ) | (uint16_t)c1.B );
}
uint32_t rgb2_32(RgbColor c1) {
	 return ( ((uint32_t)c1.R << 16) | ((uint16_t)c1.G << 8 ) | (uint16_t)c1.B );
}
uint32_t rgb2_32(uint8_t r, uint8_t g, uint8_t b) {
	 return ( ((uint32_t)r << 16) | ((uint16_t)g << 8 ) | (uint16_t)b );
}