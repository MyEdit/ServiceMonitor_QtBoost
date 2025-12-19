#pragma once

#include <QSharedPointer>

#include <iostream>
#include <vector>
#include <chrono>

#include <boost/asio.hpp>
#include <boost/asio/executor_work_guard.hpp>
#include <boost/array.hpp>

#include "AbstractChecker.h"
#include "TcpService.h"
#include "Logger.h"

namespace asio = boost::asio;
using tcp = asio::ip::tcp;
using icmp = asio::ip::icmp;

class TCPChecker : public AbstractChecker
{
	struct ServiceConnection
	{
		QSharedPointer<TcpService> service;
		tcp::socket socket;
		tcp::endpoint endpoint;
		std::chrono::steady_clock::time_point startTime;
		std::chrono::steady_clock::time_point endTime;
	};

public:
	TCPChecker();
	~TCPChecker();

	virtual void start() override;
	virtual void stop() override;
	virtual void check() override;

	virtual void addService(const QSharedPointer<AbstractService>& service) override;
	virtual void removeService(const QString& serviceName) override;

private:
	boost::system::error_code error;
	asio::io_context ioContext;
	asio::executor_work_guard<asio::io_context::executor_type> ioWorkGuard; // fake work
	std::thread ioThread;
	std::atomic<bool> running{ false };
	std::atomic<unsigned> intervalMs{ 5000 };
	std::mutex servicesMutex;
	QVector<QSharedPointer<ServiceConnection>> serviceConnections;
};

