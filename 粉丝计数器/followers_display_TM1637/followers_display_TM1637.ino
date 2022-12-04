 /*
D5  -->  CLK
D6  -->  DIO
VV  -->  VCC
GND -->  GND
*/
#include <Arduino.h>
#include <TM1637Display.h>
//在这里下载TM1637Display：https://github.com/avishorp/TM1637
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
HTTPClient http;
WiFiClient client;
    
const char *ssid = "ssid"; //这里填你家中的wifi名称
const char *password = "password";//这里填你家中的wifi密码

String UID  = "1715792681";  //改成自己的UID
String followerUrl = "http://api.bilibili.com/x/relation/stat?vmid=" + UID;   // 获取粉丝数网址
String viewAndLikesUrl = "http://api.bilibili.com/x/space/upstat?mid=" + UID; // 获取播放数、点赞数网址(未使用)
int follower; //存储粉丝数量变量

#define CLK 14 //显示器CLK引脚接D5
#define DIO 12 //显示器DIO引脚接D6

TM1637Display display(CLK, DIO);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); 
  WiFi.begin(ssid, password); //连接wifi
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("wifi已连接");
}
void loop() {
  // put your main code here, to run repeatedly:
  int httpCode = http.GET();
  http.begin(client,followerUrl); //连接信息网站地址
  String resBuff = http.getString(); //读取信息
  int len = resBuff.length(); //判断读取信息字符串长度
  if(httpCode==200){
    follower=0;//分析网站信息计算出粉丝数量
    for(int i = len-3;i>1;i--){
      if(resBuff[i]>='0'&&resBuff[i]<='9'){
        follower+=(resBuff[i]-'0')*pow(10,len-i-3);
      }
      else break;
    }
    Serial.println(follower);//打印粉丝数量
    disp(follower);//显示粉丝数量
  }
  delay(1000);
}
void disp(int x){//显示数字函数
  display.setBrightness(0x0f);//亮度调节
  uint8_t data[4];
  data[0] = display.encodeDigit(x/1000%10);
  data[1] = display.encodeDigit(x/100%10);
  data[2] = display.encodeDigit(x/10%10);
  data[3] = display.encodeDigit(x%10);
  if(x<1000) data[0]=0;
  if(x<100) data[1]=0;
  if(x<10) data[2]=0;
  display.setSegments(data);//显示四位数
}
