# ESPtoNRtoSigK
A simple, yet effective, way of sending data from an ESP866 based unit (Wemos, NodeMCU and so on) all the way to a SignalK path of you choice. Or just handle warnings and notifications straight into Node Red if you want.

Data flow: I/O Sensor -> ESP8266 -> Wifi -> Node Red -> SignalK

First revision supports Digital In, Digital Out and 1wire Temperature sensors.
