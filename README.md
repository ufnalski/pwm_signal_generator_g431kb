# Adjustable PWM generator with presets (STM32G431KB)
If you don't have a bench PWM signal generator, build one. Take an inspiration from my STM32 HAL example of an adjustable PWM generator with presets - a rudimentary one, yet quite useful :slightly_smiling_face: For starters, our DUTs (devices under test) will be: an SG90 servo (180 degrees), an SG90 drive (continuous 360 degrees), an Audi A4 throttle body, and a drone BLDC motor ESC. 

![PWM signal generator and SG90 servos](/Assets/Images/pwm_signal_generator_sg90.jpg)
![PWM signal generator (back view)](/Assets/Images/pwm_signal_generator_back.jpg)
![PWM signal generator and drone BLDC motor ESC](/Assets/Images/pwm_signal_generator_drone_esc.jpg)
![Audi A4 (B6) throttle body](/Assets/Images/audi_a4_b6_throttle_body.jpg)
![Audi A4 (B6) throttle body closed](/Assets/Images/audi_a4_b6_throttle_body_closed.jpg)
![Audi A4 (B6) throttle body open](/Assets/Images/audi_a4_b6_throttle_body_open.jpg)
![BMW F60 coolant pump PWM (Bosch PCE)](/Assets/Images/pwm_signal_generator_pce.jpg)
![BMW Countryman (F60) coolant pump](/Assets/Images/bmw_f60_coolant_pump.jpg)

# Missing files?
Don't worry :slightly_smiling_face: Just hit Alt-K to generate /Drivers/CMCIS/ and /Drivers/STM32G4xx_HAL_Driver/ based on the .ioc file. After a couple of seconds your project will be ready for building.

# Exemplary DUTs
* [SG90 micro servo [180 degrees]](https://botland.store/micro-servos/13128-servo-sg-90-micro-180-5904422350338.html)
* [SG90 micro drive [continuous 360 degrees]](https://kamami.pl/en/micro-servos/560758-servo-tower-pro-sg90-360-degrees-5906623467167.html)
* [Drone BLDC motor ESC [PWM legacy mode]](https://github.com/bitdump/BLHeli/blob/master/BLHeli_32%20ARM/BLHeli_32%20manual%20ARM%20Rev32.x.pdf)
* [Drive By Wire - Electronic Throttle Control - Explained](https://www.youtube.com/watch?v=Pgln9L5Y7sQ) (Engineering Explained)
* [Drive By Wire With PWM / Audi A4 1.8T Throttle Body](https://www.youtube.com/watch?v=h1aA5gfeU34) (0033mer)
* [Pierburg CWA400 water pump [PWM version]](https://tecomotive.com/download/datasheets/CWA400_PWM_EN.pdf)
* [Bosch PCE coolant pumps [PWM version]](https://openinverter.org/wiki/Bosch_PCE_Coolant_Pumps)

# What's inside the Bosch PCE circulation pump?
Coming soon :soon:

# What next?
I challenge you to develop a closed-loop control of a butterfly valve :sunglasses:
* [PID controlled throttle body](https://www.youtube.com/watch?v=MumY5Gwr40U) (Max LeBlanc)
* [PID Throttle Body Control. Full 3D printed Body.](https://www.youtube.com/watch?v=4qR2Q0_GDwQ) (Z-Tech Extrem)
* [DStage #18 - DIY Electronic Throttle Control [a.k.a. Drive By Wire]](https://www.youtube.com/watch?v=MTnsbOgZkUA) (DStage)

# Call to action
Create your own [home laboratory/workshop/garage](http://ufnalski.edu.pl/control_engineering_for_hobbyists/2025_high_school/Control_Engineering_for_Hobbyists_2025_02.pdf)! Get inspired by [ControllersTech](https://www.youtube.com/@ControllersTech), [DroneBot Workshop](https://www.youtube.com/@Dronebotworkshop), [Andreas Spiess](https://www.youtube.com/@AndreasSpiess), [GreatScott!](https://www.youtube.com/@greatscottlab), [ElectroBOOM](https://www.youtube.com/@ElectroBOOM), [Electronoobs](https://www.youtube.com/electronoobs), [Phil's Lab](https://www.phils-lab.net/), [atomic14](https://www.youtube.com/@atomic14), [That Project](https://www.youtube.com/@ThatProject), [Paul McWhorter](https://www.youtube.com/@paulmcwhorter), [Sara and Rui Santos (Random Nerd Tutorials)](https://randomnerdtutorials.com/), and many other professional hobbyists sharing their awesome projects and tutorials! Shout-out/kudos to all of them!

> [!WARNING]
> Control in drives - do try this at home :exclamation:

200+ challenges to start from: [Control Engineering for Hobbyists at the Warsaw University of Technology](http://ufnalski.edu.pl/control_engineering_for_hobbyists/Control_Engineering_for_Hobbyists_list_of_challenges.pdf).

Stay tuned :exclamation:
