
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected to MQTT Broker!")
        client.subscribe(TOPIC)
    else:
        print(f"Failed to connect, return code {rc}")
        sys.exit(1)


client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

try:
    client.connect(BROKER, 1883, 60)
    client.loop_forever()  # This will block until interrupted
except KeyboardInterrupt:
     print("Program interrupted by user")
except Exception as e:
      print(f"Error occurred: {str(e)}")
finally:
     GPIO.cleanup()
     client.disconnect()
     print("GPIO cleaned up and MQTT client disconnected")
