# LED Variasi Interrupt - STM32F401CCU6 Project

Proyek ini merupakan implementasi pengendalian 3 LED dengan 2 tombol menggunakan interrupt eksternal dan teknik debouncing berbasis STM32F401CCU6. Proyek dibuat menggunakan **STM32CubeIDE** dan memanfaatkan **HAL Library**.

## 🧠 Fitur Utama

- Menggunakan 2 push-button (PA0 dan PA1)
- Tiga LED dikontrol dengan berbagai pola:
  - **PA5 dan PA6** untuk LED utama
  - **PC13** untuk LED onboard
- Interrupt eksternal (EXTI) untuk kedua tombol
- Debouncing berbasis software
- Penggunaan `HAL_GPIO_EXTI_Callback()` untuk deteksi input
- Pola LED dapat diganti-ganti dengan menekan tombol

## 🔄 Tabel Pola LED

| Pola | Deskripsi                                 | PA5 (LED1) | PA6 (LED2) | PC13 (LED3 - onboard) |
|------|-------------------------------------------|------------|------------|------------------------|
| 0    | LED Onboard menyala                       | OFF        | OFF        | ON                     |
| 1    | LED1 Blinking                             | Blink      | OFF        | ON                     |
| 2    | LED2 Blinking                             | OFF        | Blink      | ON                     |
| 3    | LED1 dan LED2 Blink bersamaan             | Blink      | Blink      | OFF                    |

## ⚙️ Requirements

- STM32F401CCU6 + ST-Link V2
- STM32CubeIDE versi terbaru
- Kabel USB dan push-button eksternal
- LED tambahan

## 🛠️ Cara Build dan Upload

1. Buka `led_variasi_interrupt.ioc` menggunakan **STM32CubeIDE**.
2. Klik **Project > Build Project**.
3. Sambungkan board STM32 ke komputer via ST-Link.
4. Jalankan **Run > Debug As > STM32 Cortex-M C/C++ Application**.
5. Tekan tombol PA0 dan PA1 untuk melihat variasi pola LED.


