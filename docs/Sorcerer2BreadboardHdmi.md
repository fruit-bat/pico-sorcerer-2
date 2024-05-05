### Exidy Sorcerer 2 on breadboard with HDMI
This is a series of targets based around a breadboard prototype:

<img src="pico_sorcerer_prototype_1.jpg" height="200"/>

Support for the following:
* USB keyboard
* PS/2 keyboard
* USB joysticks
* 60Hz HDMI video
* HDMI/PWM audio
* SPI SD card
* Serial port debug

#### Circuit diagrams
Only GPIO 20 is used for PWM audio output.

![image](Sorcerer2BreadboardHdmi.png)

![image](pi_pico_sorcerer2_pwm_audio_filter.png)

#### Firmware
| Audio | Display mode | Firmware |
| - | - | - |
| HDMI | 640x480x60Hz | [sorcerer2_hdmi_hdmi_audio.uf2](/uf2/sorcerer2_hdmi_hdmi_audio.uf2) |
| 1 pin PWM | 640x480x60Hz | [sorcerer2_demo_dv_i2s_audio.uf2](/uf2/sorcerer2_demo_dv_i2s_audio.uf2) |

