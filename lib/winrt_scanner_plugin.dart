import 'dart:io';

import 'winrt_scanner_plugin_platform_interface.dart';

class WinrtScannerPlugin {
  Future<String?> getPlatformVersion() {
    return WinrtScannerPluginPlatform.instance.getPlatformVersion();
  }

  Future<List?> getScanners() {
    return WinrtScannerPluginPlatform.instance.getScanners();
  }

  Future<List?> startScan(String deviceId, String source, String colorMode,
      bool isDuplex, String directory) {
    return WinrtScannerPluginPlatform.instance
        .startScan(deviceId, source, colorMode, isDuplex, directory);
  }
}
