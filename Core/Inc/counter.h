#pragma once

class Counter : TickSubscriber {

	TIM_HandleTypeDef *htim;
	const uint32_t Channel;
	Interrupt& ic_interrupt;

	uint16_t time{0};
	uint16_t time_alarm{0};
	bool alarm{false};

	void tim_interrupt() {
		time = 0;
		alarm = false;
	}

public:

	Counter(TIM_HandleTypeDef* htim, uint32_t Channel, Interrupt& ic_interrupt, uint16_t time_alarm)
	: htim{htim}, Channel{Channel}, ic_interrupt{ic_interrupt}, time_alarm{time_alarm}

	{
		subscribed = false;
	}

	using Parent = Counter;

	struct tim_ic_interrupt : Interrupting
	{
		Parent& parent;
		tim_ic_interrupt (Parent& parent) : parent(parent) {
	    	parent.ic_interrupt.subscribe (this);
	    }
	    void interrupt() {parent.tim_interrupt();}
	}tim_ic{ *this };

	void start(){
		time = 0;
		alarm = false;
		HAL_TIM_IC_Start_IT(htim, Channel);
		subscribe();
	}

	void stop(){
		HAL_TIM_IC_Stop_IT(htim, Channel);
		unsubscribe();
	}

	bool is_alarm() {return alarm;}

	void notify() {
		if (time++ >= time_alarm) {
			time = 0;
			alarm = true;
			stop();
		}
	}

};

Interrupt interrupt_ic_tim;

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
	if(htim->Instance == TIM3) {
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) {
			interrupt_ic_tim.interrupt();
		}
	}
}
