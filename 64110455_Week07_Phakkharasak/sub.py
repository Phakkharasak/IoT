import paho.mqtt.client as mqtt
import json
import requests
import socket

# Initialize a dictionary to store the transformed data
transformed_data = {}
id_counter = 1

def get_local_ip():
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    try:
        # Doesn't even have to be reachable
        s.connect(('10.255.255.255', 1))
        local_ip = s.getsockname()[0]
    except Exception:
        local_ip = '127.0.0.1'
    finally:
        s.close()
    return local_ip

def on_connect(client, userdata, flags, rc):
    print("Connected to broker " + str(rc))
    client.subscribe("dht11")

def on_message(client, userdata, msg):
    global id_counter
    print(f"message on topic {msg.topic}: {msg.payload.decode()}")

    # Parse the received payload
    payload = msg.payload.decode()

    # Check if the payload contains a failure message
    if "Failed to read from DHT sensor!" in payload:
        handle_sensor_failure()
        return

    # Splitting the payload to extract temperature and humidity
    values = payload.split(" ")

    # Check if the values list has enough elements
    if len(values) < 2:
        print("Invalid payload format")
        return

    value_type = values[0]
    value = values[1]

    # Update the transformed data dictionary
    transformed_data[value_type] = value

    # Check if both temperature and humidity data exist, then send to the server
    if "Temperature" in transformed_data and "Humidity" in transformed_data:
        transformed_data["id"] = id_counter
        transformed_data["IP"] = get_local_ip()
        send_to_json_server()
        id_counter += 1

def handle_sensor_failure():
    global id_counter
    # Handle the case when the sensor fails to read
    transformed_data["id"] = id_counter
    transformed_data["Temperature"] = "Error"
    transformed_data["Humidity"] = "Error"
    transformed_data["SensorFailure"] = "Failed to read from DHT sensor!"
    transformed_data["IP"] = get_local_ip()
    send_to_json_server()
    id_counter += 1

def send_to_json_server():
    global transformed_data

    # Create the final payload for the server
    final_data = {
        "id": transformed_data.get("id", 0),
        "Temperature": f"{transformed_data.get('Temperature', '')} Celsius",
        "Humidity": f"{transformed_data.get('Humidity', '')} Percent",
        "SensorFailure": transformed_data.get("SensorFailure", ""),
        "IP": transformed_data.get("IP", "")
    }

    url = "http://172.20.10.12:3000/dht11"  # Change the URL accordingly

    # POST request to the JSON server
    headers = {'Content-type': 'application/json'}
    response = requests.post(url, json=final_data, headers=headers)

    # Reset transformed_data after sending
    transformed_data = {}

client = mqtt.Client()

client.on_connect = on_connect
client.on_message = on_message

client.connect("172.20.10.12", port=1883)

client.loop_forever()


