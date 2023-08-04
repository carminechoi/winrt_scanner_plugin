#ifndef FLUTTER_PLUGIN_WINRT_SCANNER_PLUGIN_H_
#define FLUTTER_PLUGIN_WINRT_SCANNER_PLUGIN_H_

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>

#include <windows.h>
#include <memory>
#include <winrt/windows.foundation.h>

using namespace winrt;

namespace winrt_scanner_plugin
{
    class WinrtScannerPlugin : public flutter::Plugin
    {
    public:
        static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

        WinrtScannerPlugin();

        virtual ~WinrtScannerPlugin();

        // Disallow copy and assign.
        WinrtScannerPlugin(const WinrtScannerPlugin &) = delete;
        WinrtScannerPlugin &operator=(const WinrtScannerPlugin &) = delete;

        // Called when a method is called on this plugin's channel from Dart.
        void HandleMethodCall(
            const flutter::MethodCall<flutter::EncodableValue> &method_call,
            std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

    private:
        void GetPlatformVersion(std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

        fire_and_forget GetScanners(std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
        fire_and_forget StartScan(std::string device_id, std::string source, std::string colorMode, bool isDuplex, std::string directory, std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    };

} // namespace winrt_scanner_plugin

#endif // FLUTTER_PLUGIN_WINRT_SCANNER_PLUGIN_H_
