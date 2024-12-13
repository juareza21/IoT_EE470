# import paho.mqtt as mqtt
import paho.mqtt.client as mqtt
import mysql.connector
from datetime import datetime



# Define the MQTT broker and topic
BROKER_URL = "broker.mqtt-dashboard.com"  # Public HiveMQ broker
BROKER_PORT = 1883               # WebSocket port
TOPIC = "testtopic/temp/outTopic/420"   # Topic to subscribe to

node = 'node-1'

HOST = "193.203.166.209"  # Replace with your Hostinger database IP
USER = "u444159065_db_AlexJuarez"    # Replace with your database username
PASSWORD = "Swiz2003" # Replace with your database password
DATABASE = "u444159065_AlexJuarez" # Replace with your database name


# Define callback for connection
def on_connect(client, userdata, flags, rc):
   if rc == 0:
       print("Connected to HiveMQ broker!")
       # Subscribe to the topic once connected
       client.subscribe(TOPIC)
       print(f"Subscribed to topic: {TOPIC}")
   else:
       print(f"Failed to connect, return code {rc}")


# Define callback for receiving messages
def on_message(client, userdata, msg):
   try:
    payload = str(msg.payload.decode())
    print(f"Received payload: {payload} from topic: {msg.topic}")

    try:
      # Establish a database connection
      connection = mysql.connector.connect(
          host=HOST,
          user=USER,
          password=PASSWORD,
          database=DATABASE
      )
      print("Connected to the database")

      # Create a cursor object to interact with the database
      cursor = connection.cursor()

      insert_query = "INSERT INTO sensor_data (node_name, time_received, voltage) VALUES (%s, %s, %s)"
      time_received = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
      values = (node, time_received, payload)
      cursor.execute(insert_query, values)

      # Commit the changes to the database
      connection.commit()
      print("Data inserted successfully")

      # Close the cursor and the database connection
    except mysql.connector.Error as err:
      print(f"Error: {err}")

    finally:
      if connection.is_connected():
        cursor.close()
        connection.close()
        print("Database connection closed")

   except Exception as e:
        print(f"An error occurred: {e}")


# Create an MQTT client instance
client = mqtt.Client()


# Attach callback functions
client.on_connect = on_connect
client.on_message = on_message


# Connect to the HiveMQ broker
print("Connecting to broker...")
client.connect(BROKER_URL, BROKER_PORT, 60)


# Start the MQTT client loop
client.loop_forever()
