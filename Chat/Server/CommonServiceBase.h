#pragma once

#include <muduo/net/TcpServer.h>
#include <muduo/base/Logging.h>
#include <boost/bind.hpp>
#include <unordered_map>
#include <functional>
#include "public.h"
#include "json.hpp"
using json = nlohmann::json;

/*
通用的服务处理抽象基类，保留统一接口，可以同时扩展单机服务器和集群服务器的派生类实现
*/
class CommonServiceBase
{	
public:
	CommonServiceBase()
	{
		// 绑定用户事件处理接口函数
		_handlerMap.insert({ MSG_LOGIN, bind(&CommonServiceBase::login, this, _1, _2, _3) });
		_handlerMap.insert({ MSG_REG, bind(&CommonServiceBase::reg, this, _1, _2, _3) });
		_handlerMap.insert({ MSG_ADD_FRIEND, bind(&CommonServiceBase::addFriend, this, _1, _2, _3) });
		_handlerMap.insert({ MSG_ADD_GROUP, bind(&CommonServiceBase::addGroup, this, _1, _2, _3) });
		_handlerMap.insert({ MSG_ONE_CHAT, bind(&CommonServiceBase::oneChat, this, _1, _2, _3) });
		_handlerMap.insert({ MSG_GROUP_CHAT, bind(&CommonServiceBase::groupChat, this, _1, _2, _3) });

		_handlerMap.insert({ MSG_ADD_GROUPUSER, bind(&CommonServiceBase::addGroupUser, this, _1, _2, _3) });
		_handlerMap.insert({ MSG_ONLINEUSER_FILE, bind(&CommonServiceBase::onlineUserFile, this, _1, _2, _3) });
		_handlerMap.insert({ MSG_GROUP_FILE, bind(&CommonServiceBase::groupFile, this, _1, _2, _3) });
		_handlerMap.insert({ MSG_FREEND_FILE, bind(&CommonServiceBase::friendFile, this, _1, _2, _3) });
	}

	// login service
	virtual void login(const muduo::net::TcpConnectionPtr &con, json &js, muduo::Timestamp time) = 0;

	// register service
	virtual void reg(const muduo::net::TcpConnectionPtr &con,
		json &js, muduo::Timestamp time) = 0;

	// add friend service
	virtual void addFriend(const muduo::net::TcpConnectionPtr &con,
		json &js, muduo::Timestamp time) = 0;

	// add group service
	virtual void addGroup(const muduo::net::TcpConnectionPtr &con,
		json &js, muduo::Timestamp time) = 0;

	// one to one chat service
	virtual void oneChat(const muduo::net::TcpConnectionPtr &con,
		json &js, muduo::Timestamp time) = 0;

	// group chat service
	virtual void groupChat(const muduo::net::TcpConnectionPtr &con,
		json &js, muduo::Timestamp time) = 0;

	// get onlineUser File
	virtual void onlineUserFile(const muduo::net::TcpConnectionPtr& con,
		json& js, muduo::Timestamp time) = 0;

	// get group File
	virtual void groupFile(const muduo::net::TcpConnectionPtr& con,
		json& js, muduo::Timestamp time) = 0;

	// get friend File
	virtual void friendFile(const muduo::net::TcpConnectionPtr& con,
		json& js, muduo::Timestamp time) = 0;

	//add groupUser service
	virtual void addGroupUser(const muduo::net::TcpConnectionPtr& con,
		json& js, muduo::Timestamp time) = 0;
	

	using Handler = std::function<void(const muduo::net::TcpConnectionPtr&, json&, muduo::Timestamp)>;
	std::unordered_map<int, Handler> handler()const { return _handlerMap; }
protected:
	std::unordered_map<int, Handler> _handlerMap;
};

//handler 处理