import csi
import time
import struct
from machine import LED, UART

FRAME_HEIGHT = 480
FRAME_WIDTH = 480
FRAME_CX = 240
FRAME_CY = 200

GOAL_MIN_AREA = 50
GOAL_MIN_PIXELS = 50

YELLOW = 1
BLUE = 2

thresholds = [
    (19, 44, -23, -7, 30, 89),  # yellow
    (17, 32, -18, 1, -27, -8),  # blue
]

led = LED("LED_GREEN")
led.on()

clock = time.clock()  # Create a clock object to track the FPS.

cam = csi.CSI()
cam.reset()
cam.pixformat(csi.RGB565)
cam.framesize(csi.VGA)
cam.window((FRAME_WIDTH, FRAME_HEIGHT))
cam.snapshot(time=2000)
# cam.auto_exposure(True)
# cam.auto_gain(True)
# cam.auto_whitebal(True)

cam.auto_exposure(False, exposure_us=8248)
cam.auto_gain(False, gain_db=24.0824)
cam.auto_whitebal(False, rgb_gain_db=(63.56803, 60.206, 62.4312))


# print("Gain:", cam.gain_db(), "Exposure:", cam.exposure_us(), "WB:", cam.rgb_gain_db())
clock = time.clock()
# clock.reset()
uart = UART(3, 115200, timeout_char=100)  # CHECK IF THIS IS CORRECT (baud rate can be 115200)
led.off()

while True:
    clock.tick()
    img = cam.snapshot()

    blobs = img.find_blobs(
        thresholds,
        x_stride=25,
        y_stride=25,
        area_threshold=GOAL_MIN_AREA,
        pixels_threshold=GOAL_MIN_PIXELS,
        merge=False,
        margin=5,
    )

    data = [255, 255, FRAME_CX, FRAME_CY, FRAME_CX, FRAME_CY]

    rawBlobs = [None, None]

    for blob in blobs:
        blobX = blob.cx - FRAME_CX
        blobY = blob.cy - FRAME_CY

        if blob.code == YELLOW:
            if rawBlobs[0] is None:
                rawBlobs[0] = blob
            else:
                if blob.area > (rawBlobs[0]).area:
                    rawBlobs[0] = blob

        if blob.code == BLUE:
            if rawBlobs[1] is None:
                rawBlobs[1] = blob
            else:
                if blob.area > (rawBlobs[1]).area:
                    rawBlobs[1] = blob

    if rawBlobs[0] is not None:
        data[2] = (rawBlobs[0]).cx
        data[3] = (rawBlobs[0]).cy

    if rawBlobs[1] is not None:
        data[4] = (rawBlobs[1]).cx
        data[5] = (rawBlobs[1]).cy

    img.draw_line((FRAME_CX, FRAME_CY, data[2], data[3]))
    img.draw_line((FRAME_CX, FRAME_CY, data[4], data[5]))
    datasend = struct.pack("6H", *data)
    uart.write(datasend)
