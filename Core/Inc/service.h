#pragma once

#include "interrupt.h"
#include "uart.h"
#include "ntc.h"

enum Function {
      read_03  = (uint8_t)0x03
    , read_04  = (uint8_t)0x04
    , write_16 = (uint8_t)0x10
}function;

enum Error_code {
      wrong_func   = (uint8_t)0x01
    , wrong_reg    = (uint8_t)0x02
    , wrong_value  = (uint8_t)0x03
}error_code;

struct State_dev{
	bool cool                : 1; //0
	bool heat                : 1; //1
	uint16_t res             : 14;
};

struct Error_dev{
	bool voltage_board_low   : 1; //0
	bool error_cool          : 1; //1
	bool error_heat          : 1; //2
	bool error_fun_1         : 1; //3
	bool error_fun_2         : 1; //4
	bool error_pump          : 1; //5
	bool error_low_press     : 1; //6
	bool error_high_press    : 1; //7
	bool error_t_dev         : 1; //8
	bool error_t_ex          : 1; //9
	bool error_t_con         : 1; //10
	bool error_t_heat1       : 1; //11
	bool error_t_heat2       : 1; //12
	uint16_t res             : 4;
};

struct In_data{

};

struct Out_data{
	uint16_t current_vent1;        // 0
	uint16_t current_vent2;        // 1
	uint16_t current_pump;         // 2
	uint16_t temp_dev;             // 3
	uint16_t temp_exchanger;       // 4
	uint16_t temp_condens;         // 5
	uint16_t temp_heat_1;          // 6
	uint16_t temp_heat_2;          // 7
	uint16_t voltage_board;        // 8
	uint16_t press_high;           // 9
	uint16_t press_low;            // 10
	State_dev state;               // 11
	Error_dev error;               // 12
};

constexpr float k_adc   = 3.33 / 4095;
constexpr float k_adc_i = 3 * k_adc / 2 / 0.0167; // 3 и 2 потому что делитель 10 и 20 кОм, 0,025 В/А

template<class In_data_t, class Out_data_t>
class Service
{
	ADC_& adc;
	NTC& ntc;
	UART_<>& uart;
	Interrupt& interrupt_DMA;
	Interrupt& interrupt_usart;

	Timer timer;

	float press_low{0};
	float press_high{0};

	uint8_t reg{0};
	bool event{false};
	bool kolhoz{false};
	bool done{true};

	void uartInterrupt(){
		event = true;
		timer.stop();
	}

	void dmaInterrupt(){
		uart.receive();
	}

	using Parent = Service;

	struct uart_interrupt: Interrupting {
		Parent &parent;
		uart_interrupt(Parent &parent) :
				parent(parent) {
			parent.interrupt_usart.subscribe(this);
		}
		void interrupt() override {
			parent.uartInterrupt();
		}
	} uart_ { *this };

	struct dma_interrupt: Interrupting {
		Parent &parent;
		dma_interrupt(Parent &parent) :
				parent(parent) {
			parent.interrupt_DMA.subscribe(this);
		}
		void interrupt() override {
			parent.dmaInterrupt();
		}
	} dma_ {*this};

public:

	static constexpr uint16_t InDataQty = sizeof(In_data_t) / 2;
	static constexpr uint16_t OutDataQty = sizeof(Out_data_t) / 2;

	union {
		In_data_t inData;
		uint16_t arInData[InDataQty];
	};
	union {
		Out_data_t outData;
		uint16_t arOutData[OutDataQty];
	};
	union {
		In_data_t inDataMin;
		uint16_t arInDataMin[InDataQty];
	};
	union {
		In_data_t inDataMax;
		uint16_t arInDataMax[InDataQty];
	};

	Service (
		  ADC_& adc
		, NTC& ntc
		, UART_<>& uart
		, Interrupt& interrupt_DMA
		, Interrupt& interrupt_usart
	) : adc              {adc}
	  , ntc              {ntc}
	  ,	uart             {uart}
	  , interrupt_DMA    {interrupt_DMA}
      , interrupt_usart  {interrupt_usart}
      , arInData { }, arOutData { }, arInDataMin { }, arInDataMax {}
	{
		uart.receive();
		timer.start(2000);
	}

	void operator()(){

//		outData.current        = (abs(adc.value(PS) - adc.offset_I_S)) * 100 / 21;
		outData.voltage_board  = k_adc * adc[VB] * 100;
		outData.temp_dev       = ntc(adc[T_DEV]);
		outData.temp_exchanger = ntc(adc[T_EX]);
		outData.temp_condens   = ntc(adc[T_CON]);
		outData.temp_heat_1    = ntc(adc[T_H1]);
		outData.temp_heat_2    = ntc(adc[T_H2]);

		press_low = 2.59 * ((k_adc * adc[7]) + 0.06) * 1.5 - 1.29;
		press_low = press_low > 0 ? press_low : 0;
		outData.press_low = press_low * 100;

		press_high = (2.59 * ((k_adc * adc[6]) + 0.06) * 1.5 - 1.29) * 2;
		press_high = press_high > 0 ? press_high : 0;
		outData.press_high = press_high * 100;

		outData.error.voltage_board_low = (outData.voltage_board <= 180);

		kolhoz ^= timer.event();

		if (event or kolhoz) {
			if(uart.buffer[0] == 4 or kolhoz) {
				uart.buffer.clear();
				uart.buffer << outData.current_vent1
						    << outData.current_vent2
							<< outData.current_pump
							<< outData.temp_dev
							<< outData.temp_exchanger
							<< outData.temp_condens
							<< outData.temp_heat_1
							<< outData.temp_heat_2
							<< outData.voltage_board
							<< outData.press_high
							<< outData.press_low
							<< arOutData[11] // state
							<< arOutData[12]; // error

			} else if(uart.buffer[0] == '+') {
				uart.buffer.clear();
				uart.buffer << 'O';
				uart.buffer << 'K';
			}
			event = false;
			kolhoz = false;
			if(uart.buffer.size())
				uart.transmit();
			else
				uart.receive();
		}
	}

};

Interrupt interrupt_dma;
Interrupt interrupt_uart;


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART3) {
		interrupt_dma.interrupt();
	}
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
	if (huart->Instance == USART3) {
		interrupt_uart.interrupt();
	}
}
