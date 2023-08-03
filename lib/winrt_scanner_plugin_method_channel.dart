import 'package:flutter/foundation.dart';
import 'package:flutter/services.dart';

import 'winrt_scanner_plugin_platform_interface.dart';

/// An implementation of [WinrtScannerPluginPlatform] that uses method channels.
class MethodChannelWinrtScannerPlugin extends WinrtScannerPluginPlatform {
  /// The method channel used to interact with the native platform.
  @visibleForTesting
  final methodChannel = const MethodChannel('winrt_scanner_plugin');

  @override
  Future<String?> getPlatformVersion() async {
    final version =
        await methodChannel.invokeMethod<String>('getPlatformVersion');
    return version;
  }

  @override
  Future<List?> getScanners() async {
    final scanners = await methodChannel.invokeMethod<List>('getScanners');
    print(scanners);
    return scanners;
  }

  @override
  Future<String?> startScan() async {
    final version = await methodChannel.invokeMethod<String>('startScan');
    return version;
  }
}
