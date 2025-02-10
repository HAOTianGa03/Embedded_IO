/*
 * BufferHandle.cpp
 *  IO_HANDLER FOR STM32
 *  NOTE: This library is only for STM32
 *  Created on: Feb 9, 2025
 *      Author: GAOTIANHAO
 */

#include "BufferHandler.h"

void BufferHandler::_startDMA()
{
	HAL_UARTEx_ReceiveToIdle_DMA(_huart, _buffer[_receiveBufferIndex],
			2 * _buffer_len);//要么触发空闲中断，要么触发传输过半中断
}

void BufferHandler::begin(const uint16_t buffer_num,const uint8_t buffer_len,UART_HandleTypeDef *huart){

	if (!huart) {
		return;
	}
	if (buffer_num == 0 || buffer_len == 0) {
		return;
	}
	_buffer_num=buffer_num;
	_buffer_len=buffer_len;
	_huart = huart;
	_buffer = new uint8_t*[buffer_num];
	for (int i = 0; i < buffer_num; i++) {
		_buffer[i] = new uint8_t[buffer_len];
	}
	_startDMA();
}


void BufferHandler::callbackHandler(UART_HandleTypeDef *huart, uint16_t Size) {
	if (huart == _huart)
	{
		// 获取帧长并存入缓冲区长度变量
		_data_len = Size;
		// 将数据存入缓冲区
		_new_data_available = true;
		_receiveBufferIndex=(_receiveBufferIndex+1)%_buffer_num;
		_startDMA();
	}
}

void BufferHandler::attachEvent(BufferCallback cb, ProcessEvent event) {
	_cb[event] = cb;
}

void BufferHandler::stopEvent(BufferCallback cb, ProcessEvent event) {
	_cb[event] = NULL;
}

void BufferHandler::processHandler() {
	if (_new_data_available) {
		_new_data_available = false;
		_processBufferIndex = (_processBufferIndex + 1) % _buffer_num;
		if (_cb[PINGPONG_BUFFER])//处理乒乓缓冲区回调事件
		{
			_cb[PINGPONG_BUFFER](_buffer[_processBufferIndex],_data_len);//将参数传递给回调函数
		}
	}
}

