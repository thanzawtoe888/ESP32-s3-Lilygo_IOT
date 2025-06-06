import paho.mqtt.client as mqtt
from datetime import datetime   #date time men you see this message
import urllib3  #to connect to the internet
import json  #to parse the json data
import sqlite3  #to connect to the database  
from pymongo import MongoClient  #to connect to the MongoDB database

http = urllib3.PoolManager() # make a connection 

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, reason_code, properties):
    print(f"Connected with result code {reason_code}")
    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe("tzt/luca/esp32/#")


# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print(msg.topic+" "+str(msg.payload))
    #heartbeat message
    if msg.topic.split("/")[-1] == "beat":
        data  = json.loads(msg.payload.decode())  #decode the payload to a json object
        print(f"Device: {data['mac']}")
        return
    if msg.topic.split("/")[-1] == "data":
        data = json.loads(msg.payload.decode())  #decode the payload to a json object
        print(f"Device: {data['name']}")
        
        station = msg.topic.split("/")[2]  #get the last part of the topic, which is the device name ('/' will be the seperation)
        device = data['name'] 
        rssi = data ['rssi']
        c.execute("INSERT INTO luca (station, device, rssi) VALUES (?, ?, ?)",
                    (station, device, rssi))  #placeholder style
        print("Inserted to SQLite") 
        conn.commit()
  
    
        #insert to MongoDB
        db = mongo_client.luca_db  #get the database
        db_col = db.ble.log
        #create a dictionary to insert to the database
        db_col.insert_one({"timestamp" : datetime.now(),
                       "station" : station,
                       "device" : device,
                       "rssi" : rssi})  #insert the data to the database
        print("Inserted to MongoDB")
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
          station TEXT,
          device TEXT,
          rssi INTEGER
          )''')  #create a table if it does not exist
conn.commit()  #commit the changes to the database **important**
# _id=name of the column, Interger= column type, PRIMARY KEY= unique value, AUTOINCREMENT= automatically increase the value of the column

#init MongoDB
mongo_client = MongoClient("luca_dokcer_compose-mongo-1',27017",
                           username= 'root', password= 'example')  #connect to the MongoDB database

#init MQTT
mqttc = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
mqttc.on_connect = on_connect
mqttc.on_message = on_message

mqttc.connect("mosquitto", 1883, 60)

# Blocking call that processes network traffic, dispatches callbacks and
# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.
mqttc.loop_forever()