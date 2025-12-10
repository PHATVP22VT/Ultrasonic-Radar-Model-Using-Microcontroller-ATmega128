/*
 * RADAR CONTROL - BLUETOOTH HC-05
 * Control: Click buttons on screen to control AVR
 */

import processing.serial.*;

Serial myPort;
String angle = "";
String distance = "";
String data = "";
String noObject;
float pixsDistance;
int iAngle, iDistance;
int index1 = 0;
float maxRadius; 

void setup() {
  size (1200, 700);
  smooth();
  maxRadius = (height * 0.9) / 2; 
  
  // --- KET NOI BLUETOOTH ---
  // Thay COM8 bang cong cua ban
  String portName = "COM8"; 
  try {
    myPort = new Serial(this, portName, 9600); 
    myPort.bufferUntil('.'); 
    println(">>> KET NOI THANH CONG DEN " + portName + " <<<");
  } catch (Exception e) {
    println("!!! LOI KET NOI !!!");
  }
}

void draw() {
  fill(98,245,31);
  noStroke();
  
  // Hieu ung mo dan
  fill(0, 10); 
  rect(0, 0, width, height);
  
  fill(98,245,31); 
  
  drawRadar();
  drawLine();
  drawObject();
  drawText();
  
  // --- VE CAC NUT DIEU KHIEN ---
  drawButtons();
}

// Ham xu ly su kien click chuot
void mousePressed() {
  // Kiem tra nut STOP (Goc tren trai)
  if (mouseX > 20 && mouseX < 120 && mouseY > 20 && mouseY < 60) {
    if (myPort != null) myPort.write('s'); // Gui ky tu 's'
    println("Sent: STOP/START");
  }
  
  // Kiem tra nut MENU
  if (mouseX > 140 && mouseX < 240 && mouseY > 20 && mouseY < 60) {
    if (myPort != null) myPort.write('m'); // Gui ky tu 'm'
    println("Sent: MENU");
  }
  
  // Kiem tra nut SELECT
  if (mouseX > 260 && mouseX < 360 && mouseY > 20 && mouseY < 60) {
    if (myPort != null) myPort.write('e'); // Gui ky tu 'e'
    println("Sent: SELECT");
  }
}

void drawButtons() {
  pushMatrix();
  stroke(255);
  strokeWeight(2);
  textSize(16);
  
  // Ve nut STOP
  if (mousePressed && mouseX > 20 && mouseX < 120 && mouseY > 20 && mouseY < 60) fill(50, 150, 50); // Hieu ung nhan
  else fill(0, 100, 0);
  rect(20, 20, 100, 40, 10); // X, Y, W, H, Radius
  fill(255); text("STOP/RUN", 35, 45);
  
  // Ve nut MENU
  if (mousePressed && mouseX > 140 && mouseX < 240 && mouseY > 20 && mouseY < 60) fill(50, 50, 150);
  else fill(0, 0, 100);
  rect(140, 20, 100, 40, 10);
  fill(255); text("MENU", 165, 45);
  
  // Ve nut SELECT
  if (mousePressed && mouseX > 260 && mouseX < 360 && mouseY > 20 && mouseY < 60) fill(150, 150, 50);
  else fill(100, 100, 0);
  rect(260, 20, 100, 40, 10);
  fill(255); text("SELECT", 280, 45);
  
  popMatrix();
}

void serialEvent (Serial myPort) {
  try {
    data = myPort.readStringUntil('.');
    if (data != null) {
      data = trim(data);
      data = data.substring(0, data.length()-1);
      index1 = data.indexOf(",");
      if (index1 > 0) {
        angle = data.substring(0, index1);
        distance = data.substring(index1+1, data.length());
        iAngle = int(angle);
        iDistance = int(distance);
        if(iAngle > 360) iAngle = 0;
      }
    }
  } catch(Exception e) {}
}

void drawRadar() {
  pushMatrix();
  translate(width/2, height/2); 
  noFill();
  stroke(98,245,31); 
  strokeWeight(2);
  float step = maxRadius / 4.0;
  ellipse(0, 0, step*2, step*2);
  ellipse(0, 0, step*4, step*4);
  ellipse(0, 0, step*6, step*6);
  ellipse(0, 0, step*8, step*8);
  for (int a = 0; a < 360; a += 30) {
    line(0, 0, maxRadius * cos(radians(a)), maxRadius * sin(radians(a)));
  }
  strokeWeight(1);
  for (int a = 0; a < 360; a += 10) {
     float x1 = (maxRadius - 10) * cos(radians(a));
     float y1 = (maxRadius - 10) * sin(radians(a));
     float x2 = maxRadius * cos(radians(a));
     float y2 = maxRadius * sin(radians(a));
     line(x1, y1, x2, y2);
  }
  popMatrix();
}

void drawObject() {
  pushMatrix();
  translate(width/2, height/2);
  strokeWeight(15); 
  stroke(255,10,10); 
  float pixelsPerCm = maxRadius / 40.0;
  pixsDistance = iDistance * pixelsPerCm;
  if(iDistance < 40 && iDistance > 0){
    float objectX = pixsDistance * cos(radians(iAngle));
    float objectY = -pixsDistance * sin(radians(iAngle)); 
    point(objectX, objectY); 
  }
  popMatrix();
}

void drawLine() {
  pushMatrix();
  strokeWeight(5);
  stroke(30,250,60); 
  translate(width/2, height/2);
  float lineX = maxRadius * cos(radians(iAngle));
  float lineY = -maxRadius * sin(radians(iAngle));
  line(0, 0, lineX, lineY);
  popMatrix();
}

void drawText() {
  pushMatrix();
  if(iDistance > 40 || iDistance <= 0) noObject = "Out of Range";
  else noObject = "Object Detected: " + iDistance + " cm";
  fill(98,245,31);
  textSize(20);
  text("10cm", width/2 + (maxRadius/4), height/2 - 5);
  text("20cm", width/2 + (maxRadius/2), height/2 - 5);
  text("30cm", width/2 + (maxRadius*0.75), height/2 - 5);
  text("40cm", width/2 + maxRadius, height/2 - 5);
  textSize(30);
  fill(98,245,31);
  text("Goc: " + iAngle + " °", 50, height - 50);
  if (iDistance < 40 && iDistance > 0) fill(255, 0, 0); 
  else fill(98, 245, 31); 
  text(noObject, 600, height - 50);
  fill(150); textSize(15);
  text("0°", width/2 + maxRadius + 10, height/2 + 5);
  text("180°", width/2 - maxRadius - 30, height/2 + 5);
  text("90°", width/2 - 10, height/2 - maxRadius - 10);
  text("270°", width/2 - 15, height/2 + maxRadius + 20);
  popMatrix();
}
