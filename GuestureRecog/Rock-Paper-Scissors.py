import cv2 
import mediapipe as mp
import numpy as np
import time
import threading 
import requests

STREAM_URL = "http://192.168.4.63:81/stream"  # Replace with your ESP32-CAM stream URL

MODEL_PATH = "GuestureRecog/gesture_recognizer.task"

latest_gesture = "Show your hand!"
latest_score = 0.0
lock = threading.Lock()

def on_result(result, output_image, timestamp_ms):
    global latest_gesture, latest_score

    gesture = "No hand detected"
    score = 0.0

    if result.gesture and result.gestures[0]:
        category = result.gestures[0][0]
        name = category.category_name
        score = category.score

        mapping = {
            "Closed_Fist": "Rock",
            "Open_Hand": "Paper",
            "Victory": "Scissors"
        }

        gesture = mapping.get(name, "Unknown gesture")


        with lock:
            latest_gesture = gesture
            latest_score = score

BaseOptions = mp.tasks.BaseOptions
GestureRecognizer = mp.tasks.vision.GestureRecognizer
Options = mp.tasks.vision.GestureRecognizerOptions
RunningMode = mp.tasks.vision.RunningMode

options = Options(
    base_options=BaseOptions(model_asset_path=MODEL_PATH),
    running_mode=RunningMode.LIVE_STREAM,
    num_hands=1,
    result_callback=on_result
)

def main():

    with GestureRecognizer.create_from_options(options) as recognizer:

        print("Connection to ESP32...")

        with requests.get(
            STREAM_URL, 
            stream=True,
            timeout=(5,15)
        ) as response:

            response.raise_for_status()
            print("Connected to Camera")

            buffer= bytearray()
            last_timestamp = 0

            for chunk in response.iter_content(chunk_size=1024):

                if not chunk:
                    continue

            buffer.extend(chunk)

        while True:

            start = buffer.find(b'\xff\xd8')  # JPEG start

            if start == -1:
                if len(buffer) > 2:
                    del buffer[-2:]  # Keep last two bytes in case the start marker is split
                break

            end = buffer.find(b'\xff\xd9', start + 2)  # JPEG end

            if end == -1:
                if start > 0:
                    del buffer[:start]  # Remove data before the start marker
                break

            jpeg = bytes(buffer[start:end + 2])
            del buffer[:end + 2]  # Remove the processed JPEG from the buffer

            frame = cv2.imdecode(
                np.frombuffer(jpeg, dtype=np.uint8), 
                cv2.IMREAD_COLOR

            )

            if frame is None:
                continue

            rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

        image = mp.Image(
            
            image_format=mp.ImageFormat.SRGB,
            data=rgb
        )

        timestamp = time.monotonic_ns() // 1_000_000  # Convert to milliseconds
        timestamp = max(timestamp, last_timestamp + 1)  # Ensure timestamp is strictly increasing
        last_timestamp = timestamp

        recognizer.recognize_async(image, timestamp)

        with lock: 
            gesture = latest_gesture
            score = latest_score

        cv2.putText(
            frame,
            gesture,
            (10,35),
            cv2.FONT_HERSHEY_SIMPLEX,
            1,
            (0,255,0),
            2,
        )

        cv2.putText(
            frame,
            f"confidence: {score:.0%}",
            (10,70),
            cv2.FONT_HERSHEY_SIMPLEX,
            0.6,
            (255,255,255),
            2
        )

        cv2.imshow("Rock Paper Scissors Recognition", frame)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            return

    if __name__ == "__main__":
        try:
            main()
        except Exception as error:
            print(f"An error occurred: {error}")
        finally:
            cv2.destroyAllWindows()