
import 'winrt_scanner_plugin_platform_interface.dart';

class WinrtScannerPlugin {
  Future<String?> getPlatformVersion() {
    return WinrtScannerPluginPlatform.instance.getPlatformVersion();
  }
}