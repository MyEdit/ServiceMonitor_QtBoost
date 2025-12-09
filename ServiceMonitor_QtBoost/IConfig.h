#pragma once

class IConfig
{
public:
	IConfig() = default;
	virtual ~IConfig() = default;

protected:
	virtual void createDefaultConfig() const = 0;
};

