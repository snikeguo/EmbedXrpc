#include"StudentService.h"
#include"StudentService.EmbedXrpcSerialization.h"

//auto code gen ! DO NOT modify this file!
//自动代码生成,请不要修改本文件!

ObjectType GetValue_Request_Type(0,nullptr);

Uint8Field GetValue_Response_Field_State("GetValue_Response.State",offsetof(GetValue_Response,State));
Uint16Field GetValue_Response_Field_ReturnValue("GetValue_Response.ReturnValue",offsetof(GetValue_Response,ReturnValue));
IField* GetValue_ResponseDesc []=
{
&GetValue_Response_Field_State,
&GetValue_Response_Field_ReturnValue,
};
ObjectType GetValue_Response_Type(sizeof(GetValue_ResponseDesc)/sizeof(IField*),GetValue_ResponseDesc);

Uint16Field SetValue_Request_Field_v("SetValue_Request.v",offsetof(SetValue_Request,v));
IField* SetValue_RequestDesc []=
{
&SetValue_Request_Field_v,
};
ObjectType SetValue_Request_Type(sizeof(SetValue_RequestDesc)/sizeof(IField*),SetValue_RequestDesc);

MessageMap RequestMessages[]=
{
{"GetValue_Request",GetValue_Request_ServiceId,ReceiveType_Request,&GetValue_Request_Type},
{"SetValue_Request",SetValue_Request_ServiceId,ReceiveType_Request,&SetValue_Request_Type},
};
