#ifndef MYNET_MTNET_CALLBACKS_H
#define MYNET_MTNET_CALLBACKS_H

#include <functional>
#include <memory>

#include "Timestamp.h"

namespace mtnet
{
	class TcpConnection;
	class Buffer;
	typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;

	typedef std::function<void()> TimerCallback;
	typedef std::function<void (const TcpConnectionPtr&)> ConnectionCallback;
	typedef std::function<void (const TcpConnectionPtr&,Buffer* buf,Timestamp)> MessageCallback;
	typedef std::function<void (TcpConnectionPtr&)> WriteCompleteCallback;
	typedef std::function<void (const TcpConnectionPtr&)> CloseCallback;
}

#endif //MYNET_MTNET_CALLBACKS_H