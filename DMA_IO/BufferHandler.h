/*
 * BufferHandle.h
 *  IO_HANDLER FOR STM32
 *  NOTE: This library is only for STM32
 *  Created on: Feb 9, 2025
 *      Author: GAOTIANHAO
 */

#ifndef INC_BUFFERHANDLER_H_
#define INC_BUFFERHANDLER_H_

#include"usart.h"

typedef enum {
	PINGPONG_BUFFER = 0,
	number_of_event,
}ProcessEvent;

typedef void (*BufferCallback)(uint8_t *data, uint16_t len);

class BufferHandler {
public:
	void begin(uint16_t buffer_num,uint8_t buffer_len,UART_HandleTypeDef *huart);
    void callbackHandler(UART_HandleTypeDef *huart,uint16_t Size);
    void attachEvent(BufferCallback cb,ProcessEvent event);
    void stopEvent(BufferCallback cb,ProcessEvent event);
    void processHandler();

private:
	UART_HandleTypeDef *_huart;
    uint8_t **_buffer;
    uint8_t _buffer_num;
    uint8_t _buffer_len;
    int _data_len;//上一次接收缓冲区帧大小
    bool _new_data_available=0;//用于通知主进程处理数据
    uint8_t _receiveBufferIndex=0;//当前的读缓冲区索引
    int8_t _processBufferIndex=-1;//当前的写缓冲区索引,初始化为-1
    BufferCallback _cb[number_of_event];
    void _startDMA();
};



#endif /* INC_BUFFERHANDLER_H_ */
