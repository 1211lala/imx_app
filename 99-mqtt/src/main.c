#include "main.h"
#include "datafile.h"
#include "MQTTClient.h" //包含 MQTT 客户端库头文件
#include "cJSON.h"

#define USER_MQTT_ID "mqttclient"
#define USER_MQTT_URL "mqtt.eclipseprojects.io"
#define USER_TOPIC_WILL "liuao/will"
#define USER_TOPIC_PUB_1 "liuao/imx6ull/soc/temp"
#define USER_TOPIC_SUB_1 "liuao/environment/O2"
#define USER_TOPIC_SUB_2 "liuao/environment/CO2"

void connlost(void *context, char *cause)
{
    printf("\n服务器断开:%s\n", cause);
}
int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    if (0 == strcmp(topicName, USER_TOPIC_SUB_1))
    {
        printf("topic: %s\n", topicName);
        printf("message: <%d>%s\n", message->payloadlen, (char *)message->payload);
    }

    /* 释放占用的内存空间 */
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}
int main(int argc, char **argv)
{
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_willOptions will_opts = MQTTClient_willOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    int ret;
    /* 创建mqtt客户端对象 */
    if (MQTTCLIENT_SUCCESS != (ret = MQTTClient_create(&client, USER_MQTT_URL, USER_MQTT_ID, MQTTCLIENT_PERSISTENCE_NONE, NULL)))
    {
        printf("Failed to create client, return code %d\n", ret);
        goto client_create_fail;
    }
    /* 设置回调 */
    if (MQTTCLIENT_SUCCESS != (ret = MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, NULL)))
    {
        printf("Failed to set callbacks, return code %d\n", ret);
        return -1;
    }
    /* 连接MQTT服务器 */
    will_opts.topicName = USER_TOPIC_WILL; // 遗嘱主题
    will_opts.message = "offline";         // 遗嘱消息
    will_opts.retained = 1;                // 保留消息
    will_opts.qos = 1;                     // QoS0

    conn_opts.will = &will_opts;
    conn_opts.keepAliveInterval = 20; // 心跳包间隔时间
    conn_opts.cleansession = 0;       // cleanSession标志
    // conn_opts.username = USERNAME;    // 用户名
    // conn_opts.password = PASSWORD;    // 密码
    if (MQTTCLIENT_SUCCESS != (ret = MQTTClient_connect(client, &conn_opts)))
    {
        printf("Failed to connect, return code %d\n", ret);
        return -1;
    }
    printf("MQTT服务器连接成功!\n");
    // /* 发布上线消息 */
    pubmsg.payload = "online";            // 消息的内容
    pubmsg.payloadlen = strlen("online"); // 内容的长度
    pubmsg.qos = 1;                       // QoS等级
    pubmsg.retained = 1;                  // 保留消息
    if (MQTTCLIENT_SUCCESS != (ret = MQTTClient_publishMessage(client, USER_TOPIC_WILL, &pubmsg, NULL)))
    {
        printf("Failed to publish message, return code %d\n", ret);
        return -1;
    }
    /* 订阅主题 */
    if (MQTTCLIENT_SUCCESS != (ret = MQTTClient_subscribe(client, USER_TOPIC_SUB_1, 0)))
    {
        printf("Failed to subscribe message, return code %d\n", ret);
        return -1;
    }
    /* 订阅主题  */
    if (MQTTCLIENT_SUCCESS != (ret = MQTTClient_subscribe(client, USER_TOPIC_SUB_2, 0)))
    {
        printf("Failed to subscribe message, return code %d\n", ret);
        return -1;
    }
    while (1)
    {
        u_int8_t temp_str[20];
        int fd = open("/sys/class/thermal/thermal_zone0/temp", O_RDONLY);
        ret = read(fd, temp_str, sizeof(temp_str)); // 读取temp属性文件即可获取温度
        temp_str[ret] = '\0';
        close(fd);

        cJSON *root = cJSON_CreateObject();
        cJSON_AddNumberToObject(root, "soctemp", atoi(temp_str)/1000);
        char *Json_string = cJSON_Print(root);

        pubmsg.payload = Json_string;
        pubmsg.payloadlen = strlen(Json_string);
        pubmsg.qos = 0;
        pubmsg.retained = 0;
        if (MQTTCLIENT_SUCCESS != (ret = MQTTClient_publishMessage(client, USER_TOPIC_PUB_1, &pubmsg, NULL)))
        {
            printf("Failed to subscribe, return code %d\n", ret);
            return -1;
        }
        cJSON_free(Json_string);
        cJSON_Delete(root);
        sleep(5);
    }

    if (MQTTCLIENT_SUCCESS != (ret = MQTTClient_disconnect(client, 10000)))
    {
        printf("Failed to disconnect, return code %d\n", ret);
        ret = EXIT_FAILURE;
    }
    MQTTClient_destroy(&client);
client_create_fail:
    return ret;
}