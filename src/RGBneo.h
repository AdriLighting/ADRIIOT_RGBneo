
#ifndef RGBNEO
	#define RGBNEO
	#include <Arduino.h>
	#include <ArduinoJson.h>
	#include <Adafruit_NeoPixel.h>

	struct RgbColor {uint8_t R; uint8_t G; uint8_t B; };
	struct HsvColor {uint8_t h; uint8_t s; uint8_t v; };

	enum RGBneoMod
	{
		RGBneoMod_rgb,
		RGBneoMod_tw
	};
	class RGBneoClass
	{
		Adafruit_NeoPixel 	* _leds	= nullptr;;
		int			_num_leds 	 	= 0;
	    int			_pin;
	    uint8_t 	_bri			= 30;
	    uint8_t 	_white 			= 60;
	    RgbColor 	_color 			= {255, 255, 0};
	    RGBneoMod 	_mod;
	    boolean 	_off 			= true;
	public:
		RGBneoClass(int n, int p, neoPixelType t); // NEO_GRB + NEO_KHZ800
		~RGBneoClass();


	    void json(JsonObject & root);
	    void domoticzJson(JsonObject & root);

		void leds_mod(RGBneoMod mod);
		void leds_mod();
		void leds_black();
		void leds_white(uint8_t w);
		void leds_bri(uint8_t bri);
		void leds_rgb(uint8_t r, uint8_t g, uint8_t b);
		void leds_rgb(uint8_t r, uint8_t g, uint8_t b, uint8_t w);
		int * getRgb(boolean & ret);
		void getStatus(boolean & ret);
		void getBri(int & ret);
	private:
	};


	class RGBneoManagment
	{
	private:
		int _pos = 0;
	public:
		RGBneoManagment();
		~RGBneoManagment(){};

		void create 	(int & result, int n, int p, int t = 0);
		RGBneoClass * module(int pos);


	};

#endif // RELAY_H