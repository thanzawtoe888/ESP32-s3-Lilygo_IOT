import paho.mqtt.client as mqtt
from datetime import datetime   #date time men you see this message
import urllib3  #to connect to the internet
import json  #to parse the json data

http = urllib3.PoolManager() # make a connection 

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, reason_code, properties):
    print(f"Connected with result code {reason_code}")
    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe("tzt/luca/esp32/status")


# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print(msg.topic+" "+str(msg.payload))
    payload ={"timestamp": datetime.now().isoformat(), "message": msg.payload.decode("utf-8")}
    encoded_payload = json.dumps(payload).encode("utf-8")
    print(encoded_payload)
    http.request('POST', 'https://tztaiot-default-rtdb.asia-southeast1.firebasedatabase.app/alexa.json', body=encoded_payload, headers={'Content-Type': 'application/json'})

mqttc = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
mqttc.on_connect = on_connect
mqttc.on_message = on_message

mqttc.connect("192.168.137.1", 1883, 60)

# Blocking call that processes network traffic, dispatches callbacks and
# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.
mqttc.loop_forever()