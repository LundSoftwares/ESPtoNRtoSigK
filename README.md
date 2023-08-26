# ESPtoNRtoSigK
# This repo is Depricated due to the release of SensESP v2! 


A simple, yet effective, way of sending data from an ESP866 based units (Wemos, NodeMCU and so on) all the way to a SignalK path of you choice. Or just handle warnings and notifications straight into Node Red if you want.

Data flow: I/O Sensor -> ESP8266 -> Wifi -> Node Red -> SignalK

First revision supports 5 Digital In, 1 Digital Out and several 1wire Temperature sensors.

<h2>INSTRUCTIONS:</h2>
<h4>Pre Conditions: SignalK server with Node Red added as an Addon. This creates all preconfigured SignalK nodes needed for this project.

<h4>1.</h4>
Get an ESP8266 device and connect your Digital Inputs, 1Wire sensors and Digital Out if necessary. Take note of the pins used, you will need it in Step 2.
<h4>2.</h4>
Download the .ino sketch file and open in Arduino IDE. Head to the Config area of the file, set your SSID, password, IP of you SignalK Server and change pins + path for you Digital Ins. If you need a DO (relay etc.) set the correct pin for that as well. Remember to change Input setting to INPUT_PULLUP if necessary for you DI's.
<h4>3.</h4>
Upload your edited sketch to an ESP8266 device of your choice. For details how to upload, google and find a guide how to set it up.
<h4>4.</h4>
Download the ESPtoRNtoSigK.json file containing the Node Red flow needed for bouncing the data from the ESP device to your SignalK Server. Import it to Node Red and Deploy.
<h4>5.</h4>
Config the Node Red according to your setup, follow the steps shown in this image:[image](https://user-images.githubusercontent.com/23386303/120547036-dcdd7380-c3f0-11eb-8956-564f9b455498.png)


  1. Insert your paths that you created in your .info file. If you need to transform you payload before sent to SignalK, do it in the Function node conencted to the correct output
  2. Show the debug window, if your 1wire sensors is correctly connected, the sensor addresses is print in that window.
  3. Copy each adress and put them on separate rows in the Switch Node
  4. Open the following Change Node and insert the SignalK path you want to send
  5. If you will use Digital Output, enter the IP address of the ESP8266 device. And create some case that will trigger the output, in this case a MQTT message or just manual Inject Nodes.
<h4>6.</h4>
That is, your data will now flow straight into SignalK!
