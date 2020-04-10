#include "mbed.h"
#include "TCPSocket.h"
#include "MQTTNetwork.h"
#include "MQTTmbed.h"
#include "MQTTClient.h"
#include "ResetReason.h"

#include "stm32f413h_discovery.h"
#include "stm32f413h_discovery_ts.h"
#include "stm32f413h_discovery_lcd.h"

#include <ctime>
#include <string>

TS_StateTypeDef  TS_State = {0};

const uint32_t TIMEOUT_MS = 3000; // Watchdog Timer
#define PressButton 1
DigitalIn  User_Button(PA_0,PullNone);

std::string reset_reason_to_string(const reset_reason_t reason)
{
    switch (reason) {
        case RESET_REASON_POWER_ON:
            return "Power On";
        case RESET_REASON_PIN_RESET:
            return "Hardware Pin";
        case RESET_REASON_SOFTWARE:
            return "Software Reset";
        case RESET_REASON_WATCHDOG:
            return "Watchdog";
        default:
            return "Other Reason";
    }
}

WiFiInterface *wifi;

//Thread
Thread thread;

int CountVal = 0;

void subscribeCallback(MQTT::MessageData& md)
{
    MQTT::Message &message = md.message;
    printf("[Subscribe] Message received: qos %d, retained %d, dup %d, packetid %d\n", message.qos, message.retained, message.dup, message.id);
    printf("[Subscribe] Payload %.*s\n", message.payloadlen, (char*)message.payload);
    ++CountVal;
}

void subThread() {
    char* topic = "/test";

    wifi = WiFiInterface::get_default_instance();
    MQTTNetwork mqttNetwork(wifi);
    MQTT::Client<MQTTNetwork, Countdown> client(mqttNetwork);

    const char* hostname = "192.168.1.102";
    int port = 1883;
    printf("[Subscribe] Connecting to %s:%d\r\n", hostname, port);
    int rc = mqttNetwork.connect(hostname, port);
    if (rc != 0)
    {
        printf("[Subscribe] rc from TCP connect is %d\r\n", rc);
    }
        
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "mbed-sub";
    data.username.cstring = "";
    data.password.cstring = "";
    
    if ((rc = client.connect(data)) != 0)
    {
        printf("[Subscribe] rc from MQTT connect is %d\r\n", rc);
    }
    else
    {
        printf("[Subscribe] Client Connected.\r\n");
    }
        

    if ((rc = client.subscribe(topic, MQTT::QOS0, subscribeCallback)) != 0)
    {
        printf("[Subscribe] rc from MQTT subscribe is %d\r\n", rc);
    }
    else
    {
        printf("[Subscribe] Client subscribed.\r\n");
    }
        

    MQTT::Message message;
    message.qos = MQTT::QOS0;
    message.retained = false;
    message.dup = false;

    while(1)
        client.yield(1000);

    printf("[Subscribe] Finishing with %d messages received\n", CountVal);
    printf("[Subscribe] Done\n\n");

}


int main()
{
    int count = 0;
    const reset_reason_t reason = ResetReason::get();
    printf("Last system reset reason: %s\r\n", reset_reason_to_string(reason).c_str());
    Watchdog &watchdog = Watchdog::get_instance(); // Watchdog setup
    

    char* topic_pub = "/TempHumid json";

    printf("[Publish] WiFi MQTT example\n");

    #ifdef MBED_MAJOR_VERSION
        printf("Mbed OS version %d.%d.%d\n\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);
    #endif

    wifi = WiFiInterface::get_default_instance();
    if (!wifi) 
    {
        printf("[Publish] ERROR: No WiFiInterface found.\n");
        return -1;
    }

    printf("\n[Publish] Connecting to %s...\n", "Mintpatts_2.4G");
    int ret = wifi->connect("Mintpatts_2.4G", "", NSAPI_SECURITY_NONE);
    if (ret != 0) 
    {
        printf("\n[Publish] Connection error: %d\n", ret);
        return -1;
    }

    uint16_t x1, y1;


    BSP_LCD_Init();

    /* Touchscreen initialization */
    if (BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize()) == TS_ERROR) {
        printf("BSP_TS_Init error\n");
    }

    /* Clear the LCD */
    BSP_LCD_Clear(LCD_COLOR_BLACK);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
    BSP_LCD_DisplayStringAt(0, 80,(uint8_t *)"WATERING PLANTS MACHINE", CENTER_MODE);


    
    

    printf("Success\n\n");
    printf("MAC: %s\n", wifi->get_mac_address());
    printf("IP: %s\n", wifi->get_ip_address());
    printf("Netmask: %s\n", wifi->get_netmask());
    printf("Gateway: %s\n", wifi->get_gateway());
    printf("RSSI: %d\n\n", wifi->get_rssi());
    
    MQTTNetwork mqttNetwork(wifi);

    MQTT::Client<MQTTNetwork, Countdown> client(mqttNetwork);

    const char* hostname = "192.168.1.102";
    int port = 1883;
    printf("Connecting to %s:%d\r\n", hostname, port);
    int rc = mqttNetwork.connect(hostname, port);
    if (rc != 0)
    {
        printf("[Publish] rc from TCP connect is %d\r\n", rc);
    }
        
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "mbed-sample";
    data.username.cstring = "";
    data.password.cstring = "";
    
    if ((rc = client.connect(data)) != 0)
    {
        printf("[Publish] rc from MQTT connect is %d\r\n", rc);
    }
    else
    {
        printf("[Publish] Client Connected.\r\n");
    }

    thread.start(subThread);

    // Random
    srand((unsigned) time(0));

    int randomtemp, randomhumi;

    MQTT::Message message;
    message.qos = MQTT::QOS0;
    message.retained = false;
    message.dup = false;    

    while(true) {
        // QoS 
        char buf[100];

        randomtemp = (rand() %  (99 - 10 + 1));
        randomhumi= (rand() %  (99 - 10 + 1));
        string payloadString1 = "Temperature:" + to_string(randomtemp);
        string payloadString2 = "Humidity:" + to_string(randomhumi);
        string payloadBuffer = "{\"temp\":\"" + to_string(randomtemp) + "\",\"humi\":\"" + to_string(randomhumi) + "\"}";

        sprintf(buf, payloadBuffer.c_str());
 
        printf("Client sent->%s \n",buf);
       
        message.payload = buf;
        message.payloadlen = strlen(buf);
        rc = client.publish(topic_pub, message);
        wait(10);
    }

    mqttNetwork.disconnect();

    if ((rc = client.disconnect()) != 0)
    {
        printf("[Publish] rc from disconnect was %d\r\n", rc);
        printf("[Publish] Client Disconnected.\r\n");
    }

    wifi->disconnect();

    printf("\n[Publish] Done\n");
}