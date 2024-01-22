#include "global.h"
#include "WebSocketPlugin.h"
#include "SocketHandler.h"

#include "RageLog.h"
#include "ScreenManager.h"
#include "Screen.h"

#include <iostream>
#include <fstream>
using namespace std;

REGISTER_PLUGIN(WebSocketPlugin, "0.0.1", "Electromuis")

AutoScreenMessage(SM_SongChanged);

unique_ptr<WebSocketHandler> handler;

WebSocketPlugin::WebSocketPlugin()
	:libraryPath(""), subscriber(this)
{
	handler = make_unique<WebSocketHandler>(this);
	LOG->Info("WebSocketPlugin loaded");
}

WebSocketPlugin::~WebSocketPlugin()
{
	delete handler.release();
	LOG->Info("WebSocketPlugin unloaded");
}

void WebSocketPlugin::RegisterFunctionInternal(RString name, SocketFunction function)
{
	handler->RegisterFunction(name, function);
}

bool RegisterFunction(PluginBase* p, RString name, SocketFunction function)
{
	WebSocketPlugin* plugin = dynamic_cast<WebSocketPlugin*>(p);
	if (!plugin)
		return false;

	plugin->RegisterFunctionInternal(name, function);
	return true;
}

void* WebSocketPlugin::GetSymbol(const char* name)
{
	if (strcmp(name, "RegisterFunction") == 0)
		return (void*)RegisterFunction;

	return nullptr;
}

RString WebSocketPlugin::GetDocRoot()
{
	return "";
	/*
	RString lib;

	PLUGINMAN->CopyString(
		GetFileNameWithoutExtension(
			PLUGINMAN->MakeString(libraryPath.c_str())
		)
	);

	return "";

	RString folder, junk;
	splitpath(libraryPath, folder, junk, junk);

	if (lib && folder) {
		return folder + "/" + lib;
	}

	return "";
	*/
}

void WebSocketPlugin::Update(float fDeltaTime)
{
	Screen* screen = SCREENMAN->GetTopScreen();
	if (screen)
	{
		RString screenName = screen->GetName();
		if (lastScreen != screenName)
		{

			lastScreen = screenName;
		}
	}

}

void SocketRequest::SetResponseField(RString key, bool value)
{
	response[key] = value;
}

void SocketRequest::SetResponseField(RString key, int value)
{
	response[key] = value;
}

void SocketRequest::SetResponseField(RString key, RString value)
{
	response[key] = value;
}

WebSocketMessageSubscriber::WebSocketMessageSubscriber(WebSocketPlugin* plugin)
	:plugin(plugin)
{
	SubscribeToMessage("DoneLoadingNextSong");
}

WebSocketMessageSubscriber::~WebSocketMessageSubscriber()
{
	UnsubscribeAll();
}

void WebSocketMessageSubscriber::HandleMessage(const Message& msg)
{
	if (handler)
	{
		while (handler->IsWaiting())
			;
	}
}
