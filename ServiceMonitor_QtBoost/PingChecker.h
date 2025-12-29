#pragma once

#include <QSharedPointer>

#include <iostream>
#include <vector>
#include <chrono>

#include <boost/asio.hpp>
#include <boost/asio/executor_work_guard.hpp>
#include <boost/array.hpp>

#include "AbstractChecker.h"
#include "PingService.h"
#include "Logger.h"
#include "icmp_header.hpp"
#include "ipv4_header.hpp"


namespace asio = boost::asio;
using icmp = asio::ip::icmp;

class PingChecker : public AbstractChecker
{
	struct ServiceConnection
	{
		QSharedPointer<PingService> service;
		icmp::socket socket;
		icmp::endpoint endpoint;
		std::chrono::steady_clock::time_point startTime;
		std::chrono::steady_clock::time_point endTime;
	};

public:
	PingChecker();
	~PingChecker();

	virtual void start() override;
	virtual void stop() override;
	virtual void check() override;

	virtual void addService(const QSharedPointer<AbstractService>& service) override;
	virtual void removeService(const QString& serviceName) override;

private:
	asio::io_context ioContext;
	asio::executor_work_guard<asio::io_context::executor_type> ioWorkGuard; // fake work
	std::thread ioThread;
	std::atomic<bool> running{ false };
	std::mutex servicesMutex;
	QVector<QSharedPointer<ServiceConnection>> serviceConnections;

	void handle_icmp_receive(
		QSharedPointer<PingChecker::ServiceConnection> serviceConnection,
		std::shared_ptr<boost::asio::streambuf> reply_buffer,
		std::shared_ptr<boost::asio::steady_timer> timer,
		uint16_t identifier,
		uint16_t sequence,
		const boost::system::error_code& ec,
		std::size_t length);

	void closeSocket(icmp::socket& socket);
};

