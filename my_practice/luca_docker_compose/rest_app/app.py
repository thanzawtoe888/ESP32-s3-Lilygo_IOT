from flask import Flask
from pymongo import MongoClient
from datetime import datetime
import json

app = Flask
mongo_client = MongoClient("luca_docker_project-mongo-1",27017,
                           username = "root", password = 'example') 

@app.route('/api/station/<station_id>', methods=['GET'])
def query_station(station_id):
    db = mongo_client.luca_db
    db_col = db.ble.logs
    resp = {}
    if station_id is None:
        resp['status'] = 'error'
        resp['message'] = 'Station ID is required'
        return json.dumps(resp)
    
    data = db_col.find({"station": station_id})
    resp['status'] = 'ok'
    resp['station'] = station_id
    resp['data'] = []
    for record in data:
        record_data = {}
        record_data['timestamp'] = record['timestamp'].isoformat() if 'timestamp' in record else None
        record_data['device'] = record['device'] 
        record_data['rssi'] = record['rssi'] 
        resp['data'].append(record_data)
    return json.dumps(resp)

@app.route('/api/asset/<asset_id>', methods=['GET'])
def query_asset(asset_id):
    db = mongo_client.luca_db
    db_col = db.ble.logs
    resp = {}
    if asset_id is None:
        resp['status'] = 'error'
        resp['message'] = 'Asset ID is required'
        return json.dumps(resp)
    
    data = db_col.find({"device": asset_id})
    resp['status'] = 'ok'
    resp['asset'] = asset_id
    resp['data'] = []
    for record in data:
        record_data = {}
        record_data['timestamp'] = record['timestamp'].isoformat() if 'timestamp' in record else None
        record_data['station'] = record['station']
        record_data['rssi'] = record['rssi']
        resp['data'].append(record_data)
    return json.dumps(resp)
     
        