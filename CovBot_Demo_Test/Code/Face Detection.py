import winsound
import cv2
from tensorflow.keras.models import load_model
from tensorflow.keras.applications.mobilenet_v2 import preprocess_input
import serial
import numpy as np
bras = serial.Serial("COM7", 9600)
face = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')
eye = cv2.CascadeClassifier('haarcascade_eye_tree_eyeglasses.xml')
mouth= cv2.CascadeClassifier(cv2.data.haarcascades+'haarcascade_mcs_mouth.xml')
font = cv2.FONT_HERSHEY_COMPLEX
model = load_model('mask_recog.h5')
def draw_face_rect(img):
    img=img.copy()
    gray=cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
    faces = face.detectMultiScale(gray, 1.1, 4)
    faces_list=[]
    preds=[]
    for x,y,w,h in faces:
        cv2.rectangle(img,(x,y),(x+w,y+h),(0,255,0),3)
        roi_img=img[y:y+h,x:x+w]
        roi_img=cv2.cvtColor(roi_img,cv2.COLOR_BGR2RGB)
        roi_img= cv2.resize(roi_img, (224, 224))
        roi_img = np.expand_dims(roi_img, axis=0)
        roi_img =  preprocess_input(roi_img)
        faces_list.append(roi_img)
        if len(faces_list) > 0:
            preds = model.predict(faces_list)
        for pred in preds:
            (mask, withoutMask) = pred
        if mask > withoutMask:
            status="Mask"
            color = (0, 255, 0)
        else:
            color = (0,0,255)
            status="No Mask"
        cv2.putText(img, status, (x, y - 20),cv2.FONT_HERSHEY_SIMPLEX, 0.45, color, 2)
        Servo=withoutMask*100
        if(Servo>80):
            frequency = 3000  
            duration = 12  
            winsound.Beep(frequency, duration)
            Xpos=x+w/2
            Ypos=y+h/2
            ch=str(int(Xpos))+'X'+str(int(Ypos))+'\n'
            bras.write(ch.encode())
        else:
            pass
    return img
cap=cv2.VideoCapture(0)
while(cap.isOpened()):
    _,frame=cap.read()
    img=draw_face_rect(frame)
    cv2.imshow('Face detection',img)
    k=cv2.waitKey(1)
    if (k==27 & 0xFF):
        break
cap.release()
cv2.destroyAllWindows()
