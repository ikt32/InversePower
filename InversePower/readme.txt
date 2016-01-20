// InversePower by ikt
1.1

InversePower tries to counteract the fact that GTA V cuts off power when your 
car starts gliding. This allows for a more realistic and less annoying driving 
experience. Drifting and slides are easier to maintain and adjust.

This mod is inspired by Drift Assist by InfamousSabre and works similarly. The
big difference is that speed is used as a negative feedback to make cars not
accelerate too much and swerve out of control at higher speeds.

// Requirements
GTA V
Alexander Blade's C++ Scripthook

// Recommended
This mod was made using Killatomatoes Realistic Driving.

// Installation
Extract InversePower.asi and InversePower.ini to your GTA V game folder.

// Configuration
This plugin won't work if engine power or torque are modified other scripts.

The ini file will look like this. These are the default values and should be 
good for usage with Killatomate's Realistic Driving mod.

Don't change the section names (between the [ ] brackets).

[CONFIG]
Power = 100
Torque = 80
Angle = 350
Speed = 200
Slope = 35

[DEBUG]
Debug = 0

Power, Torque - How much these are affected by the mod in percentage.
Power gives the engine more potential. Torque is effective for making the
wheels spin more. If a handling.meta is used with high grip, lower Torque.

Angle, Speed - How much these affect Power and Torque. No units.
Angle - The higher this value, the higher the adjustment will be when the car 
is sliding at a big angle.
Speed - The higher this value, the higher the adjustment will be when the car 
is sliding at a low speed.

Slope - In meters/second. At this speed or more, any adjustment by speed will 
be 0. This means that at low speeds, the adjustments will have more effect. 
Increasing this allows for more adjustment at higher speeds. Increase this if 
you like big power slides. Default 35m/s = 126km/h = ±78mph.

Debug - 0 or 1. Enable or disable a debug window with data from mod.

All options can be changed on the fly, so you won't need to restart the game
to try out different settings.

Have fun!
- ikt

// Changelog
1.0 - Initial version
1.1 - Allow for angles >> 90 to get a power boost
	- Clean up debug window
	- Changed default values for less erratic behavior
	
// License
Just don't be a donkey butt. If this archive is redistributed, include so this
readme.txt so users know what these configuration options do. I'm not liable
for any damages nor can you expect anything from me.