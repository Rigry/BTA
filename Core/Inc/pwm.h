#pragma once

class PWM {

	TIM_HandleTypeDef *htim;
	const uint32_t Channel;
	uint16_t max_ccr{0};


	class Power {
		uint16_t value{0};
		PWM& parent;

	public:

		Power(PWM& parent) : parent{parent} {}
		operator uint16_t() { return value; }
		uint16_t operator+=(int16_t v) {return operator=(value + v); }
		uint16_t operator=(uint16_t v) {
			if( (v != value) and (v >= 0) ) {
				value = v;
				uint16_t cnt = parent.max_ccr / 100 * value - 1;
				if (parent.Channel == TIM_CHANNEL_1)
					parent.htim->Instance->CCR1 = cnt;
				else if (parent.Channel == TIM_CHANNEL_2)
					parent.htim->Instance->CCR2 = cnt;
				else if (parent.Channel == TIM_CHANNEL_3)
					parent.htim->Instance->CCR3 = cnt;
				else if (parent.Channel == TIM_CHANNEL_4)
					parent.htim->Instance->CCR4 = cnt;
			}

			return v;
		}
	};

public:

	PWM(TIM_HandleTypeDef* htim, uint32_t Channel, uint16_t max_ccr)
	: htim{htim}, Channel{Channel}, max_ccr{max_ccr}

	{

	}

	Power power{*this};

	void start() {HAL_TIM_PWM_Start(htim, Channel);}
	void stop()  {HAL_TIM_PWM_Stop (htim, Channel);}

	void operator= (bool v) {
		v ? start() : stop();
	}

};
