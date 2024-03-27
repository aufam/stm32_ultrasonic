#include "ultrasonic/ultrasonic.h"
#include "etl/keywords.h"


fun Ultrasonic::init() -> void {
    trigger.init({.mode=GPIO_MODE_OUTPUT_PP, .pull=GPIO_NOPULL, .speed=GPIO_SPEED_FREQ_HIGH});

    inputCapture.htim.Instance->PSC = SystemCoreClock / 1'000'000 - 1;
    inputCapture.htim.Instance->ARR = 0xFFFF - 1;
    inputCapture.init();
}

fun Ultrasonic::deinit() -> void {
    trigger.off();
    inputCapture.deinit();
}

fun Ultrasonic::read() -> etl::Future<float> {
    return [this] (etl::Time timeout) -> etl::Result<float, osStatus_t> {
        trigger.on();
        inputCapture.htim.Instance->CNT = 0; 
        while (__HAL_TIM_GET_COUNTER(&inputCapture.htim) < 10); // delay 10us
        trigger.off();

        inputCapture.htim.Instance->CNT = 0; 
        inputCapture.polarity = TIM_INPUTCHANNELPOLARITY_RISING;

        return inputCapture.wait(timeout)
            .and_then([this, timeout] (uint32_t rising) {
                inputCapture.polarity = TIM_INPUTCHANNELPOLARITY_FALLING;
                return (inputCapture.wait(timeout) - rising) * factor;
            });
    };
}