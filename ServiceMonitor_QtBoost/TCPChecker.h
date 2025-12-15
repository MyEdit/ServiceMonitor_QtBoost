#pragma once

#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif

#include <QSharedPointer>

#include <iostream>
#include <vector>
#include <chrono>

#include <boost/asio.hpp>
#include <boost/asio/executor_work_guard.hpp>
#include <boost/array.hpp>

#include "TcpService.h"
#include "Logger.h"

namespace asio = boost::asio;
using tcp = asio::ip::tcp;
using icmp = asio::ip::icmp;

class TCPChecker
{
	struct ServiceConnection
	{
		tcp::socket socket;
		tcp::endpoint endpoint;
		std::chrono::steady_clock::time_point startTime;
		std::chrono::steady_clock::time_point endTime;
	};

public:
	TCPChecker(QVector<QSharedPointer<TcpService>>& services)
		: ioWorkGuard(asio::make_work_guard(ioContext)) // Сразу кидаем в контекст фейковую работу
	{
		ioThread = std::thread([this]()
			{
				try
				{
					Logger::instance()->info("IO TCP поток запущен");
					ioContext.run();
					Logger::instance()->info("IO TCP поток остановлен");
				}
				catch (const std::exception& e)
				{
					Logger::instance()->error(QString("IO TCP поток выбросил ошибку: %1").arg(e.what()).toStdString());
				}
			});

		for (auto& service : services)
		{
			// TODO: Наверное стоит в try обернуть, и мб где то проверять адрес числовой (1.1.1.1) или нет (google.com)
			QSharedPointer<ServiceConnection> serviceConnection = QSharedPointer<ServiceConnection>::create
				(
					tcp::socket(ioContext),
					tcp::endpoint(asio::ip::make_address(service->getHost().toString().toStdString()), service->getPort()),
					std::chrono::steady_clock::now(),
					std::chrono::steady_clock::now()
				);

			this->services.push_back(serviceConnection);
		}

		test();
	}

	void test()
	{
		for (auto& serviceConnection : this->services)
		{
			serviceConnection->socket.async_connect(serviceConnection->endpoint,
				[this, &serviceConnection](const boost::system::error_code& ec)
				{
					bool success = !ec;

					if (success)
					{
						Logger::instance()->debug("Connect");
					}
					else
					{
						Logger::instance()->debug("Not connect");
					}

					if (serviceConnection->socket.is_open())
					{
						serviceConnection->socket.close();
					}
				});
		}
	}

private:
	boost::system::error_code error;
	asio::io_context ioContext;
	asio::executor_work_guard<asio::io_context::executor_type> ioWorkGuard; // fake work
	std::thread ioThread;
	QVector<QSharedPointer<ServiceConnection>> services;
};

