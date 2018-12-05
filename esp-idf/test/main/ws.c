#include <string.h>
#include "esp_log.h"
#include "io.h"
#include "inc.h"
#include "mg.h"
#include "data.h"

#define TAG "ws"

static paras_t mparas={
    .ver=8;
    .eq={
        .aa=8;
        .bb=9;
        .gain={
            .value=25,
        },
    },
    .setup={
        .lang=1,
        .cnt=2,
    },
};

static int ws_send_paras(mg_conn_t *nc);
static void ws_write(mg_conn_t *nc, void *data, int len)
{
    mg_send_websocket_frame(nc, WEBSOCKET_OP_BINARY, (const void*)data, (size_t)len);
}


static void ws_proc(mg_conn_t *nc, void *data, int len)
{
    hdr_t *hdr=data;
    
    switch(hdr->dtype) {
        
        case TYPE_GAIN:
        {
            if(hdr->dlen<=sizeof(gain_t)) {
                ESP_LOGE(TAG, "gain length error");
                return;
            }
            
            gain_t *g=hdr->data;
            printf("_____gain.value: %d\n", g->value);
        }
        break;
        
        case TYPE_EQ:
        {
        }
        break;
        
        case TYPE_DYN:
        break;
        
        case TYPE_SETUP:
        break;
        
        case TYPE_PARAS:
        break;
        
        default:
        return;
        break;
        
    }
    
    
    
}

static void ev_handler(mg_conn_t *nc, int ev, void *p)
{
    mg_wsmsg_t *wm=(mg_wsmsg_t*)p;
    switch (ev) {
        case MG_EV_WEBSOCKET_HANDSHAKE_DONE:
        {
            ws_send_paras(nc);
        }
        break;

        case MG_EV_WEBSOCKET_FRAME:
        {
            ws_proc(conn, wm->data, wm->size);
        }
        break;

        case MG_EV_TIMER:
        {
            
        }
        break;

        case MG_EV_CLOSE:
        {
            
        }
        break;
    }
}


void ws_task(void *args)
{
    struct mg_mgr mgr;
    struct mg_connection *nc;

    //printf("Starting web-server on port %s\n", WS_PORT);
    mg_mgr_init(&mgr, NULL);

    nc = mg_bind(&mgr, WS_PORT, ev_handler);
    if (nc == NULL) {
        ESP_LOGE(TAG, "Error setting up listener!\n");
        return;
    }
    mg_set_protocol_http_websocket(nc);

    /* Processing events */
    while (1) {
        mg_mgr_poll(&mgr, 1000);
    }
}


static char tmpbuf[500];
static int do_pack(int type, void *data, int len)
{
    int l;
    hdr_t *hdr=(hdr_t*)tmpbuf;
    
    l = len+sizeof(hdr_t);
    if(l>sizeof(tmpbuf)) {
        ESP_LOGE(TAG, "pack len overflow!");
        return -1;
    }
    
    hdr->magic = MAGIC;
    //hdr->pack[0] = 0;
    hdr->itype = IO_WIFI;
    hdr->dtype = type;
    hdr->dlen = len;
    
    memcpy((void*)hdr->data, data, len);
    return l;
}
int ws_send(mg_conn_t *nc, int type, void *data, int len)
{
    int l;
    
    l = do_pack(data, len);
    ws_write(conn, tmpbuf, l);
    
    return 0;
}


int ws_sendparas(mg_conn_t *nc)
{
    return ws_send(nc, TYPE_PARAS, (void*)&mparas, sizeof(paras_t));
}
