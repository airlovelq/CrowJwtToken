// crowtry.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "crow.h"
#include "jwt-cpp/jwt.h"
#include "jwt-cpp/base.h"
#include "picojson/picojson.h"
#include "crow_jwt_token.h"

crow::response test(crow::json::rvalue &token_value, const crow::request& req, std::string str) {
	std::cout << str << std::endl;
	auto res = crow::response(token_value);
	return res;
}

void test2(crow::json::rvalue &token_value, const crow::request& req, crow::response& res, std::string str) {
    std::cout << str << std::endl;
	res.write(crow::json::dump(token_value));
	res.end();
}

int main()
{
	crow::SimpleApp app;

	CROW_ROUTE(app, "/test/<string>")
		(CROW_JWT_TOKEN_FUNC(test, "tokenkey"));

	CROW_ROUTE(app, "/test2/<string>")
		(CROW_JWT_TOKEN_FUNC(test2, "tokenkey"));

	app.port(18080).run();
    std::cout << "Hello World!\n";
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
