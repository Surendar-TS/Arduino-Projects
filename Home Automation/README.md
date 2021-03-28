                                                                                Home Automation
Objective:
To automate and provide user-friendly real-time analysis of most of the household appliances.

Working:
Inputs:
	Humidity & Temperature Sensor – Provides temperature and humidity in the house
	LDR Sensor Module – Resistance values change according to the light available.
	Flex Sensor – Resistance values change according to the pressure and bend applied to the sensor.
All the sensor values are uploaded to ubidots and can be viewed in dashboard in real-time.

Outputs:
Automation mode – The control is switched as Automation, in Ubidots.
	DC Light -  The lights are turned on/off according to the LDR sensor values.
	DC Fan -  The fan is turned on/off according to the LDR sensor values.



User Control – The control is switched as User, in Ubidots.
The fan and the light are controlled from ubidots from the user and automation of devices does not work in this mode. Light can also be turned on or off using the flex sensor.
