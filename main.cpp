#include <C:/msys32/ESP32/ESP32/components/arduino/cores/esp32/Arduino.h>
#include <C:/msys32/ESP32/esp-idf/components/driver/include/driver/rtc_io.h>
#include <C:/msys32/ESP32/esp-idf/components/driver/include/driver/adc.h>
#include <C:/msys32/ESP32/esp-idf/components/ulp/ulp.c>
#include <C:/msys32/ESP32/ESP32/build/main/ulp_main.h>
extern "C"
{
#include <C:/msys32/ESP32/esp-idf/components/esp32/include/esp_clk.h>
}
//Pode ser preciso arrumar os diretorios das bibliotecas
//Pode ser preciso remover o "extern 'C'{}" e definir a biblioteca fora dele, alguns usuarios relatam erro sem o uso do extern
 
extern const uint8_t ulp_main_bin_start[] asm("_binary_ulp_main_bin_start");
extern const uint8_t ulp_main_bin_end[] asm("_binary_ulp_main_bin_end");
void ulp();
 
 
extern "C" void app_main()
{
	initArduino();//inicia configuracoes do arduino, caso nao use o Arduino component, remova essa linha
	pinMode(2, OUTPUT);
 
	if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_ULP)//se o wakeup for por causa do ULP, tomara alguma atitude
	{
		digitalWrite(2, 1);
		delay(500);
		digitalWrite(2, 0);
	}
	else//se nao, iniciara o ULP
	{
		ulp();//configura e inicializa o ulp
	}
	
 
	esp_sleep_enable_ulp_wakeup();//habilita o wakeup pelo ULP
	esp_deep_sleep_start();//entra em deep sleep eterno
}
 
 
 
 
void ulp()
{
	adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_11db);
	adc1_config_width(ADC_WIDTH_12Bit);
	adc1_ulp_enable();
	//configura o ADC1 #4 (GPIO32) para 3.3V 12bit e atribui o uso ao ULP
	
	ulp_set_wakeup_period(0, 10000000);//ativa o timer de wakeup do ULP apos cada HALT para 10seg
 
	ulp_load_binary(0, ulp_main_bin_start, (ulp_main_bin_end - ulp_main_bin_start) / sizeof(uint32_t));//carrega os arquivos
	ulp_run((&ulp_main - RTC_SLOW_MEM) / sizeof(uint32_t));//inicia o ULP
}
