# CrowJwtToken

基于Crow框架的使用jwt token的模板

#### 使用说明

1.  需要有Crow，https://github.com/ipkn/crow.git
2.  需要有jwt-cpp，https://github.com/Thalhammer/jwt-cpp.git
3.  包含工程中的crow_jwt_token.h头文件，使用CROW_JWT_TOKEN_FUNC宏来注册函数至Crow，CROW_JWT_TOKEN_FUNC参数第一项为函数，第二项为token的key
4.  token格式："Authorization":"Bearer eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJ1c2VyX2lkIjoiMTQ2YmJhYTctZGFiMy00ZjE1LTk3MTctN2Q1YzQ5YWY3YWQ4IiwidXNlcl90eXBlIjoxLCJleHAiOjE1ODYzNjgzNjR9.yBByK-A4ai8H4QxetTKGxG468HzMrKg9JhXEBum2Xds"
5.  函数前两项形参必须为crow::json::rvalue&, const crow::request& ，第一项为解析出的token内容
