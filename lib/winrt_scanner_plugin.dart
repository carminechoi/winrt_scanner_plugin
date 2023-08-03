import 'winrt_scanner_plugin_platform_interface.dart';

class WinrtScannerPlugin {
  Future<String?> getPlatformVersion() {
    return WinrtScannerPluginPlatform.instance.getPlatformVersion();
  }

  Future<List?> getScanners() {
    return WinrtScannerPluginPlatform.instance.getScanners();
  }

  Future<String?> startScan() {
    return WinrtScannerPluginPlatform.instance.startScan();
  }
}
