#ifndef PROJECT_ULTRASONIC_H
#define PROJECT_ULTRASONIC_H

#include "periph/input_capture.h"
#include "periph/gpio.h"
#include "etl/event.h"
#include "etl/getter_setter.h"
#include "etl/async.h"
#include <cmath>

namespace Project { class Ultrasonic; }

/// generic distance measurement using ultrasonic time flight
class Project::Ultrasonic {
    using Event = etl::Event;
    static constexpr float factor = .034f / 2.f; ///< convert time in us to distance in cm

public:
    periph::InputCapture& inputCapture;
    periph::GPIO trigger;

    /// default constructor
    constexpr Ultrasonic(periph::InputCapture& inputCapture, const periph::GPIO& trigger)
        : inputCapture(inputCapture)
        , trigger(trigger) {}

    Ultrasonic(const Ultrasonic&) = delete; ///< disable copy constructor
    Ultrasonic& operator=(const Ultrasonic&) = delete;  ///< disable copy assignment

    /// reset trigger pin, init event and input capture
    void init();

    /// reset trigger pin, deinit event and deinit input capture
    void deinit();

    /// read distance in blocking mode
    /// @retval distance in cm
    etl::Future<float> read();
};

#endif //PROJECT_ULTRASONIC_H
