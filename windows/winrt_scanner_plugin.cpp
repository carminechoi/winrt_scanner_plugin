#include "winrt_scanner_plugin.h"

// This must be included before many other Windows headers.
#include <windows.h>

// For getPlatformVersion; remove unless needed for your plugin implementation.
#include <VersionHelpers.h>

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include <memory>
#include <sstream>

#include <winrt/windows.devices.scanners.h>
#include <winrt/windows.devices.enumeration.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Storage.h>

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Devices::Enumeration;
using namespace Windows::Devices::Scanners;
using namespace Windows::Storage;
using namespace Windows::Graphics::Printing;

namespace winrt_scanner_plugin
{

  // static
  void WinrtScannerPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarWindows *registrar)
  {
    auto channel =
        std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
            registrar->messenger(), "winrt_scanner_plugin",
            &flutter::StandardMethodCodec::GetInstance());

    auto plugin = std::make_unique<WinrtScannerPlugin>();

    channel->SetMethodCallHandler(
        [plugin_pointer = plugin.get()](const auto &call, auto result)
        {
          plugin_pointer->HandleMethodCall(call, std::move(result));
        });

    registrar->AddPlugin(std::move(plugin));
  }

  WinrtScannerPlugin::WinrtScannerPlugin() {}

  WinrtScannerPlugin::~WinrtScannerPlugin() {}

  void WinrtScannerPlugin::HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue> &method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
  {
    if (method_call.method_name().compare("getPlatformVersion") == 0)
    {
      GetPlatformVersion(std::move(result));
    }
    // else if (method_call.method_name().compare("startScan"))
    // {
    //   StartScan(std::move(result));
    // }
    else
    {
      result->NotImplemented();
    }
  }

  void WinrtScannerPlugin::GetPlatformVersion(std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
  {
    std::ostringstream version_stream;
    version_stream << "Windows ";
    if (IsWindows10OrGreater())
    {
      version_stream << "10+";
    }
    else if (IsWindows8OrGreater())
    {
      version_stream << "8";
    }
    else if (IsWindows7OrGreater())
    {
      version_stream << "7";
    }
    result->Success(flutter::EncodableValue(version_stream.str()));
  }

  // void WinrtScannerPlugin::StartScan(std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
  // {
  //   try
  //   {

  //     // Initialize scanners
  //     winrt::Windows::Devices::Enumeration::DeviceInformationCollection scanners = co_await winrt::Windows::Devices::Enumeration::DeviceInformation::FindAllAsync(winrt::Windows::Devices::Scanners::ImageScanner::GetDeviceSelector());

  //     if (scanners.Size() == 0)
  //     {
  //       result->Error("NO_SCANNER", "No scanners found.");
  //       return;
  //     }

  //     // Assuming we use the first scanner found; you can customize this based on your requirements
  //     winrt::Windows::Devices::Scanners::ImageScanner scanner = co_await winrt::Windows::Devices::Scanners::ImageScanner::FromIdAsync(scanners.GetAt(0).Id());

  //     if (!scanner)
  //     {
  //       result->Error("SCANNER_NOT_AVAILABLE", "Scanner not available.");
  //       return;
  //     }

  //     // Perform the scanning operation
  //     winrt::Windows::Storage::Streams::IRandomAccessStream scanStream = co_await scanner.ScanFilesToFolderAsync(
  //         winrt::Windows::Devices::Scanners::ImageScannerScanSource::AutoConfigured,
  //         KnownFolders::PicturesLibrary());

  //     // You can do further processing here, e.g., extracting the scanned image file path or scanned text.

  //     result->Success(flutter::EncodableValue("Scanning completed successfully."));
  //   }
  //   catch (winrt::hresult_error const &ex)
  //   {
  //     result->Error("SCANNING_ERROR", winrt::to_string(ex.message()));
  //   }
  // }

} // namespace winrt_scanner_plugin
