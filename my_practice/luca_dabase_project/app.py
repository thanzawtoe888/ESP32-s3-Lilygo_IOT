import paho.mqtt.client as mqtt
from datetime import datetime   #date time men you see this message
import urllib3  #to connect to the internet
import json  #to parse the json data
import sqlite3  #to connect to the database  

http = urllib3.PoolManager() # make a connection 

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, reason_code, properties):
    print(f"Connected with result code {reason_code}")
    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe("tzt/luca/esp32/data")


# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print(msg.topic+" "+str(msg.payload))
    
    #insert to SQLite 
    try:
        dev_name =msg.topic.split("/")[2]  #get the last part of the topic, which is the device name ('/' will be the seperation)
        dev_data = json.loads(msg.payload.decode())['value']
        # sql_cmd = f"INSERT INTO luca (device, data) VALUES ('{dev_name}', {dev_data})"  #don't use this line, it is not flexible so we need to use placeholder style under 
        c.execute("INSERT INTO luca (device, data) VALUES (?, ?)",
                    (dev_name, dev_data))  #placeholder style 
        conn.commit()  #commit the changes to the database **important**
    except :
        print("Error: cannot inset to SQLite")
    #insert to firebas e 
    
    # payload ={"timestamp": datetime.now().isoformat(), "message": msg.payload.decode("utf-8")}
    # encoded_payload = json.dumps(payload).encode("utf-8")
    # print(encoded_payload)
    # http.request('POST', 'https://tztaiot-default-rtdb.asia-southeast1.firebasedatabase.app/alexa.json', body=encoded_payload, headers={'Content-Type': 'application/json'})
#init sqlite3
conn = sqlite3.connect('luca.db')
c= conn.cursor()  #to execute sql commands
c.execute('''CREATE TABLE IF NOT EXISTS luca (
          _id INTEGER PRIMARY KEY AUTOINCREMENT,
          timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
          device TEXT,
          data REAL             )''')
conn.commit()  #commit the changes to the database **important**
# _id=name of the column, Interger= column type, PRIMARY KEY= unique value, AUTOINCREMENT= automatically increase the value of the column

#init MQTT
mqttc = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
mqttc.on_connect = on_connect
mqttc.on_message = on_message

mqttc.connect("192.168.137.1", 1883, 60)

# Blocking call that processes network traffic, dispatches callbacks and
# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.
mqttc.loop_forever()