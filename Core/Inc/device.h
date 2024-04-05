#pragma once

#include "adc.h"
#include "can.h"
#include "interrupt.h"
#include "pin.h"
#include "pwm.h"
#include "counter.h"
#include "service.h"

class Device {

	ADC_& adc;
	PWM& first_fun;
	PWM& second_fun;
	PWM& pump;
	Counter& counter;
	Service<In_data, Out_data>& service;
	CAN<In_id, Out_id>& can;

	Pin& led_red;
	Pin& led_green;
	Pin& led_can;
	Pin& in1;
	Pin& in2;
	Pin& in3;
	Pin& level;
	Pin& out1;
	Pin& out2;
	Pin& out3;
	Pin& out4;
	Pin& cool;
	Pin& fb_cool;
	Pin& heat;
	Pin& fb_heat;

	Timer timer{2000};
	Timer delay{150};
	Timer blink{500};

public:

	Device(ADC_& adc, PWM& first_fun, PWM& second_fun, PWM& pump, Counter& counter, Service<In_data, Out_data>& service, CAN<In_id, Out_id>& can
	, Pin& led_red, Pin& led_green, Pin& led_can, Pin& in1, Pin& in2, Pin& in3, Pin& level
	, Pin& out1, Pin& out2, Pin& out3, Pin& out4, Pin& cool, Pin& fb_cool, Pin& heat, Pin& fb_heat
	)
	: adc {adc}, first_fun{first_fun}, second_fun{second_fun}, pump{pump}, counter{counter}, service {service}, can {can}
	, led_red {led_red}, led_green{led_green}, led_can {led_can}
	, in1 {in1}, in2 {in2}, in3 {in3}, level {level}
	, out1 {out1}, out2 {out2}, out3 {out3}, out4 {out4}
	, cool {cool}, fb_cool {fb_cool}, heat {heat}, fb_heat {fb_heat}
	{
		pump.power = 100;
				pump.start();
				counter.start();
	}

	void operator() (){

		service();

		led_green ^= blink.event();
		led_red ^= counter.is_alarm();
//		led_green = bool(fb_heat);
//		heat ^= blink.event();

		first_fun.power = 90;
		first_fun.start();

		second_fun.power = 60;
		second_fun.start();

//		pump.power = 100;
//		pump.start();
//		counter.start();

	}

};



