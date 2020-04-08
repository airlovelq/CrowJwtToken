#ifndef _CROW_JWT_TOKEN_H
#define _CROW_JWT_TOKEN_H

#include <string>
#include "crow.h"
#include "jwt-cpp/jwt.h"
#include "jwt-cpp/base.h"
#include "picojson/picojson.h"

// "Authorization":"Bearer eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJ1c2VyX2lkIjoiMTQ2YmJhYTctZGFiMy00ZjE1LTk3MTctN2Q1YzQ5YWY3YWQ4IiwidXNlcl90eXBlIjoxLCJleHAiOjE1ODYzNjgzNjR9.yBByK-A4ai8H4QxetTKGxG468HzMrKg9JhXEBum2Xds"
bool verify_and_parse_token(const std::string& auth, const std::string& key, std::string& token_content )
{
	size_t kg_pos = auth.find(' ');
	if (kg_pos == auth.npos)
	{
		return false;
	}
	auto bearer = auth.substr(0, kg_pos);
	if (bearer.compare("Bearer") != 0)
	{
		return false;
	}
	auto token = auth.substr(kg_pos + 1);
	auto decoded = jwt::decode(token);
	auto verifier = jwt::verify().allow_algorithm(jwt::algorithm::hs256{ key });
	try 
	{
		verifier.verify(decoded);
	}
	catch (...)
	{
		return false;
	}
	token_content = decoded.get_payload();
	return true;
}

template<typename FUNC> struct crow_jwt_token_function;
template<typename Ret, typename...Args>
struct crow_jwt_token_function<Ret(crow::json::rvalue&, const crow::request&, Args...)>
{
public:
	static std::function<Ret(const crow::request&, Args...)> generate(std::function<Ret(crow::json::rvalue&, const crow::request&, Args...)> f, std::string token_key)
	{
		return [f, token_key](const crow::request &req, Args...args) {
			auto auth = req.get_header_value("Authorization");
			std::string token_content;
			if (verify_and_parse_token(auth, token_key, token_content))
			{
				auto token_map = crow::json::load(token_content);
				return f(token_map, req, args...);
			}
			else
			{
				return crow::response("token error!");
			}
		};
	}
};

template<typename Ret, typename...Args>
struct crow_jwt_token_function<Ret(crow::json::rvalue&, const crow::request&, crow::response&, Args...)>
{
public:
	static std::function<Ret(const crow::request&, crow::response&, Args...)> generate(std::function<Ret(crow::json::rvalue&, const crow::request&, crow::response&, Args...)> f, std::string token_key)
	{
		return [f, token_key](const crow::request &req, crow::response& res, Args...args) {
			auto auth = req.get_header_value("Authorization");
			std::string token_content;
			if (verify_and_parse_token(auth, token_key, token_content))
			{
				auto token_map = crow::json::load(token_content);
				return f(token_map, req, res, args...);
			}
			else
			{
				res.write("token error!");
				res.end();
			}
		};
	}
};

#define CROW_JWT_TOKEN_FUNC(func, token_key) crow_jwt_token_function<decltype(func)>::generate(func, token_key)

#endif