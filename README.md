# UPGRADED_GPS_ALARM_BICYCLE
An electronics project, including assembly and coding. Innovative part : accelerometer addition
Normal gps alarms are not efficient, since you can detect the theft after they cut your lock and started to ride it away. Even if you are close to it, you cannot run after and catch it.
Thus, you need to intervene as soon as they lay finger on your property. Touching causes vibrations, which can be detected by an accelerometer. These accelerometers are so sensitive, they can get triggered with the wind, so you add a threshold. If incase your accelerometer fails to detect(which is not likely) or you are too far from your bike to intervene, then dont worry there is still a gps sending you constant location updates.

All modules can be tested individually and combined after. You need a sim card for sim800L module. I choosed a sim card with a good mini package for about 3 dollars monthly with infinite sms to my number and some internet.
choose a suitable battery setting and you are good to go.

I will provide the combined code that sends google maps link of last location as sms when triggered.


Parts: ( tried to pick smallest available parts for hiding it on the bike)
  adxl345 accelerometer
  sim800L module for sim card communication
  arduino nano for processing
  GPS module
  batteries + wires
  case(from cardboard or else)

Test phase of individual components:
  use breadboards and jumpers to test each module first.
  write individual codes to test each module.
  combine the codes.
  combine the modules pyhsically to fit some where hidden on bike.
