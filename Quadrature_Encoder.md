 # **Encoder Module for 25GA370 DC Gear Motor (with Magnetic Hall Encoder) using Raspberry Pi Pico W**

## **1. Overview**

This document describes the hardware, encoder specifications, software layers, and SDK components required to implement an **Encoder Driver (HAL + Service Layer)** for the **25GA370 DC Gear Motor with Magnetic Hall-Effect Encoder** using the **Raspberry Pi Pico W**.

---

# **2. Motor & Encoder Description**

### **Motor Model**

* **25GA370 DC Gear Motor**
* **Voltage:** 12V DC
* **Speed:** ~210 RPM (depending on gear ratio)
* **Gearbox:** Planetary/Spur (varies by model)

---

# **3. Encoder Type**

The motor includes a **Magnetic Hall-Effect Quadrature Encoder**, featuring:

* **2 Channels:** A and B
* **Digital square-wave outputs**
* **Phase-shifted signals** to determine direction
* **Hall sensors + ring magnet** inside the motor
* **Output compatible with 3.3V logic (Pico safe)**

---

# **4. Encoder Resolution (PPR/CPR)**

* The magnetic disk generates approximately **11 Pulses Per Revolution (PPR)** **before** the gearbox.
* After the gearbox, the **CPR (Counts Per Revolution)** increases:

[
CPR = 11 \times \text{Gear Ratio}
]

Examples:

* Gear Ratio 1:30 → CPR = **330**
* Gear Ratio 1:90 → CPR = **990**

 *CPR may vary slightly by manufacturer, so measuring it experimentally is recommended.*

---

# **5. Encoder Wiring**

Typical wire colors:

| Wire Color    | Function                           |
| ------------- | ---------------------------------- |
| Red           | Encoder VCC (3.3–5V)               |
| Black         | Ground                             |
| Yellow        | Output A                           |
| Green         | Output B                           |
| *Motor wires* | Separate two wires for motor power |

---

# **6. Quadrature Signal Behavior (A/B)**

The encoder outputs two square waves:

* **A leads B → Forward rotation**
* **B leads A → Reverse rotation**

```
Forward:
A: _|‾|_‾|_‾|_
B: __|‾|_‾|_‾|_

Reverse:
A: __|‾|_‾|_‾|_
B: _|‾|_‾|_‾|_
```

This allows direction and speed measurement.

---

# **7. What I Must Implement**

I am responsible for **both layers**:

## **A) HAL Layer (Hardware Abstraction Layer)**

Low-level interface to the encoder hardware.

### **tasks:**

* Configure GPIO pins for A and B.
* Enable internal pull-up resistors.
* Configure Interrupts (IRQs) on rising and falling edges.
* Count encoder ticks reliably.
* Track direction using A/B relationship.
* Provide simple hardware-level functions:

**Required HAL Functions**

```
encoder_init();
encoder_get_ticks();
encoder_clear();
encoder_get_direction();
```

---

## **B) Service Layer (Logic/Processing Layer)**

Creates meaningful data from raw encoder ticks.

### **Your tasks:**

* Compute **RPM (speed)**.
* Compute **distance** based on wheel radius (if used).
* Compute **linear speed (cm/s)**.
* Handle optional filtering (e.g., moving average).
* Provide high-level APIs:

**Required Service Functions**

```
encoder_calculate_rpm();
encoder_get_speed_cm_s();
encoder_get_position_rotations();
encoder_get_distance_cm();
```

---


# **8. Pico SDK Components i Will Use**

From the Raspberry Pi Pico SDK, i will mainly use:

### **✔ GPIO functions**

* `gpio_init()`
* `gpio_set_dir()`
* `gpio_pull_up()`
* `gpio_get()`

### **✔ Interrupt handling**

* `gpio_set_irq_enabled()`
* `gpio_set_irq_enabled_with_callback()`

### **✔ Timer (for periodic RPM calculation)**

* `add_repeating_timer_ms()`

### **✔ Optional**

* `stdio_init_all()` — for debugging via USB/UART

---

# **9. RPM Calculation Formula**

If you sample ticks every **100 ms**:

[
RPM = \frac{\text{ticks} \times 600}{CPR}
]

(600 = 60 / 0.1)

---

# **10. Distance Calculation (if wheel used)**

[
Distance = \frac{Ticks}{CPR} \times (2\pi r)
]

Where:

* r = wheel radius in cm

---

# **12. Summary**

This module will allow my Robot to:

* Determine direction
* Measure speed (RPM)
* Compute distance traveled
* Enable closed-loop motor control (PID uses your output)

 

 