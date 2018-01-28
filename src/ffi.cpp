
#include <stdio.h>
#include "uWS.h"

#define CCALL extern "C"

CCALL void *uws_new_hub() {
    return (void *)new uWS::Hub();
}

CCALL void uws_on_message(void *hub, void (*callback)(void *server, char *msg, size_t len, int op)) {
    uWS::Hub *h = (uWS::Hub *)hub;
    h->onMessage([callback](uWS::WebSocket<uWS::SERVER> *ws, char *message, size_t length, uWS::OpCode opCode) {
        callback((void *)ws, message, length, opCode);
    });
}

CCALL void uws_server_send(void *server, char *msg, size_t len, int op) {
    uWS::WebSocket<uWS::SERVER> *ws = (uWS::WebSocket<uWS::SERVER> *)server;
    ws->send(msg, len, (uWS::OpCode) op);
}
CCALL bool uws_hub_listen(void *hub, int port) {
    uWS::Hub *h = (uWS::Hub *)hub;
    return h->listen(port);
}
CCALL void uws_hub_step(void *hub) {
    uWS::Hub *h = (uWS::Hub *)hub;
    h->run();
}

// example code
#if 0

void uws_msg_handler(void *server, char *msg, size_t len, int op) {
    //printf("connect length=%z opCode=%d\n", len, op);
    for (int i=0; i<len; i++)
        printf("%c", msg[i]);
    printf("\n");
    uws_server_send(server, msg, len, op);
    //uws_server_send(server, "LOL", 3, op);
    //uws_server_send(server, "LOL", 3, op);
    //uws_server_send(server, "LOL", 3, op);
}

int main() {
    void *hub = uws_new_hub();
    //uWS::Hub *h = (uWS::Hub *)hub;

    uws_on_message(hub, uws_msg_handler);
/*
    h->onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t length, size_t remainingBytes) {
        printf("onhttpreq: data=%s length=%d\n", data, length);
        res->end("test", 4);
    });
*/
    if (uws_hub_listen(hub, 3000)) {
        while (1) {
            //printf(".");
            uws_hub_step(hub);
            usleep(1000); // save cpu
        }
    }
    printf("End\n");
    printf("...\n");
}
#endif