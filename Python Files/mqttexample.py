from paho.mqtt.client import * #import the paho-mqtt module
import json

def on_message(client,userdata,msg):
	print('Topic: ',msg.topic)
	message=str(msg.payload.decode('utf-8'))
	print('Message: ',message)
	client.publish('iot/sub','Message Received')
	data=json.loads(message)
	print('Device ID:',data['deviceid'])
	print('Temperature:',data['temp'])
	print('Humidity:',data['hum'])

client=Client('Device-1')
c=client.connect('test.mosquitto.org',1883)
# print(c)
if c==0:
	print('Client Connected')
	client.subscribe('iot/pub')
	client.on_message=on_message
	client.loop_forever();
