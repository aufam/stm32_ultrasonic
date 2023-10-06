#include "ultrasonic/ultrasonic.h"
#include "etl/keywords.h"

using namespace Project;

fun Ultrasonic::init() -> void {
    trigger.init({.mode=GPIO_MODE_OUTPUT_PP, .pull=GPIO_NOPULL, .speed=GPIO_SPEED_FREQ_HIGH});
    event.init();
    inputCapture.init();
    inputCapture.callback = {lambda (Ultrasonic* self) { self->inputCaptureCallback(); }, this};
}

fun Ultrasonic::deinit() -> void {
    trigger.off();
    inputCapture.deinit();
}

fun Ultrasonic::delayUs(uint32_t us) -> void  {
    __HAL_TIM_SET_COUNTER(&inputCapture.htim, 0);
    while (__HAL_TIM_GET_COUNTER(&inputCapture.htim) < us);
}

fun Ultrasonic::read() -> float {
    if (etl::time::elapsed(lastReadTime) < timeout)
        return distance_;
    
    trigger.on();
    delayUs(10);
    trigger.off();

    lastReadTime = etl::time::now();
    inputCapture.enable();
    distance_ = NAN;
    event.waitFlags({.flags=1u << inputCapture.channel, .option=osFlagsWaitAny, .timeout=timeout});
    inputCapture.disable();
    return distance_;
}

fun Ultrasonic::inputCaptureCallback() -> void {
    auto& channel = inputCapture.channel;
    auto* htim = &inputCapture.htim;

    if (isRising) {
        valueRising = HAL_TIM_ReadCapturedValue(htim, channel);
        isRising = false;
        __HAL_TIM_SET_CAPTUREPOLARITY(htim, channel, TIM_INPUTCHANNELPOLARITY_FALLING);
        return;
    }

    valueFalling = HAL_TIM_ReadCapturedValue(htim, channel);
    __HAL_TIM_SET_COUNTER(htim, 0);
    isRising = true;

    auto diff = valueFalling > valueRising ? valueFalling - valueRising : 0xFFFF - valueRising + valueFalling;
    distance_ = (float) diff * factor;

    __HAL_TIM_SET_CAPTUREPOLARITY(htim, channel, TIM_INPUTCHANNELPOLARITY_RISING);
    inputCapture.disable();
    Ultrasonic::event.setFlags(1 << channel);
}