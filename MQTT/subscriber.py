import paho.mqtt.client as mqtt
import RPi.GPIO as GPIO
import time

BROKER = "broker.emqx.io"
TOPIC = "SIT210/wave4"
LED_PIN = 19

GPIO.setmode(GPIO.BCM)
GPIO.setup(LED_PIN, GPIO.OUT)

def on_message(client, userdata, msg):
    message = msg.payload.decode("utf-8")
    print("Received:", message)

    if message == "Dinith":
        for _ in range(3):  # Blink LED 3 times
            GPIO.output(LED_PIN, GPIO.HIGH)
            time.sleep(0.5)
            GPIO.output(LED_PIN, GPIO.LOW)
            time.sleep(0.5)
            
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
