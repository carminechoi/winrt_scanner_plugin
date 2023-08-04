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

#include <winrt/windows.foundation.h>
#include <winrt/windows.foundation.collections.h>
#include <winrt/windows.devices.scanners.h>
#include <winrt/windows.devices.enumeration.h>
#include <winrt/Windows.Storage.h>

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Devices::Enumeration;
using namespace Windows::Devices::Scanners;
using namespace Windows::Storage;

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
    else if (method_call.method_name().compare("getScanners") == 0)
    {
      GetScanners(std::move(result));
    }
    else if (method_call.method_name().compare("startScan") == 0)
    {
      auto args = std::get<flutter::EncodableMap>(*method_call.arguments());
      auto deviceId = std::get<std::string>(args[flutter::EncodableValue("deviceId")]);
      auto source = std::get<std::string>(args[flutter::EncodableValue("source")]);
      auto directory = std::get<std::string>(args[flutter::EncodableValue("directory")]);
      auto colorMode = std::get<std::string>(args[flutter::EncodableValue("colorMode")]);
      auto isDuplex = std::get<bool>(args[flutter::EncodableValue("isDuplex")]);

      StartScan(deviceId, source, colorMode, isDuplex, directory, std::move(result));
    }
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

  fire_and_forget WinrtScannerPlugin::GetScanners(std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
  {
    try
    {
      // Get all scanner devices with WinRT
      DeviceInformationCollection scanners = co_await DeviceInformation::FindAllAsync(DeviceClass::ImageScanner);

      flutter::EncodableList scannerList;

      for (const auto &scanner : scanners)
      {
        // Create a map for each scanner and populate it with the scanner's info
        flutter::EncodableMap scannerInfo;
        scannerInfo[flutter::EncodableValue("deviceId")] = flutter::EncodableValue(to_string(scanner.Id()));
        scannerInfo[flutter::EncodableValue("deviceName")] = flutter::EncodableValue(to_string(scanner.Name()));
        scannerInfo[flutter::EncodableValue("isEnabled")] = flutter::EncodableValue(scanner.IsEnabled());

        // Get the ImageScanner for this device
        ImageScanner imageScanner = co_await ImageScanner::FromIdAsync(scanner.Id());
        if (imageScanner)
        {
          flutter::EncodableMap flatbed;
          flutter::EncodableMap feeder;

          // Check if the flatbed is supported
          flatbed[flutter::EncodableValue("isSupported")] = flutter::EncodableValue(imageScanner.IsScanSourceSupported(ImageScannerScanSource::Flatbed));

          // Check if the feeder is supported
          feeder[flutter::EncodableValue("isSupported")] = flutter::EncodableValue(imageScanner.IsScanSourceSupported(ImageScannerScanSource::Feeder));

          // Check if flatbed color, monochrome, and grayscale are supported
          ImageScannerFlatbedConfiguration flatbedConfiguration = imageScanner.FlatbedConfiguration();
          flatbed[flutter::EncodableValue("isColorSupported")] = flutter::EncodableValue(flatbedConfiguration.IsColorModeSupported(ImageScannerColorMode::Color));
          flatbed[flutter::EncodableValue("isMonoChromeSupported")] = flutter::EncodableValue(flatbedConfiguration.IsColorModeSupported(ImageScannerColorMode::Monochrome));
          flatbed[flutter::EncodableValue("isGrayscaleSupported")] = flutter::EncodableValue(flatbedConfiguration.IsColorModeSupported(ImageScannerColorMode::Grayscale));

          // Check if feeder color, monochrome, and grayscale are supported
          ImageScannerFeederConfiguration feederConfiguration = imageScanner.FeederConfiguration();
          feeder[flutter::EncodableValue("isColorSupported")] = flutter::EncodableValue(feederConfiguration.IsColorModeSupported(ImageScannerColorMode::Color));
          feeder[flutter::EncodableValue("isMonoChromeSupported")] = flutter::EncodableValue(feederConfiguration.IsColorModeSupported(ImageScannerColorMode::Monochrome));
          feeder[flutter::EncodableValue("isGrayscaleSupported")] = flutter::EncodableValue(feederConfiguration.IsColorModeSupported(ImageScannerColorMode::Grayscale));

          // Check if feeder duplex is supported
          feeder[flutter::EncodableValue("isDuplexSupported")] = flutter::EncodableValue(feederConfiguration.CanScanDuplex());

          scannerInfo[flutter::EncodableValue("flatbed")] = flatbed;
          scannerInfo[flutter::EncodableValue("feeder")] = feeder;
        }

        // Add the completed scanner map to the list
        scannerList.push_back(scannerInfo);
      }

      // Return the scanner list as the result
      result->Success(scannerList);
    }
    catch (hresult_error const &ex)
    {
      result->Error("SCANNER_ERROR", to_string(ex.message()));
    }
  }

  fire_and_forget WinrtScannerPlugin::StartScan(std::string deviceId, std::string source, std::string colorMode, bool isDuplex, std::string directory, std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
  {
    try
    {
      // Initialize scanners
      ImageScanner imageScanner = co_await ImageScanner::FromIdAsync(to_hstring(deviceId));

      // Declare scanner source and format configuration
      IImageScannerSourceConfiguration sourceConfig;
      IImageScannerFormatConfiguration formatConfig;
      ImageScannerScanSource sourceEnum = ImageScannerScanSource::Default;

      // Set scanner configuration variables
      if (source == "Flatbed")
      {
        sourceConfig = imageScanner.FlatbedConfiguration();
        formatConfig = imageScanner.FlatbedConfiguration();
        sourceEnum = ImageScannerScanSource::Flatbed;
      }
      else
      {
        sourceConfig = imageScanner.FeederConfiguration();
        formatConfig = imageScanner.FeederConfiguration();
        sourceEnum = ImageScannerScanSource::Feeder;

        // Set to Zero to scan all pages in feeder
        imageScanner.FeederConfiguration().MaxNumberOfPages(0);

        // Configure duplex
        imageScanner.FeederConfiguration().Duplex(isDuplex);
      }

      // Configure color mode
      if (colorMode == "Color")
      {
        sourceConfig.ColorMode(ImageScannerColorMode::Color);
      }
      else if (colorMode == "Monochrome")
      {
        sourceConfig.ColorMode(ImageScannerColorMode::Monochrome);
      }
      else if (colorMode == "Grayscale")
      {
        sourceConfig.ColorMode(ImageScannerColorMode::Grayscale);
      }
      else
      {
        sourceConfig.ColorMode(ImageScannerColorMode::AutoColor);
      }

      StorageFolder storageFolder = co_await StorageFolder::GetFolderFromPathAsync(to_hstring(directory));

      // Scan images
      ImageScannerScanResult scanResult = co_await imageScanner.ScanFilesToFolderAsync(sourceEnum, storageFolder);
      Collections::IVectorView<StorageFile> scanResults = scanResult.ScannedFiles();

      // Create list of file paths from scanned images
      flutter::EncodableList paths;

      for (StorageFile path : scanResults)
      {
        paths.push_back(flutter::EncodableValue(to_string(path.Path())));
      }
      result->Success(paths);
    }
    catch (hresult_error const &ex)
    {
      result->Error("SCANNING_ERROR", to_string(ex.message()));
    }
  }

} // namespace winrt_scanner_plugin
