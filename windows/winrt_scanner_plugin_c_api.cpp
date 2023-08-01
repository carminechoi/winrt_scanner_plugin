#include "include/winrt_scanner_plugin/winrt_scanner_plugin_c_api.h"

#include <flutter/plugin_registrar_windows.h>

#include "winrt_scanner_plugin.h"

void WinrtScannerPluginCApiRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar)
{
    winrt_scanner_plugin::WinrtScannerPlugin::RegisterWithRegistrar(
        flutter::PluginRegistrarManager::GetInstance()
            ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
